#ifndef DL_ARRAY_CONTAINER_H
#define DL_ARRAY_CONTAINER_H

#include "dl_container.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  dl_natural element_size;
  dl_natural capacity;
  dl_natural end;
  dl_ptr data;
} dl_array_info;

dl_api dl_array_info *dl_init_array_info(dl_array_info *ary_info, dl_natural element_size, dl_natural capacity, dl_ptr data);
dl_api dl_container *dl_init_array_container(dl_container *container, dl_array_info *ary_info);
extern dl_container_interface dl_array_container_interface;

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)
dl_array_info *dl_init_array_info(dl_array_info *ary_info, dl_natural element_size, dl_natural capacity, dl_ptr data) {
  if (dl_safety(element_size == 0 || ary_info == NULL || data == NULL))
    return NULL;

  ary_info->end = 0;
  ary_info->capacity = capacity;
  ary_info->element_size = element_size;
  ary_info->data = data;

  return ary_info;
}

dl_container *dl_init_array_container(dl_container *container, dl_array_info *ary_info) {
  if (dl_safety(ary_info == NULL || container == NULL || ary == NULL))
    return NULL;

  container->interface = &dl_array_container_interface;
  container->storage = ary_info;

  return container;
}

dl_container *_make_array_container(dl_natural element_size, dl_natural capacity) {
  return NULL;
}

void _destroy_array_container(dl_container *c) {
  dl_unused(c);
}

dl_natural _array_container_length(dl_container *c) {
  return ((dl_array_info *)c->storage)->end;
}

dl_bool _array_container_is_empty(dl_container *c) {
  return ((dl_array_info *)c->storage)->end == 0;
}

dl_natural _array_container_element_size(dl_container *c) {
  return ((dl_array_info *)c->storage)->element_size;
}

dl_iterator _array_container_index(dl_container *c, dl_natural i) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.index = i;
  return iter;
}

dl_iterator _array_container_first(dl_container *c) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.index = 0;
  return iter;
}

dl_iterator _array_container_last(dl_container *c) {
  dl_iterator iter;
  iter.container = c;
  iter.data.index = ((dl_array_info *)c->storage)->capacity - 1;
  return iter;
}

dl_iterator _array_container_iterator_next(dl_iterator i) {
  i.data.index++;
  if (i.data.index >= ((dl_array_info *)i.container->storage)->capacity)
    i.container = NULL;
  return i;
}

dl_iterator _array_container_iterator_prev(dl_iterator i) {
  i.data.index--;
  if (i.data.index < 0)
    i.container = NULL;
  return i;
}

dl_integer _array_container_iterator_compare(dl_iterator a, dl_iterator b) {
  return a.data.index - b.data.index;
}

dl_integer _array_container_iterator_index(dl_iterator i) {
  return i.data.index;
}

dl_bool _array_container_iterator_valid(dl_iterator i) {
  return i.data.index >= 0 && !dl_safety(v == NULL) && i.data.index < ((dl_array_info *)i.container->storage)->capacity;
}

dl_ptr _array_container_iterator_get(dl_iterator i, dl_ptr out) {
}

dl_ptr _array_container_iterator_ref(dl_iterator i) {
}

dl_ptr _array_container_iterator_set(dl_iterator i, dl_ptr v) {
}

dl_ptr _array_container_push(dl_container *c, dl_ptr v) {
}

dl_ptr _array_container_pop(dl_container *c, dl_ptr out) {
}

dl_bool _array_container_iterator_swap(dl_iterator a, dl_iterator b) {
}

dl_iterator _array_container_iterator_insert(dl_iterator i, dl_ptr value) {
}

dl_bool _array_container_iterator_remove(dl_iterator pos) {
}

dl_container_interface dl_array_container_interface = {
  DL_CONTAINER_TRAIT_RANDOM_ACCESS,
  _make_array_container,
  _destroy_array_container,
  _array_container_length,
  _array_container_is_empty,
  _array_container_element_size,
  _array_container_index,
  _array_container_first,
  _array_container_last,
  _array_container_push,
  _array_container_pop,
  _array_container_iterator_valid,
  _array_container_iterator_get,
  _array_container_iterator_ref,
  _array_container_iterator_set,
  _array_container_iterator_swap,
  _array_container_iterator_insert,
  _array_container_iterator_remove,
  _array_container_iterator_next,
  _array_container_iterator_prev,
  _array_container_iterator_compare,
  _array_container_iterator_index};

#endif /* DL_IMPLEMENTATION */
#endif /* DL_ARRAY_CONTAINER_H */
