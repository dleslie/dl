#ifndef DL_LINKED_LIST_CONTAINER_H
#define DL_LINKED_LIST_CONTAINER_H

#include "dl_container.h"

#ifdef __cplusplus
extern "C" {
#endif

dl_api dl_container *dl_make_linked_list_container(dl_natural element_size, dl_natural capacity);
extern dl_container_interface dl_linked_list_container_interface;

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)
#include "dl_linked_list.h"

dl_container *dl_make_linked_list_container(dl_natural element_size, dl_natural capacity) {
  return dl_make_container(&dl_linked_list_container_interface, element_size, capacity);
}

dl_ptr _make_linked_list_container(dl_natural element_size, dl_natural capacity) {
  return dl_make_linked_list(element_size, capacity);
}

void _destroy_linked_list_container(dl_ptr c) {
  dl_destroy_linked_list((dl_linked_list *)((dl_container *)c)->storage);
}

dl_natural _linked_list_container_traits(dl_ptr c) {
  return DL_CONTAINER_TRAIT_RANDOM_INSERT;
}

dl_natural _linked_list_container_length(dl_ptr c) {
  return dl_linked_list_length((dl_linked_list *)((dl_container *)c)->storage);
}

dl_bool _linked_list_container_is_empty(dl_ptr c) {
  return dl_linked_list_is_empty((dl_linked_list *)((dl_container *)c)->storage);
}

dl_natural _linked_list_container_element_size(dl_ptr c) {
  return ((dl_linked_list *)((dl_container *)c)->storage)->element_size;
}

dl_iterator _linked_list_container_index(dl_ptr c, dl_natural i) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.node = dl_linked_list_index((dl_linked_list *)iter.container->storage, i);
  return iter;
}

dl_iterator _linked_list_container_first(dl_ptr c) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.node = ((dl_linked_list *)iter.container->storage)->first;
  return iter;
}

dl_iterator _linked_list_container_last(dl_ptr c) {
  dl_iterator iter;
  iter.container = (dl_container *)c;
  iter.data.node = ((dl_linked_list *)iter.container->storage)->last;
  return iter;
}

dl_ptr _linked_list_container_push(dl_ptr c, dl_ptr v) {
  return dl_linked_list_push((dl_linked_list *)((dl_container *)c)->storage, v);
}

dl_ptr _linked_list_container_pop(dl_ptr c, dl_ptr out) {
  return dl_linked_list_pop((dl_linked_list *)((dl_container *)c)->storage, out);
}

dl_bool _linked_list_container_iterator_valid(dl_iterator i) {
  return i.data.node != NULL;
}

dl_ptr _linked_list_container_iterator_get(dl_iterator i, dl_ptr out) {
  return dl_linked_list_get((dl_linked_list *)i.container->storage, i.data.node, out);
}

dl_ptr _linked_list_container_iterator_ref(dl_iterator i) {
  return dl_linked_list_ref((dl_linked_list *)i.container->storage, i.data.node);
}

dl_ptr _linked_list_container_iterator_set(dl_iterator i, dl_ptr v) {
  return dl_linked_list_set((dl_linked_list *)i.container->storage, i.data.node, v);
}

dl_bool _linked_list_container_iterator_swap(dl_iterator a, dl_iterator b) {
  return dl_linked_list_swap((dl_linked_list *)a.container->storage, a.data.node, b.data.node);
}

dl_iterator _linked_list_container_iterator_insert(dl_iterator i, dl_ptr value) {
  dl_iterator iter;

  iter.container = i.container;
  iter.data.node = dl_linked_list_insert((dl_linked_list *)i.container->storage, i.data.node, value);

  return iter;
}

dl_bool _linked_list_container_iterator_remove(dl_iterator pos) {
  return dl_linked_list_remove((dl_linked_list *)pos.container->storage, pos.data.node);
}

dl_iterator _linked_list_container_iterator_next(dl_iterator i) {
  i.data.node = ((dl_linked_list_node *)i.data.node)->next;
  return i;
}

dl_iterator _linked_list_container_iterator_prev(dl_iterator i) {
  i.data.node = ((dl_linked_list_node *)i.data.node)->previous;
  return i;
}

dl_integer _linked_list_container_iterator_compare(dl_iterator a, dl_iterator b) {
  return !(a.data.node == b.data.node);
}

dl_integer _linked_list_container_iterator_index(dl_iterator i) {
  dl_integer index;
  dl_linked_list_node *cur;

  index = 0;
  for (cur = ((dl_linked_list *)i.container->storage)->first; cur != NULL; cur = cur->next) {
    if (cur == i.data.node)
      return index;
    ++index;
  }

  return -1;
}

dl_container_interface dl_linked_list_container_interface = {
  _make_linked_list_container,
  _destroy_linked_list_container,
  _linked_list_container_traits,
  _linked_list_container_length,
  _linked_list_container_is_empty,
  _linked_list_container_element_size,
  _linked_list_container_index,
  _linked_list_container_first,
  _linked_list_container_last,
  _linked_list_container_push,
  _linked_list_container_pop,
  _linked_list_container_iterator_valid,
  _linked_list_container_iterator_get,
  _linked_list_container_iterator_ref,
  _linked_list_container_iterator_set,
  _linked_list_container_iterator_swap,
  _linked_list_container_iterator_insert,
  _linked_list_container_iterator_remove,
  _linked_list_container_iterator_next,
  _linked_list_container_iterator_prev,
  _linked_list_container_iterator_compare,
  _linked_list_container_iterator_index};

#endif /* DL_IMPLEMENTATION */
#endif /* DL_LINKED_LIST_CONTAINER_H */
