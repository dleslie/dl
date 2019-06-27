#ifndef DL_CONTAINERS_H
#define DL_CONTAINERS_H 1

#include "dl.h"



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

#define DL_LINKED_LIST_HEADER_SIZE (sizeof(struct dl_linked_list_node))
#define DL_LINKED_LIST_DATA(element) ((dl_any)&((struct dl_linked_list_node *)element)[1])

  typedef struct {
    struct dl_linked_list_node *first;
    struct dl_linked_list_node *last;
    struct dl_linked_list_node *free_list;
    dl_natural element_size;
  } dl_linked_list;

  dl_api dl_linked_list *dl_init_linked_list(dl_linked_list *target, dl_natural element_size, dl_natural capacity);

  dl_api dl_natural dl_linked_list_copy(dl_linked_list *target, struct dl_linked_list_node *target_position, const dl_linked_list *original, struct dl_linked_list_node *original_position, dl_natural count);
  dl_api dl_natural dl_linked_list_copy_array(dl_linked_list *target, struct dl_linked_list_node *target_position, const dl_byte *data, dl_natural data_position, dl_natural count);

  dl_api void dl_destroy_linked_list(dl_linked_list *target, dl_handler *deconstruct_entry);

  dl_api dl_natural dl_linked_list_length(const dl_linked_list *list);

  dl_api dl_bool dl_linked_list_clear_free_list(dl_linked_list *list);

  dl_api dl_any dl_linked_list_get(const dl_linked_list *list, struct dl_linked_list_node *position, dl_any out);
  dl_api dl_any dl_linked_list_ref(const struct dl_linked_list_node *position);
  dl_api dl_any dl_linked_list_set(dl_linked_list *list, struct dl_linked_list_node *position, dl_any value);
  dl_api struct dl_linked_list_node *dl_linked_list_index(const dl_linked_list *list, dl_natural position);

  dl_api struct dl_linked_list_node *dl_linked_list_insert(dl_linked_list *list, struct dl_linked_list_node *position, dl_any value);
  dl_api dl_any dl_linked_list_remove(dl_linked_list *list, struct dl_linked_list_node *position, dl_any out);
  dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list *list, struct dl_linked_list_node *position, dl_natural count, dl_handler *destruct_entry);
  dl_api dl_bool dl_linked_list_destroy(dl_linked_list *list, struct dl_linked_list_node *position, dl_handler *deconstruct_entry);

  dl_api dl_bool dl_linked_list_swap(dl_linked_list *list, struct dl_linked_list_node *position1, struct dl_linked_list_node *position2, dl_bool data);

  dl_api dl_bool dl_linked_list_push(dl_linked_list *v, dl_any value);
  dl_api dl_any dl_linked_list_pop(dl_linked_list *v, dl_any out);

#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

/*****************************************************************************
 **  Linked Lists
 ****************************************************************************/

dl_api struct dl_linked_list_node *_linked_list_node_alloc(dl_linked_list *list, struct dl_linked_list_node *after) {
  struct dl_linked_list_node *node;

  if (list->free_list != NULL) {
    node = list->free_list;
    list->free_list = list->free_list->next;
  }
  else
    node = DL_ALLOC(list->element_size + DL_LINKED_LIST_HEADER_SIZE);

  if (node == NULL)
    return NULL;

  if (after == NULL) {
    node->previous = NULL;
    if (list->first != NULL) {
      node->next = list->first;
      node->next->previous = node;
      list->first = node;
    }
    else {
      list->first = list->last = node;
      node->next = NULL;
    }
  }
  else {
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

dl_api void _linked_list_node_free(dl_linked_list *list, struct dl_linked_list_node *node) {
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

dl_api void _linked_list_node_detach_free(dl_linked_list *list, struct dl_linked_list_node *e) {
  if (list->free_list == e)
    list->free_list = e->next;
  if (e->next != NULL)
    e->next->previous = e->previous;
  if (e->previous != NULL)
    e->previous->next = e->next;
  e->next = e->previous = NULL;
}

typedef struct {
  dl_handler *original_destructor;
  dl_linked_list *list;
} _linked_list_node_deconstructor_data;

dl_api dl_any _linked_list_node_deconstructor(dl_any data, dl_any element) {
  _linked_list_node_deconstructor_data *d;
  struct dl_linked_list_node *f, *e, *new_node;
  dl_handler *destruct;

  d = (_linked_list_node_deconstructor_data *)data;
  e = (struct dl_linked_list_node *)element;

  /* Is it in the free list? */
  for (f = d->list->free_list; f != NULL; f = f->next)
    if (f == e) {
      _linked_list_node_detach_free(d->list, e);
      return NULL;
    }

  /* Swap from free list */
  new_node = _linked_list_node_alloc(d->list, e->previous);
  if (!new_node) {
    /* No free nodes, destroy it. */
    if (d->original_destructor != NULL && d->original_destructor->func != NULL) {
      destruct = d->original_destructor;
      destruct->func(destruct->data, DL_LINKED_LIST_DATA(e));
    }
  }
  else
    dl_linked_list_swap(d->list, e, new_node, true);

  _linked_list_node_free(d->list, e);
  _linked_list_node_detach_free(d->list, e);

  return NULL;
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

dl_api dl_natural dl_linked_list_copy(dl_linked_list *target, struct dl_linked_list_node *target_position, const dl_linked_list *original, struct dl_linked_list_node *original_position, dl_natural count) {
  dl_any data;
  dl_natural actual_count;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;
  if (dl_unlikely(original->element_size != target->element_size))
    return 0;

  for (actual_count = 0;
       actual_count < count && original_position != NULL && NULL != (data = dl_linked_list_ref(original_position));
       ++actual_count, original_position = original_position->next) {
    target_position = dl_linked_list_insert(target, target_position, data);
    if (dl_unlikely(target_position == NULL))
      break;
  }
  return actual_count;
}

dl_api dl_natural dl_linked_list_copy_array(dl_linked_list *target, struct dl_linked_list_node *target_position, const dl_byte *data, dl_natural data_position, dl_natural length) {
  dl_natural actual_count;
  dl_any node_data;
  
  if (dl_safety(target == NULL || data == NULL))
    return 0;

  for (actual_count = 0; actual_count < length; ++actual_count) {
    node_data = (dl_any)&data[(data_position + actual_count) * target->element_size];
    target_position = dl_linked_list_insert(target, target_position, node_data);
    if (dl_unlikely(target_position == NULL))
      break;
  }
  
  return actual_count;
}

dl_api void dl_destroy_linked_list(dl_linked_list *target, dl_handler *deconstruct_entry) {
  struct dl_linked_list_node *node, *next_node;
  
  if (dl_safety(target == NULL))
    return;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    next_node = target->last;
    while (next_node != NULL) {
      node = next_node;
      next_node = node->previous;
      deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(node));
      DL_FREE(node);
    }
  }
  else {
    next_node = target->last;
    while (next_node != NULL) {
      node = next_node;
      next_node = node->previous;
      DL_FREE(node);
    }
  }

  dl_linked_list_clear_free_list(target);

  dl_memory_set(target, 0, sizeof(dl_linked_list));
}

dl_api dl_natural dl_linked_list_length(const dl_linked_list *list) {
  dl_natural length;
  struct dl_linked_list_node *node;
  
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

dl_api dl_bool dl_linked_list_clear_free_list(dl_linked_list *list) {
  struct dl_linked_list_node *current;
  
  if (dl_safety(list == NULL))
    return false;

  while (list->free_list != NULL) {
    current = list->free_list;
    list->free_list = list->free_list->next;
    DL_FREE(current);
  }

  return true;
}

dl_api dl_any dl_linked_list_get(const dl_linked_list *list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size);
}

dl_api dl_any dl_linked_list_ref(const struct dl_linked_list_node *position) {
  if (dl_safety(position == NULL))
    return NULL;

  return (const dl_any)DL_LINKED_LIST_DATA(position);
}

dl_api dl_any dl_linked_list_set(dl_linked_list *list, struct dl_linked_list_node *position, dl_any value) {
  if (dl_safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return dl_memory_copy(DL_LINKED_LIST_DATA(position), value, list->element_size);
}

dl_api struct dl_linked_list_node *dl_linked_list_insert(dl_linked_list *list, struct dl_linked_list_node *position, dl_any value) {
  struct dl_linked_list_node *node;
  
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

dl_api dl_any dl_linked_list_remove(dl_linked_list *list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL))
    return NULL;

  if (dl_unlikely(!dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size)))
    return NULL;

  _linked_list_node_free(list, position);

  return out;
}

dl_api dl_bool dl_linked_list_destroy(dl_linked_list *list, struct dl_linked_list_node *position, dl_handler *deconstruct_entry) {
  if (dl_safety(list == NULL || position == NULL))
    return false;
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(position));

  _linked_list_node_free(list, position);

  return true;
}

dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list *list, struct dl_linked_list_node *position, dl_natural count, dl_handler *deconstruct_entry) {
  dl_natural removed;
  struct dl_linked_list_node *next;
  
  if (dl_safety(list == NULL || position == NULL || count == 0))
    return 0;

  removed = 0;
  while (removed < count) {
    next = position->next;
    if (!dl_linked_list_destroy(list, position, deconstruct_entry))
      break;

    position = next;
    ++removed;
  }

  return removed;
}

dl_api dl_bool dl_linked_list_swap(dl_linked_list *list, struct dl_linked_list_node *position1, struct dl_linked_list_node *position2, dl_bool data) {
  struct dl_linked_list_node *t;
  
  if (dl_safety(list == NULL || position1 == NULL || position2 == NULL))
    return false;

  /* It's complicated because it is a double-linked list, and we have to be careful not to create a cycle. */
  if (position1 == position2->previous) {
    if (position1->previous != NULL)
      position1->previous->next = position2;
    if (position2->next != NULL)
      position2->next->previous = position1;
    position2->previous = position1->previous;
    position1->previous = position2;
    position1->next = position2->next;
    position2->next = position1;
  }
  else if (position2 == position1->previous) {
    if (position2->previous != NULL)
      position2->previous->next = position1;
    if (position1->next != NULL)
      position1->next->previous = position2;
    position1->previous = position2->previous;
    position2->previous = position1;
    position2->next = position1->next;
    position1->next = position2;
  }
  else {
    if (position1->previous != NULL)
      position1->previous->next = position2;
    if (position1->next != NULL)
      position1->next->previous = position2;
    if (position2->previous != NULL)
      position2->previous->next = position1;
    if (position2->next != NULL)
      position2->next->previous = position1;

    t = position1->next;
    position1->next = position2->next;
    position2->next = t;

    t = position1->previous;
    position1->previous = position2->previous;
    position2->previous = t;
  }

  if (list->first == position1)
    list->first = position2;
  else if (list->first == position2)
    list->first = position1;
  if (list->last == position1)
    list->last = position2;
  else if (list->last == position2)
    list->last = position1;

  if (data)
    dl_memory_swap(DL_LINKED_LIST_DATA(position1), DL_LINKED_LIST_DATA(position2), list->element_size);

  return true;
}

dl_api struct dl_linked_list_node *dl_linked_list_index(const dl_linked_list *list, dl_natural position) {
  struct dl_linked_list_node *node;
  
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


dl_api dl_bool dl_linked_list_push(dl_linked_list *list, dl_any value) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return false;

  node = _linked_list_node_alloc(list, list->last);
  if (node == NULL)
    return false;

  if (NULL == dl_linked_list_set(list, node, value))
    return false;

  return true;
}

dl_api dl_any dl_linked_list_pop(dl_linked_list *list, dl_any out) {
  dl_any ref;
  
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
