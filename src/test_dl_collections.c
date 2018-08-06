#include "dl.h"

#if DL_USE_CONTAINERS && DL_USE_TEST

/***************************************
 * Tools
 **************************************/

dl_any _test_zip(dl_any data, dl_any left, dl_any right, dl_any out) {
  *(dl_integer *)out = *(dl_integer *)left + *(dl_integer *)right;
  return out;
}

dl_any _test_sum(dl_any data, dl_any item, dl_any left) {
  *(dl_integer *)item = *(dl_integer *)item + *(dl_integer *)left;
  return data;
}

dl_any _test_add(dl_any data, dl_any item, dl_any left) {
  dl_integer new_item = *(dl_integer *)item + *(dl_integer *)left;
  if (new_item <= 10)
    *(dl_integer *)item = new_item;
  return item;
}

dl_any _test_push(dl_any data, dl_any item, dl_any left) {
  return dl_collection_push((dl_collection *)item, left);
}

dl_integer _test_match_even_func(dl_any data, dl_any value) {
  return (*(dl_integer *)value % 2) == 0;
}

dl_filter _test_match_even = {
  .data = NULL,
  .func = _test_match_even_func
};

dl_integer _test_match_natural_func(dl_any data, dl_any value) {
  return *(dl_integer *)value > 0;
}

dl_filter _test_match_natural = {
  .data = NULL,
  .func = _test_match_natural_func
};

dl_bool _test_match_zero(dl_any data, dl_any value) {
  return *(dl_integer *)value == 0;
}

dl_integer _test_compare_integer(dl_any data, dl_any left, dl_any right) {
  return *(dl_integer *)left - *(dl_integer *)right;
}

dl_any _test_converter(dl_any data, dl_any left, dl_any out) {
  *(dl_integer *)out = 2 * *(dl_integer *)left;
  return out;
}

dl_any _test_do_nothing(dl_any data, dl_any value) {
  return value;
}

void _print_collection(dl_collection *c1) {
  char buf[256]; buf[0] = 0;
  char buf2[256]; buf2[0] = 0;
  dl_iterator i;
  dl_any ref;
  for (ref = dl_collection_begin_ref(c1, &i); ref != NULL; ref = dl_collection_next(c1, &i)) {
#if DL_IS_C89 || DL_IS_C90
    sprintf(buf2, "%s %i", buf, *(dl_integer *)ref);
    sprintf(buf, "%s", buf2);
#else
    snprintf(buf2, 256, "%s %li", buf, *(dl_integer *)ref);
    snprintf(buf, 256, "%s", buf2);
#endif
  }
  DL_INFO("%s", buf2);
}

dl_bool _confirm_properties(dl_collection *c, const char *type_name) {
  dl_iterator index = dl_collection_begin(c);
  dl_iterator last = make_invalid_dl_iterator(c);
  dl_iterator next_index, next, prev;
  dl_any item, next_item, last_item;

  for (item = dl_collection_ref(c, index); item != NULL; item = dl_collection_next(c, &index)) {
    prev = index;
    dl_collection_prev(c, &prev);
    if (!dl_check(dl_iterator_equal(c, prev, last),
	       "%s: Expected dl_iterators to monotonically increase.", type_name))
      return false;
    last = index;
      
    if (dl_iterator_is_valid(c, prev)) {
      next = prev;
      dl_collection_next(c, &next);
      if (!dl_check(dl_iterator_equal(c, next, index),
		 "%s: Expected dl_iterators to monotonically increase.", type_name))
	return false;
    }

    next = index;
    dl_collection_next(c, &next);
    prev = next;
    dl_collection_prev(c, &prev);
    if (!dl_check(dl_iterator_equal(c, prev, index),
	       "%s: Expected dl_iterators to monotonically decrease.", type_name))
      return false;

    if (!dl_check(dl_abs(*(dl_integer *)item) <= 20,
	       "%s: expected all test values to be -20<=x<=20, found %i.",
	       type_name, *(dl_integer *)item))
      return false;
  }

  if (dl_collection_is_sorted(c)) {
    last_item = NULL;
    index = dl_collection_begin(c);
    
    for (item = dl_collection_ref(c, index);
      item != NULL;
      last_item = item, item = dl_collection_next(c, &index)) {
      if (item != NULL && last_item != NULL
	  && !dl_check(c->settings.comparer.func(c->settings.comparer.data, last_item, item) <= 0,
          "%s: Expected elements to be sorted, %i came before %i.",
          type_name, *(dl_integer *)last_item, *(dl_integer *)item))
        return false;
    }
  }

  if (dl_collection_is_set(c)) {
    index = dl_collection_begin(c);
    for (item = dl_collection_ref(c, index);
      item != NULL;
      item = dl_collection_next(c, &index)) {
      next_index = index;
      for (next_item = dl_collection_next(c, &next_index);
        next_item != NULL;
        next_item = dl_collection_next(c, &next_index)) {
        if (!dl_check(c->settings.comparer.func(c->settings.comparer.data, item, next_item) != 0,
          "%s: Expected all elements to be unique, found two %i.",
          type_name, *(dl_integer *)item))
          return false;
      }
    }
  }

  return true;
}

dl_bool _contains_only(const char *type_name, dl_collection *c, dl_integer *data, dl_natural count) {
  dl_iterator idx;
  dl_any ref;
  dl_bool fail;
  dl_natural didx;
  
  idx = dl_collection_begin(c);
  for (ref = dl_collection_ref(c, idx);
       ref != NULL;
       ref = dl_collection_next(c, &idx)) {
    fail = true;
    for (didx = 0; didx < count; ++didx)
      if (*(dl_integer *)ref == data[didx]) {
	fail = false;
	break;
      }
    if (!dl_check(!fail, "%s: Expected not to see %i.", type_name, *(dl_integer *)ref))
      return false;
  }

  for (didx = 0; didx < count; ++didx) {
    fail = true;
    idx = dl_collection_begin(c);
    for (ref = dl_collection_ref(c, idx);
	 ref != NULL;
	 ref = dl_collection_next(c, &idx))
      if (*(dl_integer *)ref == data[didx]) {
	fail = false;
	break;
      }
    if (!dl_check(!fail, "%s: Expected to see %i.", type_name, data[didx]))
      return false;
  }

  return true;
}

/***************************************
 * Data
 **************************************/

dl_integer _c_data_a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
dl_integer _c_data_b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
dl_integer _c_data_c[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
dl_integer _c_data_d[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };


typedef struct {
  dl_natural a;
  dl_byte padding[256];
  dl_natural b;
  dl_byte more_padding[256];
} fat_data;

typedef struct {
  dl_collection_type type;
  dl_storage_type storage;
  char *name;
  dl_comparator comp;
  dl_handler destructor;
  dl_bool sorted;
  dl_bool fifo;
} _dl_collection_type_info;

_dl_collection_type_info _c_types[] = {
  {.type = DL_COLLECTION_TYPE_LIST,.storage = DL_STORAGE_TYPE_VECTOR,.name = "VECTOR_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = false },
  {.type = DL_COLLECTION_TYPE_LIST,.storage = DL_STORAGE_TYPE_VECTOR,.name = "~VECTOR_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = false },
  {.type = DL_COLLECTION_TYPE_QUEUE,.storage = DL_STORAGE_TYPE_VECTOR,.name = "VECTOR_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = true },
  {.type = DL_COLLECTION_TYPE_QUEUE,.storage = DL_STORAGE_TYPE_VECTOR,.name = "~VECTOR_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = true },
  {.type = DL_COLLECTION_TYPE_SORTED_LIST,.storage = DL_STORAGE_TYPE_VECTOR,.name = "VECTOR_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SORTED_LIST,.storage = DL_STORAGE_TYPE_VECTOR,.name = "~VECTOR_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SET,.storage = DL_STORAGE_TYPE_VECTOR,.name = "VECTOR_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SET,.storage = DL_STORAGE_TYPE_VECTOR,.name = "~VECTOR_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_LIST,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "LINKED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = false },
  {.type = DL_COLLECTION_TYPE_LIST,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = false },
  {.type = DL_COLLECTION_TYPE_QUEUE,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "LINKED_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = false,.fifo = true },
  {.type = DL_COLLECTION_TYPE_QUEUE,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_QUEUE",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = false,.fifo = true },
  {.type = DL_COLLECTION_TYPE_SET,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "LINKED_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SET,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_SET",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SORTED_LIST,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "LINKED_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = NULL},.sorted = true,.fifo = false },
  {.type = DL_COLLECTION_TYPE_SORTED_LIST,.storage = DL_STORAGE_TYPE_LINKED_LIST,.name = "~LINKED_SORTED_LIST",.comp = {.func = _test_compare_integer},.destructor = {.func = _test_do_nothing},.sorted = true,.fifo = false },
};
const dl_natural _c_type_count = 16;

/***************************************
 * Macros
 **************************************/

#define DUAL_TEST_BEGIN()   dl_collection c1, c2;										\
  dl_natural type1_idx, type2_idx;												\
  const char *type1_name;												\
  dl_comparator type1_comp;												\
  dl_collection_type type1;												\
  dl_handler type1_destructor;												\
  const char *type2_name;												\
  dl_comparator type2_comp;												\
  dl_collection_type type2;												\
  dl_storage_type storage2;												\
  dl_handler type2_destructor;												\
  dl_natural c1_data[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					\
															\
  for (type1_idx = 0; type1_idx < _c_type_count; ++type1_idx) {								\
    type1_name = _c_types[type1_idx].name;										\
    type1_comp = _c_types[type1_idx].comp;										\
    type1 = _c_types[type1_idx].type;											\
    type1_destructor = _c_types[type1_idx].destructor;									\
															\
    if (!dl_check(dl_init_collection_array(&c1, type1, &type1_comp, &type1_destructor, (dl_any)c1_data, sizeof(dl_integer), 20),    \
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
      if (!dl_check(dl_init_collection(&c2, type2, storage2, &type2_comp, &type2_destructor, sizeof(dl_integer)),	        \
                 "%s: Failed to initialize.", type2_name)) {								\
        dl_destroy_collection(&c1);											\
        return false;													\
      }															\
      dl_collection_clear(&c1);												

#define DUAL_TEST_END()						\
    if (!_confirm_properties(&c1, type1_name)) goto fail;	\
    if (!_confirm_properties(&c2, type2_name)) goto fail;	\
    dl_destroy_collection(&c2);					\
  }								\
								\
    dl_destroy_collection(&c1);					\
    }								\
								\
  return true;							\
fail:								\
 _print_collection(&c1);					\
 _print_collection(&c2);					\
 dl_destroy_collection(&c1);					\
 dl_destroy_collection(&c2);					\
 return false;

#define SINGLE_TEST_BEGIN()										\
  dl_collection c;												\
  dl_natural type_idx;											\
  const char *type_name;										\
  dl_comparator type_comp;											\
  dl_collection_type type;											\
  dl_storage_type storage;											\
  dl_handler type_destructor;										\
													\
  for (type_idx = 0; type_idx < _c_type_count; ++type_idx) {						\
    type_name = _c_types[type_idx].name;								\
    type_comp = _c_types[type_idx].comp;								\
    type = _c_types[type_idx].type;									\
    storage = _c_types[type_idx].storage;								\
    type_destructor = _c_types[type_idx].destructor;							\
													\
    if (!dl_check(dl_init_collection(&c, type, storage, &type_comp, &type_destructor, sizeof(dl_integer)),	\
               "%s: Failed to initialize.", type_name))							\
      return false;

#define SINGLE_TEST_END()				\
  if (!_confirm_properties(&c, type_name)) goto fail;	\
  dl_destroy_collection(&c);				\
  }							\
							\
  return true;						\
fail:							\
 _print_collection(&c);					\
 dl_destroy_collection(&c);				\
 return false;

/***************************************
 * Basic Collection Tests
 **************************************/

dl_bool test_init_collection() {
  dl_collection c;
  dl_collection_settings settings;
  dl_collection_type type;
  dl_comparator type_comp;
  const char *type_name;
  dl_handler type_destructor;
  dl_natural type_idx;
  dl_storage_type storage;
  dl_vector v;
  
  for (type_idx = 0; type_idx < _c_type_count; ++type_idx) {
    type_name = _c_types[type_idx].name;
    type_comp = _c_types[type_idx].comp;
    type = _c_types[type_idx].type;
    storage = _c_types[type_idx].storage;
    type_destructor = _c_types[type_idx].destructor;

    switch (storage) {
    case DL_STORAGE_TYPE_LINKED_LIST:
      settings = default_linked_list_collection_settings;
      break;
    case DL_STORAGE_TYPE_VECTOR:
      settings = default_vector_collection_settings;
      break;
    }
    
    settings.type = type;
    settings.storage = storage;
    settings.comparer = type_comp;
    settings.deconstruct_entry = type_destructor;

    if (!dl_check(dl_init_collection_custom(&c, settings, sizeof(dl_integer)),
      "%s: Failed to initialize.", type_name))
      return false;

    dl_destroy_collection(&c);

    if (storage == DL_STORAGE_TYPE_VECTOR) {
      if (!dl_check(dl_init_collection_array(&c, type, &type_comp, &type_destructor, (dl_byte *)_c_data_a, sizeof(dl_integer), 10),
        "%s: Failed to initialize from array.", type_name))
        return false;

      dl_destroy_collection(&c);

      if (!dl_check(dl_init_vector(&v, sizeof(dl_integer), 0), "Failed to initialize dl_vector."))
        return false;

      dl_destroy_vector(&v, NULL);
    }
  }

  return true;
}

dl_bool test_collection_push() {
  SINGLE_TEST_BEGIN();

  if (!dl_check(dl_collection_push(&c, (dl_any)&_c_data_a[0]),
    "%s: Expected push to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c) > 0,
    "%s: Expected count to increment.", type_name))
    goto fail;

  if (!dl_check(dl_collection_push(&c, (dl_any)&_c_data_a[1]),
    "%s: Expected push to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c) > 1,
    "%s: Expected count to increment.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_pop() {
  dl_natural idx;
  dl_integer item;
  dl_integer next_item;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  for (idx = 10; idx > 0; --idx) {
    next_item = *(dl_integer *)dl_collection_peek(&c);
    if (!dl_check(dl_collection_pop(&c, (dl_any)&item) && item == next_item,
	       "%s: Expected pop to work, found %i and expected %i", type_name, item, next_item))
      goto fail;
    if (!_confirm_properties(&c, type_name))
      goto fail;
  }

  SINGLE_TEST_END();
}

dl_bool test_collection_peek() {
  dl_any ref = NULL;
  dl_integer peek_item = 0;
  dl_integer pop_item = 0;
  dl_natural idx;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  for (idx = 10; idx > 0; --idx) {
    if (!dl_check(ref = dl_collection_peek(&c),
      "%s: Expected peek to work", type_name))
      goto fail;

    peek_item = *(dl_integer *)ref;

    if (!dl_check(dl_collection_pop(&c, (dl_any)&pop_item) && pop_item == peek_item,
      "%s: Expected %i to be %i", type_name, peek_item, pop_item))
      goto fail;

    if (!_confirm_properties(&c, type_name))
      goto fail;
  }

  SINGLE_TEST_END();
}

dl_bool test_collection_remove_at() {
  dl_random_state r;
  dl_any ref;
  dl_integer original, removed;
  dl_iterator pos;
  SINGLE_TEST_BEGIN();

  dl_init_random_time(&r);
  
  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  pos = dl_collection_index(&c, (dl_natural)dl_random_integer_range(&r, 0, dl_collection_count(&c)));
  if (!dl_check(ref = dl_collection_ref(&c, pos),
    "%s: expected to ref %lui", type_name, pos))
    goto fail;

  original = *(dl_integer *)ref;

  if (!dl_check(dl_collection_remove_at(&c, &pos, &removed),
    "%s: Expected remove to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(original == removed,
    "%s: Expected %i to be %i", type_name, removed, original))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_remove_all() {
  dl_collection out;
  dl_natural count;
  SINGLE_TEST_BEGIN();

  dl_init_collection(&out, DL_COLLECTION_TYPE_LIST, DL_STORAGE_TYPE_VECTOR, NULL, &type_destructor, sizeof(dl_integer));

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_remove_all(&c, &_test_match_even, &out),
    "%s: Expected remove all to work.", type_name))
    goto infail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  count = dl_collection_count(&out);
  if (!dl_check(count == 5,
    "%s: Expected 5 items to be removed, %lu were.", type_name, count))
    goto infail;

  if (!dl_check(dl_collection_all(&out, &_test_match_even),
    "%s: Expected all items to be even.", type_name))
    goto infail;

  count = dl_collection_count(&c);
  if (!dl_check(count == 5,
    "%s: Expected 5 items to remain, %lu were.", type_name, count))
    goto infail;

  if (!dl_check(!dl_collection_any(&c, &_test_match_even),
    "%s: Expected no items to be even.", type_name))
    goto infail;

  dl_destroy_collection(&out);
  goto insuccess;
infail:
  dl_destroy_collection(&out);
  goto fail;
insuccess:

  SINGLE_TEST_END();
}

dl_bool test_collection_ref() {
  dl_random_state r;
  dl_any ref;
  dl_iterator pos;
  dl_integer val;
  SINGLE_TEST_BEGIN();
  
  dl_init_random_time(&r);

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  pos = dl_collection_index(&c, (dl_natural)dl_random_integer_range(&r, 0, dl_collection_count(&c)));
  if (!dl_check(ref = dl_collection_ref(&c, pos),
    "%s: expected to ref %lui", type_name, pos))
    goto fail;

  val = *(dl_integer *)ref;

  if (!dl_check(val != DL_INTEGER_MIN,
    "%s: Expected %i not to be %i", type_name, val, DL_INTEGER_MIN))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_ref_array() {
  dl_any ref;
  dl_natural count;
  dl_integer first;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  count = 0;
  if (!dl_check(count = dl_collection_ref_array(&c, dl_collection_begin(&c), &ref),
    "%s: expected to ref %lu", type_name, 0))
    goto fail;

  if (!dl_check(count > 0,
    "%s: expected count to be more than zero.", type_name))
    goto fail;

  first = *(dl_integer *)ref;
  if (!dl_check(first != DL_INTEGER_MIN,
    "%s: Expected %i not to be %i", type_name, first, DL_INTEGER_MIN))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_clear() {
  dl_natural before_count, after_count;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  before_count = dl_collection_count(&c);
  dl_collection_clear(&c);
  if (!dl_check(0 == dl_collection_count(&c),
    "%s: Expected clear to work.", type_name))
    goto fail;

  after_count = dl_collection_count(&c);

  if (!dl_check(after_count == 0 && after_count != before_count,
    "%s: Expected clear to work; was %lu in size, is now %lu.", type_name, before_count, after_count))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_is_empty() {
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(!dl_collection_is_empty(&c),
    "%s: Expected dl_collection not to be empty, is %lu in size.", type_name, dl_collection_count(&c)))
    goto fail;

  dl_collection_clear(&c);
  if (!dl_check(0 == dl_collection_count(&c),
    "%s: Expected clear to work.", type_name))
    goto fail;

  if (!dl_check(dl_collection_is_empty(&c),
    "%s: Expected dl_collection to be empty, is %lu in size.", type_name, dl_collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_next() {
  dl_any ref;
  dl_iterator idx;
  dl_natural last_value, count;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  idx = dl_collection_begin(&c);
  last_value = DL_INTEGER_MIN;
  for (count = dl_collection_count(&c); count > 1; --count) {
    if (!dl_check(ref = dl_collection_next(&c, &idx),
      "%s: Expected next to work at count %lu.", type_name, count))
      goto fail;

    if (!dl_check(*(dl_integer *)ref != last_value,
      "%s: Expected values to change.", type_name))
      goto fail;

    last_value = *(dl_integer *)ref;
  }

  SINGLE_TEST_END();
}

dl_bool test_collection_prev() {
  dl_any ref;
  dl_iterator idx;
  dl_natural last_value, count;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  ref = NULL;
  idx = dl_collection_end(&c);
  last_value = DL_INTEGER_MIN;
  for (count = dl_collection_count(&c); count > 0; --count) {
    if (!dl_check(ref = dl_collection_prev(&c, &idx),
      "%s: Expected next to work.", type_name))
      goto fail;

    if (!dl_check(*(dl_integer *)ref != last_value,
      "%s: Expected values to change.", type_name))
      goto fail;

    last_value = *(dl_integer *)ref;
  }

  SINGLE_TEST_END();
}

dl_bool test_collection_count() {
  SINGLE_TEST_BEGIN();

  if (!dl_check(dl_collection_count(&c) == 0,
    "%s: Expected count to be 0, was %i.", type_name, dl_collection_count(&c)))
    goto fail;

  if (10 != dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c) == 10,
    "%s: Expected count to be 10, was %i.", type_name, dl_collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_copy() {
  dl_natural current;
  DUAL_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c1))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!dl_check(dl_collection_copy(&c1, &c2) && dl_collection_count(&c2) > 0,
    "%s, %s: Expected copy to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  dl_collection_clear(&c1);
  if (!dl_check(dl_collection_count(&c1) == 0,
    "%s: Expected clear to work.", type1_name))
    goto fail;

  if (0 == dl_collection_copy_array(_c_data_b, 10, &c1))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  current = dl_collection_count(&c2);

  if (!dl_check(dl_collection_copy(&c1, &c2),
    "%s, %s: Expected copy to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!dl_check((dl_integer)current < dl_collection_count(&c2),
    "%s, %s: Expected growth in size.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

dl_bool test_collection_all() {
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_all(&c, &_test_match_natural),
    "%s: Expected all values to be dl_natural.", type_name))
    goto fail;

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(!dl_collection_all(&c, &_test_match_natural),
    "%s: Expected some values not to be dl_natural.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_any() {
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(!dl_collection_any(&c, &_test_match_natural),
    "%s: Expected all values not to be dl_natural.", type_name))
    goto fail;

  if (0 == dl_collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_any(&c, &_test_match_natural),
    "%s: Expected some values to be dl_natural.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_drop() {
  SINGLE_TEST_BEGIN();

  dl_random_state r;
  dl_init_random_time(&r);
  
  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  dl_natural count = (dl_natural)dl_random_integer_range(&r, 1, 9);

  if (!dl_check(dl_collection_drop(&c, count),
    "%s: Expected drop to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((10 - (dl_integer)count) == dl_collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", type_name, (10 - count), dl_collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_map() {
  dl_converter convert;
  DUAL_TEST_BEGIN();

  if (!dl_check(dl_collection_copy_array(_c_data_b, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c1) == 10,
    "%s: Expected 10 values, found %lu.", type1_name, dl_collection_count(&c1)))
    goto fail;

  convert.func = _test_converter;
  convert.data = NULL;
  if (!dl_check(dl_collection_map(&c1, &convert, &c2),
    "%s, %s: Failed to map values.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c2) == 10,
    "%s, %s: Expected 10 values, found %lu.", type1_name, type2_name, dl_collection_count(&c2)))
    goto fail;

  if (!dl_check(dl_collection_all(&c2, &_test_match_even),
    "%s, %s: Expected all values to be even.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

dl_bool test_collection_foldl() {
  dl_folder add;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_b, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_b, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_integer total = 0;
  add.func = _test_add;
  add.data = NULL;
  if (!dl_check(NULL != dl_collection_foldl(&c, &total, &add),
    "%s: Expected foldl to work.", type_name))
    goto fail;

  if (!dl_check(total == 10,
    "%s: Expected fold to sum properly, total is %i.", type_name, total))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_foldr() {
  dl_collection reverse_c;
  dl_folder push;
  SINGLE_TEST_BEGIN();

  dl_init_collection(&reverse_c, DL_COLLECTION_TYPE_LIST, DL_STORAGE_TYPE_VECTOR, &type_comp, &type_destructor, sizeof(dl_integer));

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  push.func = _test_push;
  push.data = NULL;
  if (!dl_check(NULL != dl_collection_foldr(&c, &reverse_c, &push),
    "%s: Expected foldr to work.", type_name))
    goto fail;

  dl_any a, b;
  dl_iterator index_a = dl_collection_end(&c), index_b = dl_collection_begin(&reverse_c);
  while ((a = dl_collection_prev(&c, &index_a)) && (b = dl_collection_ref(&reverse_c, index_b))) {
    if (!dl_check(*(dl_integer *)a == *(dl_integer *)b,
      "%s: Expected %i to be %i", type_name, *(dl_integer *)a, *(dl_integer *)b)) {
      dl_destroy_collection(&reverse_c);
      goto fail;
    }
    dl_collection_next(&reverse_c, &index_b);
  }

  dl_destroy_collection(&reverse_c);

  SINGLE_TEST_END();
}

dl_bool test_collection_zip() {
  dl_collection c3;
  dl_zipper zip;
  dl_folder sum;
  dl_integer total;
  DUAL_TEST_BEGIN();

  if (!dl_check(dl_init_collection(&c3, type1, DL_STORAGE_TYPE_VECTOR, &type1_comp, &type1_destructor, sizeof(dl_integer)),
    "Expected init to work."))
    goto fail;

  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto infail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto infail;

  if (!dl_check(dl_collection_copy_array(_c_data_b, 10, &c2),
    "%s: Expected copy to work.", type1_name))
    goto infail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!_contains_only(type2_name, &c2, _c_data_b, 10))
    goto infail;

  zip.func = _test_zip;
  zip.data = NULL;
  if (!dl_check(dl_collection_zip(&c1, &c2, &zip, &c3),
    "%s, %s: Expected zip to work.", type1_name, type2_name))
    goto infail;

  if (!_confirm_properties(&c3, type1_name))
    goto fail;

  if (!dl_check(10 == dl_collection_count(&c3),
    "%s, %s: Expected zip count to be 10, was %lu.", type1_name, type2_name, dl_collection_count(&c3)))
    goto infail;

  if (!_contains_only(type1_name, &c3, _c_data_d, 10))
    goto infail;

  total = 0;
  sum.func = _test_sum;
  sum.data = NULL;
  if (!dl_check(NULL != dl_collection_foldl(&c3, &total, &sum)
    && total == 100,
    "%s, %s: Expected %i to be %i.", type1_name, type2_name, total, 100))
    goto infail;

  dl_destroy_collection(&c3);
  goto insuccess;
infail:
  dl_destroy_collection(&c3);
  goto fail;
insuccess:

  DUAL_TEST_END();
}

dl_bool test_collection_take() {
  DUAL_TEST_BEGIN();

  dl_random_state r;
  dl_init_random_time(&r);
  
  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  dl_natural count = dl_random_integer(&r, dl_collection_count(&c1));
  dl_integer taken = dl_collection_take(&c1, count, &c2);
  if (!dl_check((dl_integer)count == taken,
    "%s, %s: Expected take to work, wanted %i, got %i.", type1_name, type2_name, count, taken))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!dl_check((dl_integer)count == dl_collection_count(&c2),
    "%s, %s: Expected %lu items, found %lu.",
    type1_name, type2_name, count, dl_collection_count(&c2)))
    goto fail;

  if (!dl_check(10 - (dl_integer)count == dl_collection_count(&c1),
    "%s, %s: Expected %lu items, found %lu.",
    type1_name, type2_name, 10 - count, dl_collection_count(&c1)))
    goto fail;

  DUAL_TEST_END();
}

dl_bool test_collection_find() {
  SINGLE_TEST_BEGIN();

  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c)
    && dl_collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_iterator index = dl_collection_begin(&c);
  dl_any ref;
  if (!dl_check(ref = dl_collection_find(&c, &_test_match_natural, &index),
    "%s: Expected find to work.", type_name))
    goto fail;

  if (!dl_check(*(dl_integer *)ref > 0,
    "%s: Expected %i to be dl_natural.", type_name, *(dl_integer *)ref))
    goto fail;

  index = dl_collection_end(&c);
  if (!dl_check(!(ref = dl_collection_find(&c, &_test_match_natural, &index)),
    "%s: Expected find to fail.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_find_last() {
  SINGLE_TEST_BEGIN();

  if (!dl_check(dl_collection_copy_array(_c_data_b, 10, &c)
    && dl_collection_copy_array(_c_data_a, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_iterator index = dl_collection_end(&c);
  dl_any ref;
  if (!dl_check(ref = dl_collection_find_last(&c, &_test_match_natural, &index),
    "%s: Expected find to work.", type_name))
    goto fail;

  if (!dl_check(*(dl_integer *)ref > 0,
    "%s: Expected %i to be dl_natural.", type_name, *(dl_integer *)ref))
    goto fail;

  index = dl_collection_begin(&c);
  if (!dl_check(!(ref = dl_collection_find_last(&c, &_test_match_natural, &index)),
    "%s: Expected find to fail.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_find_all() {
  DUAL_TEST_BEGIN();

  if (!dl_check(dl_collection_copy_array(_c_data_b, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_b, 10))
    goto fail;

  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c1),
    "%s: Expected copy to work.", type1_name))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  if (!dl_check(dl_collection_find_all(&c1, &_test_match_natural, &c2),
    "%s, %s: Expected find to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (dl_collection_is_set(&c2)) {
    if (!dl_check(dl_collection_count(&c2) > 0,
      "%s, %s: Expected some items, found none.", type1_name, type2_name))
      goto fail;
  }
  else if (!dl_collection_is_set(&c1)) {
    if (!dl_check(dl_collection_count(&c2) == 19,
      "%s, %s: Expected 19 items, found %lu.", type1_name, type2_name, dl_collection_count(&c2)))
      goto fail;
  }
  else {
    if (!dl_check(dl_collection_count(&c2) == 10,
      "%s, %s: Expected 10 items, found %lu.", type1_name, type2_name, dl_collection_count(&c2)))
      goto fail;
  }

  if (!dl_check(dl_collection_all(&c2, &_test_match_natural),
    "%s, %s: Expected all items to be dl_natural.", type1_name, type2_name))
    goto fail;

  DUAL_TEST_END();
}

dl_bool test_collection_remove_first() {
  SINGLE_TEST_BEGIN();

  dl_iterator index = dl_collection_begin(&c);
  dl_integer removed = -1;
  if (!dl_check(!dl_collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c)
    && dl_collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural count = dl_collection_count(&c);

  index = dl_collection_end(&c);
  if (!dl_check(!dl_collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count == dl_collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = dl_collection_begin(&c);
  if (!dl_check(dl_collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 1 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be dl_natural, was %i.", type_name, removed))
    goto fail;

  if (!dl_check(dl_collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 2 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be dl_natural, was %i.", type_name, removed))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_remove_last() {
  SINGLE_TEST_BEGIN();

  dl_iterator index = dl_collection_begin(&c);
  dl_integer removed = -1;
  if (!dl_check(!dl_collection_remove_first(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!dl_check(dl_collection_copy_array(_c_data_a, 10, &c)
    && dl_collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural count = dl_collection_count(&c);

  index = dl_collection_begin(&c);
  if (!dl_check(!dl_collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count == dl_collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = dl_collection_end(&c);
  if (!dl_check(dl_collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 1 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be dl_natural, was %i.", type_name, removed))
    goto fail;

  if (!dl_check(dl_collection_remove_last(&c, &_test_match_natural, &index, &removed),
    "%s: Expected remove to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 2 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(_test_match_natural_func(NULL, &removed),
    "%s: Expected removed value to be dl_natural, was %i.", type_name, removed))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_destroy_at() {
  SINGLE_TEST_BEGIN();

  dl_random_state r;
  dl_init_random_time(&r);
  
  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural before_length = dl_collection_count(&c);

  dl_iterator pos = dl_collection_index(&c, (dl_natural)dl_random_integer_range(&r, 0, dl_collection_count(&c)));
  if (!dl_check(dl_collection_destroy_at(&c, &pos),
    "%s: Expected destroy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)before_length == dl_collection_count(&c) + 1,
    "%s: Expected dl_collection to shorten.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_destroy_all() {
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural before_length = dl_collection_count(&c);

  if (!dl_check(dl_collection_destroy_all(&c, &_test_match_even),
    "%s: Expected destroy_all to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)before_length > dl_collection_count(&c),
    "%s: Expected dl_collection to shorten.", type_name))
    goto fail;

  if (!dl_check(!dl_collection_any(&c, &_test_match_even),
    "%s: Expected dl_collection to have no even values.", type_name))
    goto fail;

  SINGLE_TEST_END();
}


dl_bool test_collection_destroy_first() {
  SINGLE_TEST_BEGIN();

  dl_iterator index = dl_collection_begin(&c);
  if (!dl_check(!dl_collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!dl_check(0 != dl_collection_copy_array(_c_data_a, 10, &c)
    && 0 != dl_collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural count = dl_collection_count(&c);

  index = dl_collection_end(&c);
  if (!dl_check(!dl_collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count == dl_collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = dl_collection_begin(&c);
  if (!dl_check(dl_collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 1 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(dl_collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 2 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_destroy_last() {
  SINGLE_TEST_BEGIN();

  dl_iterator index = dl_collection_begin(&c);
  if (!dl_check(!dl_collection_destroy_first(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!dl_check(0 != dl_collection_copy_array(_c_data_a, 10, &c)
    && 0 != dl_collection_copy_array(_c_data_b, 10, &c),
    "%s: Expected copy to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural count = dl_collection_count(&c);

  index = dl_collection_begin(&c);
  if (!dl_check(!dl_collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to fail.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count == dl_collection_count(&c),
    "%s: Expected count to remain unchanged.", type_name))
    goto fail;

  index = dl_collection_end(&c);
  if (!dl_check(dl_collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 1 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  if (!dl_check(dl_collection_destroy_last(&c, &_test_match_natural, &index),
    "%s: Expected destroy to succeed.", type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 2 == dl_collection_count(&c),
    "%s: Expected count to change.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_pop_destroy() {
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_natural count = dl_collection_count(&c);

  if (!dl_check(dl_collection_pop_destroy(&c),
    "%s: Expected toss to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 1 == dl_collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", count - 1, dl_collection_count(&c)))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_index_of() {
  SINGLE_TEST_BEGIN();

  dl_random_state r;
  dl_init_random_time(&r);
  
  if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_a, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  dl_integer item1 = _c_data_a[dl_random_integer_range(&r, 0, 4) + 5];
  dl_integer item2 = _c_data_a[dl_random_integer_range(&r, 0, 4)];
  dl_any ref1, ref2;

  dl_iterator index1 = dl_collection_index_of(&c, &item1);
  dl_iterator index2 = dl_collection_index_of(&c, &item2);

  ref1 = dl_collection_ref(&c, index1);
  ref2 = dl_collection_ref(&c, index2);

  if (!dl_check(ref1 != NULL && *(dl_integer *)ref1 == item1,
    "%s: Expected index_of to work.", type_name))
    goto fail;

  if (!dl_check(ref2 != NULL && *(dl_integer *)ref2 == item2,
    "%s: Expected index_of to work.", type_name))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_fifo() {
  dl_integer item;
  dl_natural idx;
  SINGLE_TEST_BEGIN();

  if (dl_collection_is_queue(&c)) {
    if (0 == dl_collection_copy_array(_c_data_a, 10, &c))
      goto fail;

    if (!_confirm_properties(&c, type_name))
      goto fail;

    for (idx = 0; idx < 10; ++idx) {
      if (!dl_check(dl_collection_pop(&c, &item),
        "%s: Expected pop to work", type_name))
        goto fail;

      if (!_confirm_properties(&c, type_name))
        goto fail;

      if (!dl_check(item == _c_data_a[idx],
        "%s: Expected %l to be %l at %lu",
        item, _c_data_a[idx], idx))
        goto fail;
    }
  }

  SINGLE_TEST_END();
}

dl_bool test_collection_remove_range() {
  dl_natural count;
  dl_integer first, second, third;
  dl_iterator index;
  DUAL_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_c, 10, &c1))
    goto fail;

  if (!_contains_only(type1_name, &c1, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c1, type1_name))
    goto fail;

  count = dl_collection_count(&c1);
  first = DL_NATURAL_MAX;
  second = DL_NATURAL_MAX;
  third = DL_NATURAL_MAX;
  
  dl_collection_get(&c1, dl_collection_index(&c1, 2), &first);
  dl_collection_get(&c1, dl_collection_index(&c1, 3), &second);
  dl_collection_get(&c1, dl_collection_index(&c1, 4), &third);

  index = dl_collection_index(&c1, 2);
  if (!dl_check(3 == dl_collection_remove_range(&c1, &index, 3, &c2),
    "%s, %s: Expected remove range to work.", type1_name, type2_name))
    goto fail;

  if (!_confirm_properties(&c2, type2_name))
    goto fail;

  if (!dl_check((dl_integer)count - 3 == dl_collection_count(&c1),
    "%s, %s: Expected count to be %lu, was %lu.", type1_name, type2_name, count - 3, dl_collection_count(&c1)))
    goto fail;

  if (!dl_check(3 == dl_collection_count(&c2),
    "%s, %s: Expected count to be 3, was %lu.", type1_name, type2_name, dl_collection_count(&c2)))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c1, &first),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, first))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c1, &second),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, second))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c1, &third),
    "%s, %s: Expected %i to be removed.", type1_name, type2_name, third))
    goto fail;

  if (!dl_check(dl_collection_contains(&c2, &first),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, first))
    goto fail;

  if (!dl_check(dl_collection_contains(&c2, &second),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, second))
    goto fail;

  if (!dl_check(dl_collection_contains(&c2, &third),
    "%s, %s: Expected %i to be in output.", type1_name, type2_name, third))
    goto fail;

  DUAL_TEST_END();
}

dl_bool test_collection_destroy_range() {
  dl_natural count;
  dl_integer first, second, third;
  dl_iterator index;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  count = dl_collection_count(&c);
  if (!dl_check(count == 10 && dl_iterator_is_valid(&c, dl_collection_index(&c, 2)),
    "%s: Expected 2 to be a valid index.", type_name))
    goto fail;

  first = DL_NATURAL_MAX;
  second = DL_NATURAL_MAX;
  third = DL_NATURAL_MAX;
  
  dl_collection_get(&c, dl_collection_index(&c, 2), &first);
  dl_collection_get(&c, dl_collection_index(&c, 3), &second);
  dl_collection_get(&c, dl_collection_index(&c, 4), &third);
  index = dl_collection_index(&c, 2);

  if (!dl_check(3 == dl_collection_destroy_range(&c, &index, 3),
    "%s: Expected remove range to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check((dl_integer)count - 3 == dl_collection_count(&c),
    "%s: Expected count to be %lu, was %lu.", type_name, count - 3, dl_collection_count(&c)))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c, &first),
    "%s: Expected %i to be removed.", type_name, first))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c, &second),
    "%s: Expected %i to be removed.", type_name, second))
    goto fail;

  if (!dl_check(!dl_collection_contains(&c, &third),
    "%s: Expected %i to be removed.", type_name, third))
    goto fail;

  SINGLE_TEST_END();
}

dl_bool test_collection_insert() {
  dl_integer value;
  dl_iterator index;
  SINGLE_TEST_BEGIN();

  if (0 == dl_collection_copy_array(_c_data_c, 10, &c))
    goto fail;

  if (!_contains_only(type_name, &c, _c_data_c, 10))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  value = 11;
  index = dl_collection_index(&c, 5);

  if (!dl_check(dl_collection_insert(&c, &index, &value),
    "%s: Expected insert to work.", type_name))
    goto fail;

  if (!_confirm_properties(&c, type_name))
    goto fail;

  if (!dl_check(dl_collection_count(&c) == 11,
    "%s: Expected dl_collection to grow.", type_name))
    goto fail;

  if (!dl_check(*(dl_integer *)dl_collection_ref(&c, index) == value,
    "%s: Expected %i to be %i.", type_name, *(dl_integer *)dl_collection_ref(&c, index), value))
    goto fail;

  SINGLE_TEST_END();
}

/***************************************
 * Vectors
 **************************************/

dl_bool test_init_destroy_vector() {
  dl_vector v;

  if (!dl_check(dl_init_vector(&v, sizeof(dl_real), 10),
    "Failed to initialize dl_vector."))
    return false;
  if (!dl_check(v.settings.element_size == sizeof(dl_real),
    "Expected element size to be %lui.", sizeof(dl_real)))
    return false;
  if (!dl_check(dl_vector_capacity(&v) >= 10,
    "Expected dl_vector to have capacity of at least 10."))
    return false;

  dl_destroy_vector(&v, NULL);

  return true;
}

dl_bool test_init_vector_array() {
  dl_vector v;
  char data[] = { 'v', 'e', 'c', 't', 'o', 'r' };

  if (!dl_check(dl_init_vector_array(&v, (dl_byte *)&data, sizeof(char), 6),
    "Failed to initialize dl_vector."))
    return false;
  if (!dl_check(v.settings.element_size == sizeof(char),
    "Expected element size to be %lui.", sizeof(char)))
    return false;
  if (!dl_check(dl_vector_capacity(&v) == 6,
    "Expected dl_vector to have capacity of 6."))
    return false;

  return true;
}

dl_bool test_vector_get() {
  dl_vector v1, v2;
  char data[] = { '1', '2', '3', '4', '5', '6' };
  dl_random_state r;
  char out;
  dl_natural idx;
  
  dl_init_random_time(&r);  
  dl_init_vector_array(&v1, (dl_any)&data, sizeof(char), 6);

  out = 'F';
  idx = (dl_natural)dl_random_integer_range(&r, 0, 5);
  if (!dl_check(dl_vector_get(&v1, idx, (dl_any)&out) && out == data[idx],
    "Expected %c to be %c", out, data[idx]))
    return false;

  if (!dl_check(!dl_vector_get(&v1, 6 + idx, (dl_any)&out),
    "Expected get to fail."))
    return false;

  dl_init_vector(&v2, sizeof(char), 6);

  dl_vector_set(&v2, idx, &data[idx]);
  if (!dl_check(dl_vector_get(&v2, idx, (dl_any)&out) && out == data[idx],
    "Expected %c to be %c", out, data[idx])) {
    dl_destroy_vector(&v2, NULL);
    return false;
  }

  dl_destroy_vector(&v2, NULL);

  return true;
}

dl_bool test_vector_set() {
  dl_vector v1, v2;
  char data[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
  char set_data[] = { '1', '2', '3', '4', '5', '6' };
  dl_natural idx;
  dl_random_state r;
  char out;
  
  dl_init_random_time(&r);
  dl_init_vector_array(&v1, (dl_any)&data, sizeof(char), 6);

  idx = (dl_natural)dl_random_integer_range(&r, 0, 5);
  out = 'F';
  if (!dl_check(dl_vector_set(&v1, idx, (dl_any)&set_data[idx]) &&
    dl_vector_get(&v1, idx, (dl_any)&out) &&
    out == set_data[idx],
    "Expected %c to be %c", out, data[idx]))
    return false;

  if (!dl_check(!dl_vector_set(&v1, 6 + idx, (dl_any)&set_data[idx]),
    "Expected set to fail."))
    return false;

  dl_init_vector(&v2, sizeof(char), 6);

  if (!dl_check(dl_vector_set(&v2, idx, (dl_any)&set_data[idx]) &&
    dl_vector_get(&v2, idx, (dl_any)&out) &&
    out == set_data[idx],
    "Expected %c to be %c", out, data[idx])) {
    dl_destroy_vector(&v2, NULL);
    return false;
  }

  dl_destroy_vector(&v2, NULL);

  return true;
}

dl_bool test_vector_ref() {
  dl_vector v1, v2;
  char data[] = { '1', '2', '3', '4', '5', '6' };
  dl_any out;
  dl_random_state r;
  dl_natural idx;
  
  dl_init_random_time(&r);
  dl_init_vector_array(&v1, (dl_any)&data, sizeof(char), 6);

  out = NULL;
  idx = (dl_natural)dl_random_integer_range(&r, 0, 5);
  if (!dl_check((out = dl_vector_ref(&v1, idx)) && *(char *)out == data[idx],
    "Expected %c to be %c", *(char *)out, data[idx]))
    return false;

  dl_init_vector(&v2, sizeof(char), 6);

  dl_vector_set(&v2, idx, (dl_any)&data[idx]);
  if (!dl_check((out = dl_vector_ref(&v2, idx)) && *(char *)out == data[idx],
    "Expected %c to be %c", *(char *)out, data[idx]))
    return false;

  dl_destroy_vector(&v2, NULL);

  return true;
}

dl_bool test_vector_copy() {
  dl_vector v1, v2, v3, v4, v5, v6;
  char data1[] = { 'v', 'e', 'c', 't', 'o', 'r' };
  char data2[] = { '1', '2', '3', '4', '5', '6' };
  char data3[] = { '1', '2', '3', '4', '5' };
  char data4[] = { '1', '2', '3', '4', '5', '6', '7' };
  char a, b;
  dl_vector_settings settings_5, settings_6;
  dl_natural idx;
  
  dl_init_vector_array(&v1, (dl_any)&data1, sizeof(char), 6);
  dl_init_vector_array(&v2, (dl_any)&data2, sizeof(char), 6);
  dl_init_vector_array(&v3, (dl_any)&data3, sizeof(char), 5);
  dl_init_vector_array(&v4, (dl_any)&data4, sizeof(char), 7);

  settings_5 = default_vector_settings;
  settings_5.slice_length = 12;
  settings_5.element_size = sizeof(char);

  settings_6 = default_vector_settings;
  settings_6.slice_length = 4;
  settings_6.element_size = sizeof(char);

  dl_init_vector_custom(&v5, settings_5, 24);
  dl_init_vector_custom(&v6, settings_6, 28);

  if (!dl_check(dl_vector_capacity(&v5) == 24,
    "Expected capacity to be 24, was %i.", dl_vector_capacity(&v5)))
    goto dl_vector_copy_fail;

  if (!dl_check(dl_vector_capacity(&v6) == 28,
    "Expected capacity to be 28, was %i.", dl_vector_capacity(&v6)))
    goto dl_vector_copy_fail;

  if (!dl_check(dl_vector_copy(&v2, 0, &v1),
    "Expected copy to succeed."))
    goto dl_vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    dl_vector_get(&v1, idx, (dl_any)&a);
    dl_vector_get(&v2, idx, (dl_any)&b);

    if (!dl_check(a == b,
      "Expected %c to be %c.", b, a))
      goto dl_vector_copy_fail;
  }

  if (!dl_check(!dl_vector_copy(&v3, 0, &v1),
    "Expected copy to fail."))
    goto dl_vector_copy_fail;

  if (!dl_check(dl_vector_copy(&v4, 0, &v1),
    "Expected copy to succeed."))
    goto dl_vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    dl_vector_get(&v1, idx, (dl_any)&a);
    dl_vector_get(&v4, idx, (dl_any)&b);

    if (!dl_check(a == b,
      "Expected %c to be %c.", b, a))
      goto dl_vector_copy_fail;
  }

  if (!dl_check(dl_vector_copy(&v5, 0, &v1),
    "Expected copy to succeed."))
    goto dl_vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    dl_vector_get(&v1, idx, (dl_any)&a);
    dl_vector_get(&v5, idx, (dl_any)&b);

    if (!dl_check(a == b,
      "Expected %c to be %c.", b, a))
      goto dl_vector_copy_fail;
  }

  for (idx = 0; idx < 24; ++idx) {
    dl_vector_set(&v5, idx, &data4[idx % 7]);
  }

  if (!dl_check(dl_vector_resize(&v6, dl_vector_capacity(&v5) + dl_vector_capacity(&v6), NULL),
    "Expected resize to work."))
    goto dl_vector_copy_fail;

  if (!dl_check(dl_vector_copy(&v6, 24, &v5),
    "Expected copy to succeed."))
    goto dl_vector_copy_fail;

  for (idx = 0; idx < 14; ++idx) {
    dl_vector_get(&v5, idx, (dl_any)&a);
    dl_vector_get(&v6, idx + 24, (dl_any)&b);

    if (!dl_check(a == b,
      "Expected %c to be %c at %lu.", b, a, idx + 24))
      goto dl_vector_copy_fail;
  }

  if (!dl_check(!dl_vector_copy(&v1, 0, &v6),
    "Expected copy to fail."))
    goto dl_vector_copy_fail;

  if (!dl_check(dl_vector_copy_array(&v5, 0, (dl_byte *)data1, 6),
    "Expected copy to work."))
    goto dl_vector_copy_fail;

  for (idx = 0; idx < 6; ++idx) {
    dl_vector_get(&v5, idx, (dl_any)&a);
    b = data1[idx];

    if (!dl_check(a == b,
      "Expected %c to be %c at %lu.", a, b, idx))
      goto dl_vector_copy_fail;
  }

  dl_destroy_vector(&v5, NULL);
  dl_destroy_vector(&v6, NULL);
  return true;

dl_vector_copy_fail:
  dl_destroy_vector(&v5, NULL);
  dl_destroy_vector(&v6, NULL);
  return false;
}

dl_bool test_vector_grow() {
  dl_vector v1, v2;
  dl_byte data[] = { 0x0 };
  dl_byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  dl_natural original_capacity, idx;
  dl_any value;
  
  dl_init_vector(&v1, sizeof(dl_byte), 32);
  dl_init_vector_array(&v2, (dl_byte *)&data, sizeof(dl_byte), 1);

  original_capacity = dl_vector_capacity(&v1);

  for (idx = 0; idx < original_capacity; ++idx)
    dl_vector_set(&v1, idx, &set_data[idx % 7]);

  if (!dl_check(dl_vector_grow(&v1) && dl_vector_capacity(&v1) > original_capacity,
    "Expected dl_vector to grow."))
    goto test_vector_grow_fail;

  for (idx = 0; idx < original_capacity; ++idx) {
    value = dl_vector_ref(&v1, idx);
    if (!dl_check(*(dl_byte *)value == set_data[idx % 7],
      "Expected original values to remain."))
      goto test_vector_grow_fail;
  }

  if (!dl_check(!dl_vector_grow(&v2),
    "Expected dl_vector not to grow."))
    goto test_vector_grow_fail;

  dl_destroy_vector(&v1, NULL);

  return true;

test_vector_grow_fail:
  dl_destroy_vector(&v1, NULL);
  return false;
}

dl_bool test_vector_shrink() {
  dl_vector v1, v2;
  dl_byte data[] = { 0x0 };
  dl_byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  dl_vector_settings settings_1;
  dl_natural original_capacity, idx;
  dl_any value;
  
  settings_1 = default_vector_settings;
  settings_1.slice_length = 8;
  settings_1.element_size = sizeof(dl_byte);

  dl_init_vector_custom(&v1, settings_1, 32);
  dl_init_vector_array(&v2, (dl_byte *)&data, sizeof(dl_byte), 1);

  original_capacity = dl_vector_capacity(&v1);

  for (idx = 0; idx < original_capacity; ++idx)
    dl_vector_set(&v1, idx, &set_data[idx % 7]);

  if (!dl_check(dl_vector_shrink(&v1, NULL) && dl_vector_capacity(&v1) < original_capacity,
    "Expected dl_vector to shrink."))
    goto test_vector_shrink_fail;

  for (idx = 0; idx < dl_vector_capacity(&v1); ++idx) {
    value = dl_vector_ref(&v1, idx);
    if (!dl_check(*(dl_byte *)value == set_data[idx % 7],
      "Expected original values to remain."))
      goto test_vector_shrink_fail;
  }

  if (!dl_check(!dl_vector_shrink(&v2, NULL),
    "Expected dl_vector not to shrink."))
    goto test_vector_shrink_fail;

  dl_destroy_vector(&v1, NULL);

  return true;

test_vector_shrink_fail:
  dl_destroy_vector(&v1, NULL);
  return false;
}

dl_bool test_vector_resize() {
  dl_vector v1, v2;
  dl_byte item;
  dl_byte data[] = { 0x0 };
  dl_byte set_data[] = { 0x1, 0x2, 0x4, 0x8, 0x16, 0x32, 0x64 };
  dl_vector_settings settings;
  dl_natural original_capacity, new_capacity, idx;
  
  dl_init_vector_array(&v1, (dl_byte *)&data, sizeof(dl_byte), 1);

  settings = default_vector_settings;
  settings.slice_length = 12;
  settings.element_size = sizeof(dl_byte);

  dl_init_vector_custom(&v2, settings, 24);

  original_capacity = dl_vector_capacity(&v2);
  for (idx = 0; idx < original_capacity; ++idx)
    dl_vector_set(&v2, idx, &set_data[idx % 7]);

  if (!dl_check(!dl_vector_resize(&v1, 2, NULL),
    "Expected dl_vector not to resize"))
    goto test_vector_resize_fail;

  if (!dl_check(!dl_vector_resize(&v2, 0, NULL),
    "Expected dl_vector not to resize"))
    goto test_vector_resize_fail;

  if (!dl_check(dl_vector_resize(&v2, 32, NULL) && dl_vector_capacity(&v2) > original_capacity,
    "Expected dl_vector to resize"))
    goto test_vector_resize_fail;

  for (idx = 0; idx < original_capacity; ++idx) {
    if (!dl_check(dl_vector_get(&v2, idx, (dl_any)&item) && item == set_data[idx % 7],
      "Expected dl_vector data not to change."))
      goto test_vector_resize_fail;
  }

  if (!dl_check(dl_vector_resize(&v2, 8, NULL) && dl_vector_capacity(&v2) < original_capacity,
    "Expected dl_vector to resize"))
    goto test_vector_resize_fail;

  new_capacity = dl_vector_capacity(&v2);
  for (idx = 0; idx < new_capacity; ++idx) {
    if (!dl_check(dl_vector_get(&v2, idx, (dl_any)&item) && item == set_data[idx % 7],
      "Expected dl_vector data not to change."))
      goto test_vector_resize_fail;
  }

  dl_destroy_vector(&v2, NULL);

  return true;

test_vector_resize_fail:
  dl_destroy_vector(&v2, NULL);
  return false;
}

dl_bool test_memory_swap() {
  char data_a[64], data_b[64], data_c[64], data_d[64];
  dl_natural idx;

  dl_memory_copy(data_a, "The quicker brown fox jumped over the lazy dog.", 64);
  dl_memory_copy(data_b, "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-", 64);
  dl_memory_copy(data_c, "The quicker brown fox jumped over the lazy dog.", 64);
  dl_memory_copy(data_d, "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-", 64);

  dl_memory_swap(data_a, data_b, sizeof(char) * 47);
  for (idx = 0; idx < 47; ++idx) {
    if (!dl_check(data_a[idx] == data_d[idx],
      "Expected %c to be %c.", data_a[idx], data_d[idx]))
      return false;
    if (!dl_check(data_b[idx] == data_c[idx],
      "Expected %c to be %c.", data_b[idx], data_c[idx]))
      return false;
  }

  for (idx = 0; idx < 47; ++idx)
    dl_memory_swap(data_a + idx, data_b + idx, sizeof(char));

  for (idx = 0; idx < 47; ++idx) {
    if (!dl_check(data_a[idx] == data_c[idx],
      "Expected %c to be %c.", data_a[idx], data_c[idx]))
      return false;
    if (!dl_check(data_b[idx] == data_d[idx],
      "Expected %c to be %c.", data_b[idx], data_d[idx]))
      return false;
  }

  return true;
}

/***************************************
 * Linked Lists
 **************************************/

dl_bool test_init_linked_list() {
  dl_linked_list list;
  dl_natural count;
  struct dl_linked_list_node *node;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!dl_check(dl_vector_capacity(&list.node_cache) == list.settings.cache_length,
    "Expected node cache to be %i in length, was %i.",
    list.settings.cache_length,
    dl_vector_capacity(&list.node_cache)))
    goto fail;

  if (!dl_check(list.free != NULL,
    "Expected free list to exist."))
    goto fail;

  if (!dl_check(list.first == NULL,
    "Expected first to be NULL."))
    goto fail;

  if (!dl_check(list.last == NULL,
    "Expected last to be NULL."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!dl_check(count == dl_vector_capacity(&list.node_cache),
    "Expected list node cache to fill free list, filled %i/%i.",
    count, dl_vector_capacity(&list.node_cache)))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_init_linked_list_fat() {
  dl_linked_list list;
  dl_natural count;
  struct dl_linked_list_node *node;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(fat_data), 32),
    "Expected linked list to initialize."))
    return false;

  if (!dl_check(dl_vector_capacity(&list.node_cache) == list.settings.cache_length,
    "Expected node cache to be %i in length, was %i.",
    list.settings.cache_length,
    dl_vector_capacity(&list.node_cache)))
    goto fail;

  if (!dl_check(list.free != NULL,
    "Expected free list to exist."))
    goto fail;

  if (!dl_check(list.first == NULL,
    "Expected first to be NULL."))
    goto fail;

  if (!dl_check(list.last == NULL,
    "Expected last to be NULL."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!dl_check(count == dl_vector_capacity(&list.node_cache),
    "Expected list node cache to fill free list, filled %i/%i.",
    count, dl_vector_capacity(&list.node_cache)))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_linked_list_add() {
  dl_natural buf[256];
  dl_natural count, current, value;
  dl_linked_list list;
  struct dl_linked_list_node *node;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_natural), 32),
    "Expected linked list to initialize."))
    return false;

  count = 0;
  if (!dl_check(dl_linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!dl_check(dl_linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!dl_check(dl_linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  buf[0] = 0;
  buf[1] = 2;
  buf[2] = 1;

  while (count < 256 && dl_linked_list_add(&list, list.last, &count)) {
    buf[count] = count;
    ++count;
  }

  if (!dl_check(list.free == NULL,
    "Expected free list to be exhausted."))
    goto fail;

  if (!dl_check(list.first != NULL,
    "Expected first node to exist."))
    goto fail;

  if (!dl_check(list.last != NULL,
    "Expected last node to exist."))
    goto fail;

  node = list.first;
  current = 0;
  while (node != NULL) {
    value = *(dl_natural *)LINKED_LIST_DATA(node);
    if (!dl_check(value == buf[current],
      "Expected %i to be %i.", value, buf[current]))
      goto fail;
    node = node->next;
    ++current;
  }

  if (!dl_check(current == count,
    "Expected to find %i items, found %i.", count, current))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_linked_list_add_fat() {
  fat_data count, value;
  dl_linked_list list;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(fat_data), 32),
    "Expected linked list to initialize."))
    return false;

  count.a = 0;
  count.b = 37;
  if (!dl_check(dl_linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  if (!dl_check(dl_linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  if (!dl_check(dl_linked_list_add(&list, list.first, &count),
    "Expected add to work."))
    goto fail;
  ++count.a; ++count.b;

  dl_linked_list_get(&list, dl_linked_list_index(&list, 0), &value);
  if (!dl_check(value.a == 0 && value.b == 37,
	     "Expected {%ul, %ul} to be {0, 37}", value.a, value.b))
    goto fail;

  dl_linked_list_get(&list, dl_linked_list_index(&list, 2), &value);
  if (!dl_check(value.a == 1 && value.b == 38,
	     "Expected {%ul, %ul} to be {1, 38}", value.a, value.b))
    goto fail;

  dl_linked_list_get(&list, dl_linked_list_index(&list, 1), &value);
  if (!dl_check(value.a == 2 && value.b == 39,
	     "Expected {%u, %u} to be {2, 39}", value.a, value.b))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_linked_list_capacity() {
  dl_linked_list list;
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!dl_check(dl_linked_list_capacity(&list) == dl_vector_capacity(&list.node_cache),
    "Expected linked list capacity to be that of its node cache."))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_linked_list_length() {
  dl_linked_list list;
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_natural), 32),
    "Expected linked list to initialize."))
    return false;

  if (!dl_check(dl_linked_list_length(&list) == 0,
    "Expected linked list to be empty."))
    goto fail;

  dl_natural count = 0;
  if (!dl_check(dl_linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!dl_check(dl_linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!dl_check(dl_linked_list_add(&list, NULL, &count),
    "Expected add to work."))
    goto fail;
  ++count;

  if (!dl_check(dl_linked_list_length(&list) == count,
    "Expected length to be %i, was %i.", count, dl_linked_list_length(&list)))
    goto fail;

  dl_destroy_linked_list(&list, NULL);
  return true;
fail:
  dl_destroy_linked_list(&list, NULL);
  return false;
}

dl_bool test_linked_list_copy() {
  dl_linked_list a, b;
  dl_natural num, count, value_a, value_b;
  struct dl_linked_list_node *node_a;
  struct dl_linked_list_node *node_b;
  dl_bool success;

  if (!dl_check(dl_init_linked_list(&a, sizeof(dl_natural), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_init_linked_list(&b, sizeof(dl_natural), 32),
    "Expected init to work.")) {
    dl_destroy_linked_list(&a, NULL);
    return false;
  }

  num = dl_linked_list_capacity(&a) / 4;
  for (count = 0; count < num; ++count)
    if (!dl_check(dl_linked_list_add(&a, NULL, &count),
      "Expected add to work."))
      goto fail;

  if (!dl_check(dl_linked_list_length(&b) == 0,
    "Expected list to be empty."))
    goto fail;

  if (!dl_check(dl_linked_list_copy(&b, NULL, &a),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(dl_linked_list_length(&a) == dl_linked_list_length(&b),
    "Expected lists to be the same length."))
    goto fail;

  node_a = a.first;
  node_b = b.first;
  while (node_a != NULL && node_b != NULL) {
    value_a = *(dl_natural *)LINKED_LIST_DATA(node_a);
    value_b = *(dl_natural *)LINKED_LIST_DATA(node_b);
    if (!dl_check(value_a == value_b,
      "Expected data to be the same, found %i and %i.",
      value_a, value_b))
      goto fail;

    node_a = node_a->next;
    node_b = node_b->next;
  }

  if (!dl_check(dl_linked_list_copy(&b, b.first, &a),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(dl_linked_list_copy(&b, b.last, &a),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(3 * dl_linked_list_length(&a) == dl_linked_list_length(&b),
    "Expected lists to be the triple the length."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&a, NULL);
  dl_destroy_linked_list(&b, NULL);
  return success;
}

dl_bool test_linked_list_copy_array() {
  dl_linked_list a;
  dl_natural num, idx;
  dl_integer value;
  struct dl_linked_list_node *node;
  dl_bool success;

  if (!dl_check(dl_init_linked_list(&a, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  num = dl_linked_list_capacity(&a) / 4;

  if (!dl_check(dl_linked_list_length(&a) == 0,
    "Expected list to be empty."))
    goto fail;

  if (!dl_check(dl_linked_list_copy_array(&a, NULL, (dl_byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(dl_linked_list_length(&a) == num,
    "Expected list to grow in length."))
    goto fail;


  node = a.first;
  for (idx = 0;
    idx < num && node != NULL;
    ++idx, node = node->next) {
    value = *(dl_integer *)LINKED_LIST_DATA(node);
    if (!dl_check(value == _c_data_a[idx],
      "Expected data to be the same, expected %i to be %i.",
      value, _c_data_a[idx]))
      goto fail;
  }

  if (!dl_check(dl_linked_list_copy_array(&a, a.first, (dl_byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(dl_linked_list_copy_array(&a, a.first, (dl_byte *)_c_data_a, num),
    "Expected copy to work."))
    goto fail;

  if (!dl_check(3 * num == dl_linked_list_length(&a),
    "Expected lists to be the triple the length."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&a, NULL);
  return success;
}

dl_bool test_linked_list_grow() {
  dl_linked_list list;
  dl_natural sz, new_sz, count;
  dl_bool success;
  struct dl_linked_list_node *node;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  sz = dl_linked_list_capacity(&list);
  if (!dl_check(dl_linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  new_sz = dl_linked_list_capacity(&list);
  if (!dl_check(sz < new_sz,
    "Expected new size to be larger."))
    goto fail;

  count = 0;
  node = list.free;
  while (node != NULL) {
    node = node->next;
    ++count;
  }

  if (!dl_check(count == new_sz,
    "Expected node cache to be size %i, was %i.", new_sz, count))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_shrink() {
  dl_linked_list list;
  dl_integer count;
  dl_natural length, capacity;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  count = 0;
  while (dl_linked_list_add(&list, NULL, &count))
    ++count;

  length = dl_linked_list_length(&list);
  capacity = dl_linked_list_capacity(&list);

  if (!dl_check(length == (dl_natural)count && length == capacity,
    "Expected list to be exhausted."))
    goto fail;

  if (!dl_check(dl_linked_list_shrink(&list, NULL),
    "Expected shrink to work."))
    goto fail;

  if (!dl_check(length > dl_linked_list_length(&list) && capacity > dl_linked_list_capacity(&list),
    "Expected size to change."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_resize() {
  dl_linked_list list;
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_linked_list_grow(&list),
    "Expected grow to work."))
    goto fail;

  dl_integer count = 0;
  while (dl_linked_list_add(&list, NULL, &count))
    ++count;

  dl_natural length0 = dl_linked_list_length(&list);
  dl_natural capacity0 = dl_linked_list_capacity(&list);

  if (!dl_check(length0 == capacity0,
    "Expected list to be exhausted."))
    goto fail;

  if (!dl_check(dl_linked_list_resize(&list, capacity0 / 2, NULL),
    "Expected resize smaller to work."))
    goto fail;

  dl_natural length1 = dl_linked_list_length(&list);
  dl_natural capacity1 = dl_linked_list_capacity(&list);

  if (!dl_check(length0 > length1 && capacity0 > capacity1,
    "Expected length and capacity to decrease."))
    goto fail;

  if (!dl_check(dl_linked_list_resize(&list, capacity0, NULL),
    "Expected resize larger to work."))
    goto fail;

  dl_natural length2 = dl_linked_list_length(&list);
  dl_natural capacity2 = dl_linked_list_capacity(&list);

  if (!dl_check(capacity2 >= capacity0,
    "Expected capacity to increase."))
    goto fail;

  if (!dl_check(length1 == length2,
    "Expected length not to change."))
    goto fail;

  dl_bool success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_get() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char buf[256];
  dl_linked_list list;
  dl_natural idx;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!dl_check(dl_linked_list_get(&list, list.first, buf),
    "Expected get to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!dl_check(buf[idx] == data[idx],
      "Expected %c to be %c", buf[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_set() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char atad[] = "The quick brown dog jumped over the lazy fox.";
  char buf[256];
  dl_linked_list list;
  dl_natural idx;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!dl_check(dl_linked_list_set(&list, list.first, atad),
    "Expected set to work."))
    goto fail;

  if (!dl_check(dl_linked_list_get(&list, list.first, buf),
    "Expected get to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!dl_check(buf[idx] == atad[idx],
      "Expected %c to be %c", buf[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_ref() {
  char data[] = "The quick brown fox jumped over the lazy dog.";
  char *ref = NULL;
  dl_linked_list list;
  dl_natural idx;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(data), 32),
    "Expected init to work."))
    return false;

  if (!dl_check(dl_linked_list_add(&list, NULL, data),
    "Expected add to work."))
    goto fail;

  if (!dl_check((ref = dl_linked_list_ref(list.first)),
    "Expected ref to work."))
    goto fail;

  for (idx = 0; idx < sizeof(data); ++idx)
    if (!dl_check(ref[idx] == data[idx],
      "Expected %c to be %c", ref[idx], data[idx]))
      goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_index() {
  dl_linked_list list;
  dl_integer count, found;
  dl_natural idx;
  struct dl_linked_list_node *node;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (dl_linked_list_add(&list, list.last, &count))
    ++count;

  for (idx = 0; idx < dl_linked_list_length(&list); ++idx) {
    if (!dl_check((node = dl_linked_list_index(&list, idx)),
      "Expected index to work for %i.", idx))
      goto fail;

    if (!dl_check(dl_linked_list_get(&list, node, &found),
      "Expected get to work."))
      goto fail;

    if (!dl_check(found == (dl_integer)idx,
      "Expected %i to be %i.", found, (dl_integer)idx))
      goto fail;
  }

  if (!dl_check(!dl_linked_list_index(&list, DL_NATURAL_MAX),
    "Expected index to fail."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_destroy_range() {
  dl_linked_list list;
  dl_integer count;
  dl_natural length;
  struct dl_linked_list_node *node;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (dl_linked_list_add(&list, NULL, &count))
    ++count;

  length = dl_linked_list_length(&list);
  node = dl_linked_list_index(&list, length / 4);

  if (!dl_check(dl_linked_list_destroy_range(&list, node, length / 4, NULL) == length / 4,
    "Expected destroy range to work."))
    goto fail;

  if (!dl_check(dl_linked_list_length(&list) == length / 4 * 3,
    "Expected destroy range to remove items."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

dl_bool test_linked_list_swap() {
  dl_linked_list list;
  dl_integer count, value0, value1;
  dl_natural length;
  struct dl_linked_list_node *node0, *node1, *next0, *next1, *prev0, *prev1;
  dl_bool success;
  
  if (!dl_check(dl_init_linked_list(&list, sizeof(dl_integer), 32),
    "Expected init to work."))
    return false;

  count = 0;
  while (dl_linked_list_add(&list, NULL, &count))
    ++count;

  length = dl_linked_list_length(&list);
  node0 = dl_linked_list_index(&list, 0);
  node1 = dl_linked_list_index(&list, length / 4);
  next0 = node0->next;
  next1 = node1->next;
  prev0 = node0->previous;
  prev1 = node1->previous;

  dl_linked_list_get(&list, node0, &value0);
  dl_linked_list_get(&list, node1, &value1);

  if (!dl_check(dl_linked_list_swap(&list, node0, node1, false),
    "Expected swap to work."))
    goto fail;

  if (!dl_check(value0 == *(dl_integer *)dl_linked_list_ref(node0),
    "Expected data not to move."))
    goto fail;

  if (!dl_check(value1 == *(dl_integer *)dl_linked_list_ref(node1),
    "Expected data not to move."))
    goto fail;

  if (!dl_check(next1 == node0->next && next0 == node1->next,
    "Expected next to change."))
    goto fail;

  if (!dl_check(prev1 == node0->previous && prev0 == node1->previous,
    "Expected previous to change."))
    goto fail;

  if (!dl_check(dl_linked_list_swap(&list, node0, node1, true),
    "Expected swap to work."))
    goto fail;

  if (!dl_check(value1 == *(dl_integer *)dl_linked_list_ref(node0),
    "Expected data to move."))
    goto fail;

  if (!dl_check(value0 == *(dl_integer *)dl_linked_list_ref(node1),
    "Expected data to move."))
    goto fail;

  success = true;
  goto finish;
fail:
  success = false;;
finish:
  dl_destroy_linked_list(&list, NULL);
  return success;
}

#endif
