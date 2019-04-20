#ifndef DL_COLLECTIONS_H
#define DL_COLLECTIONS_H 1

#include "dl.h"
#include "dl_containers.h"



#ifdef __cplusplus
extern "C" {
#endif

  typedef union {
    struct {
      dl_natural index;
    } dl_vector;
    struct {
      struct dl_linked_list_node *node;
    } dl_linked_list;
  } dl_iterator;

  enum {
    /* Unsorted */
    DL_COLLECTION_TYPE_LIST,
    DL_COLLECTION_TYPE_QUEUE,
    /* Sorted */
    DL_COLLECTION_TYPE_SORTED_LIST,
    DL_COLLECTION_TYPE_SET
  };
  typedef dl_byte dl_collection_type;

  enum {
    DL_STORAGE_TYPE_VECTOR,
    DL_STORAGE_TYPE_LINKED_LIST
  };
  typedef dl_byte dl_storage_type;  

  typedef struct {
    dl_collection_type type;
    dl_storage_type storage;
    
    dl_comparator comparer;
    dl_handler deconstruct_entry;
    dl_natural capacity;
    dl_natural element_size;
  } dl_collection_settings;

  extern dl_collection_settings default_vector_collection_settings;
  extern dl_collection_settings default_linked_list_collection_settings;

  typedef struct {
    dl_collection_settings settings;

    union {
      struct {
        dl_vector container;
        dl_natural index[2];
      } dl_vector;
      struct {
        dl_linked_list container;
      } dl_linked_list;
    } data;
  } dl_collection;
  
  dl_api dl_integer dl_iterator_compare(const dl_collection *col, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_iterator_equal(const dl_collection *col, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_iterator_is_valid(const dl_collection *col, dl_iterator index);
  dl_api dl_iterator dl_make_invalid_iterator(const dl_collection *col);

  dl_api dl_collection *dl_init_collection(dl_collection *col, dl_collection_type type, dl_storage_type storage, dl_comparator *compare, dl_handler *destructor, dl_natural element_size);
  dl_api dl_collection *dl_init_collection_custom(dl_collection *col, dl_collection_settings *settings, dl_natural element_size);

  dl_api dl_collection *dl_init_collection_array(dl_collection *col, dl_collection_type type, dl_comparator *comp, dl_handler *destruct_entry, dl_byte * data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_collection_element_size(const dl_collection *col);

  dl_api void dl_destroy_collection(dl_collection *col);

  dl_api dl_bool dl_collection_is_set(const dl_collection *col);
  dl_api dl_bool dl_collection_is_sorted(const dl_collection *col);
  dl_api dl_bool dl_collection_is_queue(const dl_collection *col);
  dl_api dl_bool dl_collection_is_vector(const dl_collection *col);
  dl_api dl_bool dl_collection_is_linked_list(const dl_collection *col);

  dl_api dl_integer dl_collection_copy(const dl_collection *original, dl_collection *target);
  dl_api dl_integer dl_collection_copy_array(const dl_any data, dl_natural count, dl_collection *target);

  dl_api dl_iterator dl_collection_begin(const dl_collection *col);
  dl_api dl_iterator dl_collection_end(const dl_collection *col);

  dl_api dl_any dl_collection_pop(dl_collection *col, dl_any out);
  dl_api dl_bool dl_collection_pop_forget(dl_collection *col);
  dl_api dl_bool dl_collection_pop_destroy(dl_collection *col);

  dl_api dl_any dl_collection_push(dl_collection *col, const dl_any value);
  dl_api dl_any dl_collection_push_index(dl_collection *col, const dl_any value, dl_iterator *out_index);
  dl_api dl_any dl_collection_push_start(dl_collection *col, dl_iterator *iter);
  dl_api dl_any dl_collection_push_finish(dl_collection *col, dl_iterator *iter);

  dl_api dl_any dl_collection_begin_ref(const dl_collection *col, dl_iterator *iter);
  dl_api dl_any dl_collection_end_ref(const dl_collection *col, dl_iterator *iter);

  dl_api dl_iterator dl_collection_index(dl_collection *col, dl_natural index);
  dl_api dl_iterator dl_collection_index_of(const dl_collection *col, const dl_any item);

  dl_api dl_any dl_collection_next(const dl_collection *col, dl_iterator *iter);
  dl_api dl_any dl_collection_prev(const dl_collection *col, dl_iterator *iter);

  dl_api dl_any dl_collection_find(const dl_collection *col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_any dl_collection_find_last(const dl_collection *col, dl_filter *predicate, dl_iterator *iter);

  dl_api dl_any dl_collection_foldl(const dl_collection *col, dl_any initial, dl_folder *func);
  dl_api dl_any dl_collection_foldr(const dl_collection *col, dl_any initial, dl_folder *func);
  dl_api dl_any dl_collection_search(const dl_collection *col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_any dl_collection_search_region(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter);

  dl_api dl_any dl_collection_get(const dl_collection *col, dl_iterator index, dl_any out);
  dl_api dl_any dl_collection_insert(dl_collection *col, dl_iterator *index, dl_any item);
  dl_api dl_any dl_collection_peek(const dl_collection *col);
  dl_api dl_any dl_collection_ref(const dl_collection *col, dl_iterator iter);
  dl_api dl_any dl_collection_remove_at(dl_collection *col, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_remove_first(dl_collection *col, dl_filter *predicate, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_remove_last(dl_collection *col, dl_filter *predicate, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_set(dl_collection *col, dl_iterator *index, dl_any value);

  dl_api dl_bool dl_collection_all(const dl_collection *col, dl_filter *func);
  dl_api dl_bool dl_collection_any(const dl_collection *col, dl_filter *func);
  dl_api dl_bool dl_collection_contains(const dl_collection *col, const dl_any item);
  dl_api dl_bool dl_collection_is_empty(const dl_collection *col);

  dl_api dl_integer dl_collection_map(const dl_collection *col, dl_converter *func, dl_collection *out);
  dl_api dl_integer dl_collection_zip(const dl_collection *col1, const dl_collection *col2, dl_zipper *zip, dl_collection *out);

  dl_api dl_bool dl_collection_forget_at(dl_collection *col, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_at(dl_collection *col, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_first(dl_collection *col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_last(dl_collection *col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_bool dl_collection_swap(dl_collection *col, dl_iterator *iter_a, dl_iterator *iter_b);
  dl_api dl_bool dl_collection_quick_sort(dl_collection *col, dl_comparator *compare);
  dl_api dl_bool dl_collection_quick_sort_region(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_collection_sort(dl_collection *col);

  dl_api void dl_collection_clear(dl_collection *col);

  dl_api dl_integer dl_collection_count(const dl_collection *col);
  dl_api dl_integer dl_collection_find_all(const dl_collection *col, dl_filter *predicate, dl_collection *out);

  dl_api dl_integer dl_collection_take(dl_collection *col, dl_natural count, dl_collection *out);
  dl_api dl_integer dl_collection_destroy_all(dl_collection *col, dl_filter *predicate);
  dl_api dl_integer dl_collection_destroy_range(dl_collection *col, dl_iterator *index, dl_natural count);
  dl_api dl_integer dl_collection_drop(dl_collection *col, dl_natural count);
  dl_api dl_integer dl_collection_ref_array(dl_collection *col, dl_iterator index, dl_any *out_array);
  dl_api dl_integer dl_collection_remove_all(dl_collection *col, dl_filter *predicate, dl_collection *out);
  dl_api dl_integer dl_collection_remove_range(dl_collection *col, dl_iterator *index, dl_natural count, dl_collection *out);

  dl_api dl_bool dl_collection_compact(dl_collection *col);

#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION
/*****************************************************************************
 **  Iterators
 ****************************************************************************/

dl_api dl_inline dl_integer dl_iterator_compare(const dl_collection *col, dl_iterator left, dl_iterator right) {
  dl_natural left_valid, right_valid;

  left_valid = (dl_natural)dl_iterator_is_valid(col, left);
  right_valid = (dl_natural)dl_iterator_is_valid(col, right);

  switch(left_valid + right_valid)
  {
    case 0:
      return 0;
    case 1:
      return left_valid + -right_valid;
    default:
      switch (col->settings.storage)
      {
        case DL_STORAGE_TYPE_LINKED_LIST:
          return (dl_integer)(left.dl_linked_list.node - right.dl_linked_list.node);
        case DL_STORAGE_TYPE_VECTOR:
          return left.dl_vector.index - right.dl_vector.index;
        default:
          return 0;
      }
  }
}

dl_api dl_inline dl_bool dl_iterator_equal(const dl_collection *col, const dl_iterator left, const dl_iterator right) {
  return 0 == dl_iterator_compare(col, left, right);
}

dl_api dl_inline dl_bool dl_iterator_is_valid(const dl_collection *col, const dl_iterator index) {
  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return index.dl_linked_list.node != NULL;
    case DL_STORAGE_TYPE_VECTOR:
      return col != NULL && index.dl_vector.index < col->data.dl_vector.index[1] && index.dl_vector.index >= col->data.dl_vector.index[0];
    default:
      return false;
  }
}

dl_api dl_inline dl_iterator dl_make_invalid_iterator(const dl_collection *col) {
  dl_iterator bad;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      bad.dl_linked_list.node = NULL;
      break;
    default:
    case DL_STORAGE_TYPE_VECTOR:
      bad.dl_vector.index = (dl_natural)DL_NATURAL_MAX;
      break;
  }

  return bad;
}



/*****************************************************************************
 **  Container Dependent
 ****************************************************************************/

dl_api dl_inline dl_bool _dl_collection_swap(dl_collection *col, dl_iterator *iter_a, dl_iterator *iter_b) {
  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_LINKED_LIST:
    {
      dl_linked_list *l = &col->data.dl_linked_list.container;
      dl_any left, right;

      if (dl_unlikely(!(left = dl_linked_list_ref(iter_a->dl_linked_list.node)) || !(right = dl_linked_list_ref(iter_b->dl_linked_list.node))))
        return false;

      return NULL != dl_memory_swap(left, right, l->element_size);
    }
  case DL_STORAGE_TYPE_VECTOR:
    {
      dl_any left, right;
      dl_vector *v = &col->data.dl_vector.container;

      if (dl_unlikely(!(left = dl_vector_ref(v, iter_a->dl_vector.index)) || !(right = dl_vector_ref(v, iter_b->dl_vector.index))))
        return false;

      return NULL != dl_memory_swap(left, right, v->element_size);
    }
  default:
      return false;
  }
}

dl_api dl_bool dl_collection_swap(dl_collection *col, dl_iterator *iter_a, dl_iterator *iter_b) {
  if (dl_safety(col == NULL || iter_a == NULL || iter_b == NULL || !dl_iterator_is_valid(col, *iter_a) || !dl_iterator_is_valid(col, *iter_b)))
    return false;

  return _dl_collection_swap(col, iter_a, iter_b);
}

dl_api void _force_collection_properties(dl_collection *col) {
  dl_iterator current, next;
  dl_any ref;
  
  if (dl_collection_is_sorted(col))
    dl_collection_sort(col);
  if (dl_collection_is_set(col)) {
    for (ref = dl_collection_begin_ref(col, &current); ref != NULL; ref = dl_collection_next(col, &current)) {
      next = current;
      if (!dl_collection_next(col, &next))
        break;
      while (dl_iterator_is_valid(col, next) && 0 == col->settings.comparer.func(col->settings.comparer.data, ref, dl_collection_ref(col, next)))
        dl_collection_destroy_at(col, &next);
    }
  }
}

dl_api dl_any dl_collection_push_start(dl_collection *col, dl_iterator *iter) {
  if (dl_safety(col == NULL))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
    {
      dl_linked_list *list;
  
      list = &col->data.dl_linked_list.container;
      if ((iter->dl_linked_list.node = dl_linked_list_add(list, col->data.dl_linked_list.container.last, NULL)))
        return dl_linked_list_ref(iter->dl_linked_list.node);
      else {
        if (dl_linked_list_grow(list) && (iter->dl_linked_list.node = dl_linked_list_add(list, col->data.dl_linked_list.container.last, NULL)))
          return dl_linked_list_ref(iter->dl_linked_list.node);
        return NULL;
      }
    }
    case DL_STORAGE_TYPE_VECTOR:
    {
      if (col->data.dl_vector.index[1] >= dl_vector_capacity(&col->data.dl_vector.container)
          && !dl_vector_grow(&col->data.dl_vector.container))
        return NULL;

      iter->dl_vector.index = col->data.dl_vector.index[1];
      col->data.dl_vector.index[1]++;

      return dl_vector_ref(&col->data.dl_vector.container, iter->dl_vector.index);
    }
    default:
      return NULL;
  }
}

dl_api dl_any dl_collection_push_finish(dl_collection *col, dl_iterator *iter) {
  dl_comparator *comp;
  dl_bool is_set;
  dl_any ref, prev_ref;
  dl_iterator prev, t;
  dl_integer c;

  if (dl_safety(col == NULL || iter == NULL || !dl_iterator_is_valid(col, *iter)))
    return NULL;

  if (dl_collection_is_sorted(col)) {
    comp = &col->settings.comparer;
    is_set = dl_collection_is_set(col);

    while (dl_iterator_is_valid(col, *iter)) {
      ref = dl_collection_ref(col, *iter);
      if (ref == NULL)
        break;

      prev = *iter;
      prev_ref = dl_collection_prev(col, &prev);

      if (prev_ref == NULL)
        break;

      c = comp->func(comp->data, prev_ref, ref);

      if (c < 0)
        break;

      if (is_set && c == 0) {
        dl_collection_destroy_at(col, iter);
        return NULL;
      }

      _dl_collection_swap(col, &prev, iter);
      t = prev;
      prev = *iter;
      *iter = t;
    }
  }

  return dl_collection_ref(col, *iter);
}

dl_api void _vector_queue_roll_slices(dl_collection *col) {
  dl_byte *slice_left, *slice_first;
  dl_natural last_idx, slice_idx;
  
  if (col->data.dl_vector.index[0] == col->data.dl_vector.index[1]) {
    col->data.dl_vector.index[0] = col->data.dl_vector.index[1] = 0;
    return;
  }

  if (col->data.dl_vector.container.slice_count <= 1)
    return;

  if (col->data.dl_vector.index[0] < col->data.dl_vector.container.slice_length)
    return;

  slice_left = col->data.dl_vector.container.data.slices[0];
  slice_first = slice_left;

  last_idx = col->data.dl_vector.container.slice_count - 1;

  for (slice_idx = 0; slice_idx < last_idx; ++slice_idx)
    col->data.dl_vector.container.data.slices[slice_idx] = col->data.dl_vector.container.data.slices[slice_idx + 1];

  col->data.dl_vector.container.data.slices[last_idx] = slice_first;

  col->data.dl_vector.index[0] -= col->data.dl_vector.container.slice_length;
  col->data.dl_vector.index[1] -= col->data.dl_vector.container.slice_length;
}

dl_api dl_inline dl_bool dl_collection_is_empty(const dl_collection *col) {
  if (dl_safety(col == NULL))
    return true;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return col->data.dl_linked_list.container.first == NULL;
    case DL_STORAGE_TYPE_VECTOR:
      return col->data.dl_vector.index[0] == col->data.dl_vector.index[1];
    default:
      return false;
  }
}

dl_api dl_any dl_collection_pop(dl_collection *col, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        dl_linked_list *list;
        struct dl_linked_list_node *position;

        list = &col->data.dl_linked_list.container;
        
        if (dl_collection_is_queue(col))
          position = list->first;
        else
          position = list->last;

        return dl_linked_list_remove(list, position, out);
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_vector *v;
        dl_natural idx;

        if (dl_collection_is_queue(col)) {
          v = &col->data.dl_vector.container;
          idx = col->data.dl_vector.index[0];

          if (dl_unlikely(!dl_vector_get(v, idx, out)))
            return NULL;

          col->data.dl_vector.index[0]++;

          _vector_queue_roll_slices(col);

          return out;
        }
        else {
          v = &col->data.dl_vector.container;
          idx = dl_collection_end(col).dl_vector.index - 1;

          col->data.dl_vector.index[1] = idx;
          return dl_vector_get(v, idx, out);
        }
      }
    default:
      return NULL;
  }
}

dl_api dl_bool dl_collection_pop_destroy(dl_collection *col) {
  if (dl_safety(col == NULL))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  switch (col->settings.storage)
    {
      case DL_STORAGE_TYPE_LINKED_LIST:
      {
        dl_linked_list *list;
        struct dl_linked_list_node *position;

        list = &col->data.dl_linked_list.container;
        
        if (dl_collection_is_queue(col))
          position = list->first;
        else
          position = list->last;

        return dl_linked_list_destroy(list, position, &col->settings.deconstruct_entry);
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_natural index;
        dl_any ref;
        
        if (dl_collection_is_queue(col)) {
          index = col->data.dl_vector.index[0];

          if (col->settings.deconstruct_entry.func != NULL) {
            if (!(ref = dl_vector_ref(&col->data.dl_vector.container, index)))
              return false;

            col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
          }

          col->data.dl_vector.index[0]++;

          _vector_queue_roll_slices(col);

          return true;
        }
        else {
          index = dl_collection_end(col).dl_vector.index - 1;

          if (col->settings.deconstruct_entry.func != NULL) {
            if (!(ref = dl_vector_ref(&col->data.dl_vector.container, index)))
              return false;

            col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
          }

          col->data.dl_vector.index[1] = index;

          return true;
        }
      }
    default:
      return false;
  }
}

dl_api dl_bool dl_collection_pop_forget(dl_collection *col) {
  if (dl_safety(col == NULL))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        dl_linked_list *list;
        struct dl_linked_list_node *position;

        list = &col->data.dl_linked_list.container;
        
        if (dl_collection_is_queue(col))
          position = list->first;
        else
          position = list->last;

        return dl_linked_list_destroy(list, position, NULL);
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_natural index;
        
        if (dl_collection_is_queue(col)) {
          col->data.dl_vector.index[0]++;

          _vector_queue_roll_slices(col);

          return true;
        }
        else {
          index = dl_collection_end(col).dl_vector.index - 1;

          col->data.dl_vector.index[1] = index;

          return true;
        }      
      }
    default:
      return false;
  }
}

dl_api dl_integer dl_collection_ref_array(dl_collection *col, dl_iterator iter, dl_any *out_array) {  
  if (dl_safety(col == NULL || out_array == NULL))
    return 0;

  if (!dl_iterator_is_valid(col, iter))
    return 0;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      out_array[0] = dl_linked_list_ref(iter.dl_linked_list.node);
      return 1;
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_natural count, limit;
      
        count = dl_vector_ref_array(&col->data.dl_vector.container, iter.dl_vector.index, out_array);
        limit = col->data.dl_vector.index[1] - iter.dl_vector.index;

        return count < limit ? count : limit;
      }
    default:
      return 0;
  }
}

dl_api dl_inline dl_any dl_collection_ref(const dl_collection *col, dl_iterator iter) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, iter))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return dl_linked_list_ref(iter.dl_linked_list.node);
    case DL_STORAGE_TYPE_VECTOR:
      return dl_vector_ref(&col->data.dl_vector.container, iter.dl_vector.index);
    default:
      return NULL;
  }
}

dl_any dl_collection_get(const dl_collection *col, dl_iterator iter, dl_any out) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, iter))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return dl_linked_list_get(&col->data.dl_linked_list.container, iter.dl_linked_list.node, out);
    case DL_STORAGE_TYPE_VECTOR:
      return dl_vector_get(&col->data.dl_vector.container, iter.dl_vector.index, out);
    default:
      return NULL;
  }
}

dl_any dl_collection_set(dl_collection *col, dl_iterator *iter, dl_any value) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, *iter))
    return NULL;

  if (dl_collection_is_set(col) && dl_collection_contains(col, value))
    return NULL;

  if (dl_collection_is_sorted(col)) {
    dl_collection_forget_at(col, iter);
    return dl_collection_push_index(col, value, iter);
  }

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
    {
      dl_any ref;

      if ((ref = dl_linked_list_set(&col->data.dl_linked_list.container, iter->dl_linked_list.node, value)))
        return dl_collection_ref(col, *iter);
      return NULL;  
    }
    case DL_STORAGE_TYPE_VECTOR:
    {
      dl_any ref;

      if ((ref = dl_vector_set(&col->data.dl_vector.container, iter->dl_vector.index, value)))
        return dl_collection_ref(col, *iter);
      return NULL;  
    }
    default:
      return NULL;
  }
}

dl_api dl_iterator dl_collection_index(dl_collection *col, dl_natural index) {
  if (dl_safety(col == NULL) || (dl_integer)index >= dl_collection_count(col))
    return dl_collection_end(col);

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
    {
      dl_iterator iter;
      iter.dl_linked_list.node = dl_linked_list_index(&col->data.dl_linked_list.container, index);
      return iter;  
    }
    case DL_STORAGE_TYPE_VECTOR:
    {
      dl_iterator iter;
      iter.dl_vector.index = index;
      return iter;
    }
    default:
      return dl_make_invalid_iterator(col);
  }
}

dl_api dl_inline dl_any dl_collection_next(const dl_collection *col, dl_iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL)) {
    *iter = dl_make_invalid_iterator(col);
    return NULL;
  }

  if (!dl_iterator_is_valid(col, *iter)) {
    *iter = dl_make_invalid_iterator(col);
    return NULL;
  }

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      iter->dl_linked_list.node = iter->dl_linked_list.node->next;
      break;
    case DL_STORAGE_TYPE_VECTOR:
      iter->dl_vector.index++;
      break;
    default:
      break;
  }

  return dl_collection_ref(col, *iter);
}

dl_api dl_any dl_collection_prev(const dl_collection *col, dl_iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        if (!dl_iterator_is_valid(col, *iter))
          iter->dl_linked_list.node = col->data.dl_linked_list.container.last;
        else if (iter->dl_linked_list.node != col->data.dl_linked_list.container.first)
          iter->dl_linked_list.node = iter->dl_linked_list.node->previous;
        else
          *iter = dl_make_invalid_iterator(col);  
      }
      break;
    case DL_STORAGE_TYPE_VECTOR:
      {
        if (!dl_iterator_is_valid(col, *iter))
          iter->dl_vector.index = col->data.dl_vector.index[1] - 1;
        else if (iter->dl_vector.index > col->data.dl_vector.index[0])
          iter->dl_vector.index--;
        else
          *iter = dl_make_invalid_iterator(col);  
      }
      break;
    default:
      return NULL;
  }
  
  return dl_collection_ref(col, *iter);
}

dl_api dl_integer dl_collection_count(const dl_collection *col) {
  if (dl_safety(col == NULL))
    return 0;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return dl_linked_list_length(&col->data.dl_linked_list.container);
    case DL_STORAGE_TYPE_VECTOR:
      return col->data.dl_vector.index[1] - col->data.dl_vector.index[0];
    default:
      return -1;
  }
}

dl_api dl_iterator dl_collection_begin(const dl_collection *col) {
  dl_iterator iter;

  if (dl_safety(col == NULL))
    return dl_make_invalid_iterator(col);

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      iter.dl_linked_list.node = col->data.dl_linked_list.container.first;
      break;
    default:
    case DL_STORAGE_TYPE_VECTOR:
      iter.dl_vector.index = col->data.dl_vector.index[0];
      break;
  }

  return iter;
}

dl_api dl_iterator dl_collection_end(const dl_collection *col) {
  dl_iterator iter;

  if (dl_safety(col == NULL))
    return dl_make_invalid_iterator(col);

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      iter.dl_linked_list.node = NULL;
      break;
    default:
    case DL_STORAGE_TYPE_VECTOR:
      iter.dl_vector.index = col->data.dl_vector.index[1];
      break;
  }

  return iter;
}

const dl_any _collection_search_region_vector(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  dl_iterator iter_next;
  
  if (left.dl_vector.index >= right.dl_vector.index)
    return NULL;

  iter->dl_vector.index = ((right.dl_vector.index - left.dl_vector.index) >> 1) + left.dl_vector.index;

  if (dl_unlikely(!(ref = dl_vector_ref(&col->data.dl_vector.container, iter->dl_vector.index))))
    return NULL;

  outcome = predicate->func(predicate->data, ref);
  
  if (outcome == 0)
    return ref;
  else if (outcome > 0)
    return _collection_search_region_vector(col, predicate, left, *iter, iter);
  else {
    iter_next.dl_vector.index = iter->dl_vector.index + 1;
    return _collection_search_region_vector(col, predicate, iter_next, right, iter);
  }
}

const dl_any _collection_linear_search(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  
  ref = dl_collection_ref(col, left);
  *iter = left;
  while (!dl_iterator_equal(col, *iter, right) && ref != NULL) {
    outcome = predicate->func(predicate->data, ref);
    if (outcome == 0)
      return ref;
    ref = dl_collection_next(col, iter);
  }

  return NULL;
}

dl_any dl_collection_search_region(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  if (dl_safety(col == NULL || predicate == NULL || iter == NULL))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      return _collection_linear_search(col, predicate, left, right, iter);
    case DL_STORAGE_TYPE_VECTOR:
      if (dl_collection_is_sorted(col))
        return _collection_search_region_vector(col, predicate, left, right, iter);
      return _collection_linear_search(col, predicate, left, right, iter);
    default:
      return NULL;
  }
}

dl_any dl_collection_search(const dl_collection *col, dl_filter *predicate, dl_iterator *iter) {
  return dl_collection_search_region(col, predicate, dl_collection_begin(col), dl_collection_end(col), iter);
}

dl_api dl_bool dl_collection_destroy_at(dl_collection *col, dl_iterator *iter) {
  dl_handler *destructor;
  
  if (dl_safety(col == NULL || iter == NULL) || !dl_iterator_is_valid(col, *iter))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  destructor = col->settings.deconstruct_entry.func == NULL ? NULL : &col->settings.deconstruct_entry;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        struct dl_linked_list_node *next;
        next = iter->dl_linked_list.node->next;
        if (dl_linked_list_destroy(&col->data.dl_linked_list.container, iter->dl_linked_list.node, destructor)) {
          iter->dl_linked_list.node = next;
          return true;
        }
        return false;
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_vector *v;
        dl_natural index, swap_index;
        dl_any item;
        
        v = &col->data.dl_vector.container;
        index = iter->dl_vector.index;

        if (destructor != NULL) {
          if (dl_unlikely(!(item = dl_vector_ref(v, index))))
            return false;
          destructor->func(destructor->data, item);
        }

        if (dl_collection_count(col) > 1) {
	  for (swap_index = col->data.dl_vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
	    dl_vector_swap(v, swap_index, index);
        }

        col->data.dl_vector.index[1]--;

        return true;        
      }
    default:
      return false;
  }
}

dl_api dl_bool dl_collection_forget_at(dl_collection *col, dl_iterator *iter) {
  dl_handler destruct_entry;
  dl_bool success;

  if (dl_safety(col == NULL))
    return false;

  destruct_entry = col->settings.deconstruct_entry;
  col->settings.deconstruct_entry.func = NULL;
  success = dl_collection_destroy_at(col, iter);
  col->settings.deconstruct_entry = destruct_entry;

  return success;
}

dl_api dl_any dl_collection_remove_at(dl_collection *col, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        struct dl_linked_list_node *next;
        
        next = iter->dl_linked_list.node->next;
        if ((out = dl_linked_list_remove(&col->data.dl_linked_list.container, iter->dl_linked_list.node, out))) {
          iter->dl_linked_list.node = next;
          return out;
        }

        return NULL;        
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_vector *v;
        dl_natural index, swap_index;
        
        v = &col->data.dl_vector.container;
        index = iter->dl_vector.index;

        if (!(out = dl_vector_get(v, index, out)))
          return NULL;

        if (dl_collection_count(col) > 1)
          for (swap_index = col->data.dl_vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
            dl_vector_swap(v, swap_index, index);
        col->data.dl_vector.index[1]--;

        return out;        
      }
    default:
      return NULL;
  }
}

dl_api dl_integer dl_collection_destroy_range(dl_collection *col, dl_iterator *index, dl_natural count) {
  if (dl_safety(col == NULL || !dl_iterator_is_valid(col, *index)))
    return 0;

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        struct dl_linked_list_node *next;
        dl_natural idx;

        next = index->dl_linked_list.node;
        for (idx = 0; idx < count && next != NULL; ++idx)
          next = next->next;
        count = dl_linked_list_destroy_range(&col->data.dl_linked_list.container, index->dl_linked_list.node, count, &col->settings.deconstruct_entry);
        index->dl_linked_list.node = next;
        return count;        
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_natural idx, start, total;
        dl_handler destruct;
        dl_vector *v;
        dl_any item;
        
        idx = index->dl_vector.index;
        start = idx;
        v = &col->data.dl_vector.container;

        total = 0;
        destruct = col->settings.deconstruct_entry;

        for (item = dl_vector_ref(v, idx);
            item != NULL && total < count;
            ++idx, item = dl_vector_ref(v, idx), ++total)
          if (destruct.func != NULL)
            destruct.func(destruct.data, item);

        if (total != 0) {
          while (NULL != dl_vector_ref(v, idx)) {
            dl_vector_swap(v, start, idx);
            ++start;
            ++idx;
          };
        }

        col->data.dl_vector.index[1] -= total;

        return total;
      }
    default:
      return -1;
  }
}

dl_any dl_collection_insert(dl_collection *col, dl_iterator *position, dl_any item) {
  if (dl_safety(col == NULL || position == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, *position))
    *position = dl_collection_end(col);

  if (dl_collection_is_set(col) && dl_collection_contains(col, item))
    return NULL;

  if (dl_collection_is_sorted(col))
    return dl_collection_push_index(col, item, position);

  switch (col->settings.storage)
  {
    case DL_STORAGE_TYPE_LINKED_LIST:
      {
        struct dl_linked_list_node *node;
        dl_linked_list *list;

        node = position->dl_linked_list.node;
        list = &col->data.dl_linked_list.container;

        position->dl_linked_list.node = dl_linked_list_add(list, node, item);

        return dl_collection_ref(col, *position);    
      }
    case DL_STORAGE_TYPE_VECTOR:
      {
        dl_iterator index, next, t;
        
        index = dl_collection_end(col);

        if (!dl_collection_push(col, item))
          return NULL;

        while (!dl_iterator_equal(col, index, *position)) {
          next = index;
          dl_collection_prev(col, &next);
          _dl_collection_swap(col, &next, &index);
	  t = next;
	  next = index;
	  index = t;
        }

        return dl_collection_ref(col, *position);        
      }
    default:
      return NULL;
  }
}

dl_api dl_natural dl_collection_element_size(const dl_collection *col) {
  if (dl_safety(col == NULL))
    return 0;

  return col->settings.element_size;
}



/*****************************************************************************
 **  Container Independent
 ****************************************************************************/

dl_api dl_bool dl_collection_is_vector(const dl_collection *col) {
  return col != NULL && col->settings.storage == DL_STORAGE_TYPE_VECTOR;
}

dl_api dl_bool dl_collection_is_linked_list(const dl_collection *col) {
  return col != NULL && col->settings.storage == DL_STORAGE_TYPE_LINKED_LIST;
}

dl_api dl_bool dl_collection_is_queue(const dl_collection *col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_QUEUE);
}

dl_api dl_bool dl_collection_is_set(const dl_collection *col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_SET);
}

dl_api dl_bool dl_collection_is_sorted(const dl_collection *col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_SORTED_LIST
     || col->settings.type == DL_COLLECTION_TYPE_SET);
}

dl_api dl_any dl_collection_begin_ref(const dl_collection *col, dl_iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = dl_collection_begin(col);
  return dl_collection_ref(col, *iter);
}

dl_api dl_any dl_collection_end_ref(const dl_collection *col, dl_iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = dl_collection_end(col);
  return dl_collection_prev(col, iter);
}

dl_api dl_bool dl_collection_sort(dl_collection *col) {
  if (dl_safety(col == NULL))
    return false;

  return dl_collection_quick_sort(col, &col->settings.comparer);
}

dl_api void dl_collection_clear(dl_collection *col) {
  while (dl_collection_pop_destroy(col));
}

dl_api dl_any dl_collection_peek(const dl_collection *col) {
  dl_iterator iter;
  
  if (dl_safety(col == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  if (dl_collection_is_queue(col)) {
    iter = dl_collection_begin(col);

    return dl_collection_ref(col, iter);
  }
  else {
    iter = dl_collection_end(col);
    return dl_collection_prev(col, &iter);
  }
}

dl_api dl_iterator dl_collection_index_of(const dl_collection *col, const dl_any item) {
  dl_iterator bad, iter, index;
  dl_any candidate;
  _collection_sorted_list_predicate_data data;
  dl_filter predicate;
  
  bad = dl_make_invalid_iterator(col);

  if (dl_safety(col == NULL))
    return bad;

  if (dl_collection_is_empty(col))
    return bad;

  if (dl_collection_is_sorted(col)) {
    data.c = col->settings.comparer;
    data.item = item;

    predicate.func = _collection_sorted_list_predicate_func;
    predicate.data = &data;

    iter = dl_collection_begin(col);
    return NULL != dl_collection_search(col, &predicate, &iter) ? iter : bad;
  }
  else {
    index = dl_collection_begin(col);
    for (candidate = dl_collection_ref(col, index); candidate != NULL; candidate = dl_collection_next(col, &index)) {
      if (0 == col->settings.comparer.func(col->settings.comparer.data, item, candidate))
        return index;
    }
    return bad;
  }
}

dl_api dl_bool dl_collection_all(const dl_collection *col, dl_filter *f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
    if (!f->func(f->data, item))
      return false;

  return true;
}

dl_api dl_bool dl_collection_any(const dl_collection *col, dl_filter *f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index);
       item != NULL;
       item = dl_collection_next(col, &index))
    if (f->func(f->data, item))
      return true;

  return false;
}

dl_api dl_integer dl_collection_drop(dl_collection *col, dl_natural count) {
  dl_natural removed = 0;

  if (dl_safety(col == NULL))
    return 0;

  for (; count > 0; --count, ++removed)
    if (!dl_collection_pop_destroy(col))
      return removed;

  return removed;
}

dl_api dl_integer dl_collection_map(const dl_collection *col, dl_converter *func, dl_collection *out) {
  dl_iterator iter, new_iter;
  dl_integer count;
  dl_any new_ref;
  dl_any ref;
  
  if (dl_safety(col == NULL || func == NULL || out == NULL))
    return 0;

  iter = dl_make_invalid_iterator(col);
  new_iter = dl_make_invalid_iterator(out);

  count = 0;
  for (ref = dl_collection_begin_ref(col, &iter); ref != NULL; ref = dl_collection_next(col, &iter)) {
    new_ref = dl_collection_push_start(out, &new_iter);
    if (!dl_iterator_is_valid(out, new_iter))
      return -count;

    func->func(func->data, ref, new_ref);

    dl_collection_push_finish(out, &new_iter);
    ++count;
  }

  return count;
}

dl_api dl_any dl_collection_foldl(const dl_collection *col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_any dl_collection_foldr(const dl_collection *col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = dl_collection_end(col);

  while ((item = dl_collection_prev(col, &index)))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_integer dl_collection_zip(const dl_collection *col1, const dl_collection *col2, dl_zipper *zip, dl_collection *out) {
  dl_iterator iter1, iter2, new_iter;
  dl_integer added;
  dl_any ref1, ref2, new_ref;
  
  if (dl_safety(col1 == NULL || col2 == NULL || zip == NULL || out == NULL))
    return 0;

  iter1 = dl_make_invalid_iterator(col1);
  iter2 = dl_make_invalid_iterator(col2);
  new_iter = dl_make_invalid_iterator(out);

  added = 0;
  for (ref1 = dl_collection_begin_ref(col1, &iter1), ref2 = dl_collection_begin_ref(col2, &iter2);
       ref1 != NULL && ref2 != NULL;
       ref1 = dl_collection_next(col1, &iter1), ref2 = dl_collection_next(col2, &iter2)) {
    new_ref = dl_collection_push_start(out, &new_iter);
    if (!dl_iterator_is_valid(out, new_iter))
      return -added;

    zip->func(zip->data, ref1, ref2, new_ref);

    dl_collection_push_finish(out, &new_iter);
    ++added;
  }

  return added;
}

dl_api dl_integer dl_collection_take(dl_collection *col, dl_natural count, dl_collection *out) {
  dl_any ref;
  dl_natural added;
  if (dl_safety(col == NULL || out == NULL))
    return 0;

  added = 0;
  while ((ref = dl_collection_peek(col)) && count > added) {
    if (!(ref = dl_collection_push(out, ref)))
      break;
    dl_collection_pop_forget(col);
    ++added;
  }

  return added;
}

dl_api dl_any dl_collection_find(const dl_collection *col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  for (item = dl_collection_ref(col, *index); item != NULL; item = dl_collection_next(col, index))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_any dl_collection_find_last(const dl_collection *col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  while ((item = dl_collection_prev(col, index)))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_integer dl_collection_find_all(const dl_collection *col, dl_filter *f, dl_collection *out) {
  dl_natural start_count;
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || out == NULL))
    return 0;

  start_count = dl_collection_count(out);

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index)) {
    if (f->func(f->data, item) && !dl_collection_push(out, item))
      break;
  }

  return dl_collection_count(out) - start_count;
}

dl_api dl_any dl_collection_remove_first(dl_collection *col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_collection_find(col, f, iter))
    return NULL;

  return dl_collection_remove_at(col, iter, out);
}

dl_api dl_any dl_collection_remove_last(dl_collection *col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_collection_find_last(col, f, iter))
    return NULL;

  return dl_collection_remove_at(col, iter, out);
}

dl_api dl_bool dl_collection_destroy_first(dl_collection *col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_collection_find(col, f, index))
    return false;

  return dl_collection_destroy_at(col, index);
}

dl_api dl_bool dl_collection_destroy_last(dl_collection *col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_collection_find_last(col, f, index))
    return false;

  return dl_collection_destroy_at(col, index);
}

dl_iterator _dl_collection_quick_sort_partition(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  dl_iterator i, j, high;
  dl_any pivot_ref, ref_j;
  
  high = right;
  pivot_ref = dl_collection_prev(col, &high);

  ref_j = dl_collection_ref(col, left);
  for (i = j = left; !dl_iterator_equal(col, high, j); ref_j = dl_collection_next(col, &j)) {
    if (compare->func(compare->data, ref_j, pivot_ref)) {
      _dl_collection_swap(col, &i, &j);
      dl_collection_next(col, &i);
    }
  }
  _dl_collection_swap(col, &i, &high);
  return i;
}

void _dl_collection_quick_sort_region(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  dl_iterator pivot;

  if (dl_iterator_equal(col, left, right))
    return;

  pivot = _dl_collection_quick_sort_partition(col, compare, left, right);
  
  if (!dl_iterator_equal(col, left, pivot))
    _dl_collection_quick_sort_region(col, compare, left, pivot);
  
  if (dl_collection_next(col, &pivot) && !dl_iterator_equal(col, pivot, right))
    _dl_collection_quick_sort_region(col, compare, pivot, right);
} 

dl_api dl_bool dl_collection_quick_sort_region(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  if (dl_safety(col == NULL || compare == NULL || compare->func == NULL))
    return false;
  if (!dl_iterator_is_valid(col, left))
    return false;
  if (dl_iterator_equal(col, left, right))
    return true;

  _dl_collection_quick_sort_region(col, compare, left, right);
  
  return true;
}

dl_api dl_bool dl_collection_quick_sort(dl_collection *col, dl_comparator *compare) {
  if (dl_safety(col == NULL || compare == NULL || compare->func == NULL))
    return false;
  _dl_collection_quick_sort_region(col, compare, dl_collection_begin(col), dl_collection_end(col));
  return true;
}

dl_api dl_integer dl_collection_destroy_all(dl_collection *col, dl_filter *f) {
  dl_natural count;
  dl_any item;
  dl_iterator iter;
  
  if (dl_safety(col == NULL || f == NULL))
    return 0;

  if (dl_collection_is_empty(col))
    return 0;

  count = 0;
  iter = dl_collection_begin(col);

  while ((item = dl_collection_ref(col, iter))) {
    if (f->func(f->data, item)) {
      dl_collection_destroy_at(col, &iter);
      count++;
    }
    else
      dl_collection_next(col, &iter);
  }

  return count;
}

dl_api dl_bool dl_collection_contains(const dl_collection *col, dl_any item) {
  dl_iterator iter = dl_collection_index_of(col, item);
  return dl_iterator_is_valid(col, iter);
}

dl_api dl_any dl_collection_push(dl_collection *col, dl_any value) {
  dl_iterator iter = dl_make_invalid_iterator(col);
  return dl_collection_push_index(col, value, &iter);
}

dl_api dl_any dl_collection_push_index(dl_collection *col, dl_any value, dl_iterator *out_index) {
  dl_any ref;
  
  ref = dl_collection_push_start(col, out_index);
  if (!dl_iterator_is_valid(col, *out_index))
    return NULL;

  dl_memory_copy(ref, value, dl_collection_element_size(col));
  return dl_collection_push_finish(col, out_index);
}

dl_api dl_integer dl_collection_remove_range(dl_collection *col, dl_iterator *index, dl_natural count, dl_collection *out) {
  dl_natural total;
  dl_iterator new_iter;
  dl_any ref;
  
  if (dl_safety(col == NULL || out == NULL || index == NULL || !dl_iterator_is_valid(col, *index)))
    return 0;

  total = 0;
  while (total < count && dl_iterator_is_valid(col, *index)) {
    ref = dl_collection_push_start(out, &new_iter);
    if (!ref)
      break;

    if (!dl_collection_remove_at(col, index, ref)) {
      dl_collection_forget_at(col, &new_iter);
      break;
    }
    else
      dl_collection_push_finish(out, &new_iter);
    ++total;
  }

  return total;
}


dl_api dl_integer dl_collection_remove_all(dl_collection *col, dl_filter *f, dl_collection *out) {
  dl_iterator index, new_iter;
  dl_natural total;
  dl_any ref, new_ref;
  
  if (dl_safety(col == NULL || out == NULL || f == NULL))
    return 0;

  index = dl_make_invalid_iterator(col);
  new_iter = dl_make_invalid_iterator(out);

  total = 0;
  for (ref = dl_collection_begin_ref(col, &index); dl_iterator_is_valid(col, index);) {
    if (f->func(f->data, ref)) {
      new_ref = dl_collection_push_start(out, &new_iter);
      if (!ref)
        break;

      if (!dl_collection_remove_at(col, &index, new_ref)) {
        dl_collection_forget_at(col, &new_iter);
        break;
      }
      else
        dl_collection_push_finish(out, &new_iter);

      ref = dl_collection_ref(col, index);
      ++total;
    }
    else
      ref = dl_collection_next(col, &index);
  }

  return total;
}

dl_api dl_integer dl_collection_copy(const dl_collection *original, dl_collection *target) {
  dl_iterator iter, new_iter;
  dl_integer count;
  dl_any ref, new_ref;
  dl_bool is_set;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  is_set = dl_collection_is_set(target);

  count = 0;
  for (ref = dl_collection_begin_ref(original, &iter); ref != NULL; ref = dl_collection_next(original, &iter)) {
    if (is_set && dl_collection_contains(target, ref))
      continue;

    new_ref = dl_collection_push_start(target, &new_iter);
    if (!new_ref) {
      count = -count;
      break;
    }

    ref = dl_collection_ref(original, iter);

    dl_memory_copy(new_ref, ref, dl_collection_element_size(original));
    dl_collection_push_finish(target, &new_iter);

    ++count;
  }

  return count;
}

dl_api dl_integer dl_collection_copy_array(const dl_any data, dl_natural count, dl_collection *target) {
  dl_collection source;

  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (dl_unlikely(!dl_init_collection_array(&source, DL_COLLECTION_TYPE_LIST, NULL, NULL, data, dl_collection_element_size(target), count)))
    return 0;

  return dl_collection_copy(&source, target);
}


/*****************************************************************************
 **  Init/Destroy Collections
 ****************************************************************************/

dl_collection_settings default_vector_collection_settings = {
  DL_COLLECTION_TYPE_LIST,
  DL_STORAGE_TYPE_VECTOR,
  {0},
  {0},
  32,
  sizeof(dl_byte)
};

dl_collection_settings default_linked_list_collection_settings = {
  DL_COLLECTION_TYPE_LIST,
  DL_STORAGE_TYPE_LINKED_LIST,
  {0},
  {0},
  32,
  sizeof(dl_byte)
};

void _check_init_collection(dl_collection *col, dl_collection_settings *settings, dl_natural count) {
  col->settings = *settings;
  
  if (col->settings.comparer.func == NULL) {
    switch (col->settings.element_size) {
    case 1:
      col->settings.comparer.func = _default_compare_8;
      break;
    case 2:
      col->settings.comparer.func = _default_compare_16;
      break;
    case 4:
      col->settings.comparer.func = _default_compare_32;
      break;
    case 8:
      col->settings.comparer.func = _default_compare_64;
      break;
    default:
      col->settings.comparer.func = _default_compare_any;
      break;
    }
  }

  if (col->settings.storage == DL_STORAGE_TYPE_VECTOR) {
    col->data.dl_vector.index[0] = 0;
    col->data.dl_vector.index[1] = count;
  }
}

dl_api dl_collection *dl_init_collection(dl_collection *col, dl_collection_type type, dl_storage_type storage, dl_comparator *compare, dl_handler *destructor, dl_natural element_size) {
  dl_collection_settings settings;
  
  if (dl_safety(col == NULL || element_size < 1))
    return NULL;

  switch (storage) {
  case DL_STORAGE_TYPE_VECTOR: {
    settings = default_vector_collection_settings;
    break;
  }
  case DL_STORAGE_TYPE_LINKED_LIST: {
    settings = default_linked_list_collection_settings;
    break;
  }
  default:
    return NULL;
  }

  settings.type = type;
  settings.element_size = element_size;
  
  if (destructor != NULL)
    settings.deconstruct_entry = *destructor;
  else
    settings.deconstruct_entry.func = NULL;
  if (compare != NULL)
    settings.comparer = *compare;
  else
    settings.comparer.func = NULL;

  _check_init_collection(col, &settings, 0);
  
  switch (settings.storage) {
  case DL_STORAGE_TYPE_VECTOR: {
    if (!dl_init_vector(&col->data.dl_vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  case DL_STORAGE_TYPE_LINKED_LIST: {
    if (!dl_init_linked_list(&col->data.dl_linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  default:
    return NULL;
  }
  
  _force_collection_properties(col);

  return col;
}

dl_api dl_collection *dl_init_collection_custom(dl_collection *col, dl_collection_settings *settings, dl_natural element_size) {
  if (dl_safety(col == NULL))
    return NULL;

  _check_init_collection(col, settings, 0);

  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_VECTOR:
    if (!dl_init_vector(&col->data.dl_vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  case DL_STORAGE_TYPE_LINKED_LIST:
    if (!dl_init_linked_list(&col->data.dl_linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  default:
    return NULL;
  }

  _force_collection_properties(col);

  return col;
}

dl_api dl_collection *dl_init_collection_array(dl_collection *col, dl_collection_type type, dl_comparator *comp, dl_handler *deconstruct_entry, dl_byte *data, dl_natural element_size, dl_natural count) {
  dl_collection_settings settings;
  
  if (dl_safety(col == NULL || data == NULL))
    return NULL;

  if (!dl_init_vector_array(&col->data.dl_vector.container, data, element_size, count))
    return NULL;

  settings = default_vector_collection_settings;
  settings.storage = DL_STORAGE_TYPE_VECTOR;
  settings.type = type;
  settings.element_size = element_size;
  
  if (comp != NULL)
    settings.comparer = *comp;
  else
    settings.comparer.func = NULL;
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    settings.deconstruct_entry = *deconstruct_entry;
  else
    settings.deconstruct_entry.func = NULL;

  _check_init_collection(col, &settings, count);

  _force_collection_properties(col);

  return col;
}

dl_api void dl_destroy_collection(dl_collection *col) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL))
    return;

  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_VECTOR:
    if (col->settings.deconstruct_entry.func != NULL) {
      index = dl_collection_begin(col);
      for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
	      col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, item);
    }

    dl_destroy_vector(&col->data.dl_vector.container, NULL);
    break;
  case DL_STORAGE_TYPE_LINKED_LIST:
    dl_destroy_linked_list(&col->data.dl_linked_list.container, &col->settings.deconstruct_entry);
    break;
  }
}

dl_api dl_bool dl_collection_compact(dl_collection *col) {
  if (dl_safety(col == NULL))
    return false;

  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_VECTOR:
    return dl_vector_resize(&col->data.dl_vector.container, col->data.dl_vector.index[1], &col->settings.deconstruct_entry);
  case DL_STORAGE_TYPE_LINKED_LIST:
    return dl_linked_list_resize(&col->data.dl_linked_list.container, dl_linked_list_length(&col->data.dl_linked_list.container), &col->settings.deconstruct_entry);
  }

  return false;
}
#endif /* DL_IMPLEMENTATION */

#endif
