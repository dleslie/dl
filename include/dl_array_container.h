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

#define dl_define_array(name, capacity, type) \
  type name__data[capacity];                  \
  dl_array_info name__array_info;             \
  dl_container name;

#define dl_init_array(name, capacity, type)                                   \
  dl_init_array_info(&name__array_info, sizeof(type), capacity, &name__data); \
  dl_init_array_container(&name, &name__array_info);

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)
#include "dl_memory.h"

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
  dl_unused(element_size);
  dl_unused(capacity);
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
  iter.data.index = ((dl_array_info *)c->storage)->end - 1;
  return iter;
}

dl_iterator _array_container_iterator_next(dl_iterator i) {
  i.data.index++;
  if (i.data.index >= ((dl_array_info *)i.container->storage)->end)
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
  return i.data.index >= 0 && !dl_safety(v == NULL) && i.data.index < ((dl_array_info *)i.container->storage)->end;
}

dl_ptr _array_container_iterator_get(dl_iterator i, dl_ptr out) {
  dl_array_info *ary = (dl_array_info *)i.container->storage;
  return dl_memory_copy(out, &ary->data[i.data.index * ary->element_size], ary->element_size);
}

dl_ptr _array_container_iterator_ref(dl_iterator i) {
  dl_array_info *ary = (dl_array_info *)i.container->storage;
  return &ary->data[i.data.index * ary->element_size];
}

dl_ptr _array_container_iterator_set(dl_iterator i, dl_ptr v) {
  dl_array_info *ary = (dl_array_info *)i.container->storage;
  return dl_memory_copy(&ary->data[i.data.index * ary->element_size], v, ary->element_size);
}

dl_bool _array_container_iterator_swap(dl_iterator a, dl_iterator b) {
  dl_array_info *ary_a = (dl_array_info *)a.container->storage;
  dl_array_info *ary_b = (dl_array_info *)b.container->storage;
  dl_natural sz = ary_a->element_size;
  return NULL != dl_memory_swap(&ary_a[a.data.index * sz], &ary_b[b.data.index * sz], sz);
}

dl_ptr _array_container_push(dl_container *c, dl_ptr v) {
  dl_array_info *ary = (dl_array_info *)c->storage;
  dl_natural idx = ary->end;
  dl_natural sz = ary->element_size;

  if (ary->end >= ary->capacity)
    return NULL;

  ary->end++;
  return dl_memory_copy(&ary->data[idx * sz], v, sz);
}

dl_ptr _array_container_pop(dl_container *c, dl_ptr out) {
  dl_array_info *ary = (dl_array_info *)c->storage;
  dl_natural idx = ary->end;
  dl_natural sz = ary->element_size;

  if (ary->end == 0)
    return NULL;

  ary->end = idx - 1;
  return dl_memory_copy(out, &ary->data[idx * sz], sz);
}

dl_bool _array_container_iterator_remove(dl_iterator pos) {
  dl_array_info *ary = (dl_array_info *)pos.container->storage;
  dl_natural idx = pos.data.index;
  dl_natural sz = ary->element_size;

  if (ary->end == 0)
    return NULL;

  for (; idx < ary->end; ++idx)
    dl_memory_copy(&ary->data[idx * sz], &ary->data[(idx + 1) * sz], sz);

  ary->end--;

  return true;
}

dl_iterator _array_container_iterator_insert(dl_iterator pos, dl_ptr value) {
  dl_array_info *ary = (dl_array_info *)pos.container->storage;
  dl_natural idx = ary->end;
  dl_natural sz = ary->element_size;

  if (ary->end >= ary->capacity)
    return NULL;

  for (; idx > pos.data.index; --idx)
    dl_memory_copy(&ary->data[idx * sz], &ary->data[(idx - 1) * sz], sz);

  dl_memory_copy(&ary->data[pos.data.index * sz], value, sz);

  ary->end++;

  return true;
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
