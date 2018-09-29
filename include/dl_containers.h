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
    dl_natural slice_length;
    dl_integer slice_count;

    dl_any (*alloc)(dl_natural byte_count);
    void (*free)(dl_any data);

    union {
      dl_byte **slices;
      dl_byte *array;
    } data;
  } dl_vector;

  dl_api dl_vector *dl_init_vector(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity);
  dl_api dl_vector *dl_init_vector_custom(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity, dl_any (*alloc)(dl_natural byte_count), void (*free)(dl_any data));

  dl_api dl_vector *dl_init_vector_array(dl_vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_vector_copy(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_vector * dl_restrict original);
  dl_api dl_natural dl_vector_copy_array(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void dl_destroy_vector(dl_vector * dl_restrict target, const dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural dl_vector_capacity(const dl_vector * dl_restrict v);

  dl_api dl_bool dl_vector_grow(dl_vector * dl_restrict v);
  dl_api dl_bool dl_vector_shrink(dl_vector * dl_restrict v, dl_handler *deconstruct_entry);
  dl_api dl_bool dl_vector_resize(dl_vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry);

  dl_api dl_any dl_vector_get(const dl_vector * dl_restrict v, dl_natural index, dl_any out);
  dl_api dl_any dl_vector_ref(const dl_vector * dl_restrict v, dl_natural index);
  dl_api dl_any dl_vector_set(dl_vector * dl_restrict v, dl_natural index, dl_any value);

  dl_api dl_bool dl_vector_swap(dl_vector * dl_restrict v, dl_natural index1, dl_natural index2);
  dl_api dl_natural dl_vector_ref_array(dl_vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out);



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
    dl_natural cache_length;
    dl_any (*alloc)(dl_natural byte_count);
    void (*free)(dl_any data);

    dl_vector node_cache;
  } dl_linked_list;

  dl_api dl_linked_list *dl_init_linked_list(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length);
  dl_api dl_linked_list *dl_init_linked_list_custom(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length, dl_any (*alloc)(dl_natural byte_count), void (*free)(dl_any data));

  dl_api dl_natural dl_linked_list_copy(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_linked_list *dl_restrict original);
  dl_api dl_natural dl_linked_list_copy_array(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void dl_destroy_linked_list(dl_linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural dl_linked_list_capacity(const dl_linked_list * dl_restrict list);
  dl_api dl_natural dl_linked_list_length(const dl_linked_list * dl_restrict list);

  dl_api dl_bool dl_linked_list_grow(dl_linked_list * dl_restrict list);
  dl_api dl_bool dl_linked_list_shrink(dl_linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry);
  dl_api dl_bool dl_linked_list_resize(dl_linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_any dl_linked_list_get(const dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any out);
  dl_api dl_any dl_linked_list_ref(const struct dl_linked_list_node *dl_restrict position);
  dl_api dl_any dl_linked_list_set(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any value);
  dl_api struct dl_linked_list_node *dl_linked_list_index(dl_linked_list * dl_restrict list, dl_natural position);

  dl_api struct dl_linked_list_node *dl_linked_list_add(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any value);
  dl_api dl_any dl_linked_list_remove(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict position, dl_any out);
  dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_natural count, dl_handler *dl_restrict destruct_entry);
  dl_api dl_bool dl_linked_list_destroy(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_handler *deconstruct_entry);

  dl_api dl_bool dl_linked_list_swap(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position1, struct dl_linked_list_node *dl_restrict position2, dl_bool data);

  
#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

#define _DL_DECLARE_ALLOC_MEMBERS()	(dl_any (*)(dl_natural))DL_ALLOC, (void (*)(dl_any))DL_FREE

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

dl_api dl_vector *dl_init_vector(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity) {
  return dl_init_vector_custom(target, element_size, capacity, _DL_DECLARE_ALLOC_MEMBERS());
}

dl_api dl_vector *dl_init_vector_custom(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity, dl_any (*alloc)(dl_natural byte_count), void (*free)(dl_any data)) {
  dl_real dl_real_count;
  dl_natural slice_count, idx;
  
  if (target == NULL)
    return NULL;

  target->alloc = alloc;
  target->free = free;
  target->element_size = element_size;
  target->slice_length = DL_DEFAULT_SLICE_LENGTH / ((element_size + 7) & ~7);
  if (target->slice_length == 0) target->slice_length = 1;
  target->slice_count = -1;
  target->data.array = 0;
  target->data.slices = NULL;

  if (target->alloc == NULL || target->free == NULL)
    return NULL;

  if (target->element_size < 1)
    return NULL;

  dl_real_count = ((dl_real)capacity / (dl_real)target->slice_length);
  slice_count = (dl_natural)dl_real_count;
  if (dl_real_count > (dl_real)(dl_natural)dl_real_count)
    slice_count++;

  target->slice_count = slice_count < 1 ? 1 : slice_count;

  target->data.slices = (dl_byte **)target->alloc(target->slice_count * sizeof(dl_byte *));
  if (dl_unlikely(target->data.slices == NULL))
    return NULL;

  for (idx = 0; idx < target->slice_count; ++idx) {
    target->data.slices[idx] = (dl_byte *)target->alloc(target->slice_length * target->element_size);

    if (dl_unlikely(target->data.slices[idx] == NULL))
      break;
    
    dl_memory_set(target->data.slices[idx], 0, target->slice_length * target->element_size);
  }

  if (dl_unlikely(idx < slice_count)) {
    for (--idx; idx != 0; --idx) {
      target->free((dl_any)target->data.slices[idx]);
    }
    
    target->free((dl_any)target->data.slices);
    target->data.slices = NULL;
    target->slice_count = -1;

    return NULL;
  }

  return target;
}

dl_api dl_vector *dl_init_vector_array(dl_vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count) {
  if (dl_safety(target == NULL || data == NULL))
    return NULL;

  target->alloc = NULL;
  target->free = NULL;
  target->element_size = element_size;
  target->slice_length = count;
  target->slice_count = 0;

  target->data.slices = (dl_byte **)data;

  return target;
}

dl_api void dl_destroy_vector(dl_vector * dl_restrict target, const dl_handler *deconstruct_entry) {
  dl_any entry;
  dl_natural slice_idx, idx;
  
  if (target == NULL || target->free == NULL)
    return;

  if (target->data.slices != NULL) {
    for (slice_idx = 0; slice_idx < target->slice_count; ++slice_idx) {
      if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
        for (idx = 0; idx < target->slice_length; ++idx) {
          entry = &target->data.slices[slice_idx][idx * target->element_size];
	        deconstruct_entry->func(deconstruct_entry->data, entry);
        }
      }

      target->free((dl_any)target->data.slices[slice_idx]);
    }

    target->free((dl_any)target->data.slices);
    target->data.slices = NULL;
  }
}

dl_api dl_inline dl_natural dl_vector_capacity(const dl_vector * dl_restrict v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->slice_count > 0 ? v->slice_count * v->slice_length : v->slice_length;
}

dl_api dl_any dl_vector_get(const dl_vector * dl_restrict v, dl_natural index, dl_any out) {
  dl_any ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(out, ref, v->element_size);
}

dl_api dl_any dl_vector_set(dl_vector * dl_restrict v, dl_natural index, dl_any value) {
  dl_any ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(ref, value, v->element_size);
}

dl_api dl_any dl_vector_ref(const dl_vector * dl_restrict v, dl_natural index) {
  if (dl_safety(v == NULL))
    return NULL;

  switch(v->slice_count) {
  case -1:
    return NULL;
  case 0:
    {
      if (dl_safety(index >= v->slice_length))
	return NULL;
      
      return &v->data.array[index * v->element_size];
    }
  case 1:
    {
      if (dl_safety(index >= v->slice_length))
	return NULL;

      return &(v->data.slices[0][index * v->element_size]);
    }
  default:
    {
      dl_natural slice, slice_index;
      
      if (dl_safety(index >= v->slice_length * v->slice_count))
	return NULL;

      slice = index / v->slice_length;
      slice_index = index % v->slice_length;

      return &(v->data.slices[slice][slice_index * v->element_size]);
    }
  }
}

dl_api dl_bool dl_vector_grow(dl_vector * dl_restrict v) {
  dl_byte **new_slices, *new_slice, **existing_slices;
  dl_natural idx;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->free == NULL || v->alloc == NULL)
    return false;

  new_slices = (dl_byte **)v->alloc((v->slice_count + 1) * sizeof(dl_byte *));
  if (dl_unlikely(new_slices == NULL))
    return false;

  new_slice = (dl_byte *)v->alloc(v->slice_length * v->element_size);
  if (dl_unlikely(new_slice == NULL)) {
    v->free((dl_any)new_slices);
    return false;
  }

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->data.slices[v->slice_count] = new_slice;

  v->free((dl_any)existing_slices);

  v->slice_count++;

  return true;
}

dl_api dl_bool dl_vector_swap(dl_vector * dl_restrict v, dl_natural index1, dl_natural index2) {
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

dl_api dl_bool dl_vector_shrink(dl_vector * dl_restrict v, dl_handler *dl_restrict deconstruct_entry) {
  dl_byte **new_slices, **existing_slices;
  dl_natural idx;
  dl_any entry;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->slice_count <= 1 || v->free == NULL || v->alloc == NULL)
    return false;

  new_slices = (dl_byte **)v->alloc((v->slice_count - 1) * sizeof(dl_byte *));
  if (dl_unlikely(new_slices == NULL))
    return false;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    for (idx = 0; idx < v->slice_length; ++idx) {
      entry = &v->data.slices[v->slice_count - 1][idx * v->element_size];
      deconstruct_entry->func(deconstruct_entry->data, entry);
    }
  }

  v->free((dl_any)v->data.slices[v->slice_count - 1]);

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count - 1; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->slice_count--;

  v->free((dl_any)existing_slices);

  return true;
}

dl_api dl_bool dl_vector_resize(dl_vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry) {
  dl_natural current_capacity, new_slice_count, slice_idx, slice_count, item_idx;
  dl_real needed;
  dl_integer needed_count;
  dl_byte **new_slices, *slice;
  dl_any item;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->free == NULL || v->alloc == NULL || v->slice_count < 1 || minimum_capacity < 1)
    return false;

  current_capacity = dl_vector_capacity(v);
  needed = (dl_real)((dl_integer)minimum_capacity - (dl_integer)current_capacity) / (dl_real)v->slice_length;

  needed_count = needed < 0 ? (dl_integer)needed : (dl_integer)needed + 1;

  if (dl_likely(needed_count != 0)) {
    new_slice_count = v->slice_count + needed_count;
    new_slices = (dl_byte **)v->alloc(new_slice_count * sizeof(dl_byte **));
    if (dl_unlikely(new_slices == NULL))
      return false;

    slice_count = new_slice_count < v->slice_count ? new_slice_count : v->slice_count;

    for (slice_idx = 0; slice_idx < slice_count; ++slice_idx)
      new_slices[slice_idx] = v->data.slices[slice_idx];

    /* Shrinking */
    if (needed_count < 0) {
      for (; slice_idx < v->slice_count; ++slice_idx) {
        slice = v->data.slices[slice_idx];
        if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
          for (item_idx = 0; item_idx < v->slice_length; ++item_idx) {
            item = &slice[item_idx * v->element_size];
            deconstruct_entry->func(deconstruct_entry->data, item);
          }

        v->free((dl_any)slice);
      }
    }
    /* Growing */
    else {
      for (; slice_idx < new_slice_count; ++slice_idx) {
        new_slices[slice_idx] = (dl_byte *)v->alloc(v->slice_length * v->element_size);
        if (dl_unlikely(new_slices[slice_idx] == NULL))
          break;
      }
      if (dl_unlikely(slice_idx != new_slice_count)) {
        for (slice_idx--; slice_idx >= v->slice_count; --slice_idx)
          v->free((dl_any)new_slices[slice_idx]);
        v->free((dl_any)new_slices);
        return false;
      }
    }

    v->free((dl_any)v->data.slices);
    v->data.slices = new_slices;
    v->slice_count = new_slice_count;
  }

  return true;
}

dl_api dl_natural dl_vector_copy_array(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *data, dl_natural count) {
  dl_vector source;
  if (dl_unlikely(!dl_init_vector_array(&source, (dl_byte *)data, target->element_size, count)))
    return 0;

  return dl_vector_copy(target, target_offset_index, &source);
}

dl_api dl_natural dl_vector_copy(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_vector * dl_restrict original) {
  dl_natural original_capacity, element_size, target_length, original_length, target_remainder, original_remainder, total_remainder, target_slice_idx, original_slice_idx, min_remainder, count_to_copy;
  dl_byte *target_slice, *original_slice;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (dl_unlikely(original->element_size != target->element_size))
    return 0;

  /* target is too small */
  original_capacity = dl_vector_capacity(original);
  if (dl_unlikely(dl_vector_capacity(target) - target_offset_index < original_capacity))
    return 0;

  element_size = original->element_size;

  target_length = target->slice_length * element_size;
  original_length = original->slice_length * element_size;

  target_remainder = target_length - ((target_offset_index % target->slice_length) * element_size);
  original_remainder = original_length;

  total_remainder = original_capacity * element_size;

  target_slice_idx = target_offset_index / target->slice_length;
  original_slice_idx = 0;

  target_slice = target->slice_count == 0 ? target->data.array : target->data.slices[target_slice_idx];
  original_slice = original->slice_count == 0 ? original->data.array : original->data.slices[0];

  while (total_remainder != 0) {
    if (target_remainder == 0) {
      ++target_slice_idx;
      target_slice = target->data.slices[target_slice_idx];
      target_remainder = target_length;
    }
    if (original_remainder == 0) {
      ++original_slice_idx;
      original_slice = original->data.slices[original_slice_idx];
      original_remainder = original_length;
    }

    min_remainder = target_remainder < original_remainder ? target_remainder : original_remainder;
    count_to_copy = total_remainder < min_remainder ? total_remainder : min_remainder;

    dl_memory_copy(&target_slice[target_length - target_remainder], &original_slice[original_length - original_remainder], count_to_copy);

    total_remainder -= count_to_copy;
    target_remainder -= count_to_copy;
    original_remainder -= count_to_copy;
  }

  return original_capacity;
}

dl_api dl_natural dl_vector_ref_array(dl_vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out) {
  dl_natural last_idx, slice, slice_index;
  
  if (dl_safety(v == NULL))
    return 0;

  last_idx = v->slice_length - 1;

  if (v->slice_count == 0 && index < last_idx) {
    *((dl_byte **)out) = &v->data.array[index];
    return last_idx - index;
  }
  else {
    slice = index / v->slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return 0;

    slice_index = index - (slice * v->slice_length);
    *((dl_byte **)out) = &v->data.slices[slice][slice_index];
    return last_idx - slice_index;
  }
}




/*****************************************************************************
 **  Linked Lists
 ****************************************************************************/

dl_api struct dl_linked_list_node *_linked_list_node_alloc(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict after) {
  struct dl_linked_list_node *node;
  
  if (list->free_list == NULL)
    return NULL;

  node = list->free_list;
  list->free_list = node->next;
  if (list->free_list != NULL)
    list->free_list->previous = NULL;

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

dl_api void _linked_list_node_free(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict node) {
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

dl_api void _linked_list_node_detach_free(dl_linked_list *dl_restrict list, struct dl_linked_list_node *e) {
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

dl_api dl_linked_list *dl_init_linked_list(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length) {
  if (dl_safety(target == NULL || element_size == 0))
    return false;
  
  return dl_init_linked_list_custom(target, element_size, cache_length, _DL_DECLARE_ALLOC_MEMBERS());
}

dl_api dl_linked_list *_linked_list_cache_grow(dl_linked_list * dl_restrict target) {
  dl_vector *v;
  dl_natural zero, length, idx;
  struct dl_linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return NULL;

  if (target->cache_length == 0)
    return NULL;

  v = &target->node_cache;

  if (target->free_list == NULL && target->first == NULL) {
    zero = 0;
    if (!dl_init_vector(v, DL_LINKED_LIST_HEADER_SIZE + target->element_size, target->cache_length))
      return NULL;
  }
  else {
    zero = dl_vector_capacity(v);
    if (!dl_vector_grow(v))
      return NULL;
  }

  length = dl_vector_capacity(v);

  for (idx = zero; idx < length; ++idx) {
    node = (struct dl_linked_list_node *)dl_vector_ref(v, idx);
    node->next = node->previous = NULL;
    _linked_list_node_free(target, node);
  }

  return target;
}

dl_api dl_linked_list *dl_init_linked_list_custom(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length, dl_any (*alloc)(dl_natural byte_count), void (*free)(dl_any data)) {
  if (dl_safety(target == NULL || element_size < 1))
    return NULL;

  target->first = target->last = target->free_list = NULL;
  target->element_size = element_size;
  target->cache_length = cache_length;
  target->alloc = alloc;
  target->free = free;

  return _linked_list_cache_grow(target);
}

dl_api dl_natural dl_linked_list_copy(dl_linked_list * dl_restrict target, struct dl_linked_list_node *dl_restrict target_position, const dl_linked_list * dl_restrict original) {
  struct dl_linked_list_node *source_node, *next;
  dl_natural count;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  source_node = original->first;
  count = 0;

  while (source_node != NULL) {
    next = dl_linked_list_add(target, target_position, DL_LINKED_LIST_DATA(source_node));
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    source_node = source_node->next;
    ++count;
  }

  return count;
}

dl_api dl_natural dl_linked_list_copy_array(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_byte *data, dl_natural length) {
  dl_natural count, byte_size, idx;
  struct dl_linked_list_node *next;
  
  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  count = 0;
  byte_size = target->element_size;

  for (idx = 0; idx < length; ++idx) {
    next = dl_linked_list_add(target, target_position, (dl_byte *)&data[idx * byte_size]);
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    ++count;
  }

  return count;
}

dl_api void dl_destroy_linked_list(dl_linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    node = target->first;
    while (node != NULL) {
      deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(node));
      node = node->previous;
    }
  }

  dl_destroy_vector(&target->node_cache, NULL);

  dl_memory_set(target, 0, sizeof(dl_linked_list));
}

dl_api dl_natural dl_linked_list_capacity(const dl_linked_list * dl_restrict list) {
  if (dl_safety(list == NULL))
    return 0;

  return dl_vector_capacity(&list->node_cache);
}

dl_api dl_natural dl_linked_list_length(const dl_linked_list * dl_restrict list) {
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

dl_api dl_bool dl_linked_list_grow(dl_linked_list * dl_restrict list) {
  return _linked_list_cache_grow(list) != NULL;
}

dl_api dl_bool dl_linked_list_shrink(dl_linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;

  return dl_vector_shrink(&list->node_cache, &destructor);
}

dl_api dl_bool dl_linked_list_resize(dl_linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;
  
  return dl_vector_resize(&list->node_cache, minimum_capacity, &destructor);
}

dl_api dl_any dl_linked_list_get(const dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size);
}

dl_api dl_any dl_linked_list_ref(const struct dl_linked_list_node *position) {
  if (dl_safety(position == NULL))
    return NULL;

  return (const dl_any)DL_LINKED_LIST_DATA(position);
}

dl_api dl_any dl_linked_list_set(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any value) {
  if (dl_safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return dl_memory_copy(DL_LINKED_LIST_DATA(position), value, list->element_size);
}

dl_api struct dl_linked_list_node *dl_linked_list_add(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any value) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return NULL;

  if (list->free_list == NULL)
    return NULL;

  node = _linked_list_node_alloc(list, position);
  if (dl_unlikely(value != NULL && !dl_memory_copy(DL_LINKED_LIST_DATA(node), value, list->element_size))) {
    _linked_list_node_free(list, node);
    return NULL;
  }

  return node;
}

dl_api dl_any dl_linked_list_remove(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL))
    return NULL;

  if (dl_unlikely(!dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->element_size)))
    return NULL;

  _linked_list_node_free(list, position);

  return out;
}

dl_api dl_bool dl_linked_list_destroy(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_handler *deconstruct_entry) {
  if (dl_safety(list == NULL || position == NULL))
    return false;
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(position));

  _linked_list_node_free(list, position);

  return true;
}

dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_natural count, dl_handler *deconstruct_entry) {
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

dl_api dl_bool dl_linked_list_swap(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position1, struct dl_linked_list_node *position2, dl_bool data) {
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

dl_api struct dl_linked_list_node *dl_linked_list_index(dl_linked_list * dl_restrict list, dl_natural position) {
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



#endif /* DL_IMPLEMENTATION */

#endif
