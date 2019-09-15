/*
 * Where left and right iterators are requested, it will span from left to right, excluding right;
 * to span a whole container start from the first and provide an invalid iterator as the right.
 *
 * When the function is stated to be "reverse" then the left/right rules are also reversed, and
 * so it will span from right to left, excluding left or until the iterator is invalid.
 */

#ifndef DL_ALGORITHMS_H
#define DL_ALGORITHMS_H 1

#include "dl_container.h"
#include "dl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

dl_api dl_integer dl_count(dl_iterator left, dl_iterator right);

dl_api dl_iterator dl_find(dl_iterator left, dl_iterator right, dl_filter predicate);
dl_api dl_iterator dl_find_reverse(dl_iterator left, dl_iterator right, dl_filter predicate);
dl_api dl_integer dl_find_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);

dl_api dl_ptr dl_foldl(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder func);
dl_api dl_ptr dl_foldr(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder func);

dl_api dl_bool dl_all(dl_iterator left, dl_iterator right, dl_filter func);
dl_api dl_bool dl_any(dl_iterator left, dl_iterator right, dl_filter func);

dl_api dl_integer dl_map(dl_iterator left, dl_iterator right, dl_handler func, dl_handler out);
dl_api dl_integer dl_zip(dl_iterator left1, dl_iterator right1, dl_iterator left2, dl_iterator right2, dl_zipper zip, dl_handler out);

dl_api dl_integer dl_take(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out);
dl_api dl_integer dl_drop(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out);

dl_api dl_bool dl_remove(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
dl_api dl_bool dl_remove_reverse(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
dl_api dl_natural dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
dl_api dl_natural dl_remove_count(dl_iterator left, dl_iterator right, dl_filter predicate, dl_natural count, dl_handler out);

dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare);

dl_api dl_handler dl_make_handler(dl_ptr data, dl_ptr (*func)(dl_ptr data, dl_ptr value));
dl_api dl_zipper dl_make_zipper(dl_ptr data, dl_ptr (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right));
dl_api dl_filter dl_make_filter(dl_ptr data, dl_integer (*func)(dl_ptr data, const dl_ptr value));
dl_api dl_folder dl_make_folder(dl_ptr data, dl_ptr (*func)(dl_ptr data, dl_ptr item, const dl_ptr left));
dl_api dl_comparator dl_make_comparator(dl_ptr data, dl_integer (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right));

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)

#include "dl_container.h"

#define ITER_UNSAFE(left, right) (!dl_iterator_is_valid(left) || !dl_iterator_is_valid(right) || left.container != right.container)
#define FUNC_UNSAFE(f) (f.func == NULL)

dl_api dl_handler dl_make_handler(dl_ptr data, dl_ptr (*func)(dl_ptr data, dl_ptr value)) {
  dl_handler h;
  h.func = func;
  h.data = data;
  return h;
}

dl_api dl_zipper dl_make_zipper(dl_ptr data, dl_ptr (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right)) {
  dl_zipper h;
  h.func = func;
  h.data = data;
  return h;
}

dl_api dl_filter dl_make_filter(dl_ptr data, dl_integer (*func)(dl_ptr data, const dl_ptr value)) {
  dl_filter h;
  h.func = func;
  h.data = data;
  return h;
}

dl_api dl_folder dl_make_folder(dl_ptr data, dl_ptr (*func)(dl_ptr data, dl_ptr item, const dl_ptr left)) {
  dl_folder h;
  h.func = func;
  h.data = data;
  return h;
}

dl_api dl_comparator dl_make_comparator(dl_ptr data, dl_integer (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right)) {
  dl_comparator h;
  h.func = func;
  h.data = data;
  return h;
}

dl_api dl_integer dl_count(dl_iterator left, dl_iterator right) {
  dl_integer count;
  count = 0;
  if (dl_safety(ITER_UNSAFE(left, right))) return count;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
    ++count;
    left = dl_iterator_next(left);
  }

  return count;
}

dl_api dl_iterator _dl_find_region_linear(dl_iterator left, dl_iterator right, dl_filter predicate) {
  while (dl_iterator_is_valid(left)) {
    if (DL_CALL1(predicate, dl_iterator_ref(left)) == 0)
      return left;
    if (dl_iterator_equal(left, right))
      break;
    left = dl_iterator_next(left);
  }
  return dl_make_invalid_iterator();
}

dl_api dl_iterator _dl_find_reverse_region_linear(dl_iterator left, dl_iterator right, dl_filter predicate) {
  while (dl_iterator_is_valid(right)) {
    if (DL_CALL1(predicate, dl_iterator_ref(right)) == 0)
      return right;
    if (dl_iterator_equal(left, right))
      break;
    right = dl_iterator_prev(right);
  }
  return dl_make_invalid_iterator();
}

dl_api dl_iterator _dl_find_region_binary(dl_iterator left, dl_iterator right, dl_filter predicate) {
  dl_integer index_middle, index_left, index_right, match, range;
  dl_iterator mid_iter;

  index_left = dl_iterator_index(left);
  if (index_left < 0)
    return dl_make_invalid_iterator();

  index_right = dl_iterator_index(right);
  if (index_right < 0)
    return dl_make_invalid_iterator();

  range = index_right - index_left;
  if (range < 0)
    return dl_make_invalid_iterator();

  index_middle = (range >> 1) + index_left;
  mid_iter = dl_container_index(left.container, index_middle);

  match = DL_CALL1(predicate, dl_iterator_ref(mid_iter));
  if (match == 0)
    return mid_iter;
  else if (range == 0)
    return dl_make_invalid_iterator();
  else if (match < 0)
    return _dl_find_region_binary(left, dl_iterator_prev(mid_iter), predicate);
  else
    return _dl_find_region_binary(dl_iterator_next(mid_iter), right, predicate);
}

dl_api dl_iterator dl_find(dl_iterator left, dl_iterator right, dl_filter predicate) {
  dl_natural traits;

  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(predicate))) return dl_make_invalid_iterator();

  traits = dl_container_traits(left.container);
  if (traits & DL_CONTAINER_TRAIT_RANDOM_ACCESS) {
    dl_iterator found, prev;
    found = _dl_find_region_binary(left, right, predicate);

    if (!(traits & DL_CONTAINER_TRAIT_SET))
      while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, left)) {
        prev = dl_iterator_prev(found);
        if (DL_CALL1(predicate, dl_iterator_ref(prev)) != 0)
          break;
        found = prev;
      }

    return found;
  } else
    return _dl_find_region_linear(left, right, predicate);
}

dl_api dl_iterator dl_find_reverse(dl_iterator left, dl_iterator right, dl_filter predicate) {
  dl_natural traits;

  if (dl_safety(ITER_UNSAFE(right, left) || FUNC_UNSAFE(predicate))) return dl_make_invalid_iterator();

  traits = dl_container_traits(left.container);
  if ((traits & DL_CONTAINER_TRAIT_RANDOM_ACCESS) && (traits & DL_CONTAINER_TRAIT_SORTED)) {
    dl_iterator found, next;
    found = _dl_find_region_binary(left, right, predicate);

    if (!(traits & DL_CONTAINER_TRAIT_SET))
      while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right)) {
        next = dl_iterator_next(found);
        if (DL_CALL1(predicate, dl_iterator_ref(next)) != 0)
          break;
        found = next;
      }

    return found;
  } else
    return _dl_find_reverse_region_linear(left, right, predicate);
}

dl_api dl_integer dl_find_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out) {
  dl_ptr ref;
  dl_iterator found;
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(predicate))) return -1;

  count = 0;

  if (!FUNC_UNSAFE(out)) {
    found = dl_find(left, right, predicate);
    if (!dl_iterator_is_valid(found)) return 0;
    while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right)) {
      ref = dl_iterator_ref(found);
      if (!DL_CALL1(predicate, ref)) break;
      ++count;
      if (!DL_CALL1(out, ref)) break;
      found = dl_iterator_next(found);
    }
    return count;
  } else {
    found = dl_find(left, right, predicate);
    if (!dl_iterator_is_valid(found)) return 0;
    while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right)) {
      ref = dl_iterator_ref(found);
      if (!DL_CALL1(predicate, ref)) break;
      ++count;
      found = dl_iterator_next(found);
    }
    return count;
  }
}

dl_api dl_ptr dl_foldl(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder folder) {
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(folder))) return NULL;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
    DL_CALL2(folder, initial, dl_iterator_ref(left));
    left = dl_iterator_next(left);
  }

  return initial;
}

dl_api dl_ptr dl_foldr(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder folder) {
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(folder))) return NULL;

  while (dl_iterator_is_valid(right) && !dl_iterator_equal(left, right)) {
    DL_CALL2(folder, initial, dl_iterator_ref(right));
    right = dl_iterator_prev(right);
  }

  return initial;
}

dl_api dl_bool dl_all(dl_iterator left, dl_iterator right, dl_filter filter) {
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(filter))) return false;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
    if (!DL_CALL1(filter, dl_iterator_ref(left))) return false;
    left = dl_iterator_next(left);
  }

  return true;
}

dl_api dl_bool dl_any(dl_iterator left, dl_iterator right, dl_filter filter) {
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(filter))) return false;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
    if (DL_CALL1(filter, dl_iterator_ref(left))) return true;
    left = dl_iterator_next(left);
  }

  return false;
}

dl_api dl_integer dl_map(dl_iterator left, dl_iterator right, dl_handler handler, dl_handler out) {
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left, right))) return -1;

  count = 0;
  if (!FUNC_UNSAFE(out)) {
    while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
      if (!DL_CALL1(out, DL_CALL1(handler, dl_iterator_ref(left)))) break;
      ++count;
      left = dl_iterator_next(left);
    }
    return count;
  } else {
    while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
      if (NULL == DL_CALL1(handler, dl_iterator_ref(left))) break;
      ++count;
      left = dl_iterator_next(left);
    }
    return count;
  }

  return count;
}

dl_api dl_integer dl_zip(dl_iterator left1, dl_iterator right1, dl_iterator left2, dl_iterator right2, dl_zipper zip, dl_handler out) {
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left1, right1) || ITER_UNSAFE(left2, right2) || zip.func == NULL)) return -1;

  count = 0;
  if (!FUNC_UNSAFE(out)) {
    while (dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1) && dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1)) {
      if (!DL_CALL1(out, DL_CALL2(zip, dl_iterator_ref(left1), dl_iterator_ref(left2)))) break;
      ++count;
      left1 = dl_iterator_next(left1);
      left2 = dl_iterator_next(left2);
    }
    return count;
  } else {
    while (dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1) && dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1)) {
      if (NULL == DL_CALL2(zip, dl_iterator_ref(left1), dl_iterator_ref(left2))) break;
      ++count;
      left1 = dl_iterator_next(left1);
      left2 = dl_iterator_next(left2);
    }
    return count;
  }
}

dl_api dl_integer dl_take(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out) {
  dl_natural processed;
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(out))) return 0;

  for (processed = 0; processed < count && dl_iterator_is_valid(left); ++processed) {
    if (!DL_CALL1(out, dl_iterator_ref(left))) break;
    left = dl_iterator_next(left);
  }

  return processed;
}

dl_api dl_integer dl_drop(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out) {
  dl_natural processed;
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(out))) return 0;

  while (count > 0) {
    --count;
    left = dl_iterator_next(left);
    if (!dl_iterator_is_valid(left)) return 0;
  }

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right)) {
    DL_CALL1(out, dl_iterator_ref(left));
    ++processed;
  }

  return processed;
}

dl_api dl_bool dl_remove(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out) {
  dl_iterator found;
  found = dl_find(left, right, predicate);

  if (!dl_iterator_is_valid(found)) return false;

  if (!FUNC_UNSAFE(out)) DL_CALL1(out, dl_iterator_ref(found));

  return dl_iterator_remove(found);
}

dl_api dl_bool dl_remove_reverse(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out) {
  dl_iterator found;
  found = dl_find_reverse(left, right, predicate);

  if (!dl_iterator_is_valid(found)) return false;

  if (!FUNC_UNSAFE(out)) DL_CALL1(out, dl_iterator_ref(found));

  return dl_iterator_remove(found);
}

dl_api dl_natural dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out) {
  dl_iterator found;
  dl_bool valid;
  dl_natural removed;

  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(predicate))) return 0;

  removed = 0;
  do {
    found = dl_find(left, right, predicate);
    valid = dl_iterator_is_valid(found);

    if (valid) {
      ++removed;
      if (!FUNC_UNSAFE(out)) DL_CALL1(out, dl_iterator_ref(found));
    }
  } while (valid);

  return removed;
}

dl_api dl_natural dl_remove_count(dl_iterator left, dl_iterator right, dl_filter predicate, dl_natural count, dl_handler out) {
  dl_iterator found;
  dl_bool valid;
  dl_natural removed;

  if (dl_safety(count == 0 || ITER_UNSAFE(left, right) || FUNC_UNSAFE(predicate))) return 0;

  removed = 0;
  do {
    found = dl_find(left, right, predicate);
    valid = dl_iterator_is_valid(found);

    if (valid) {
      ++removed;
      if (!FUNC_UNSAFE(out)) DL_CALL1(out, dl_iterator_ref(found));
    }
  } while (valid && removed < count);

  return removed;
}

dl_iterator _quick_sort_partition(dl_iterator left, dl_iterator right, dl_comparator compare) {
  dl_iterator i, j, pivot;
  dl_ptr pivot_ref, ref_j;

  pivot = right;
  dl_iterator_prev_ref(&pivot, &pivot_ref);

  ref_j = dl_iterator_ref(left);

  for (i = j = left; !dl_iterator_equal(pivot, j); dl_iterator_next_ref(&j, &ref_j)) {
    if (DL_CALL2(compare, ref_j, pivot_ref)) {
      dl_iterator_swap(i, j);
      dl_swap(i, j);
      i = dl_iterator_next(i);
    }
  }

  dl_iterator_swap(i, pivot);
  dl_swap(i, pivot);

  return i;
}

dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare) {
  dl_iterator pivot;

  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(compare))) return false;

  if (dl_iterator_equal(left, right)) return true;

  pivot = _quick_sort_partition(left, right, compare);

  if (!dl_iterator_equal(left, pivot)) dl_quick_sort(left, pivot, compare);
  pivot = dl_iterator_next(pivot);
  if (!dl_iterator_equal(pivot, right)) dl_quick_sort(pivot, right, compare);

  return true;
}

#endif /* DL_IMPLEMENTATION */

#endif
