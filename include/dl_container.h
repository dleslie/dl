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
  DL_CONTAINER_TRAIT_NONE = 0,
  DL_CONTAINER_TRAIT_RANDOM_ACCESS = 1,
  DL_CONTAINER_TRAIT_RANDOM_INSERT = 2,
  DL_CONTAINER_TRAIT_SET = 4,
  DL_CONTAINER_TRAIT_SORTED = 8
} dl_container_trait;

struct dl_container_interface;
typedef struct {
  struct dl_container_interface *interface;
  dl_ptr storage;
} dl_container;

typedef struct {
  dl_container *container;
  union {
    dl_integer index;
    dl_ptr node;
  } data;
} dl_iterator;

struct dl_container_interface {
  dl_ptr (*create)(dl_natural element_size, dl_natural capacity, dl_ptr extra_data);
  void (*destroy)(dl_ptr c);
  dl_natural (*traits)(dl_ptr c);
  dl_natural (*length)(dl_ptr c);
  dl_bool (*is_empty)(dl_ptr c);
  dl_natural (*element_size)(dl_ptr c);
  dl_iterator (*index)(dl_ptr c, dl_natural i);
  dl_iterator (*first)(dl_ptr c);
  dl_iterator (*last)(dl_ptr c);
  dl_ptr (*push)(dl_ptr c, dl_ptr v);
  dl_ptr (*pop)(dl_ptr c, dl_ptr out);

  dl_bool (*iterator_valid)(dl_iterator i);
  dl_ptr (*iterator_get)(dl_iterator i, dl_ptr out);
  dl_ptr (*iterator_ref)(dl_iterator i);
  dl_ptr (*iterator_set)(dl_iterator i, dl_ptr v);
  dl_bool (*iterator_swap)(dl_iterator a, dl_iterator b);
  dl_iterator (*iterator_insert)(dl_iterator pos, dl_ptr value);
  dl_bool (*iterator_remove)(dl_iterator pos);
  dl_iterator (*iterator_next)(dl_iterator i);
  dl_iterator (*iterator_prev)(dl_iterator i);
  dl_integer (*iterator_compare)(dl_iterator a, dl_iterator b);
  dl_integer (*iterator_index)(dl_iterator i);
};
typedef struct dl_container_interface dl_container_interface;

dl_api dl_container *dl_init_container(dl_container *target, struct dl_container_interface *interface, dl_natural element_size, dl_natural capacity, dl_ptr extra_data);
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

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)

/*******************************************************************************
 ** Generic Interface
 ******************************************************************************/

dl_api dl_container *dl_init_container(dl_container *target, struct dl_container_interface *interface, dl_natural element_size, dl_natural capacity, dl_ptr extra_data) {
  if (dl_safety(target == NULL || element_size == 0 || capacity == 0 || interface == NULL)) return NULL;

  target->interface = interface;
  target->storage = target->interface->create(element_size, capacity, extra_data);

  if (dl_unlikely(target->storage == NULL))
    return NULL;
  return target;
}

dl_api void dl_destroy_container(dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return;

  target->interface->destroy(target->storage);
  target->storage = NULL;
}

dl_api dl_natural dl_container_length(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return -1;

  return target->interface->length(target->storage);
}

dl_api dl_bool dl_container_is_empty(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return false;

  return target->interface->is_empty(target->storage);
}

dl_api dl_natural dl_container_element_size(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return 0;

  return target->interface->element_size(target->storage);
}

dl_api dl_iterator dl_container_index(const dl_container *target, dl_natural position) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return dl_make_invalid_iterator();

  return target->interface->index(target->storage, position);
}

dl_api dl_iterator dl_container_first(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return dl_make_invalid_iterator();

  return target->interface->first(target->storage);
}

dl_api dl_iterator dl_container_last(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return dl_make_invalid_iterator();

  return target->interface->last(target->storage);
}

dl_api dl_ptr dl_container_push(dl_container *target, dl_ptr value) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return NULL;

  return target->interface->push(target->storage, value);
}

dl_api dl_ptr dl_container_pop(dl_container *target, dl_ptr out) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return NULL;

  return target->interface->pop(target->storage, out);
}

dl_api dl_natural dl_container_traits(const dl_container *target) {
  if (dl_safety(target == NULL || target->storage == NULL || target->interface == NULL)) return 0;

  return target->interface->traits(target->storage);
}

/*******************************************************************************
 ** Iterator Interface
 ******************************************************************************/
dl_api dl_iterator dl_make_invalid_iterator() {
  dl_iterator iter;
  iter.container = NULL;
  return iter;
}

dl_api dl_bool dl_iterator_is_valid(dl_iterator iter) {
  return iter.container != NULL && iter.container->interface != NULL && iter.container->storage != NULL && dl_safety(iter.container->interface->iterator_valid(iter));
}

dl_api dl_ptr dl_iterator_get(const dl_iterator target, dl_ptr out) {
  if (dl_safety(!dl_iterator_is_valid(target) || out == NULL))
    return NULL;

  return target.container->interface->iterator_get(target, out);
}

dl_api dl_ptr dl_iterator_ref(dl_iterator target) {
  if (dl_safety(!dl_iterator_is_valid(target)))
    return NULL;

  return target.container->interface->iterator_ref(target);
}

dl_api dl_ptr dl_iterator_set(dl_iterator target, dl_ptr value) {
  if (dl_safety(!dl_iterator_is_valid(target) || value == NULL))
    return NULL;

  return target.container->interface->iterator_set(target, value);
}

dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2) {
  if (dl_safety(!dl_iterator_is_valid(position1) || !dl_iterator_is_valid(position2) || position1.container != position2.container))
    return false;

  return position1.container->interface->iterator_swap(position1, position2);
}

dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_ptr value) {
  if (dl_safety(!dl_iterator_is_valid(position) || value == NULL))
    return dl_make_invalid_iterator();

  return position.container->interface->iterator_insert(position, value);
}

dl_api dl_bool dl_iterator_remove(dl_iterator position) {
  if (dl_safety(!dl_iterator_is_valid(position)))
    return false;

  return position.container->interface->iterator_remove(position);
}

dl_api dl_iterator dl_iterator_next(dl_iterator target) {
  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  return target.container->interface->iterator_next(target);
}

dl_api dl_iterator dl_iterator_prev(dl_iterator target) {
  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  return target.container->interface->iterator_prev(target);
}

dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right) {
  if (dl_safety(!dl_iterator_is_valid(left) || !dl_iterator_is_valid(right) || left.container != right.container))
    return -1;

  return left.container->interface->iterator_compare(left, right);
}

dl_api dl_integer dl_iterator_index(dl_iterator iter) {
  if (dl_safety(!dl_iterator_is_valid(iter)))
    return -1;

  return iter.container->interface->iterator_index(iter);
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
  return 0 == dl_iterator_compare(left, right);
}

#endif /* DL_IMPLEMENTATION */

#endif
