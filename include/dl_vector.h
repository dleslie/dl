#ifndef DL_CONTAINERS_H
#define DL_CONTAINERS_H 1

#include "dl.h"



#ifdef __cplusplus
extern "C" {
#endif

  /*****************************************************************************
   **  Vectors
   ****************************************************************************/

  typedef struct {
    dl_natural element_size;
    dl_natural capacity;
    dl_byte *array;
    dl_bool should_free;
  } dl_vector;

  dl_api dl_vector *dl_init_vector(dl_vector *target, dl_natural element_size, dl_natural capacity);
  dl_api dl_vector *dl_init_vector_custom(dl_vector *target, dl_natural element_size, dl_natural capacity);

  dl_api dl_vector *dl_init_vector_array(dl_vector *target, dl_byte *data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_vector_copy(dl_vector *target, dl_natural target_offset_index, const dl_vector *original, dl_natural original_offset_index, dl_natural count);

  dl_api void dl_destroy_vector(dl_vector *target, const dl_handler *deconstruct_entry);

  dl_api dl_natural dl_vector_capacity(const dl_vector *v);

  dl_api dl_bool dl_vector_grow(dl_vector *v, dl_natural amount);
  dl_api dl_bool dl_vector_shrink(dl_vector *v, dl_natural amount, dl_handler *deconstruct_entry);
  dl_api dl_bool dl_vector_resize(dl_vector *v, dl_natural new_capacity, dl_handler *deconstruct_entry);

  dl_api dl_any dl_vector_get(const dl_vector *v, dl_natural index, dl_any out);
  dl_api dl_any dl_vector_ref(const dl_vector *v, dl_natural index);
  dl_api dl_any dl_vector_set(dl_vector *v, dl_natural index, dl_any value);

  dl_api dl_bool dl_vector_swap(dl_vector *v, dl_natural index1, dl_natural index2);
  dl_api dl_natural dl_vector_ref_array(dl_vector *v, dl_natural index, dl_any *out);

  dl_api dl_integer dl_vector_insert(dl_vector *v, dl_natural position, dl_any value);
  dl_api dl_any dl_vector_remove(dl_vector *v, dl_natural position, dl_any out);
  dl_api dl_bool dl_vector_destroy(dl_vector *v, dl_natural position, dl_handler *deconstruct_entry);

  dl_api dl_bool dl_vector_push(dl_vector *v, dl_any value);
  dl_api dl_any dl_vector_pop(dl_vector *v, dl_any out);


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



  /*****************************************************************************
   **  Generic Interface
   ****************************************************************************/

  typedef enum { DL_CONTAINER_TYPE_VECTOR, DL_CONTAINER_TYPE_LINKED_LIST } dl_container_type;
  
  typedef struct {
    dl_container_type type;
    union {
      dl_linked_list list;
      dl_vector vector;
      dl_any container;
    } data;
  } dl_container;

  typedef struct {
    dl_container *container;
    union {
      dl_integer index;
      struct dl_linked_list_node *node;
    } data;
  } dl_iterator;

  dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity);

  dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count);

  dl_api void dl_destroy_container(dl_container *target, dl_handler *deconstruct_entry);
  dl_api dl_natural dl_container_length(const dl_container *container);
  dl_api dl_iterator dl_container_index(const dl_container *container, dl_natural position);

  dl_api dl_iterator dl_container_first(dl_container *target);
  dl_api dl_iterator dl_container_last(dl_container *target);

  dl_api dl_bool dl_container_push(dl_container *v, dl_any value);
  dl_api dl_any dl_container_pop(dl_container *v, dl_any out);

  /*****************************************************************************
   **  Iterator Interface
   ****************************************************************************/

  dl_api dl_iterator dl_make_invalid_iterator();

  dl_api dl_bool dl_iterator_is_valid(dl_iterator iter);
  
  dl_api dl_natural dl_iterator_copy(dl_iterator target, const dl_iterator original, dl_natural count);

  dl_api dl_any dl_iterator_get(const dl_iterator position, dl_any out);
  dl_api dl_any dl_iterator_ref(dl_iterator position);
  dl_api dl_any dl_iterator_set(dl_iterator position, dl_any value);
  
  dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2);

  dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_any value);
  dl_api dl_any dl_iterator_remove( dl_iterator position, dl_any out);
  dl_api dl_bool dl_iterator_destroy(dl_iterator position, dl_handler *deconstruct_entry);

  dl_api dl_iterator dl_iterator_next(dl_iterator target);
  dl_api dl_iterator dl_iterator_prev(dl_iterator target);
  dl_api dl_iterator dl_iterator_next_ref(dl_iterator target, dl_any *out);
  dl_api dl_iterator dl_iterator_prev_ref(dl_iterator target, dl_any *out);

  dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right);
  dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right);
  dl_api dl_integer dl_iterator_index(dl_iterator iter);
#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

dl_integer _default_compare_8(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned char *)left - *(unsigned char *)right);
}

dl_integer _default_compare_16(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned int *)left - *(unsigned int *)right);
}

dl_integer _default_compare_32(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned long int *)left - *(unsigned long int *)right);
}

dl_integer _default_compare_64(dl_any data, dl_any left, dl_any right) {
#if DL_IS_ATLEAST_C99
  return (dl_integer)(*(unsigned long long int *)left - *(unsigned long long int *)right);
#else
  return 0;
#endif
}

dl_integer _default_compare_any(dl_any data, dl_any left, dl_any right) {
  return 0;
}

typedef struct {
  dl_comparator c;
  dl_any item;
} _collection_sorted_list_predicate_data;

dl_integer _collection_sorted_list_predicate_func(dl_any data, dl_any value) {
  _collection_sorted_list_predicate_data *d = (_collection_sorted_list_predicate_data *)data;
  return d->c.func(d->c.data, value, d->item);
}

/*****************************************************************************
 **  Vectors
 ****************************************************************************/

dl_api dl_vector *dl_init_vector(dl_vector *target, dl_natural element_size, dl_natural capacity) {
  return dl_init_vector_custom(target, element_size, capacity);
}

dl_api dl_vector *dl_init_vector_custom(dl_vector *target, dl_natural element_size, dl_natural capacity) {
  
  if (target == NULL)
    return NULL;

  target->element_size = element_size;
  target->array = NULL;
  target->should_free = false;
  target->capacity = 0;

  if (dl_unlikely(target->element_size == 0 || capacity == 0))
    return NULL;

  target->array = (dl_byte *)DL_ALLOC(capacity * element_size);
  if (dl_unlikely(target->array == NULL)) {
    target->capacity = 0;
    return NULL;
  }
  target->capacity = capacity;
  target->should_free = true;

  return target;
}

dl_api dl_vector *dl_init_vector_array(dl_vector *target, dl_byte *data, dl_natural element_size, dl_natural count) {
  if (dl_safety(target == NULL || data == NULL))
    return NULL;

  target->element_size = element_size;
  target->capacity = 0;
  target->array = data;
  target->should_free = false;

  return target;
}

dl_api void dl_destroy_vector(dl_vector *target, const dl_handler *deconstruct_entry) {
  dl_any entry;
  dl_natural idx;
  
  if (target == NULL)
    return;

  if (target->array != NULL && deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    for (idx = 0; idx < target->capacity; ++idx) {
      entry = &target->array[idx * target->element_size];
      deconstruct_entry->func(deconstruct_entry->data, entry);
    }

  if (target->should_free)
    DL_FREE((dl_any)target->array);

  target->array = NULL;
  target->capacity = 0;
}

dl_api dl_inline dl_natural dl_vector_capacity(const dl_vector *v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->capacity;
}

dl_api dl_any dl_vector_get(const dl_vector *v, dl_natural index, dl_any out) {
  dl_any ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(out, ref, v->element_size);
}

dl_api dl_any dl_vector_set(dl_vector *v, dl_natural index, dl_any value) {
  dl_any ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(ref, value, v->element_size);
}

dl_api dl_any dl_vector_ref(const dl_vector *v, dl_natural index) {
  if (dl_safety(v == NULL))
    return NULL;

  if (index >= v->capacity)
    return NULL;
  return &v->array[index * v->element_size];
}

dl_api dl_bool dl_vector_grow(dl_vector *v, dl_natural amount) {
  dl_byte *new_array;
  dl_natural new_capacity;
  
  if (dl_safety(v == NULL))
    return false;
  
  new_capacity = (v->capacity + amount);
  new_array = (dl_byte *)DL_ALLOC(new_capacity * v->element_size);
  if (dl_unlikely(new_array == NULL))
    return false;

  dl_memory_copy((dl_any)new_array, (dl_any)v->array, v->capacity * v->element_size);
  
  if (v->should_free)
    DL_FREE(v->array);
  v->should_free = true;
  v->array = new_array;
  v->capacity = new_capacity;

  return true;
}

dl_api dl_bool dl_vector_swap(dl_vector *v, dl_natural index1, dl_natural index2) {
  dl_any left, right;
  
  if (dl_safety(v == NULL))
    return false;

  if (dl_unlikely(index1 == index2))
    return true;

  if (dl_unlikely(!(left = dl_vector_ref(v, index1)) || !(right = dl_vector_ref(v, index2))))
    return false;

  dl_memory_swap(left, right, v->element_size);

  return true;
} 

dl_api dl_bool dl_vector_shrink(dl_vector *v, dl_natural amount, dl_handler *deconstruct_entry) {
  dl_byte *new_array;
  dl_natural new_capacity, idx;
  
  if (dl_safety(v == NULL))
    return false;
  
  new_capacity = (v->capacity - amount);
  new_array = (dl_byte *)DL_ALLOC(new_capacity * v->element_size);
  if (dl_unlikely(new_array == NULL))
    return false;

  dl_memory_copy((dl_any)new_array, (dl_any)v->array, new_capacity * v->element_size);
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    for (idx = new_capacity; idx < v->capacity; ++idx)
      deconstruct_entry->func(deconstruct_entry->data, (dl_any)&v->array[idx * v->element_size]);
  
  if (v->should_free)
    DL_FREE(v->array);
  v->should_free = true;
  v->array = new_array;
  v->capacity = new_capacity;

  return true;
}

dl_api dl_bool dl_vector_resize(dl_vector *v, dl_natural new_capacity, dl_handler *deconstruct_entry) {
  if (dl_unlikely(v == NULL))
    return false;

  if (v->capacity == new_capacity)
    return true;
  if (v->capacity < new_capacity)
    return dl_vector_grow(v, new_capacity - v->capacity);
  else
    return dl_vector_shrink(v, v->capacity - new_capacity, deconstruct_entry);
}

dl_api dl_natural dl_vector_copy(dl_vector *target, dl_natural target_offset_index, const dl_vector *original, dl_natural original_offset_index, dl_natural count) {
  dl_natural target_window, original_window, actual_count;
  
  if (dl_safety(target == NULL || original == NULL || target->element_size != original->element_size))
    return 0;
  if (dl_unlikely(target->capacity <= target_offset_index))
    return 0;
  if (dl_unlikely(original->capacity <= original_offset_index))
    return 0;

  target_window = target->capacity - target_offset_index;
  original_window = original->capacity - original_offset_index;
  actual_count = dl_min(dl_min(count, original_window), target_window);
  
  if (dl_memory_copy((dl_any)&target->array[target_offset_index], (dl_any)&original->array[original_offset_index], target->element_size * actual_count) != NULL)
    return actual_count;
  return 0;
}

dl_api dl_natural dl_vector_ref_array(dl_vector *v, dl_natural index, dl_any *out) {
  dl_natural count;
  
  if (dl_safety(v == NULL || out == NULL))
    return 0;
  if (dl_safety(v->capacity <= index))
    return 0;

  count = v->capacity - index;
  *out = (dl_any)&v->array[index * v->element_size];

  return count;
}

dl_api dl_integer dl_vector_insert(dl_vector *v, dl_natural position, dl_any value) {
  dl_vector new_vec;
  
  if (dl_safety(v == NULL || value == NULL))
    return -1;
  
  if (NULL == dl_init_vector(&new_vec, v->element_size, 1 + dl_vector_capacity(v)))
    return -1;

  if (position > 0 && position < v->capacity) {
    if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * position)
	|| (NULL != value && NULL == dl_vector_set(&new_vec, position, value))
	|| NULL == dl_memory_copy(&new_vec.array[new_vec.element_size * (position +1)],
				  &v->array[new_vec.element_size * position],
				  dl_vector_capacity(&new_vec) - position)) {
      dl_destroy_vector(&new_vec, NULL);
      return-1;
    }
  }
  else {
    if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * new_vec.capacity)
	|| (NULL != value && NULL == dl_vector_set(&new_vec, v->capacity, value))) {
      dl_destroy_vector(&new_vec, NULL);
      return-1;
    }
    position = v->capacity;
  }

  dl_destroy_vector(v, NULL);
  *v = new_vec;

  return position;
}

dl_api dl_any dl_vector_remove(dl_vector *v, dl_natural position, dl_any out) {
  dl_vector new_vec;
  
  if (dl_safety(v == NULL || position >= v->capacity))
    return NULL;

  if (NULL == dl_init_vector(&new_vec, v->element_size, v->capacity - 1))
    return NULL;
  
  if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * position - 1)
      || (out != NULL && NULL == (out = dl_vector_ref(v, position)))
      || NULL == dl_memory_copy(&new_vec.array[new_vec.element_size * position], &v->array[new_vec.element_size * (position + 1)], v->capacity - (position + 1))) {
    dl_destroy_vector(&new_vec, NULL);
    return NULL;
  }
  
  dl_destroy_vector(v, NULL);
  *v = new_vec;
      
  return out;
}

dl_api dl_bool dl_vector_destroy(dl_vector *v, dl_natural position, dl_handler *deconstruct_entry) {
  dl_vector new_vec;
  dl_any out;
  
  if (dl_safety(v == NULL || position >= v->capacity))
    return false;

  if (NULL == dl_init_vector(&new_vec, v->element_size, v->capacity - 1))
    return false;
  
  if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * position - 1)
      || NULL == (out = dl_vector_ref(v, position))
      || NULL == dl_memory_copy(&new_vec.array[new_vec.element_size * position], &v->array[new_vec.element_size * (position + 1)], v->capacity - (position + 1))) {
    dl_destroy_vector(&new_vec, NULL);
    return false;
  }
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, out);

  dl_destroy_vector(v, NULL);
  *v = new_vec;
      
  return true;
}

dl_api dl_bool dl_vector_push(dl_vector *v, dl_any value) {
  if (dl_safety(v == NULL))
    return false;

  if (!dl_vector_grow(v, 1))
    return false;

  return dl_vector_set(v, v->capacity - 1, value);
}

dl_api dl_any dl_vector_pop(dl_vector *v, dl_any out) {
  dl_any ref;

  if (dl_safety(v == NULL))
    return NULL;

  if (out != NULL)
    ref = dl_vector_get(v, v->capacity - 1, out);
  else
    ref = dl_vector_ref(v, v->capacity - 1);

  if (!dl_vector_shrink(v 1, NULL))
    return NULL;

  return ref;
}



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



/*******************************************************************************
 ** Generic Interface
 ******************************************************************************/

dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity) {
  if (dl_safety(target == NULL || element_size == 0 || capacity == 0))
    return NULL;

  target->type = type;
  switch (type) {
  default:
    return NULL;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    if (NULL == dl_init_linked_list(&target->data.list, element_size, capacity))
      return NULL;
    return target;
  case DL_CONTAINER_TYPE_VECTOR:
    if (NULL == dl_init_vector(&target->data.vector, element_size, capacity))
      return NULL;
    return target;
  }
}

dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count) {
  if (dl_safety(target == NULL || element_size == 0 || count == 0 || array_data == NULL))
    return NULL;

  target->type = DL_CONTAINER_TYPE_VECTOR;
  if (NULL == dl_init_vector_array(&target->data.vector, array_data, element_size, count))
    return NULL;
  
  return target;
}

dl_api dl_natural dl_container_copy(dl_iterator target,  const dl_iterator original, dl_natural count) {
  if (dl_safety(!dl_iterator_is_valid(target) || !dl_iterator_is_valid(original)))
    return 0;
  
  switch (target.container->type) {
  default:
    return 0;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_copy((dl_linked_list *)target.container, target.data.node, (const dl_linked_list *)original.container, original.data.node, count);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_copy((dl_vector *)target.container, target.data.index, (dl_vector *)original.container, original.data.index, count);
  }
}

dl_api void dl_destroy_container(dl_container *target, dl_handler *deconstruct_entry) {
  if (dl_safety(target == NULL || target->data.container == NULL))
    return;

  switch (target->type) {
  default:
    break;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    dl_destroy_linked_list(&target->data.list, deconstruct_entry);
    break;
  case DL_CONTAINER_TYPE_VECTOR:
    dl_destroy_vector(&target->data.vector, deconstruct_entry);
    break;
  }

  target->data.container = NULL;
}

dl_api dl_natural dl_container_length(const dl_container *target) {
  if (dl_safety(target == NULL || target->data.container == NULL))
    return 0;

  switch (target->type) {
  default:
    return 0;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_length(&target->data.list);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_capacity(&target->data.vector);
  }
}



/*******************************************************************************
 ** Iterator Interface
 ******************************************************************************/

dl_api dl_iterator dl_make_invalid_iterator() {
  dl_iterator iter;
  iter.container = NULL;
  iter.data.index = 0;
  iter.data.node = NULL;
  return iter;
}

dl_api dl_iterator dl_container_index(const dl_container *target, dl_natural position) {
  dl_iterator iter;
  
  if (dl_safety(target == NULL || target->data.container == NULL))
    return dl_make_invalid_iterator();

  switch(target->type) {
  default:
    return dl_make_invalid_iterator();
  case DL_CONTAINER_TYPE_LINKED_LIST:
    iter.container = (dl_container *)target;
    iter.data.node = dl_linked_list_index(&target->data.list, position);
    return iter;
  case DL_CONTAINER_TYPE_VECTOR:
    if (dl_safety(position >= dl_vector_capacity(&target->data.vector)))
      return dl_make_invalid_iterator();
    iter.container = (dl_container *)target;
    iter.data.index = position;
    return iter;
  }
}

dl_api dl_iterator dl_container_first(dl_container *target) {
  dl_iterator iter;
  
  if (dl_safety(target == NULL || target->data.container == NULL))
    return dl_make_invalid_iterator();

  switch(target->type) {
  default:
    return dl_make_invalid_iterator();
  case DL_CONTAINER_TYPE_LINKED_LIST:
    iter.container = target;
    iter.data.node = target->data.list.first;
    return iter;
  case DL_CONTAINER_TYPE_VECTOR:
    if (dl_safety(target->data.vector.array == NULL))
      return dl_make_invalid_iterator();
    iter.container = target;
    iter.data.index = 0;
    return iter;
  }
}

dl_api dl_iterator dl_container_last(dl_container *target) {
  dl_iterator iter;

  if (dl_safety(target == NULL || target->data.container == NULL))
    return dl_make_invalid_iterator();

  switch(target->type) {
  default:
    return dl_make_invalid_iterator();
  case DL_CONTAINER_TYPE_LINKED_LIST:
    iter.container = target;
    iter.data.node = target->data.list.last;
    return iter;
  case DL_CONTAINER_TYPE_VECTOR:
    if (dl_safety(target->data.vector.array == NULL))
      return dl_make_invalid_iterator();
    iter.container = target;
    iter.data.index = target->data.vector.capacity - 1;
    return iter;
  }
}

dl_api dl_bool dl_container_push(dl_container *c, dl_any value) {
  if (dl_safety(c == NULL))
    return false;

  switch (c->type) {
  default:
    return false;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_push(&c->data.list, value);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_push(&c->data.vector, value);
  }
}

dl_api dl_any dl_container_pop(dl_container *c, dl_any out) {
  if (dl_safety(c == NULL))
    return false;

  switch (c->type) {
  default:
    return false;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_pop(&c->data.list, out);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_pop(&c->data.vector, out);
  }
}

dl_api dl_bool dl_iterator_is_valid(dl_iterator iter) {
  return iter.container != NULL
    && iter.container->data.container != NULL
    && ((iter.container->type == DL_CONTAINER_TYPE_LINKED_LIST && iter.data.node != NULL)
	|| (iter.container->type == DL_CONTAINER_TYPE_VECTOR && iter.data.index >= 0 && iter.data.index < dl_vector_capacity(&iter.container->data.vector)));
}

dl_api dl_any dl_iterator_get(const dl_iterator target, dl_any out) {
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

dl_api dl_any dl_iterator_ref(dl_iterator target) {
  if (dl_safety(!dl_iterator_is_valid(target)))
    return NULL;

  switch (target.container->type) {
  default:
    return NULL;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_ref(target.data.node);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_ref((dl_vector *)target.container, target.data.index);
  }
}

dl_api dl_any dl_iterator_set(dl_iterator target, dl_any value) {
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
    return dl_linked_list_swap((dl_linked_list *)position1.container, position1.data.node, position2.data.node, false);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_swap((dl_vector *)position1.container, position1.data.index, position2.data.index);
  }
}

dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_any value) {
  dl_iterator iter;
  
  if (dl_safety(!dl_iterator_is_valid(position) || value == NULL))
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

dl_api dl_any dl_iterator_remove( dl_iterator position, dl_any out) {
  if (dl_safety(!dl_iterator_is_valid(position) || out == NULL))
    return NULL;
  
  switch (position.container->type) {
  default:
    return NULL;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_remove((dl_linked_list *)position.container, position.data.node, out);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_remove((dl_vector *)position.container, position.data.index, out);
  }
}

dl_api dl_bool dl_iterator_destroy(dl_iterator position, dl_handler *deconstruct_entry) {
  if (dl_safety(!dl_iterator_is_valid(position)))
    return false;
  
  switch (position.container->type) {
  default:
    return false;
  case DL_CONTAINER_TYPE_LINKED_LIST:
    return dl_linked_list_destroy((dl_linked_list *)position.container, position.data.node, deconstruct_entry);
  case DL_CONTAINER_TYPE_VECTOR:
    return dl_vector_destroy((dl_vector *)position.container, position.data.index, deconstruct_entry);
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
    iter.data.node = target.data.node == NULL ? NULL : target.data.node->next;
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
    iter.data.node = target.data.node == NULL ? NULL : target.data.node->previous;
    return iter;
  case DL_CONTAINER_TYPE_VECTOR:
    iter.container = target.container;
    iter.data.index = target.data.index - 1;
    return iter;
  }  
}

dl_api dl_iterator dl_iterator_next_ref(dl_iterator iter, dl_any *out) {
  dl_iterator next;
  next = dl_iterator_next(iter);
  *out = dl_iterator_ref(next);
  return next;
}

dl_api dl_iterator dl_iterator_prev_ref(dl_iterator iter, dl_any *out) {
  dl_iterator prev;
  prev = dl_iterator_prev(iter);
  *out = dl_iterator_ref(prev);
  return prev;
}

dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right) {
  dl_bool leftValid, rightValid;

  leftValid = dl_iterator_is_valid(left);
  rightValid = dl_iterator_is_valid(right);
  
  return
    (leftValid == rightValid)
    && (!leftValid
	|| (left.container == right.container
	    && ((left.container->type = DL_CONTAINER_TYPE_LINKED_LIST && left.data.node == right.data.node)
		|| (left.container->type = DL_CONTAINER_TYPE_VECTOR && left.data.index == right.data.index))));
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
  case DL_CONTAINER_TYPE_LINKED_LIST:
    {
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
