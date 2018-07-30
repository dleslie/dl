#include "dl.h"

#if DL_USE_CONTAINERS && DL_USE_TEST

/***************************************
 * Tools
 **************************************/

any _test_zip(any data, any left, any right, any out) {
  *(integer *)out = *(integer *)left + *(integer *)right;
  return out;
}

any _test_sum(any data, any item, any left) {
  *(integer *)item = *(integer *)item + *(integer *)left;
  return data;
}

any _test_add(any data, any item, any left) {
  integer new_item = *(integer *)item + *(integer *)left;
  if (new_item <= 10)
    *(integer *)item = new_item;
  return item;
}

any _test_push(any data, any item, any left) {
  return collection_push((collection *)item, left);
}

integer _test_match_even_func(any data, any value) {
  return (*(integer *)value % 2) == 0;
}

dl_filter _test_match_even = {
  .data = NULL,
  .func = _test_match_even_func
};

integer _test_match_natural_func(any data, any value) {
  return *(integer *)value > 0;
}

dl_filter _test_match_natural = {
  .data = NULL,
  .func = _test_match_natural_func
};

bool _test_match_zero(any data, any value) {
  return *(integer *)value == 0;
}

integer _test_compare_integer(any data, any left, any right) {
  return *(integer *)left - *(integer *)right;
}

any _test_converter(any data, any left, any out) {
  *(integer *)out = 2 * *(integer *)left;
  return out;
}

any _test_do_nothing(any data, any value) {
  return value;
}

void _print_collection(collection *c1) {
  char buf[256]; buf[0] = 0;
  char buf2[256]; buf2[0] = 0;
  iterator i;
  any ref;
  for (ref = collection_begin_ref(c1, &i); ref != NULL; ref = collection_next(c1, &i)) {
#if IS_C89 || IS_C90
    sprintf(buf2, "%s %i", buf, *(integer *)ref);
    sprintf(buf, "%s", buf2);
#else
    snprintf(buf2, 256, "%s %li", buf, *(integer *)ref);
    snprintf(buf, 256, "%s", buf2);
#endif
  }
  INFO("%s", buf2);
}

bool _confirm_properties(collection *c, const char *type_name) {
  iterator index = collection_begin(c);
  iterator last = make_invalid_iterator(c);
  iterator next_index, next, prev;
  any item, next_item, last_item;

  for (item = collection_ref(c, index); item != NULL; item = collection_next(c, &index)) {
    prev = index;
    collection_prev(c, &prev);
    if (!check(iterator_equal(c, prev, last),
	       "%s: Expected iterators to monotonically increase.", type_name))
      return false;
    last = index;
      
    if (iterator_is_valid(c, prev)) {
      next = prev;
      collection_next(c, &next);
      if (!check(iterator_equal(c, next, index),
		 "%s: Expected iterators to monotonically increase.", type_name))
	return false;
    }

    next = index;
    collection_next(c, &next);
    prev = next;
    collection_prev(c, &prev);
    if (!check(iterator_equal(c, prev, index),
	       "%s: Expected iterators to monotonically decrease.", type_name))
      return false;

    if (!check(_abs(*(integer *)item) <= 20,
	       "%s: expected all test values to be -20<=x<=20, found %i.",
	       type_name, *(integer *)item))
      return false;
  }

  if (collection_is_sorted(c)) {
    last_item = NULL;
    index = collection_begin(c);
    
    for (item = collection_ref(c, index);
      item != NULL;
      last_item = item, item = collection_next(c, &index)) {
      if (item != NULL && last_item != NULL
	  && !check(c->settings.comparer.func(c->settings.comparer.data, last_item, item) <= 0,
          "%s: Expected elements to be sorted, %i came before %i.",
          type_name, *(integer *)last_item, *(integer *)item))
        return false;
    }
  }

  if (collection_is_set(c)) {
    index = collection_begin(c);
    for (item = collection_ref(c, index);
      item != NULL;
      item = collection_next(c, &index)) {
      next_index = index;
      for (next_item = collection_next(c, &next_index);
        next_item != NULL;
        next_item = collection_next(c, &next_index)) {
        if (!check(c->settings.comparer.func(c->settings.comparer.data, item, next_item) != 0,
          "%s: Expected all elements to be unique, found two %i.",
          type_name, *(integer *)item))
          return false;
      }
    }
  }

  return true;
}

bool _contains_only(const char *type_name, collection *c, integer *data, natural count) {
  iterator idx;
  any ref;
  bool fail;
  natural didx;
  
  idx = collection_begin(c);
  for (ref = collection_ref(c, idx);
       ref != NULL;
       ref = collection_next(c, &idx)) {
    fail = true;
    for (didx = 0; didx < count; ++didx)
      if (*(integer *)ref == data[didx]) {
	fail = false;
	break;
      }
    if (!check(!fail, "%s: Expected not to see %i.", type_name, *(integer *)ref))
      return false;
  }

  for (didx = 0; didx < count; ++didx) {
    fail = true;
    idx = collection_begin(c);
    for (ref = collection_ref(c, idx);
	 ref != NULL;
	 ref = collection_next(c, &idx))
      if (*(integer *)ref == data[didx]) {
	fail = false;
	break;
      }
    if (!check(!fail, "%s: Expected to see %i.", type_name, data[didx]))
      return false;
  }

  return true;
}

/***************************************
 * Data
 **************************************/

integer _c_data_a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
integer _c_data_b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
integer _c_data_c[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
integer _c_data_d[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };


typedef struct {
  natural a;
  byte padding[256];
  natural b;
  byte more_padding[256];
} fat_data;

typedef struct {
  collection_type type;
  storage_type storage;
  char *name;
  comparator comp;
  dl_handler destructor;
  bool sorted;
  bool fifo;
} _collection_type_info;

_collection_type_info _c_types[] = {
  {.type = COLLECTION_TYPE_LIST,.storage = STORAGE_TYPE_VECTOR,.name = "VECTOR_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = false },
  {.type = COLLECTION_TYPE_LIST,.storage = STORAGE_TYPE_VECTOR,.name = "~VECTOR_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = false },
  {.type = COLLECTION_TYPE_QUEUE,.storage = STORAGE_TYPE_VECTOR,.name = "VECTOR_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = true },
  {.type = COLLECTION_TYPE_QUEUE,.storage = STORAGE_TYPE_VECTOR,.name = "~VECTOR_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = true },
  {.type = COLLECTION_TYPE_SORTED_LIST,.storage = STORAGE_TYPE_VECTOR,.name = "VECTOR_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SORTED_LIST,.storage = STORAGE_TYPE_VECTOR,.name = "~VECTOR_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SET,.storage = STORAGE_TYPE_VECTOR,.name = "VECTOR_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SET,.storage = STORAGE_TYPE_VECTOR,.name = "~VECTOR_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_LIST,.storage = STORAGE_TYPE_LINKED_LIST,.name = "LINKED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = false },
  {.type = COLLECTION_TYPE_LIST,.storage = STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = false },
  {.type = COLLECTION_TYPE_QUEUE,.storage = STORAGE_TYPE_LINKED_LIST,.name = "LINKED_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = true },
  {.type = COLLECTION_TYPE_QUEUE,.storage = STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = true },
  {.type = COLLECTION_TYPE_SET,.storage = STORAGE_TYPE_LINKED_LIST,.name = "LINKED_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SET,.storage = STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SORTED_LIST,.storage = STORAGE_TYPE_LINKED_LIST,.name = "LINKED_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = COLLECTION_TYPE_SORTED_LIST,.storage = STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
};
const natural _c_type_count = 16;

/***************************************
 * Macros
 **************************************/

#define DUAL_TEST_BEGIN()   collection c1, c2;										\
  natural type1_idx, type2_idx;												\
  const char *type1_name;												\
  comparator type1_comp;												\
  collection_type type1;												\
  dl_handler type1_destructor;												\
  const char *type2_name;												\
  comparator type2_comp;												\
  collection_type type2;												\
  storage_type storage2;												\
  dl_handler type2_destructor;												\
  natural c1_data[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					\
															\
  for (type1_idx = 0; type1_idx < _c_type_count; ++type1_idx) {								\
    type1_name = _c_types[type1_idx].name;										\
    type1_comp = _c_types[type1_idx].comp;										\
    type1 = _c_types[type1_idx].type;											\
    type1_destructor = _c_types[type1_idx].destructor;									\
															\
    if (!check(init_collection_array(&c1, type1, &type1_comp, &type1_destructor, (any)c1_data, sizeof(integer), 20),    \
               "%s: Failed to initialize.", type1_name))								\
      return false;													\
															\
    for (type2_idx = 0; type2_idx < _c_type_count; ++type2_idx) {							\
      type2_name = _c_types[type2_idx].name;										\
      type2_comp = _c_types[type2_idx].comp;										\
      type2 = _c_types[type2_idx].type;											\
      storage2 = _c_types[type2_idx].storage;										\
      type2_destructor = _c_types[type2_idx].destructor;								\
															\
      if (!check(init_collection(&c2, type2, storage2, &type2_comp, &type2_destructor, sizeof(integer)),	        \
                 "%s: Failed to initialize.", type2_name)) {								\
        destroy_collection(&c1);											\
        return false;													\
      }															\
      collection_clear(&c1);												

#define DUAL_TEST_END()						\
    if (!_confirm_properties(&c1, type1_name)) goto fail;	\
    if (!_confirm_properties(&c2, type2_name)) goto fail;	\
    destroy_collection(&c2);					\
  }								\
								\
    destroy_collection(&c1);					\
    }								\
								\
  return true;							\
fail:								\
 _print_collection(&c1);					\
 _print_collection(&c2);					\
 destroy_collection(&c1);					\
 destroy_collection(&c2);					\
 return false;

#define SINGLE_TEST_BEGIN()										\
  collection c;												\
  natural type_idx;											\
  const char *type_name;										\
  comparator type_comp;											\
  collection_type type;											\
  storage_type storage;											\
  dl_handler type_destructor;										\
													\
  for (type_idx = 0; type_idx < _c_type_count; ++type_idx) {						\
    type_name = _c_types[type_idx].name;								\
    type_comp = _c_types[type_idx].comp;								\
    type = _c_types[type_idx].type;									\
    storage = _c_types[type_idx].storage;								\
    type_destructor = _c_types[type_idx].destructor;							\
													\
    if (!check(init_collection(&c, type, storage, &type_comp, &type_destructor, sizeof(integer)),	\
               "%s: Failed to initialize.", type_name))							\
      return false;

#define SINGLE_TEST_END()				\
  if (!_confirm_properties(&c, type_name)) goto fail;	\
  destroy_collection(&c);				\
  }							\
							\
  return true;						\
fail:							\
 _print_collection(&c);					\
 destroy_collection(&c);				\
 return false;

/***************************************
 * Basic Collection Tests
 **************************************/

bool test_init_collection() {
  collection c;
  collection_settings settings;
  collection_type type;
  comparator type_comp;
  const char *type_name;
  dl_handler type_destructor;
  natural type_idx;
  storage_type storage;
  vector v;
  
  for (type_idx = 0; type_idx < _c_type_count; ++type_idx) {
    type_name = _c_types[type_idx].name;
    type_comp = _c_types[type_idx].comp;
    type = _c_types[type_idx].type;
    storage = _c_types[type_idx].storage;
    type_destructor = _c_types[type_idx].destructor;

    switch (storage) {
    case STORAGE_TYPE_LINKED_LIST:
      settings = default_linked_list_collection_settings;
      break;
    case STORAGE_TYPE_VECTOR:
      settings = default_vector_collection_settings;
      break;
    }
    
    settings.type = type;
    settings.storage = storage;
    settings.comparer = type_comp;
    settings.deconstruct_entry = type_destructor;

    if (!check(init_collection_custom(&c, settings, sizeof(integer)),
      "%s: Failed to initialize.", type_name))
      return false;

    destroy_collection(&c);

    if (storage == STORAGE_TYPE_VECTOR) {
      if (!check(init_collection_array(&c, type, &type_comp, &type_destructor, (byte *)_c_data_a, sizeof(integer), 10),
        "%s: Failed to initialize from array.", type_name))
        return false;

      destroy_collection(&c);

      if (!check(init_vector(&v, sizeof(integer), 0), "Failed to initialize vector."))
        return false;

      destroy_vector(&v, NULL);
    }
  }

  return true;
}

bool test_collection_push() {
  SINGLE_TEST_BEGIN();

  if (!check(collection_push(&c, (any)&_c_data_a[0]),
    "%s: Expected push to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_count(&c) > 0,
    "%s: Expected count to increment.", type_name))
    goto fail;

  if (!check(collection_push(&c, (any)&_c_data_a[1]),
    "%s: Expected push to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_count(&c) > 1,
    "%s: Expected count to increment.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_pop() {
  natural idx;
  integer item;
  integer next_item;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  for (idx = 10; idx > 0; --idx) {
    next_item = *(integer *)collection_peek(&c);
    if (!check(collection_pop(&c, (any)&item) && item == next_item,
	       "%s: Expected pop to work, found %i and expected %i", type_name, item, next_item))
      goto fail;
    if (!_confirm_properties(&c, type_name))
      goto fail;
  }

  SINGLE_TEST_END();
}

bool test_collection_peek() {
  any ref = NULL;
  integer peek_item = 0;
  integer pop_item = 0;
  natural idx;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  for (idx = 10; idx > 0; --idx) {
    if (!check(ref = collection_peek(&c),
      "%s: Expected peek to work", type_name))
      goto fail;

    peek_item = *(integer *)ref;

    if (!check(collection_pop(&c, (any)&pop_item) && pop_item == peek_item,
      "%s: Expected %i to be %i", type_name, peek_item, pop_item))
      goto fail;

    if (!_confirm_properties(&c, type_name))
      goto fail;
  }

  SINGLE_TEST_END();
}

bool test_collection_remove_at() {
  random_state r;
  any ref;
  integer original, removed;
  iterator pos;
  SINGLE_TEST_BEGIN();

  init_random_time(&r);
  
  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  pos = collection_index(&c, (natural)random_integer_range(&r, 0, collection_count(&c)));
  if (!check(ref = collection_ref(&c, pos),
    "%s: expected to ref %lui", type_name, pos))
    goto fail;

  original = *(integer *)ref;

  if (!check(collection_remove_at(&c, &pos, &removed),
    "%s: Expected remove to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(original == removed,
    "%s: Expected %i to be %i", type_name, removed, original))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_remove_all() {
  collection out;
  natural count;
  SINGLE_TEST_BEGIN();

  init_collection(&out, COLLECTION_TYPE_LIST, STORAGE_TYPE_VECTOR, NULL, &type_destructor, sizeof(integer));

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_remove_all(&c, &_test_match_even, &out),
    "%s: Expected remove all to work.", type_name))
    goto infail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  count = collection_count(&out);
  if (!check(count == 5,
    "%s: Expected 5 items to be removed, %lu were.", type_name, count))
    goto infail;

  if (!check(collection_all(&out, &_test_match_even),
    "%s: Expected all items to be even.", type_name))
    goto infail;

  count = collection_count(&c);
  if (!check(count == 5,
    "%s: Expected 5 items to remain, %lu were.", type_name, count))
    goto infail;

  if (!check(!collection_any(&c, &_test_match_even),
    "%s: Expected no items to be even.", type_name))
    goto infail;

  destroy_collection(&out);
  goto insuccess;
infail:
  destroy_collection(&out);
  goto fail;
insuccess:

  SINGLE_TEST_END();
}

bool test_collection_ref() {
  random_state r;
  any ref;
  iterator pos;
  integer val;
  SINGLE_TEST_BEGIN();
  
  init_random_time(&r);

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  pos = collection_index(&c, (natural)random_integer_range(&r, 0, collection_count(&c)));
  if (!check(ref = collection_ref(&c, pos),
    "%s: expected to ref %lui", type_name, pos))
    goto fail;

  val = *(integer *)ref;

  if (!check(val != INTEGER_MIN,
    "%s: Expected %i not to be %i", type_name, val, INTEGER_MIN))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_ref_array() {
  any ref;
  natural count;
  integer first;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  count = 0;
  if (!check(count = collection_ref_array(&c, collection_begin(&c), &ref),
    "%s: expected to ref %lu", type_name, 0))
    goto fail;

  if (!check(count > 0,
    "%s: expected count to be more than zero.", type_name))
    goto fail;

  first = *(integer *)ref;
  if (!check(first != INTEGER_MIN,
    "%s: Expected %i not to be %i", type_name, first, INTEGER_MIN))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_clear() {
  natural before_count, after_count;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  before_count = collection_count(&c);
  collection_clear(&c);
  if (!check(0 == collection_count(&c),
    "%s: Expected clear to work.", type_name))
    goto fail;

  after_count = collection_count(&c);

  if (!check(after_count == 0 && after_count != before_count,
    "%s: Expected clear to work; was %lu in size, is now %lu.", type_name, before_count, after_count))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_is_empty() {
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(!collection_is_empty(&c),
    "%s: Expected collection not to be empty, is %lu in size.", type_name, collection_count(&c)))
    goto fail;

  collection_clear(&c);
  if (!check(0 == collection_count(&c),
    "%s: Expected clear to work.", type_name))
    goto fail;

  if (!check(collection_is_empty(&c),
    "%s: Expected collection to be empty, is %lu in size.", type_name, collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_next() {
  any ref;
  iterator idx;
  natural last_value, count;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  idx = collection_begin(&c);
  last_value = INTEGER_MIN;
  for (count = collection_count(&c); count > 1; --count) {
    if (!check(ref = collection_next(&c, &idx),
      "%s: Expected next to work at count %lu.", type_name, count))
      goto fail;

    if (!check(*(integer *)ref != last_value,
      "%s: Expected values to change.", type_name))
      goto fail;

    last_value = *(integer *)ref;
  }

  SINGLE_TEST_END();
}

bool test_collection_prev() {
  any ref;
  iterator idx;
  natural last_value, count;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  idx = collection_end(&c);
  last_value = INTEGER_MIN;
  for (count = collection_count(&c); count > 0; --count) {
    if (!check(ref = collection_prev(&c, &idx),
      "%s: Expected next to work.", type_name))
      goto fail;

    if (!check(*(integer *)ref != last_value,
      "%s: Expected values to change.", type_name))
      goto fail;

    last_value = *(integer *)ref;
  }

  SINGLE_TEST_END();
}

bool test_collection_count() {
  SINGLE_TEST_BEGIN();

  if (!check(collection_count(&c) == 0,
    "%s: Expected count to be 0, was %i.", type_name, collection_count(&c)))
    goto fail;

  if (10 != collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_count(&c) == 10,
    "%s: Expected count to be 10, was %i.", type_name, collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_copy() {
  natural current;
  DUAL_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c1))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!check(collection_copy(&c1, &c2) && collection_count(&c2) > 0,
    "%s, %s: Expected copy to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  collection_clear(&c1);
  if (!check(collection_count(&c1) == 0,
    "%s: Expected clear to work.", type1_name))
    goto fail;

  if (0 == collection_copy_array(_c_data_b, 10, &c1))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  current = collection_count(&c2);

  if (!check(collection_copy(&c1, &c2),
    "%s, %s: Expected copy to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!check((integer)current < collection_count(&c2),
    "%s, %s: Expected growth in size.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

bool test_collection_all() {
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_all(&c, &_test_match_natural),
    "%s: Expected all values to be natural.", type_name))
    goto fail;

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(!collection_all(&c, &_test_match_natural),
    "%s: Expected some values not to be natural.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_any() {
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(!collection_any(&c, &_test_match_natural),
    "%s: Expected all values not to be natural.", type_name))
    goto fail;

  if (0 == collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_any(&c, &_test_match_natural),
    "%s: Expected some values to be natural.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_drop() {
  SINGLE_TEST_BEGIN();

  random_state r;
  init_random_time(&r);
  
  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  natural count = (natural)random_integer_range(&r, 1, 9);

  if (!check(collection_drop(&c, count),
    "%s: Expected drop to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((10 - (integer)count) == collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", type_name, (10 - count), collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_map() {
  converter convert;
  DUAL_TEST_BEGIN();

  if (!check(collection_copy_array(_c_data_b, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!check(collection_count(&c1) == 10,
    "%s: Expected 10 values, found %lu.", type1_name, collection_count(&c1)))
    goto fail;

  convert.func = _test_converter;
  convert.data = NULL;
  if (!check(collection_map(&c1, &convert, &c2),
    "%s, %s: Failed to map values.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!check(collection_count(&c2) == 10,
    "%s, %s: Expected 10 values, found %lu.", type1_name, type2_name, collection_count(&c2)))
    goto fail;

  if (!check(collection_all(&c2, &_test_match_even),
    "%s, %s: Expected all values to be even.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

bool test_collection_foldl() {
  folder add;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  integer total = 0;
  add.func = _test_add;
  add.data = NULL;
  if (!check(NULL != collection_foldl(&c, &total, &add),
    "%s: Expected foldl to work.", type_name))
    goto fail;

  if (!check(total == 10,
    "%s: Expected fold to sum properly, total is %i.", type_name, total))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_foldr() {
  collection reverse_c;
  folder push;
  SINGLE_TEST_BEGIN();

  init_collection(&reverse_c, COLLECTION_TYPE_LIST, STORAGE_TYPE_VECTOR, &type_comp, &type_destructor, sizeof(integer));

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  push.func = _test_push;
  push.data = NULL;
  if (!check(NULL != collection_foldr(&c, &reverse_c, &push),
    "%s: Expected foldr to work.", type_name))
    goto fail;

  any a, b;
  iterator index_a = collection_end(&c), index_b = collection_begin(&reverse_c);
  while ((a = collection_prev(&c, &index_a)) && (b = collection_ref(&reverse_c, index_b))) {
    if (!check(*(integer *)a == *(integer *)b,
      "%s: Expected %i to be %i", type_name, *(integer *)a, *(integer *)b)) {
      destroy_collection(&reverse_c);
      goto fail;
    }
    collection_next(&reverse_c, &index_b);
  }

  destroy_collection(&reverse_c);

  SINGLE_TEST_END();
}

bool test_collection_zip() {
  collection c3;
  zipper zip;
  folder sum;
  integer total;
  DUAL_TEST_BEGIN();

  if (!check(init_collection(&c3, type1, STORAGE_TYPE_VECTOR, &type1_comp, &type1_destructor, sizeof(integer)),
    "Expected init to work."))
    goto fail;

  if (!check(collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto infail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto infail;

  if (!check(collection_copy_array(_c_data_b, 10, &c2),
    "%s: Expected copy to work.", type1_name))
    goto infail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!_contains_only(type2_name, &c2, _c_data_b, 10))
    goto infail;

  zip.func = _test_zip;
  zip.data = NULL;
  if (!check(collection_zip(&c1, &c2, &zip, &c3),
    "%s, %s: Expected zip to work.", type1_name, type2_name))
    goto infail;

  if (!_confirm_properties(&c3, type1_name))
    goto fail;

  if (!check(10 == collection_count(&c3),
    "%s, %s: Expected zip count to be 10, was %lu.", type1_name, type2_name, collection_count(&c3)))
    goto infail;

  if (!_contains_only(type1_name, &c3, _c_data_d, 10))
    goto infail;

  total = 0;
  sum.func = _test_sum;
  sum.data = NULL;
  if (!check(NULL != collection_foldl(&c3, &total, &sum)
    && total == 100,
    "%s, %s: Expected %i to be %i.", type1_name, type2_name, total, 100))
    goto infail;

  destroy_collection(&c3);
  goto insuccess;
infail:
  destroy_collection(&c3);
  goto fail;
insuccess:

  DUAL_TEST_END();
}

bool test_collection_take() {
  DUAL_TEST_BEGIN();

  random_state r;
  init_random_time(&r);
  
  if (!check(collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  natural count = random_integer(&r, collection_count(&c1));
  integer taken = collection_take(&c1, count, &c2);
  if (!check((integer)count == taken,
    "%s, %s: Expected take to work, wanted %i, got %i.", type1_name, type2_name, count, taken))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!check((integer)count == collection_count(&c2),
    "%s, %s: Expected %lu items, found %lu.",
    type1_name, type2_name, count, collection_count(&c2)))
    goto fail;

  if (!check(10 - (integer)count == collection_count(&c1),
    "%s, %s: Expected %lu items, found %lu.",
    type1_name, type2_name, 10 - count, collection_count(&c1)))
    goto fail;

  DUAL_TEST_END();
}

bool test_collection_find() {
  SINGLE_TEST_BEGIN();

  if (!check(collection_copy_array(_c_data_a, 10, &c)
    && collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  iterator index = collection_begin(&c);
  any ref;
  if (!check(ref = collection_find(&c, &_test_match_natural, &index),
    "%s: Expected find to work.", type_name))
    goto fail;

  if (!check(*(integer *)ref > 0,
    "%s: Expected %i to be natural.", type_name, *(integer *)ref))
    goto fail;

  index = collection_end(&c);
  if (!check(!(ref = collection_find(&c, &_test_match_natural, &index)),
    "%s: Expected find to fail.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_find_last() {
  SINGLE_TEST_BEGIN();

  if (!check(collection_copy_array(_c_data_b, 10, &c)
    && collection_copy_array(_c_data_a, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  iterator index = collection_end(&c);
  any ref;
  if (!check(ref = collection_find_last(&c, &_test_match_natural, &index),
    "%s: Expected find to work.", type_name))
    goto fail;

  if (!check(*(integer *)ref > 0,
    "%s: Expected %i to be natural.", type_name, *(integer *)ref))
    goto fail;

  index = collection_begin(&c);
  if (!check(!(ref = collection_find_last(&c, &_test_match_natural, &index)),
    "%s: Expected find to fail.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_find_all() {
  DUAL_TEST_BEGIN();

  if (!check(collection_copy_array(_c_data_b, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_b, 10))
    goto fail;

  if (!check(collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!check(collection_find_all(&c1, &_test_match_natural, &c2),
    "%s, %s: Expected find to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (collection_is_set(&c2)) {
    if (!check(collection_count(&c2) > 0,
      "%s, %s: Expected some items, found none.", type1_name, type2_name))
      goto fail;
  }
  else if (!collection_is_set(&c1)) {
    if (!check(collection_count(&c2) == 19,
      "%s, %s: Expected 19 items, found %lu.", type1_name, type2_name, collection_count(&c2)))
      goto fail;
  }
  else {
    if (!check(collection_count(&c2) == 10,
      "%s, %s: Expected 10 items, found %lu.", type1_name, type2_name, collection_count(&c2)))
      goto fail;
  }

  if (!check(collection_all(&c2, &_test_match_natural),
    "%s, %s: Expected all items to be natural.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

bool test_collection_remove_first() {
  SINGLE_TEST_BEGIN();

  iterator index = collection_begin(&c);
  integer removed = -1;
  if (!check(!collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!check(collection_copy_array(_c_data_a, 10, &c)
    && collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural count = collection_count(&c);

  index = collection_end(&c);
  if (!check(!collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count == collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = collection_begin(&c);
  if (!check(collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 1 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be natural, was %i.", type_name, removed))
    goto fail;

  if (!check(collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 2 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be natural, was %i.", type_name, removed))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_remove_last() {
  SINGLE_TEST_BEGIN();

  iterator index = collection_begin(&c);
  integer removed = -1;
  if (!check(!collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!check(collection_copy_array(_c_data_a, 10, &c)
    && collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural count = collection_count(&c);

  index = collection_begin(&c);
  if (!check(!collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count == collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = collection_end(&c);
  if (!check(collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 1 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be natural, was %i.", type_name, removed))
    goto fail;

  if (!check(collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 2 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be natural, was %i.", type_name, removed))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_destroy_at() {
  SINGLE_TEST_BEGIN();

  random_state r;
  init_random_time(&r);
  
  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural before_length = collection_count(&c);

  iterator pos = collection_index(&c, (natural)random_integer_range(&r, 0, collection_count(&c)));
  if (!check(collection_destroy_at(&c, &pos),
    "%s: Expected destroy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)before_length == collection_count(&c) + 1,
    "%s: Expected collection to shorten.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_destroy_all() {
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural before_length = collection_count(&c);

  if (!check(collection_destroy_all(&c, &_test_match_even),
    "%s: Expected destroy_all to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)before_length > collection_count(&c),
    "%s: Expected collection to shorten.", type_name))
    goto fail;

  if (!check(!collection_any(&c, &_test_match_even),
    "%s: Expected collection to have no even values.", type_name))
    goto fail;

  SINGLE_TEST_END();
}


bool test_collection_destroy_first() {
  SINGLE_TEST_BEGIN();

  iterator index = collection_begin(&c);
  if (!check(!collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!check(0 != collection_copy_array(_c_data_a, 10, &c)
    && 0 != collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural count = collection_count(&c);

  index = collection_end(&c);
  if (!check(!collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count == collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = collection_begin(&c);
  if (!check(collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 1 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 2 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_destroy_last() {
  SINGLE_TEST_BEGIN();

  iterator index = collection_begin(&c);
  if (!check(!collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!check(0 != collection_copy_array(_c_data_a, 10, &c)
    && 0 != collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural count = collection_count(&c);

  index = collection_begin(&c);
  if (!check(!collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count == collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = collection_end(&c);
  if (!check(collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 1 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!check(collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!check((integer)count - 2 == collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_pop_destroy() {
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  natural count = collection_count(&c);

  if (!check(collection_pop_destroy(&c),
    "%s: Expected toss to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 1 == collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", count - 1, collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_index_of() {
  SINGLE_TEST_BEGIN();

  random_state r;
  init_random_time(&r);
  
  if (0 == collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  integer item1 = _c_data_a[random_integer_range(&r, 0, 4) + 5];
  integer item2 = _c_data_a[random_integer_range(&r, 0, 4)];
  any ref1, ref2;

  iterator index1 = collection_index_of(&c, &item1);
  iterator index2 = collection_index_of(&c, &item2);

  ref1 = collection_ref(&c, index1);
  ref2 = collection_ref(&c, index2);

  if (!check(ref1 != NULL && *(integer *)ref1 == item1,
    "%s: Expected index_of to work.", type_name))
    goto fail;

  if (!check(ref2 != NULL && *(integer *)ref2 == item2,
    "%s: Expected index_of to work.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_fifo() {
  integer item;
  natural idx;
  SINGLE_TEST_BEGIN();

  if (collection_is_queue(&c)) {
    if (0 == collection_copy_array(_c_data_a, 10, &c))
      goto fail;

    if (!_confirm_properties(&c, type_name))
      goto fail;

    for (idx = 0; idx < 10; ++idx) {
      if (!check(collection_pop(&c, &item),
        "%s: Expected pop to work", type_name))
        goto fail;

      if (!_confirm_properties(&c, type_name))
        goto fail;

      if (!check(item == _c_data_a[idx],
        "%s: Expected %l to be %l at %lu",
        item, _c_data_a[idx], idx))
        goto fail;
    }
  }

  SINGLE_TEST_END();
}

bool test_collection_remove_range() {
  natural count;
  integer first, second, third;
  iterator index;
  DUAL_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_c, 10, &c1))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  count = collection_count(&c1);
  first = NATURAL_MAX;
  second = NATURAL_MAX;
  third = NATURAL_MAX;
  
  collection_get(&c1, collection_index(&c1, 2), &first);
  collection_get(&c1, collection_index(&c1, 3), &second);
  collection_get(&c1, collection_index(&c1, 4), &third);

  index = collection_index(&c1, 2);
  if (!check(3 == collection_remove_range(&c1, &index, 3, &c2),
    "%s, %s: Expected remove range to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!check((integer)count - 3 == collection_count(&c1),
    "%s, %s: Expected count to be %lu, was %lu.", type1_name, type2_name, count - 3, collection_count(&c1)))
    goto fail;

  if (!check(3 == collection_count(&c2),
    "%s, %s: Expected count to be 3, was %lu.", type1_name, type2_name, collection_count(&c2)))
    goto fail;

  if (!check(!collection_contains(&c1, &first),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, first))
    goto fail;

  if (!check(!collection_contains(&c1, &second),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, second))
    goto fail;

  if (!check(!collection_contains(&c1, &third),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, third))
    goto fail;

  if (!check(collection_contains(&c2, &first),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, first))
    goto fail;

  if (!check(collection_contains(&c2, &second),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, second))
    goto fail;

  if (!check(collection_contains(&c2, &third),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, third))
    goto fail;

  DUAL_TEST_END();
}

bool test_collection_destroy_range() {
  natural count;
  integer first, second, third;
  iterator index;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  count = collection_count(&c);
  if (!check(count == 10 && iterator_is_valid(&c, collection_index(&c, 2)),
    "%s: Expected 2 to be a valid index.", type_name))
    goto fail;

  first = NATURAL_MAX;
  second = NATURAL_MAX;
  third = NATURAL_MAX;
  
  collection_get(&c, collection_index(&c, 2), &first);
  collection_get(&c, collection_index(&c, 3), &second);
  collection_get(&c, collection_index(&c, 4), &third);
  index = collection_index(&c, 2);

  if (!check(3 == collection_destroy_range(&c, &index, 3),
    "%s: Expected remove range to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check((integer)count - 3 == collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", type_name, count - 3, collection_count(&c)))
    goto fail;

  if (!check(!collection_contains(&c, &first),
    "%s: Expected %i to be removed.", type_name, first))
    goto fail;

  if (!check(!collection_contains(&c, &second),
    "%s: Expected %i to be removed.", type_name, second))
    goto fail;

  if (!check(!collection_contains(&c, &third),
    "%s: Expected %i to be removed.", type_name, third))
    goto fail;

  SINGLE_TEST_END();
}

bool test_collection_insert() {
  integer value;
  iterator index;
  SINGLE_TEST_BEGIN();

  if (0 == collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  value = 11;
  index = collection_index(&c, 5);

  if (!check(collection_insert(&c, &index, &value),
    "%s: Expected insert to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!check(collection_count(&c) == 11,
    "%s: Expected collection to grow.", type_name))
    goto fail;

  if (!check(*(integer *)collection_ref(&c, index) == value,
    "%s: Expected %i to be %i.", type_name, *(integer *)collection_ref(&c, index), value))
    goto fail;

  SINGLE_TEST_END();
}

/***************************************
 * Vectors
 **************************************/

bool test_init_destroy_vector() {
  vector v;

  if (!check(init_vector(&v, sizeof(real), 10),
    "Failed to initialize vector."))
    return false;
  if (!check(v.settings.element_size == sizeof(real),
    "Expected element size to be %lui.", sizeof(real)))
    return false;
  if (!check(vector_capacity(&v) >= 10,
    "Expected vector to have capacity of at least 10."))
    return false;

  destroy_vector(&v, NULL);

  return true;
}

bool test_init_vector_array() {
  vector v;
  char data[] = { 'v', 'e', 'c', 't', 'o', 'r' };

  if (!check(init_vector_array(&v, (byte *)&data, sizeof(char), 6),
    "Failed to initialize vector."))
    return false;
  if (!check(v.settings.element_size == sizeof(char),
    "Expected element size to be %lui.", sizeof(char)))
    return false;
  if (!check(vector_capacity(&v) == 6,
    "Expected vector to have capacity of 6."))
    return false;

  return true;
}

bool test_vector_get() {
  vector v1, v2;
  char data[] = { '1', '2', '3', '4', '5', '6' };
  random_state r;
  char out;
  natural idx;
  
  init_random_time(&r);  
  init_vector_array(&v1, (any)&data, sizeof(char), 6);

  out = 'F';
  idx = (natural)random_integer_range(&r, 0, 5);
  if (!check(vector_get(&v1, idx, (any)&out) && out == data[idx],
    "Expected %c to be %c", out, data[idx]))
    return false;

  if (!check(!vector_get(&v1, 6 + idx, (any)&out),
    "Expected get to fail."))
    return false;

  init_vector(&v2, sizeof(char), 6);

  vector_set(&v2, idx, &data[idx]);
  if (!check(vector_get(&v2, idx, (any)&out) && out == data[idx],
    "Expected %c to be %c", out, data[idx])) {
    destroy_vector(&v2, NULL);
    return false;
  }

  destroy_vector(&v2, NULL);

  return true;
}

bool test_vector_set() {
  vector v1, v2;
  char data[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
  char set_data[] = { '1', '2', '3', '4', '5', '6' };
  natural idx;
  random_state r;
  char out;
  
  init_random_time(&r);
  init_vector_array(&v1, (any)&data, sizeof(char), 6);

  idx = (natural)random_integer_range(&r, 0, 5);
  out = 'F';
  if (!check(vector_set(&v1, idx, (any)&set_data[idx]) &&
    vector_get(&v1, idx, (any)&out) &&
    out == set_data[idx],
    "Expected %c to be %c", out, data[idx]))
    return false;

  if (!check(!vector_set(&v1, 6 + idx, (any)&set_data[idx]),
    "Expected set to fail."))
    return false;

  init_vector(&v2, sizeof(char), 6);

  if (!check(vector_set(&v2, idx, (any)&set_data[idx]) &&
    vector_get(&v2, idx, (any)&out) &&
    out == set_data[idx],
    "Expected %c to be %c", out, data[idx])) {
    destroy_vector(&v2, NULL);
    return false;
  }

  destroy_vector(&v2, NULL);

  return true;
}

bool test_vector_ref() {
  vector v1, v2;
  char data[] = { '1', '2', '3', '4', '5', '6' };
  any out;
  random_state r;
  natural idx;
  
  init_random_time(&r);
  init_vector_array(&v1, (any)&data, sizeof(char), 6);

  out = NULL;
  idx = (natural)random_integer_range(&r, 0, 5);
  if (!check((out = vector_ref(&v1, idx)) && *(char *)out == data[idx],
    "Expected %c to be %c", *(char *)out, data[idx]))
    return false;

  init_vector(&v2, sizeof(char), 6);

  vector_set(&v2, idx, (any)&data[idx]);
  if (!check((out = vector_ref(&v2, idx)) && *(char *)out == data[idx],
    "Expected %c to be %c", *(char *)out, data[idx]))
    return false;

  destroy_vector(&v2, NULL);

  return true;
}

bool test_vector_copy() {
  vector v1, v2, v3, v4, v5, v6;
  char data1[] = { 'v', 'e', 'c', 't', 'o', 'r' };
  char data2[] = { '1', '2', '3', '4', '5', '6' };
  char data3[] = { '1', '2', '3', '4', '5' };
  char data4[] = { '1', '2', '3', '4', '5', '6', '7' };
  char a, b;
  vector_settings settings_5, settings_6;
  natural idx;
  
  init_vector_array(&v1, (any)&data1, sizeof(char), 6);
  init_vector_array(&v2, (any)&data2, sizeof(char), 6);
  init_vector_array(&v3, (any)&data3, sizeof(char), 5);
  init_vector_array(&v4, (any)&data4, sizeof(char), 7);

  settings_5 = default_vector_settings;
  settings_5.slice_length = 12;
  settings_5.element_size = sizeof(char);

  settings_6 = default_vector_settings;
  settings_6.slice_length = 4;
  settings_6.element_size = sizeof(char);

  init_vector_custom(&v5, settings_5, 24);
  init_vector_custom(&v6, settings_6, 28);

  if (!check(vector_capacity(&v5) == 24,
    "Expected capacity to be 24, was %i.", vector_capacity(&v5)))
    goto vector_copy_fail;

  if (!check(vector_capacity(&v6) == 28,
    "Expected capacity to be 28, was %i.", vector_capacity(&v6)))
    goto vector_copy_fail;

  if (!check(vector_copy(&v2, 0, &v1),
    "Expected copy to succeed."))
    goto vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    vector_get(&v1, idx, (any)&a);
    vector_get(&v2, idx, (any)&b);

    if (!check(a == b,
      "Expected %c to be %c.", b, a))
      goto vector_copy_fail;
  }

  if (!check(!vector_copy(&v3, 0, &v1),
    "Expected copy to fail."))
    goto vector_copy_fail;

  if (!check(vector_copy(&v4, 0, &v1),
    "Expected copy to succeed."))
    goto vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    vector_get(&v1, idx, (any)&a);
    vector_get(&v4, idx, (any)&b);

    if (!check(a == b,
      "Expected %c to be %c.", b, a))
      goto vector_copy_fail;
  }

  if (!check(vector_copy(&v5, 0, &v1),
    "Expected copy to succeed."))
    goto vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    vector_get(&v1, idx, (any)&a);
    vector_get(&v5, idx, (any)&b);

    if (!check(a == b,
      "Expected %c to be %c.", b, a))
      goto vector_copy_fail;
  }

  for (idx = 0; idx < 24; ++idx) {
    vector_set(&v5, idx, &data4[idx % 7]);
  }

  if (!check(vector_resize(&v6, vector_capacity(&v5) + vector_capacity(&v6), NULL),
    "Expected resize to work."))
    goto vector_copy_fail;

  if (!check(vector_copy(&v6, 24, &v5),
    "Expected copy to succeed."))
    goto vector_copy_fail;

  for (idx = 0; idx < 14; ++idx) {
    vector_get(&v5, idx, (any)&a);
    vector_get(&v6, idx + 24, (any)&b);

    if (!check(a == b,
      "Expected %c to be %c at %lu.", b, a, idx + 24))
      goto vector_copy_fail;
  }

  if (!check(!vector_copy(&v1, 0, &v6),
    "Expected copy to fail."))
    goto vector_copy_fail;

  if (!check(vector_copy_array(&v5, 0, (byte *)data1, 6),
    "Expected copy to work."))
    goto vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    vector_get(&v5, idx, (any)&a);
    b = data1[idx];

    if (!check(a == b,
      "Expected %c to be %c at %lu.", a, b, idx))
      goto vector_copy_fail;
  }

  destroy_vector(&v5, NULL);
  destroy_vector(&v6, NULL);
  return true;

vector_copy_fail:
  destroy_vector(&v5, NULL);
  destroy_vector(&v6, NULL);
  return false;
}

bool test_vector_grow() {
  vector v1, v2;
  byte data[] = { 0x0 };
  byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  natural original_capacity, idx;
  any value;
  
  init_vector(&v1, sizeof(byte), 32);
  init_vector_array(&v2, (byte *)&data, sizeof(byte), 1);

  original_capacity = vector_capacity(&v1);

  for (idx = 0; idx < original_capacity; ++idx)
    vector_set(&v1, idx, &set_data[idx % 7]);

  if (!check(vector_grow(&v1) && vector_capacity(&v1) > original_capacity,
    "Expected vector to grow."))
    goto test_vector_grow_fail;

  for (idx = 0; idx < original_capacity; ++idx) {
    value = vector_ref(&v1, idx);
    if (!check(*(byte *)value == set_data[idx % 7],
      "Expected original values to remain."))
      goto test_vector_grow_fail;
  }

  if (!check(!vector_grow(&v2),
    "Expected vector not to grow."))
    goto test_vector_grow_fail;

  destroy_vector(&v1, NULL);

  return true;

test_vector_grow_fail:
  destroy_vector(&v1, NULL);
  return false;
}

bool test_vector_shrink() {
  vector v1, v2;
  byte data[] = { 0x0 };
  byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  vector_settings settings_1;
  natural original_capacity, idx;
  any value;
  
  settings_1 = default_vector_settings;
  settings_1.slice_length = 8;
  settings_1.element_size = sizeof(byte);

  init_vector_custom(&v1, settings_1, 32);
  init_vector_array(&v2, (byte *)&data, sizeof(byte), 1);

  original_capacity = vector_capacity(&v1);

  for (idx = 0; idx < original_capacity; ++idx)
    vector_set(&v1, idx, &set_data[idx % 7]);

  if (!check(vector_shrink(&v1, NULL) && vector_capacity(&v1) < original_capacity,
    "Expected vector to shrink."))
    goto test_vector_shrink_fail;

  for (idx = 0; idx < vector_capacity(&v1); ++idx) {
    value = vector_ref(&v1, idx);
    if (!check(*(byte *)value == set_data[idx % 7],
      "Expected original values to remain."))
      goto test_vector_shrink_fail;
  }

  if (!check(!vector_shrink(&v2, NULL),
    "Expected vector not to shrink."))
    goto test_vector_shrink_fail;

  destroy_vector(&v1, NULL);

  return true;

test_vector_shrink_fail:
  destroy_vector(&v1, NULL);
  return false;
}

bool test_vector_resize() {
  vector v1, v2;
  byte item;
  byte data[] = { 0x0 };
  byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  vector_settings settings;
  natural original_capacity, new_capacity, idx;
  
  init_vector_array(&v1, (byte *)&data, sizeof(byte), 1);

  settings = default_vector_settings;
  settings.slice_length = 12;
  settings.element_size = sizeof(byte);

  init_vector_custom(&v2, settings, 24);

  original_capacity = vector_capacity(&v2);
  for (idx = 0; idx < original_capacity; ++idx)
    vector_set(&v2, idx, &set_data[idx % 7]);

  if (!check(!vector_resize(&v1, 2, NULL),
    "Expected vector not to resize"))
    goto test_vector_resize_fail;

  if (!check(!vector_resize(&v2, 0, NULL),
    "Expected vector not to resize"))
    goto test_vector_resize_fail;

  if (!check(vector_resize(&v2, 32, NULL) && vector_capacity(&v2) > original_capacity,
    "Expected vector to resize"))
    goto test_vector_resize_fail;

  for (idx = 0; idx < original_capacity; ++idx) {
    if (!check(vector_get(&v2, idx, (any)&item) && item == set_data[idx % 7],
      "Expected vector data not to change."))
      goto test_vector_resize_fail;
  }

  if (!check(vector_resize(&v2, 8, NULL) && vector_capacity(&v2) < original_capacity,
    "Expected vector to resize"))
    goto test_vector_resize_fail;

  new_capacity = vector_capacity(&v2);
  for (idx = 0; idx < new_capacity; ++idx) {
    if (!check(vector_get(&v2, idx, (any)&item) && item == set_data[idx % 7],
      "Expected vector data not to change."))
      goto test_vector_resize_fail;
  }

  destroy_vector(&v2, NULL);

  return true;

test_vector_resize_fail:
  destroy_vector(&v2, NULL);
  return false;
}

bool test_memory_swap() {
  char data_a[64], data_b[64], data_c[64], data_d[64];
  natural idx;

  dl_memory_copy(data_a, "The quicker brown fox jumped over the lazy dog.", 64);
  dl_memory_copy(data_b, "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-", 64);
  dl_memory_copy(data_c, "The quicker brown fox jumped over the lazy dog.", 64);
  dl_memory_copy(data_d, "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-", 64);

  dl_memory_swap(data_a, data_b, sizeof(char) * 47);
  for (idx = 0; idx < 47; ++idx) {
    if (!check(data_a[idx] == data_d[idx],
      "Expected %c to be %c.", data_a[idx], data_d[idx]))
      return false;
    if (!check(data_b[idx] == data_c[idx],
      "Expected %c to be %c.", data_b[idx], data_c[idx]))
      return false;
  }

  for (idx = 0; idx < 47; ++idx)
    dl_memory_swap(data_a + idx, data_b + idx, sizeof(char));

  for (idx = 0; idx < 47; ++idx) {
    if (!check(data_a[idx] == data_c[idx],
      "Expected %c to be %c.", data_a[idx], data_c[idx]))
      return false;
    if (!check(data_b[idx] == data_d[idx],
      "Expected %c to be %c.", data_b[idx], data_d[idx]))
      return false;
  }

  return true;
}

/***************************************
 * Linked Lists
 **************************************/

bool test_init_linked_list() {
  linked_list list;
  natural count;
  struct linked_list_node *node;
  
  if (!check(init_linked_list(&list, sizeof(natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!check(vector_capacity(&list.node_cache) == list.settings.cache_length,
    "Expected node cache to be %i in length, was %i.",
    list.settings.cache_length,
    vector_capacity(&list.node_cache)))
    goto fail;

  if (!check(list.free != NULL,
    "Expected free list to exist."))
    goto fail;

  if (!check(list.first == NULL,
    "Expected first to be NULL."))
    goto fail;

  if (!check(list.last == NULL,
    "Expected last to be NULL."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!check(count == vector_capacity(&list.node_cache),
    "Expected list node cache to fill free list, filled %i/%i.",
    count, vector_capacity(&list.node_cache)))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_init_linked_list_fat() {
  linked_list list;
  natural count;
  struct linked_list_node *node;
  
  if (!check(init_linked_list(&list, sizeof(fat_data), 32),
    "Expected linked list to initialize."))
    return false;

  if (!check(vector_capacity(&list.node_cache) == list.settings.cache_length,
    "Expected node cache to be %i in length, was %i.",
    list.settings.cache_length,
    vector_capacity(&list.node_cache)))
    goto fail;

  if (!check(list.free != NULL,
    "Expected free list to exist."))
    goto fail;

  if (!check(list.first == NULL,
    "Expected first to be NULL."))
    goto fail;

  if (!check(list.last == NULL,
    "Expected last to be NULL."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!check(count == vector_capacity(&list.node_cache),
    "Expected list node cache to fill free list, filled %i/%i.",
    count, vector_capacity(&list.node_cache)))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_linked_list_add() {
  natural buf[256];
  natural count, current, value;
  linked_list list;
  struct linked_list_node *node;
  
  if (!check(init_linked_list(&list, sizeof(natural), 32),
    "Expected linked list to initialize."))
    return false;

  count = 0;
  if (!check(linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!check(linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!check(linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  buf[0] = 0;
  buf[1] = 2;
  buf[2] = 1;

  while (count < 256 && linked_list_add(&list, list.last, &count)) {
    buf[count] = count;
    ++count;
  }

  if (!check(list.free == NULL,
    "Expected free list to be exhausted."))
    goto fail;

  if (!check(list.first != NULL,
    "Expected first node to exist."))
    goto fail;

  if (!check(list.last != NULL,
    "Expected last node to exist."))
    goto fail;

  node = list.first;
  current = 0;
  while (node != NULL) {
    value = *(natural *)LINKED_LIST_DATA(node);
    if (!check(value == buf[current],
      "Expected %i to be %i.", value, buf[current]))
      goto fail;
    node = node->next;
    ++current;
  }

  if (!check(current == count,
    "Expected to find %i items, found %i.", count, current))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_linked_list_add_fat() {
  fat_data count, value;
  linked_list list;
  
  if (!check(init_linked_list(&list, sizeof(fat_data), 32),
    "Expected linked list to initialize."))
    return false;

  count.a = 0;
  count.b = 37;
  if (!check(linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  if (!check(linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  if (!check(linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  linked_list_get(&list, linked_list_index(&list, 0), &value);
  if (!check(value.a == 0 && value.b == 37,
	     "Expected {%ul, %ul} to be {0, 37}", value.a, value.b))
    goto fail;

  linked_list_get(&list, linked_list_index(&list, 2), &value);
  if (!check(value.a == 1 && value.b == 38,
	     "Expected {%ul, %ul} to be {1, 38}", value.a, value.b))
    goto fail;

  linked_list_get(&list, linked_list_index(&list, 1), &value);
  if (!check(value.a == 2 && value.b == 39,
	     "Expected {%u, %u} to be {2, 39}", value.a, value.b))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_linked_list_capacity() {
  linked_list list;
  if (!check(init_linked_list(&list, sizeof(natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!check(linked_list_capacity(&list) == vector_capacity(&list.node_cache),
    "Expected linked list capacity to be that of its node cache."))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_linked_list_length() {
  linked_list list;
  if (!check(init_linked_list(&list, sizeof(natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!check(linked_list_length(&list) == 0,
    "Expected linked list to be empty."))
    goto fail;

  natural count = 0;
  if (!check(linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!check(linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!check(linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!check(linked_list_length(&list) == count,
    "Expected length to be %i, was %i.", count, linked_list_length(&list)))
    goto fail;

  destroy_linked_list(&list, NULL);
  return true;
fail:
  destroy_linked_list(&list, NULL);
  return false;
}

bool test_linked_list_copy() {
  linked_list a, b;
  natural num, count, value_a, value_b;
  struct linked_list_node *node_a;
  struct linked_list_node *node_b;
  bool success;

  if (!check(init_linked_list(&a, sizeof(natural), 32),
    "Expected init to work."))
    return false;

  if (!check(init_linked_list(&b, sizeof(natural), 32),
    "Expected init to work.")) {
    destroy_linked_list(&a, NULL);
    return false;
  }

  num = linked_list_capacity(&a) / 4;
  for (count = 0; count < num; ++count)
    if (!check(linked_list_add(&a, NULL, &count),
      "Expected add to work."))
      goto fail;

  if (!check(linked_list_length(&b) == 0,
    "Expected list to be empty."))
    goto fail;

  if (!check(linked_list_copy(&b, NULL, &a),
    "Expected copy to work."))
    goto fail;

  if (!check(linked_list_length(&a) == linked_list_length(&b),
    "Expected lists to be the same length."))
    goto fail;

  node_a = a.first;
  node_b = b.first;
  while (node_a != NULL && node_b != NULL) {
    value_a = *(natural *)LINKED_LIST_DATA(node_a);
    value_b = *(natural *)LINKED_LIST_DATA(node_b);
    if (!check(value_a == value_b,
      "Expected data to be the same, found %i and %i.",
      value_a, value_b))
      goto fail;

    node_a = node_a->next;
    node_b = node_b->next;
  }

  if (!check(linked_list_copy(&b, b.first, &a),
    "Expected copy to work."))
    goto fail;

  if (!check(linked_list_copy(&b, b.last, &a),
    "Expected copy to work."))
    goto fail;

  if (!check(3 * linked_list_length(&a) == linked_list_length(&b),
    "Expected lists to be the triple the length."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&a, NULL);
  destroy_linked_list(&b, NULL);
  return success;
}

bool test_linked_list_copy_array() {
  linked_list a;
  natural num, idx;
  integer value;
  struct linked_list_node *node;
  bool success;

  if (!check(init_linked_list(&a, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  num = linked_list_capacity(&a) / 4;

  if (!check(linked_list_length(&a) == 0,
    "Expected list to be empty."))
    goto fail;

  if (!check(linked_list_copy_array(&a, NULL, (byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!check(linked_list_length(&a) == num,
    "Expected list to grow in length."))
    goto fail;


  node = a.first;
  for (idx = 0;
    idx < num && node != NULL;
    ++idx, node = node->next) {
    value = *(integer *)LINKED_LIST_DATA(node);
    if (!check(value == _c_data_a[idx],
      "Expected data to be the same, expected %i to be %i.",
      value, _c_data_a[idx]))
      goto fail;
  }

  if (!check(linked_list_copy_array(&a, a.first, (byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!check(linked_list_copy_array(&a, a.first, (byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!check(3 * num == linked_list_length(&a),
    "Expected lists to be the triple the length."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&a, NULL);
  return success;
}

bool test_linked_list_grow() {
  linked_list list;
  natural sz, new_sz, count;
  bool success;
  struct linked_list_node *node;
  
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  sz = linked_list_capacity(&list);
  if (!check(linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  new_sz = linked_list_capacity(&list);
  if (!check(sz < new_sz,
    "Expected new size to be larger."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!check(count == new_sz,
    "Expected node cache to be size %i, was %i.", new_sz, count))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_shrink() {
  linked_list list;
  integer count;
  natural length, capacity;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  if (!check(linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  count = 0;
  while (linked_list_add(&list, NULL, &count))
    ++count;

  length = linked_list_length(&list);
  capacity = linked_list_capacity(&list);

  if (!check(length == (natural)count && length == capacity,
    "Expected list to be exhausted."))
    goto fail;

  if (!check(linked_list_shrink(&list, NULL),
    "Expected shrink to work."))
    goto fail;

  if (!check(length > linked_list_length(&list) && capacity > linked_list_capacity(&list),
    "Expected size to change."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_resize() {
  linked_list list;
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  if (!check(linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  integer count = 0;
  while (linked_list_add(&list, NULL, &count))
    ++count;

  natural length0 = linked_list_length(&list);
  natural capacity0 = linked_list_capacity(&list);

  if (!check(length0 == capacity0,
    "Expected list to be exhausted."))
    goto fail;

  if (!check(linked_list_resize(&list, capacity0 / 2, NULL),
    "Expected resize smaller to work."))
    goto fail;

  natural length1 = linked_list_length(&list);
  natural capacity1 = linked_list_capacity(&list);

  if (!check(length0 > length1 && capacity0 > capacity1,
    "Expected length and capacity to decrease."))
    goto fail;

  if (!check(linked_list_resize(&list, capacity0, NULL),
    "Expected resize larger to work."))
    goto fail;

  natural length2 = linked_list_length(&list);
  natural capacity2 = linked_list_capacity(&list);

  if (!check(capacity2 >= capacity0,
    "Expected capacity to increase."))
    goto fail;

  if (!check(length1 == length2,
    "Expected length not to change."))
    goto fail;

  bool success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_get() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char buf[256];
  linked_list list;
  natural idx;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!check(linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!check(linked_list_get(&list, list.first, buf),
    "Expected get to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!check(buf[idx] == data[idx],
      "Expected %c to be %c", buf[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_set() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char atad[] = "The quick brown dog jumped over the lazy fox.";
  char buf[256];
  linked_list list;
  natural idx;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!check(linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!check(linked_list_set(&list, list.first, atad),
    "Expected set to work."))
    goto fail;

  if (!check(linked_list_get(&list, list.first, buf),
    "Expected get to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!check(buf[idx] == atad[idx],
      "Expected %c to be %c", buf[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_ref() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char *ref = NULL;
  linked_list list;
  natural idx;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!check(linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!check((ref = linked_list_ref(list.first)),
    "Expected ref to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!check(ref[idx] == data[idx],
      "Expected %c to be %c", ref[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_index() {
  linked_list list;
  integer count, found;
  natural idx;
  struct linked_list_node *node;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (linked_list_add(&list, list.last, &count))
    ++count;

  for (idx = 0; idx < linked_list_length(&list); ++idx) {
    if (!check((node = linked_list_index(&list, idx)),
      "Expected index to work for %i.", idx))
      goto fail;

    if (!check(linked_list_get(&list, node, &found),
      "Expected get to work."))
      goto fail;

    if (!check(found == (integer)idx,
      "Expected %i to be %i.", found, (integer)idx))
      goto fail;
  }

  if (!check(!linked_list_index(&list, NATURAL_MAX),
    "Expected index to fail."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_destroy_range() {
  linked_list list;
  integer count;
  natural length;
  struct linked_list_node *node;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (linked_list_add(&list, NULL, &count))
    ++count;

  length = linked_list_length(&list);
  node = linked_list_index(&list, length / 4);

  if (!check(linked_list_destroy_range(&list, node, length / 4, NULL) == length / 4,
    "Expected destroy range to work."))
    goto fail;

  if (!check(linked_list_length(&list) == length / 4 * 3,
    "Expected destroy range to remove items."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

bool test_linked_list_swap() {
  linked_list list;
  integer count, value0, value1;
  natural length;
  struct linked_list_node *node0, *node1, *next0, *next1, *prev0, *prev1;
  bool success;
  
  if (!check(init_linked_list(&list, sizeof(integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (linked_list_add(&list, NULL, &count))
    ++count;

  length = linked_list_length(&list);
  node0 = linked_list_index(&list, 0);
  node1 = linked_list_index(&list, length / 4);
  next0 = node0->next;
  next1 = node1->next;
  prev0 = node0->previous;
  prev1 = node1->previous;

  linked_list_get(&list, node0, &value0);
  linked_list_get(&list, node1, &value1);

  if (!check(linked_list_swap(&list, node0, node1, false),
    "Expected swap to work."))
    goto fail;

  if (!check(value0 == *(integer *)linked_list_ref(node0),
    "Expected data not to move."))
    goto fail;

  if (!check(value1 == *(integer *)linked_list_ref(node1),
    "Expected data not to move."))
    goto fail;

  if (!check(next1 == node0->next && next0 == node1->next,
    "Expected next to change."))
    goto fail;

  if (!check(prev1 == node0->previous && prev0 == node1->previous,
    "Expected previous to change."))
    goto fail;

  if (!check(linked_list_swap(&list, node0, node1, true),
    "Expected swap to work."))
    goto fail;

  if (!check(value1 == *(integer *)linked_list_ref(node0),
    "Expected data to move."))
    goto fail;

  if (!check(value0 == *(integer *)linked_list_ref(node1),
    "Expected data to move."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  destroy_linked_list(&list, NULL);
  return success;
}

#endif
