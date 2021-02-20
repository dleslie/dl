#ifndef DL_LINKED_LIST_H
#define DL_LINKED_LIST_H 1

#include "dl_core.h"
#include "dl_memory.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 **  Doubly Linked Lists
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

dl_api dl_linked_list *dl_make_linked_list(dl_natural element_size, dl_natural capacity);

dl_api void dl_destroy_linked_list(dl_linked_list *target);

dl_api dl_natural dl_linked_list_length(const dl_linked_list *list);
dl_api dl_bool dl_linked_list_is_empty(const dl_linked_list *list);

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

dl_api dl_linked_list_node *_linked_list_node_alloc(dl_linked_list *list, dl_linked_list_node *after, dl_bool for_free_list) {
  dl_linked_list_node *node;

  if (list->free_list != NULL) {
    node = list->free_list;
    list->free_list = list->free_list->next;
  } else
    node = (dl_linked_list_node *)dl_alloc(list->element_size + DL_LINKED_LIST_HEADER_SIZE);

  if (node == NULL)
    return NULL;

  if (after == NULL) {
    node->previous = NULL;

    if (for_free_list) {
      node->next = list->free_list;
      list->free_list = node;
    } else if (list->first != NULL) {
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
    if (!for_free_list) {
      if (node->next == NULL)
        list->last = node;
      else
        node->next->previous = node;
    }
  }

  return node;
}

dl_api dl_linked_list *dl_make_linked_list(dl_natural element_size, dl_natural capacity) {
  dl_linked_list *target;
  dl_linked_list_node *node;

  if (dl_safety(element_size < 1))
    return NULL;

  if (dl_unlikely(NULL == (target = (dl_linked_list *)dl_alloc(sizeof(dl_linked_list)))))
    return NULL;

  target->first = target->last = target->free_list = NULL;
  target->element_size = element_size;

  while (capacity > 0) {
    if (dl_unlikely(NULL == (node = (dl_linked_list_node *)dl_alloc(target->element_size + DL_LINKED_LIST_HEADER_SIZE))))
      break;

    node->next = target->free_list;
    node->previous = NULL;
    target->free_list = node;
    --capacity;
  }

  return target;
}

dl_api void dl_destroy_linked_list(dl_linked_list *target) {
  dl_linked_list_node *node, *kill;

  if (dl_safety(target == NULL))
    return;

  node = target->first;
  while (node != NULL) {
    kill = node;
    node = node->next;
    dl_free(kill);
  }

  node = target->free_list;
  while (node != NULL) {
    kill = node;
    node = node->next;
    dl_free(kill);
  }

  target->free_list = target->last = target->first = NULL;

  dl_free(target);
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

dl_api dl_ptr dl_linked_list_get(const dl_linked_list *list, dl_linked_list_node *position, dl_ptr out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size);
}

dl_api dl_ptr dl_linked_list_ref(const dl_linked_list *list, const dl_linked_list_node *position) {
    dl_unused(list);
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

  if (dl_likely(list->free_list != NULL)) {
    node = list->free_list;
    list->free_list = node->next;
    node->next = NULL;
  } else if (dl_unlikely(NULL == (node = (dl_linked_list_node *)dl_alloc(list->element_size + DL_LINKED_LIST_HEADER_SIZE))))
    return NULL;

  if (dl_likely(value != NULL) && dl_unlikely(!dl_memory_copy(DL_LINKED_LIST_DATA(node), value, list->element_size))) {
    node->next = list->free_list;
    node->previous = NULL;
    list->free_list = node;
    return NULL;
  }

  if (position == NULL) {
    if (dl_unlikely(list->last == NULL)) {
      list->last = list->first = node;
      node->next = node->previous = NULL;
    } else {
      list->last->next = node;
      node->previous = list->last;
      node->next = NULL;
      list->last = node;
    }
  } else {
    if (position == list->first) {
      list->first = node;
    } else {
      position->previous->next = node;
      node->previous = position->previous;
    }

    node->next = position;
    position->previous = node;
  }

  return node;
}

dl_api dl_bool dl_linked_list_remove(dl_linked_list *list, dl_linked_list_node *position) {
  if (dl_safety(list == NULL) || position == NULL)
    return false;

  if (list->first == position) {
    list->first = position->next;
    if (list->first == NULL)
      list->last = NULL;
    else
      list->first->previous = NULL;
  } else if (list->last == position) {
    list->last = position->previous;
    if (list->last == NULL)
      list->first = NULL;
    else
      list->last->next = NULL;
  } else {
    position->previous->next = position->next;
    position->next->previous = position->previous;
  }

  position->next = list->free_list;
  position->previous = NULL;
  list->free_list = position;

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

  if (dl_unlikely(NULL == (node = dl_linked_list_insert(list, NULL, value))))
    return NULL;

  return DL_LINKED_LIST_DATA(node);
}

dl_api dl_ptr dl_linked_list_pop(dl_linked_list *list, dl_ptr out) {
  dl_ptr ref;

  if (dl_safety(list == NULL) || list->last == NULL)
    return NULL;

  if (out != NULL)
    ref = dl_linked_list_get(list, list->last, out);
  else
    ref = NULL;

  dl_linked_list_remove(list, list->last);

  return ref;
}

#endif /* DL_IMPLEMENTATION */

#endif
