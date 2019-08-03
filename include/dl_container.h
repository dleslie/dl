/*
 * This entire file is a testament to the travesty of C's limitations.
 * What is being done with switch statements at run time could be better done
 * with static analysis at compile time. Alternativey, a structure with function
 * pointers could be used, but the differences in behaviour at runtime are
 * negligible; while at compile-time there is greater ambiguity. It would be
 * better if traits/generics were available to C, so this logic could be done at
 * compile time.
 *
 */

#ifndef DL_CONTAINER_H
#define DL_CONTAINER_H 1

#include "dl_core.h"
#include "dl_linked_list.h"
#include "dl_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 **  Generic Interface
 ****************************************************************************/

typedef enum {
  DL_CONTAINER_TYPE_NONE = 0,
  DL_CONTAINER_TYPE_VECTOR,
  DL_CONTAINER_TYPE_LINKED_LIST
} dl_container_type;

typedef enum {
  DL_CONTAINER_TRAIT_NONE = 0,
  DL_CONTAINER_TRAIT_RANDOM_ACCESS = 1,
  DL_CONTAINER_TRAIT_RANDOM_MUTATE = 2,
  DL_CONTAINER_TRAIT_SET = 4,
  DL_CONTAINER_TRAIT_SORTED = 8
} dl_container_trait;

typedef struct {
  dl_container_type type;
  dl_ptr storage;
} dl_container;

typedef struct {
  dl_container *container;
  union {
    dl_integer index;
    dl_ptr node;
  } data;
} dl_iterator;

dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity);
dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count);
dl_api void dl_destroy_container(dl_container *target);

dl_api dl_natural dl_container_element_size(const dl_container *container);
dl_api dl_natural dl_container_length(const dl_container *container);
dl_api dl_bool dl_container_is_empty(const dl_container *container);

dl_api dl_iterator dl_container_index(const dl_container *container, dl_natural position);
dl_api dl_iterator dl_container_first(const dl_container *target);
dl_api dl_iterator dl_container_last(const dl_container *target);

dl_api dl_ptr dl_container_push(dl_container *v, dl_ptr value);
dl_api dl_ptr dl_container_pop(dl_container *v, dl_ptr out);

dl_api dl_natural dl_container_traits(const dl_container *v);

/*****************************************************************************
 **  Iterator Interface
 ****************************************************************************/

dl_api dl_iterator dl_make_invalid_iterator();

dl_api dl_bool dl_iterator_is_valid(dl_iterator iter);

dl_api dl_natural dl_iterator_copy(dl_iterator target, const dl_iterator original, dl_natural count);

dl_api dl_ptr dl_iterator_get(const dl_iterator position, dl_ptr out);
dl_api dl_ptr dl_iterator_ref(dl_iterator position);
dl_api dl_ptr dl_iterator_set(dl_iterator position, dl_ptr value);

dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2);

dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_ptr value);
dl_api dl_bool dl_iterator_remove(dl_iterator position);

dl_api dl_iterator dl_iterator_next(dl_iterator target);
dl_api dl_iterator dl_iterator_prev(dl_iterator target);
dl_api dl_iterator dl_iterator_next_ref(dl_iterator target, dl_ptr *out);
dl_api dl_iterator dl_iterator_prev_ref(dl_iterator target, dl_ptr *out);

dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right);
dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right);
dl_api dl_integer dl_iterator_index(dl_iterator iter);

dl_api dl_container *dl_iterator_container(dl_iterator target);

#ifdef __cplusplus
}
#endif

#if DL_IMPLEMENTATION

/*******************************************************************************
 ** Generic Interface
 ******************************************************************************/

dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity) {
  if (dl_safety(target == NULL || element_size == 0 || capacity == 0)) return NULL;

  target->type = type;
  switch (type) {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      if (NULL == dl_init_linked_list((dl_linked_list *)target->storage, element_size, capacity)) return NULL;
      return target;
    case DL_CONTAINER_TYPE_VECTOR:
      if (NULL == dl_init_vector((dl_vector *)target->storage, element_size, capacity)) return NULL;
      return target;
  }
}

dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count) {
  if (dl_safety(target == NULL || element_size == 0 || count == 0 || array_data == NULL)) return NULL;

  target->type = DL_CONTAINER_TYPE_VECTOR;
  if (NULL == dl_init_vector_array((dl_vector *)target->storage, array_data, element_size, count)) return NULL;

  return target;
}

dl_api void dl_destroy_container(dl_container *target) {
  if (dl_safety(target == NULL)) return;

  switch (target->type) {
    default:
      break;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      dl_destroy_linked_list((dl_linked_list *)target->storage);
      break;
    case DL_CONTAINER_TYPE_VECTOR:
      dl_destroy_vector((dl_vector *)target->storage);
      break;
  }

  target->type = DL_CONTAINER_TYPE_NONE;
}

dl_api dl_natural dl_container_length(const dl_container *target) {
  if (dl_safety(target == NULL)) return 0;

  switch (target->type) {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_length((dl_linked_list *)target->storage);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_capacity((dl_vector *)target->storage);
  }
}

dl_api dl_bool dl_container_is_empty(const dl_container *target) {
  if (dl_safety(target == NULL)) return true;

  switch (target->type) {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_is_empty((dl_linked_list *)target->storage);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_is_empty((dl_vector *)target->storage);
  }
}

dl_api dl_natural dl_container_element_size(const dl_container *target) {
  if (dl_safety(target == NULL)) return 0;

  switch (target->type) {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return ((dl_linked_list *)target->storage)->element_size;
    case DL_CONTAINER_TYPE_VECTOR:
      return ((dl_vector *)target->storage)->element_size;
  }
}

dl_api dl_iterator dl_container_index(const dl_container *target, dl_natural position) {
  dl_iterator iter;

  if (dl_safety(target == NULL)) return dl_make_invalid_iterator();

  switch (target->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = (dl_container *)target;
      iter.data.node = dl_linked_list_index((dl_linked_list *)target->storage, position);
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_safety(position >= dl_vector_capacity((dl_vector *)target->storage))) return dl_make_invalid_iterator();
      iter.container = (dl_container *)target;
      iter.data.index = position;
      return iter;
  }
}

dl_api dl_iterator dl_container_first(const dl_container *target) {
  dl_iterator iter;

  if (dl_safety(target == NULL)) return dl_make_invalid_iterator();

  switch (target->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = (dl_container *)target;
      iter.data.node = ((dl_linked_list *)target->storage)->first;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_safety(((dl_vector *)target->storage)->array == NULL)) return dl_make_invalid_iterator();
      iter.container = (dl_container *)target;
      iter.data.index = 0;
      return iter;
  }
}

dl_api dl_iterator dl_container_last(const dl_container *target) {
  dl_iterator iter;

  if (dl_safety(target == NULL)) return dl_make_invalid_iterator();

  switch (target->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = (dl_container *)target;
      iter.data.node = ((dl_linked_list *)target->storage)->last;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_safety(((dl_vector *)target->storage)->array == NULL)) return dl_make_invalid_iterator();
      iter.container = (dl_container *)target;
      iter.data.index = ((dl_vector *)target->storage)->capacity - 1;
      return iter;
  }
}

dl_api dl_ptr dl_container_push(dl_container *target, dl_ptr value) {
  if (dl_safety(target == NULL)) return false;

  switch (target->type) {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_push((dl_linked_list *)target->storage, value);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_push((dl_vector *)target->storage, value);
  }
}

dl_api dl_ptr dl_container_pop(dl_container *target, dl_ptr out) {
  if (dl_safety(target == NULL)) return false;

  switch (target->type) {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_pop((dl_linked_list *)target->storage, out);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_pop((dl_vector *)target->storage, out);
  }
}

dl_api dl_natural dl_container_traits(const dl_container *c) {
  dl_natural traits;

  traits = 0;

  switch (c->type) {
    default:
      break;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      traits |= DL_CONTAINER_TRAIT_RANDOM_MUTATE;
    case DL_CONTAINER_TYPE_VECTOR:
      traits |= DL_CONTAINER_TRAIT_RANDOM_ACCESS;
      break;
  }

  return traits;
}

/*******************************************************************************
 ** Iterator Interface
 ******************************************************************************/
dl_api dl_container *dl_iterator_container(dl_iterator target) {
  return target.container;
}

dl_api dl_iterator dl_make_invalid_iterator() {
  dl_iterator iter;
  iter.container = NULL;
  iter.data.index = 0;
  iter.data.node = NULL;
  return iter;
}

dl_api dl_bool dl_iterator_is_valid(dl_iterator iter) {
  return iter.container != NULL && ((iter.container->type == DL_CONTAINER_TYPE_LINKED_LIST && iter.data.node != NULL) || (iter.container->type == DL_CONTAINER_TYPE_VECTOR && iter.data.index >= 0 && iter.data.index < dl_vector_capacity((dl_vector *)iter.container->storage)));
}

dl_api dl_ptr dl_iterator_get(const dl_iterator target, dl_ptr out) {
  if (dl_safety(!dl_iterator_is_valid(target) || out != NULL))
    return NULL;

  switch (target.container->type) {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_get((dl_linked_list *)target.container, target.data.node, out);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_get((dl_vector *)target.container, target.data.index, out);
  }
}

dl_api dl_ptr dl_iterator_ref(dl_iterator target) {
  if (dl_safety(!dl_iterator_is_valid(target)))
    return NULL;

  switch (target.container->type) {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_ref((dl_linked_list *)target.container, target.data.node);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_ref((dl_vector *)target.container, target.data.index);
  }
}

dl_api dl_ptr dl_iterator_set(dl_iterator target, dl_ptr value) {
  if (dl_safety(!dl_iterator_is_valid(target) || value == NULL))
    return NULL;

  switch (target.container->type) {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_set((dl_linked_list *)target.container, target.data.node, value);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_set((dl_vector *)target.container, target.data.index, value);
  }
}

dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2) {
  if (dl_safety(!dl_iterator_is_valid(position1) || !dl_iterator_is_valid(position2) || position2.container != position1.container))
    return false;

  switch (position1.container->type) {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_swap((dl_linked_list *)position1.container, position1.data.node, position2.data.node);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_swap((dl_vector *)position1.container, position1.data.index, position2.data.index);
  }
}

dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_ptr value) {
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(position)) || value == NULL)
    return dl_make_invalid_iterator();

  switch (position.container->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = position.container;
      iter.data.node = dl_linked_list_insert((dl_linked_list *)position.container, position.data.node, value);
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      iter.container = position.container;
      iter.data.index = dl_vector_insert((dl_vector *)position.container, position.data.index, value);
      return iter;
  }
}

dl_api dl_bool dl_iterator_remove(dl_iterator position) {
  if (dl_safety(!dl_iterator_is_valid(position)))
    return false;

  switch (position.container->type) {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_remove((dl_linked_list *)position.container, position.data.node);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_remove((dl_vector *)position.container, position.data.index);
  }
}

dl_api dl_iterator dl_iterator_next(dl_iterator target) {
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  switch (target.container->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = target.container;
      iter.data.node = target.data.node == NULL ? NULL : ((dl_linked_list_node *)target.data.node)->next;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      iter.container = target.container;
      iter.data.index = target.data.index + 1;
      return iter;
  }
}

dl_api dl_iterator dl_iterator_prev(dl_iterator target) {
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  switch (target.container->type) {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = target.container;
      iter.data.node = target.data.node == NULL ? NULL : ((dl_linked_list_node *)target.data.node)->previous;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      iter.container = target.container;
      iter.data.index = target.data.index - 1;
      return iter;
  }
}

dl_api dl_iterator dl_iterator_next_ref(dl_iterator iter, dl_ptr *out) {
  dl_iterator next;
  next = dl_iterator_next(iter);
  *out = dl_iterator_ref(next);
  return next;
}

dl_api dl_iterator dl_iterator_prev_ref(dl_iterator iter, dl_ptr *out) {
  dl_iterator prev;
  prev = dl_iterator_prev(iter);
  *out = dl_iterator_ref(prev);
  return prev;
}

dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right) {
  dl_bool leftValid, rightValid;

  leftValid = dl_iterator_is_valid(left);
  rightValid = dl_iterator_is_valid(right);

  return (leftValid == rightValid) && (!leftValid || (left.container == right.container && ((left.container->type = DL_CONTAINER_TYPE_LINKED_LIST && left.data.node == right.data.node) || (left.container->type = DL_CONTAINER_TYPE_VECTOR && left.data.index == right.data.index))));
}

dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right) {
  dl_bool leftValid, rightValid;

  leftValid = dl_iterator_is_valid(left);
  rightValid = dl_iterator_is_valid(right);

  if (!leftValid && !rightValid)
    return 0;
  if (leftValid && !rightValid)
    return -1;
  if (!leftValid && rightValid)
    return 1;
  if (dl_safety(left.container != right.container))
    return 0;
  return dl_iterator_index(left) - dl_iterator_index(right);
}

dl_api dl_integer dl_iterator_index(dl_iterator iter) {
  if (dl_safety(!dl_iterator_is_valid(iter)))
    return -1;

  switch (iter.container->type) {
    default:
      return -1;
    case DL_CONTAINER_TYPE_VECTOR:
      return iter.data.index;
    case DL_CONTAINER_TYPE_LINKED_LIST: {
      dl_integer index = 0;
      dl_iterator cur;
      for (cur = dl_container_first(iter.container);
           dl_iterator_is_valid(cur) && !dl_iterator_equal(cur, iter);
           cur = dl_iterator_next(iter))
        ++index;
      if (!dl_iterator_is_valid(cur))
        return -1;
      return index;
    }
  }
}

#endif /* DL_IMPLEMENTATION */

#endif
