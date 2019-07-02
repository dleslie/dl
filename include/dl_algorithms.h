/*
 * Where left and right iterators are requested, it will span from left to right, excluding right;
 * to span a whole container start from the first and provide an invalid iterator as the right.
 * 
 * When the function is stated to be "reverse" then the left/right rules are also reversed, and
 * so it will span from right to left, excluding left or until the iterator is invalid.
 */

#ifndef DL_ALGORITHMS_H
#define DL_ALGORITHMS_H 1

#include "dl_core.h"
#include "dl_iterator.h"
#include "dl_container.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct {
    dl_integer (*func)(dl_ptr data, const dl_ptr value);
    dl_ptr data;
  } dl_filter;

  typedef struct {
    dl_ptr (*func)(dl_ptr data, dl_ptr value);
    dl_ptr data;
  } dl_handler;

  typedef struct {
    dl_ptr (*func)(dl_ptr data, dl_ptr item, const dl_ptr left);
    dl_ptr data;
  } dl_folder;

  typedef struct {
    dl_integer (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right);
    dl_ptr data;
  } dl_comparator;

  typedef struct {
    dl_ptr (*func)(dl_ptr data, const dl_ptr left, const dl_ptr right);
    dl_ptr data;
  } dl_zipper;

#  if DL_IS_ATLEAST_C99
  #define DL_CALL(c, ...) c.func(c.data, ##__VAR_ARGS__)
#  endif
  #define DL_CALL1(c, arg1) c.func(c.data, arg1)
  #define DL_CALL2(c, arg1, arg2) c.func(c.data, arg1, arg2)
  
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
  dl_api dl_integer dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
  dl_api dl_integer dl_remove_count(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out);

  dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare);

  dl_api dl_handler dl_make_handler(dl_container target);
  dl_api dl_zipper dl_make_zipper(dl_container target);

  extern dl_handler dl_null_handler;
  extern dl_zipper dl_null_zipper;

#ifdef __cplusplus
}
#endif

#if DL_IMPLEMENTATION

#define ITER_UNSAFE(left, right) (!dl_iterator_is_valid(left) || (dl_iterator_is_valid(right) && dl_iterator_container(left) != dl_iterator_container(right)))
#define FUNC_UNSAFE(f) (f.func == NULL)

dl_api dl_integer dl_count(dl_iterator left, dl_iterator right)
{
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left, right)))
    return 0;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
  {
    ++count;
    left = dl_iterator_next(left);
  }

  return count;
}

dl_api dl_iterator _dl_find_region_linear(dl_iterator left, dl_iterator right, dl_filter predicate)
{
  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right) && !DL_CALL1(predicate, dl_iterator_ref(left)))
    left = dl_iterator_next(left);
  return left;
}

dl_api dl_iterator _dl_find_reverse_region_linear(dl_iterator left, dl_iterator right, dl_filter predicate)
{
  while (dl_iterator_is_valid(right) && !dl_iterator_equal(left, right) && !DL_CALL1(predicate, dl_iterator_ref(right)))
    right = dl_iterator_prev(right);
  return right;
}

dl_api dl_iterator _dl_find_region_binary(dl_iterator left, dl_iterator right, dl_filter predicate)
{
  dl_natural mid_index;
  dl_iterator mid_iter;
  dl_integer match;

  mid_index = (dl_iterator_index(right) - dl_iterator_index(left)) >> 1;
  mid_iter = dl_container_index(dl_iterator_container(left), mid_index);

  match = DL_CALL1(predicate, dl_iterator_ref(mid_iter));
  if (match == 0)
    return mid_iter;
  else if (dl_iterator_equal(left, right))
    return dl_make_invalid_iterator();
  else if (match < 0)
    return _dl_find_region_binary(left, dl_iterator_prev(mid_iter), predicate);
  else
    return _dl_find_region_binary(dl_iterator_next(mid_iter), right, predicate);
}

dl_api dl_iterator dl_find(dl_iterator left, dl_iterator right, dl_filter predicate)
{
  dl_natural traits;

  if (dl_safety(ITER_UNSAFE(left, right) || predicate.func == NULL))
    return dl_make_invalid_iterator();

  if (!dl_iterator_is_valid(right))
    right = dl_container_last(dl_iterator_container(left));

  traits = dl_container_traits(left.container);
  if (traits & DL_CONTAINER_TRAIT_RANDOM_ACCESS)
  {
    dl_iterator found, prev;
    found = _dl_find_region_binary(left, right, predicate);

    if (!(traits & DL_CONTAINER_TRAIT_SET))
      for (prev = dl_iterator_prev(found); dl_iterator_is_valid(prev) && DL_CALL1(predicate, dl_iterator_ref(prev)) == 0; prev = dl_iterator_prev(prev))
      {
        found = prev;
        if (dl_iterator_equal(found, left))
          break;
      }

    return found;
  }
  else
    return _dl_find_region_linear(left, right, predicate);
}

dl_api dl_iterator dl_find_reverse(dl_iterator left, dl_iterator right, dl_filter predicate)
{
  dl_natural traits;

  if (dl_safety(ITER_UNSAFE(right, left) || predicate.func == NULL))
    return dl_make_invalid_iterator();

  if (!dl_iterator_is_valid(left))
    left = dl_container_first(dl_iterator_container(right));

  traits = dl_container_traits(left.container);
  if (traits & (DL_CONTAINER_TRAIT_RANDOM_ACCESS | DL_CONTAINER_TRAIT_SORTED))
  {
    dl_iterator found, next;
    found = _dl_find_region_binary(left, right, predicate);

    if (!(traits & DL_CONTAINER_TRAIT_SET))
      for (next = dl_iterator_next(found); dl_iterator_is_valid(next) && DL_CALL1(predicate, dl_iterator_ref(next)) == 0; next = dl_iterator_next(next))
      {
        found = next;
        if (dl_iterator_equal(found, right))
          break;
      }

    return found;
  }
  else
    return _dl_find_reverse_region_linear(left, right, predicate);
}

dl_api dl_integer dl_find_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out)
{
  dl_ptr ref;
  dl_iterator found;
  dl_integer count;
  dl_bool is_set;
  dl_natural traits;

  if (dl_safety(ITER_UNSAFE(left, right)))
    return -1;

  traits = dl_container_traits(left.container);
  is_set = 0 != (traits & DL_CONTAINER_TRAIT_SET);
  count = 0;
  if (!FUNC_UNSAFE(out))
  {
    found = dl_find(left, right, predicate);
    if (!dl_iterator_is_valid(found))
      return 0;
    while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right))
    {
      ref = dl_iterator_ref(found);
      if (!DL_CALL1(predicate, ref))
        break;
      ++count;
      if (!DL_CALL1(out, ref))
        break;
      found = dl_iterator_next(found);
    }
    return count;
  }
  else
  {
    found = dl_find(left, right, predicate);
    if (!dl_iterator_is_valid(found))
      return 0;
    while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right))
    {
      ref = dl_iterator_ref(found);
      if (!DL_CALL1(predicate, ref))
        break;
      ++count;
      found = dl_iterator_next(found);
    }
    return count;
  }
}

dl_api dl_ptr dl_foldl(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder folder)
{
  if (dl_safety(ITER_UNSAFE(left, right) || initial == NULL || folder.func == NULL))
    return NULL;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
  {
    DL_CALL2(folder, initial, dl_iterator_ref(left));
    left = dl_iterator_next(left);
  }

  return initial;
}

dl_api dl_ptr dl_foldr(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder folder)
{
  if (dl_safety(ITER_UNSAFE(left, right) || initial == NULL || folder.func == NULL))
    return NULL;

  while (dl_iterator_is_valid(right) && !dl_iterator_equal(left, right))
  {
    DL_CALL2(folder, initial, dl_iterator_ref(right));
    right = dl_iterator_prev(right);
  }

  return initial;
}

dl_api dl_bool dl_all(dl_iterator left, dl_iterator right, dl_filter filter)
{
  if (dl_safety(ITER_UNSAFE(left, right) || filter.func == NULL))
    return false;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
  {
    if (!DL_CALL1(filter, dl_iterator_ref(left)))
      return false;
    left = dl_iterator_next(left);
  }

  return true;
}

dl_api dl_bool dl_any(dl_iterator left, dl_iterator right, dl_filter filter)
{
  if (dl_safety(ITER_UNSAFE(left, right) || filter.func == NULL))
    return false;

  while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
  {
    if (DL_CALL1(filter, dl_iterator_ref(left)))
      return true;
    left = dl_iterator_next(left);
  }

  return false;
}

dl_api dl_integer dl_map(dl_iterator left, dl_iterator right, dl_handler handler, dl_handler out)
{
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left, right)))
    return -1;

  count = 0;
  if (!FUNC_UNSAFE(out))
  {
    while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
    {
      if (!DL_CALL1(out, DL_CALL1(handler, dl_iterator_ref(left))))
        break;
      ++count;
      left = dl_iterator_next(left);
    }
    return count;
  }
  else
  {
    while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
    {
      if (NULL == DL_CALL1(handler, dl_iterator_ref(left)))
        break;
      ++count;
      left = dl_iterator_next(left);
    }
    return count;
  }

  return count;
}

dl_api dl_integer dl_zip(dl_iterator left1, dl_iterator right1, dl_iterator left2, dl_iterator right2, dl_zipper zip, dl_handler out)
{
  dl_integer count;

  if (dl_safety(ITER_UNSAFE(left1, right1) || ITER_UNSAFE(left2, right2) || zip.func == NULL))
    return -1;

  count = 0;
  if (!FUNC_UNSAFE(out))
  {
    while (dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1)
      &&  dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1))
    {
      if (!DL_CALL1(out, DL_CALL2(zip, dl_iterator_ref(left1), dl_iterator_ref(left2))))
        break;
      ++count;
      left1 = dl_iterator_next(left1);
      left2= dl_iterator_next(left2);
    }
    return count;
  }
  else
  {
    while (dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1)
      &&  dl_iterator_is_valid(left1) && !dl_iterator_equal(left1, right1))
    {
      if (NULL == DL_CALL2(zip, dl_iterator_ref(left1), dl_iterator_ref(left2)))
        break;
      ++count;
      left1 = dl_iterator_next(left1);
      left2= dl_iterator_next(left2);
    }
    return count;
  }
}

dl_api dl_integer dl_take(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out)
{
  dl_natural processed;
  if (dl_safety(ITER_UNSAFE(left, right) || FUNC_UNSAFE(out)))
    return 0;

  for (processed = 0; processed < count && dl_iterator_is_valid(left); ++processed) 
  {
    if (!DL_CALL1(out, dl_iterator_ref(left)))
      break; 
    left = dl_iterator_next(left);
  }

  return processed;
}

dl_api dl_integer dl_drop(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out);

dl_api dl_bool dl_remove(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
dl_api dl_bool dl_remove_reverse(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);

dl_api dl_integer dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_handler out);
dl_api dl_integer dl_remove_count(dl_iterator left, dl_iterator right, dl_natural count, dl_handler out);

dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare);

dl_api dl_handler dl_make_handler(dl_container target);
dl_api dl_zipper dl_make_zipper(dl_container target);

#endif /* DL_IMPLEMENTATION */

#endif
