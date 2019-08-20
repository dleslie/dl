#ifndef DL_VECTOR_CONTAINER_H
#define DL_VECTOR_CONTAINER_H

#include "dl_container.h"

#ifdef __cplusplus
extern "C" {
#endif

dl_api dl_container *dl_make_vector_container(dl_natural element_size, dl_natural capacity);
extern dl_container_interface dl_vector_container_interface;

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)
#include "dl_vector.h"

dl_container *dl_make_vector_container(dl_natural element_size, dl_natural capacity) {
  return dl_make_container(&dl_vector_container_interface, element_size, capacity);
}

dl_ptr _make_vector_container(dl_natural element_size, dl_natural capacity) {
  return dl_make_vector(element_size, capacity);
}

void _destroy_vector_container(dl_ptr c) {
  if (dl_safety(c == NULL || c->storage == NULL))
    return;
  dl_destroy_vector((dl_vector *)((dl_container *)c)->storage);
}

dl_natural _vector_container_traits(dl_ptr c) {
  return DL_CONTAINER_TRAIT_RANDOM_ACCESS;
}

dl_natural _vector_container_length(dl_ptr c) {
  if (dl_safety(c == NULL || c->storage == NULL))
    return 0;
  return dl_vector_length((dl_vector *)((dl_container *)c)->storage);
}

dl_bool _vector_container_is_empty(dl_ptr c) {
  if (dl_safety(c == NULL || c->storage == NULL))
    return 0;
  return dl_vector_is_empty((dl_vector *)((dl_container *)c)->storage);
}

dl_natural _vector_container_element_size(dl_ptr c) {
  if (dl_safety(c == NULL || c->storage == NULL))
    return 0;
  return ((dl_vector *)((dl_container *)c)->storage)->element_size;
}

dl_iterator _vector_container_index(dl_ptr c, dl_natural i) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.index = i;
  return iter;
}

dl_iterator _vector_container_first(dl_ptr c) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.index = 0;
  return iter;
}

dl_iterator _vector_container_last(dl_ptr c) {
  dl_iterator iter;

  if (dl_safety(c == NULL || c->storage == null))
    return dl_make_invalid_iterator();

  iter.container = (dl_container *)c;
  iter.data.index = dl_vector_length((dl_vector *)((dl_container *)c)->storage) - 1;
  return iter;
}

dl_ptr _vector_container_push(dl_ptr c, dl_ptr v) {
  if (dl_safety(c == NULL || c->storage == null))
    return NULL;

  return dl_vector_push((dl_vector *)((dl_container *)c)->storage, v);
}

dl_ptr _vector_container_pop(dl_ptr c, dl_ptr out) {
  if (dl_safety(c == NULL || c->storage == null))
    return NULL;

  return dl_vector_pop((dl_vector *)((dl_container *)c)->storage, out);
}

dl_bool _vector_container_iterator_valid(dl_iterator i) {
  return i.container != NULL && i.container->storage != NULL && i.data.index >= 0 && i.data.index < dl_vector_length((dl_vector *)((dl_container *)i.container)->storage);
}

dl_ptr _vector_container_iterator_get(dl_iterator i, dl_ptr out) {
  if (dl_safety(!dl_iterator_is_valid(i) || out == NULL))
    return NULL;
  return dl_vector_get((dl_vector *)i.container->storage, i.data.index, out);
}

dl_ptr _vector_container_iterator_ref(dl_iterator i) {
  if (dl_safety(!dl_iterator_is_valid(i)\))
    return NULL;
  return dl_vector_ref((dl_vector *)i.container->storage, i.data.index);
}

dl_ptr _vector_container_iterator_set(dl_iterator i, dl_ptr v) {
  if (dl_safety(!dl_iterator_is_valid(i) || v == NULL))
    return NULL;
  return dl_vector_set((dl_vector *)i.container->storage, i.data.index, v);
}

dl_bool _vector_container_iterator_swap(dl_iterator a, dl_iterator b) {
  if (dl_safety(!dl_iterator_is_valid(a) || !dl_iterator_is_valid(b) || a.container != b.container))
    return NULL;
  return dl_vector_swap((dl_vector *)a.container, a.data.index, b.data.index);
}

dl_iterator _vector_container_iterator_insert(dl_iterator i, dl_ptr value) {
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(i) || value == NULL))
    return dl_make_invalid_iterator();

  iter.container = i.container;
  iter.data.index = dl_vector_insert((dl_vector *)i.container->storage, i.data.index, value);

  return iter;
}

dl_bool _vector_container_iterator_remove(dl_iterator pos) {
}

dl_iterator _vector_container_iterator_next(dl_iterator i) {
}

dl_iterator _vector_container_iterator_prev(dl_iterator i) {
}

dl_integer _vector_container_iterator_compare(dl_iterator a, dl_iterator b) {
}

dl_integer _vector_container_iterator_index(dl_iterator i) {
}

dl_container_interface dl_vector_container_interface = {
  _make_vector_container,
  _destroy_vector_container,
  _vector_container_traits,
  _vector_container_length,
  _vector_container_is_empty,
  _vector_container_element_size,
  _vector_container_index,
  _vector_container_first,
  _vector_container_last,
  _vector_container_push,
  _vector_container_pop,
  _vector_container_iterator_valid,
  _vector_container_iterator_get,
  _vector_container_iterator_ref,
  _vector_container_iterator_set,
  _vector_container_iterator_swap,
  _vector_container_iterator_insert,
  _vector_container_iterator_remove,
  _vector_container_iterator_next,
  _vector_container_iterator_prev,
  _vector_container_iterator_compare,
  _vector_container_iterator_index};

#endif /* DL_IMPLEMENTATION */
#endif /* DL_VECTOR_CONTAINER_H */