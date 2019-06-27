#ifndef DL_ALGORITHMS_H
#define DL_ALGORITHMS_H 1

#include "dl.h"
#include "dl_containers.h"



#ifdef __cplusplus
extern "C" {
#endif
  dl_api dl_integer dl_count(dl_iterator left, dl_iterator right);
  
  dl_api dl_iterator dl_find(dl_iterator start, dl_filter predicate);
  dl_api dl_iterator dl_find_last(dl_iterator start, dl_filter predicate);
  dl_api dl_iterator dl_find_region(dl_iterator left, dl_iterator right, dl_filter predicate);
  dl_api dl_integer dl_find_all(dl_iterator start, dl_filter predicate, dl_container *out);

  dl_api dl_any dl_foldl(dl_any initial, dl_folder func);
  dl_api dl_any dl_foldr(dl_any initial, dl_folder func);

  dl_api dl_bool dl_all(dl_filter func);
  dl_api dl_bool dl_any(dl_filter func);
  
  dl_api dl_bool dl_contains(const dl_container *con, const dl_any item);
  dl_api dl_bool dl_is_empty(const dl_container *con);

  dl_api dl_integer dl_map(dl_iterator start, dl_handler func, dl_container *out);
  dl_api dl_integer dl_zip(dl_iterator start1, dl_iterator start2, dl_zipper *zip, dl_container *out);

  dl_api dl_bool dl_quick_sort(dl_container *col, dl_comparator *compare);
  dl_api dl_bool dl_quick_sort_region(dl_iterator left, dl_iterator right, dl_comparator *compare);
  dl_api dl_bool dl_sort(dl_container *col);
  
  dl_api dl_integer dl_take(dl_iterator start, dl_natural count, dl_container *out);
  dl_api dl_integer dl_drop(dl_iterator start, dl_natural count);

  dl_api dl_any dl_remove_first(dl_iterator start, dl_filter predicate, dl_any out);
  dl_api dl_any dl_remove_last(dl_iterator start, dl_filter predicate, dl_any out);
  
  dl_api dl_integer dl_remove_all(dl_iterator start, dl_filter predicate, dl_container *out);
  dl_api dl_integer dl_remove_count(dl_iterator start, dl_natural count, dl_container *out);
 
#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

const dl_any _binary_search(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter);

const dl_any _linear_search(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter);

dl_any dl_search_region(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  if (dl_safety(col == NULL || predicate == NULL || iter == NULL))
    return NULL;

  switch (col->container.type)
  {
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return _linear_search(col, predicate, left, right, iter);
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_is_sorted(col))
        return _binary_search(col, predicate, left, right, iter);
      return _linear_search(col, predicate, left, right, iter);
    default:
      return NULL;
  }
}

const dl_any _linear_search(const dl_collection *col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  
  ref = dl_iterator_ref(left);
  *iter = left;
  while (!dl_iterator_equal(*iter, right) && ref != NULL) {
    outcome = predicate->func(predicate->data, ref);
    if (outcome == 0)
      return ref;
    ref = dl_next_ref(col, iter);
  }

  return NULL;
}

dl_any dl_search(const dl_collection *col, dl_filter *predicate, dl_iterator *iter) {
  return dl_search_region(col, predicate, dl_begin(col), dl_end(col), iter);
}


dl_api dl_bool dl_all(const dl_collection *col, dl_filter *f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_begin(col);
  for (item = dl_iterator_ref(index); item != NULL; item = dl_next_ref(col, &index))
    if (!f->func(f->data, item))
      return false;

  return true;
}

dl_api dl_bool dl_any(const dl_collection *col, dl_filter *f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_begin(col);
  for (item = dl_iterator_ref(index);
       item != NULL;
       item = dl_next_ref(col, &index))
    if (f->func(f->data, item))
      return true;

  return false;
}

dl_api dl_integer dl_drop(dl_collection *col, dl_natural count) {
  dl_natural removed = 0;

  if (dl_safety(col == NULL))
    return 0;

  for (; count > 0; --count, ++removed)
    if (!dl_pop_destroy(col))
      return removed;

  return removed;
}

dl_api dl_integer dl_map(const dl_collection *col, dl_handler *func, dl_collection *out) {
  dl_iterator iter;
  dl_integer count;
  dl_any new_ref;
  dl_any ref;
  
  if (dl_safety(col == NULL || func == NULL || out == NULL))
    return 0;

  if (dl_is_set(out) || dl_is_sorted(out))
    return 0;
   
  count = 0;
  for (ref = dl_begin_ref(col, &iter); ref != NULL; ref = dl_next_ref(col, &iter)) {
    new_ref = dl_push(out, NULL);
    if (new_ref == NULL)
      return count;

    func->func(func->data, new_ref);

    ++count;
  }

  return count;
}

dl_api dl_any dl_foldl(const dl_collection *col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = dl_begin(col);
  for (item = dl_iterator_ref(index); item != NULL; item = dl_next_ref(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_any dl_foldr(const dl_collection *col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  for (item = dl_end_ref(col, &index);
       item != NULL && dl_iterator_is_valid(index);
       item = dl_prev_ref(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_integer dl_zip(const dl_collection *col1, const dl_collection *col2, dl_zipper *zip, dl_collection *out) {
  dl_iterator iter1, iter2;
  dl_integer added;
  dl_any ref1, ref2, new_ref;
  
  if (dl_safety(col1 == NULL || col2 == NULL || zip == NULL || out == NULL))
    return 0;

  if (dl_is_set(out) || dl_is_sorted(out))
    return 0;

  iter1 = dl_make_invalid_iterator(col1);
  iter2 = dl_make_invalid_iterator(col2);

  added = 0;
  for (ref1 = dl_begin_ref(col1, &iter1), ref2 = dl_begin_ref(col2, &iter2);
       ref1 != NULL && ref2 != NULL;
       ref1 = dl_next_ref(col1, &iter1), ref2 = dl_next_ref(col2, &iter2)) {

    new_ref = dl_push(out, NULL);
    if (new_ref == NULL)
      return added;
    
    zip->func(zip->data, ref1, ref2, new_ref);
    
    ++added;
  }

  return added;
}

dl_api dl_integer dl_take(dl_collection *col, dl_natural count, dl_collection *out) {
  dl_any ref;
  dl_natural added;
  if (dl_safety(col == NULL || out == NULL))
    return 0;

  added = 0;
  while ((ref = dl_peek(col)) && count > added) {
    if (!(ref = dl_push(out, ref)))
      break;
    dl_pop_forget(col);
    ++added;
  }

  return added;
}

dl_api dl_any dl_find(const dl_collection *col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  for (item = dl_iterator_ref(*index); item != NULL; item = dl_next_ref(col, index))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_any dl_find_last(const dl_collection *col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  while ((item = dl_prev_ref(col, index)))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_integer dl_find_all(const dl_collection *col, dl_filter *f, dl_collection *out) {
  dl_natural start_count;
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || out == NULL))
    return 0;

  start_count = dl_count(out);

  index = dl_begin(col);
  for (item = dl_iterator_ref(index); item != NULL; item = dl_next_ref(col, &index)) {
    if (f->func(f->data, item) && !dl_push(out, item))
      break;
  }

  return dl_count(out) - start_count;
}

dl_api dl_any dl_remove_first(dl_collection *col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_find(col, f, iter))
    return NULL;

  return dl_remove_at(col, iter, out);
}

dl_api dl_any dl_remove_last(dl_collection *col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_find_last(col, f, iter))
    return NULL;

  return dl_remove_at(col, iter, out);
}

dl_api dl_bool dl_destroy_first(dl_collection *col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_find(col, f, index))
    return false;

  return dl_destroy(col, index);
}

dl_api dl_bool dl_destroy_last(dl_collection *col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_find_last(col, f, index))
    return false;

  return dl_destroy(col, index);
}

dl_iterator _dl_quick_sort_partition(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  dl_iterator i, j, high;
  dl_any pivot_ref, ref_j;
  
  high = right;
  pivot_ref = dl_prev_ref(col, &high);

  ref_j = dl_iterator_ref(left);
  for (i = j = left; !dl_iterator_equal(high, j); ref_j = dl_next_ref(col, &j)) {
    if (compare->func(compare->data, ref_j, pivot_ref)) {
      dl_swap(col, i, j);
      dl_next_ref(col, &i);
    }
  }

  dl_swap(col, i, high);
  return i;
}

void _dl_quick_sort_region(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  dl_iterator pivot;

  if (dl_iterator_equal(left, right))
    return;

  pivot = _dl_quick_sort_partition(col, compare, left, right);
  
  if (!dl_iterator_equal(left, pivot))
    _dl_quick_sort_region(col, compare, left, pivot);
  
  if (dl_next_ref(col, &pivot) && !dl_iterator_equal(pivot, right))
    _dl_quick_sort_region(col, compare, pivot, right);
} 

dl_api dl_bool dl_quick_sort_region(dl_collection *col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  if (dl_safety(col == NULL || compare == NULL || compare->func == NULL))
    return false;
  if (!dl_iterator_is_valid(left))
    return false;
  if (dl_iterator_equal(left, right))
    return true;

  _dl_quick_sort_region(col, compare, left, right);
  
  return true;
}

dl_api dl_bool dl_quick_sort(dl_collection *col, dl_comparator *compare) {
  if (dl_safety(col == NULL || compare == NULL || compare->func == NULL))
    return false;
  _dl_quick_sort_region(col, compare, dl_begin(col), dl_end(col));
  return true;
}

dl_api dl_integer dl_destroy_all(dl_collection *col, dl_filter *f) {
  dl_natural count;
  dl_any item;
  dl_iterator iter;
  
  if (dl_safety(col == NULL || f == NULL))
    return 0;

  if (dl_is_empty(col))
    return 0;

  count = 0;
  iter = dl_begin(col);

  while ((item = dl_iterator_ref(iter))) {
    if (f->func(f->data, item)) {
      dl_destroy(col, &iter);
      count++;
    }
    else
      dl_next_ref(col, &iter);
  }

  return count;
}

dl_api dl_bool dl_contains(const dl_collection *col, dl_any item) {
  dl_iterator iter = dl_index_of(col, item);
  return dl_iterator_is_valid(iter);
}

#endif /* DL_IMPLEMENTATION */

#endif
