#ifndef DL_LINKED_LIST_H
#define DL_LINKED_LIST_H 1

#include "dl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 **  Linked Lists
 ****************************************************************************/

struct dl_linked_list_node {
  struct dl_linked_list_node *next;
  struct dl_linked_list_node *previous;
#if DL_IS_ATLEAST_C99
  dl_byte data[];
#endif
};
typedef struct dl_linked_list_node dl_linked_list_node;

#define DL_LINKED_LIST_HEADER_SIZE (sizeof(struct dl_linked_list_node))
#define DL_LINKED_LIST_DATA(element) ((dl_ptr) & ((struct dl_linked_list_node *)element)[1])

typedef struct
{
  dl_linked_list_node *first;
  dl_linked_list_node *last;
  dl_linked_list_node *free_list;
  dl_natural element_size;
} dl_linked_list;

dl_api dl_linked_list *dl_init_linked_list(dl_linked_list *target, dl_natural element_size, dl_natural capacity);

dl_api dl_bool dl_linked_list_copy(dl_linked_list *target, dl_linked_list *source);

dl_api void dl_destroy_linked_list(dl_linked_list *target);

dl_api dl_natural dl_linked_list_length(const dl_linked_list *list);
dl_api dl_bool dl_linked_list_is_empty(const dl_linked_list *list);

dl_api dl_bool dl_linked_list_clear_free_list(dl_linked_list *list);

dl_api dl_ptr dl_linked_list_get(const dl_linked_list *list, dl_linked_list_node *position, dl_ptr out);
dl_api dl_ptr dl_linked_list_ref(const dl_linked_list *list, const dl_linked_list_node *position);
dl_api dl_ptr dl_linked_list_set(dl_linked_list *list, dl_linked_list_node *position, dl_ptr value);
dl_api dl_linked_list_node *dl_linked_list_index(const dl_linked_list *list, dl_natural position);

dl_api dl_linked_list_node *dl_linked_list_insert(dl_linked_list *list, dl_linked_list_node *position, dl_ptr value);
dl_api dl_bool dl_linked_list_remove(dl_linked_list *list, dl_linked_list_node *position);

dl_api dl_bool dl_linked_list_swap(dl_linked_list *list, dl_linked_list_node *position1, dl_linked_list_node *position2);

dl_api dl_ptr dl_linked_list_push(dl_linked_list *v, dl_ptr value);
dl_api dl_ptr dl_linked_list_pop(dl_linked_list *v, dl_ptr out);

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)

/*****************************************************************************
 **  Linked Lists
 ****************************************************************************/

dl_api dl_linked_list_node *_linked_list_node_alloc(dl_linked_list *list, dl_linked_list_node *after) {
  dl_linked_list_node *node;

  if (list->free_list != NULL) {
    node = list->free_list;
    list->free_list = list->free_list->next;
  } else
#if DL_USE_ALLOC
    node = (dl_linked_list_node *)DL_ALLOC(list->element_size + DL_LINKED_LIST_HEADER_SIZE);
#else
    return NULL;
#endif

  if (node == NULL)
    return NULL;

  if (after == NULL) {
    node->previous = NULL;
    if (list->first != NULL) {
      node->next = list->first;
      node->next->previous = node;
      list->first = node;
    } else {
      list->first = list->last = node;
      node->next = NULL;
    }
  } else {
    node->previous = after;
    node->next = after->next;
    after->next = node;
    if (node->next == NULL)
      list->last = node;
    else
      node->next->previous = node;
  }

  return node;
}

dl_api void _linked_list_node_free(dl_linked_list *list, dl_linked_list_node *node) {
  if (list->first == node)
    list->first = node->next;
  if (node->next != NULL)
    node->next->previous = node->previous;
  if (node->previous != NULL)
    node->previous->next = node->next;
  if (list->last == node)
    list->last = node->previous;

  node->next = list->free_list;
  if (node->next != NULL)
    node->next->previous = node;
  node->previous = NULL;
  list->free_list = node;
}

dl_api void _linked_list_node_detach_free(dl_linked_list *list, dl_linked_list_node *e) {
  if (list->free_list == e)
    list->free_list = e->next;
  if (e->next != NULL)
    e->next->previous = e->previous;
  if (e->previous != NULL)
    e->previous->next = e->next;
  e->next = e->previous = NULL;
}

dl_api dl_linked_list *dl_init_linked_list(dl_linked_list *target, dl_natural element_size, dl_natural capacity) {
  if (dl_safety(target == NULL || element_size < 1))
    return NULL;

  target->first = target->last = target->free_list = NULL;
  target->element_size = element_size;
  target->free_list = NULL;

  while (capacity > 0) {
    if (target->free_list == NULL)
      target->free_list = _linked_list_node_alloc(target, target->free_list);
    else if (_linked_list_node_alloc(target, target->free_list) == NULL)
      break;
    --capacity;
  }

  return target;
}

dl_api dl_bool dl_linked_list_copy(dl_linked_list *target, dl_linked_list *source) {
  dl_linked_list_node *node;

  if (dl_safety(source == NULL || target == NULL || source->element_size != target->element_size))
    return false;

  if (target->first != NULL) {
    target->last->next = target->free_list;
    target->free_list = target->first;
    target->first = NULL;
  }

  for (node = source->first; node != NULL; node = node->next)
    if (NULL == dl_linked_list_push(target, DL_LINKED_LIST_DATA(node)))
      return false;

  return true;
}

dl_api void dl_destroy_linked_list(dl_linked_list *target) {
  dl_linked_list_node *node, *next_node;

  if (dl_safety(target == NULL))
    return;

  next_node = target->last;
  while (next_node != NULL) {
    node = next_node;
    next_node = node->previous;
    DL_FREE(node);
  }

  dl_linked_list_clear_free_list(target);

  dl_memory_set(target, 0, sizeof(dl_linked_list));
}

dl_api dl_natural dl_linked_list_length(const dl_linked_list *list) {
  dl_natural length;
  dl_linked_list_node *node;

  if (dl_safety(list == NULL))
    return 0;

  length = 0;
  node = list->first;
  while (node != NULL) {
    node = node->next;
    ++length;
  }

  return length;
}

dl_api dl_inline dl_bool dl_linked_list_is_empty(const dl_linked_list *list) {
  if (dl_safety(list == NULL))
    return true;

  return list->first == NULL;
}

dl_api dl_bool dl_linked_list_clear_free_list(dl_linked_list *list) {
  dl_linked_list_node *current;

  if (dl_safety(list == NULL))
    return false;

  while (list->free_list != NULL) {
    current = list->free_list;
    list->free_list = list->free_list->next;
    DL_FREE(current);
  }

  return true;
}

dl_api dl_ptr dl_linked_list_get(const dl_linked_list *list, dl_linked_list_node *position, dl_ptr out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size);
}

dl_api dl_ptr dl_linked_list_ref(const dl_linked_list *list, const dl_linked_list_node *position) {
  if (dl_safety(position == NULL))
    return NULL;

  return (const dl_ptr)DL_LINKED_LIST_DATA(position);
}

dl_api dl_ptr dl_linked_list_set(dl_linked_list *list, dl_linked_list_node *position, dl_ptr value) {
  if (dl_safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return dl_memory_copy(DL_LINKED_LIST_DATA(position), value, list->element_size);
}

dl_api dl_linked_list_node *dl_linked_list_insert(dl_linked_list *list, dl_linked_list_node *position, dl_ptr value) {
  dl_linked_list_node *node;

  if (dl_safety(list == NULL))
    return NULL;

  /* alloc expects "after" whereas add expects "at" */
  position = position != NULL ? position->previous : NULL;
  node = _linked_list_node_alloc(list, position);
  if (dl_unlikely(value != NULL && !dl_memory_copy(DL_LINKED_LIST_DATA(node), value, list->element_size))) {
    _linked_list_node_free(list, node);
    return NULL;
  }

  return node;
}

dl_api dl_bool dl_linked_list_remove(dl_linked_list *list, dl_linked_list_node *position) {
  if (dl_safety(list == NULL || position == NULL))
    return false;

  _linked_list_node_free(list, position);

  return true;
}

dl_api dl_bool dl_linked_list_swap(dl_linked_list *list, dl_linked_list_node *position1, dl_linked_list_node *position2) {
  if (dl_safety(list == NULL || position1 == NULL || position2 == NULL))
    return false;

  dl_memory_swap(DL_LINKED_LIST_DATA(position1), DL_LINKED_LIST_DATA(position2), list->element_size);

  return true;
}

dl_api dl_linked_list_node *dl_linked_list_index(const dl_linked_list *list, dl_natural position) {
  dl_linked_list_node *node;

  if (dl_safety(list == NULL))
    return NULL;

  node = list->first;
  while (node != NULL && position > 0) {
    node = node->next;
    --position;
  }

  if (position == 0)
    return node;
  return NULL;
}

dl_api dl_ptr dl_linked_list_push(dl_linked_list *list, dl_ptr value) {
  dl_linked_list_node *node;

  if (dl_safety(list == NULL))
    return NULL;

  node = _linked_list_node_alloc(list, list->last);
  if (node == NULL)
    return NULL;

  if (NULL == dl_linked_list_set(list, node, value))
    return NULL;

  return DL_LINKED_LIST_DATA(node);
}

dl_api dl_ptr dl_linked_list_pop(dl_linked_list *list, dl_ptr out) {
  dl_ptr ref;

  if (dl_safety(list == NULL))
    return NULL;

  if (out != NULL)
    ref = dl_linked_list_get(list, list->last, out);
  else
    ref = dl_linked_list_ref(list, list->last);

  _linked_list_node_free(list, list->last);

  return ref;
}

#endif /* DL_IMPLEMENTATION */

#endif
