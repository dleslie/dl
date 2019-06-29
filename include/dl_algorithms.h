/*
 * Where left and right iterators are requested, it will span from left to right, excluding right;
 * to span a whole container start from the first and provide an invalid iterator as the right.
 * 
 * When the function is stated to be "reverse" then the left/right rules are also reversed, and
 * so it will span from right to left, excluding left or until the iterator is invalid.
 */

#ifndef DL_ALGORITHMS_H
#define DL_ALGORITHMS_H 1

#include "dl.h"
#include "dl_iterator.h"
#include "dl_container.h"

#ifdef __cplusplus
extern "C"
{
#endif
	dl_api dl_integer dl_count(dl_iterator left, dl_iterator right);

	dl_api dl_iterator dl_find(dl_iterator left, dl_iterator right, dl_filter predicate);
	dl_api dl_iterator dl_find_reverse(dl_iterator left, dl_iterator right, dl_filter predicate);
	dl_api dl_integer dl_find_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_iterator out);

	dl_api dl_ptr dl_foldl(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder func);
	dl_api dl_ptr dl_foldr(dl_iterator left, dl_iterator right, dl_ptr initial, dl_folder func);

	dl_api dl_bool dl_all(dl_iterator left, dl_iterator right, dl_filter func);
	dl_api dl_bool dl_any(dl_iterator left, dl_iterator right, dl_filter func);

	dl_api dl_integer dl_map(dl_iterator left, dl_iterator right, dl_handler func, dl_iterator out);
	dl_api dl_integer dl_zip(dl_iterator left1, dl_iterator right1, dl_iterator left2, dl_iterator right2, dl_zipper zip, dl_iterator out);

	dl_api dl_integer dl_take(dl_iterator left, dl_iterator right, dl_natural count, dl_iterator out);
	dl_api dl_integer dl_drop(dl_iterator left, dl_iterator right, dl_natural count);

	dl_api dl_ptr dl_remove(dl_iterator left, dl_iterator right, dl_filter predicate, dl_ptr out);
	dl_api dl_ptr dl_remove_reverse(dl_iterator left, dl_iterator right, dl_filter predicate, dl_ptr out);

	dl_api dl_integer dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_iterator out);
	dl_api dl_integer dl_remove_count(dl_iterator left, dl_iterator right, dl_natural count, dl_iterator out);

	dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare);

#ifdef __cplusplus
}
#endif

#if DL_IMPLEMENTATION

#define ITER_UNSAFE(left, right) (!dl_iterator_is_valid(left) || (dl_iterator_is_valid(right) && dl_iterator_container(left) != dl_iterator_container(right)))

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
	while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right) && !predicate.func(predicate.data, dl_iterator_ref(left)))
		left = dl_iterator_next(left);
	return left;
}

dl_api dl_iterator _dl_find_reverse_region_linear(dl_iterator left, dl_iterator right, dl_filter predicate)
{
	while (dl_iterator_is_valid(right) && !dl_iterator_equal(left, right) && !predicate.func(predicate.data, dl_iterator_ref(right)))
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

	match = predicate.func(predicate.data, dl_iterator_ref(mid_iter));
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
			for (prev = dl_iterator_prev(found); dl_iterator_is_valid(prev) && predicate.func(predicate.data, dl_iterator_ref(prev)) == 0; prev = dl_iterator_prev(prev))
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
			for (next = dl_iterator_next(found); dl_iterator_is_valid(next) && predicate.func(predicate.data, dl_iterator_ref(next)) == 0; next = dl_iterator_next(next))
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

dl_api dl_integer dl_find_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_iterator out)
{
	dl_ptr ref;
	dl_iterator found;
	dl_integer count;
	dl_bool is_set;
	dl_natural traits;

	if (dl_safety(ITER_UNSAFE(left, right) || predicate.func == NULL || (dl_iterator_is_valid(out) && dl_container_element_size(dl_iterator_container(out)) != dl_container_element_size(dl_iterator_container(leter)))))
		return -1;

	traits = dl_container_traits(left.container);
	is_set = 0 != (traits & DL_CONTAINER_TRAIT_SET);
	count = 0;
	if (dl_iterator_is_valid(out))
	{
		found = dl_find(left, right, predicate);
		if (!dl_iterator_is_valid(found))
			return 0;
		while (dl_iterator_is_valid(found) && !dl_iterator_equal(found, right))
		{
			ref = dl_iterator_ref(found);
			if (!predicate.func(predicate.data, ref))
				break;
			++count;
			out = dl_iterator_insert(out, ref);
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
			if (!predicate.func(predicate.data, ref))
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
		folder.func(folder.data, initial, dl_iterator_ref(left));
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
		folder.func(folder.data, initial, dl_iterator_ref(right));
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
		if (!filter.func(filter.data, dl_iterator_ref(left)))
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
		if (filter.func(filter.data, dl_iterator_ref(left)))
			return true;
		left = dl_iterator_next(left);
	}

	return false;
}

dl_api dl_integer dl_map(dl_iterator left, dl_iterator right, dl_handler handler, dl_iterator out)
{
	dl_integer count;

	if (dl_safety(ITER_UNSAFE(left, right) || handler.func == NULL))
		return -1;

	count = 0;
	if (dl_iterator_is_valid(out))
	{
		while (dl_iterator_is_valid(left) && !dl_iterator_equal(left, right))
		{
			if (!dl_iterator_is_valid(dl_iterator_insert(out, handler.func(handler.data, dl_iterator_ref(right)))))
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
			if (NULL == handler.func(handler.data, dl_iterator_ref(right)))
				break;
			++count;
			left = dl_iterator_next(left);
		}
		return count;
	}

	return count;
}

dl_api dl_integer dl_zip(dl_iterator left1, dl_iterator right1, dl_iterator left2, dl_iterator right2, dl_zipper zip, dl_iterator out);

dl_api dl_integer dl_take(dl_iterator left, dl_iterator right, dl_natural count, dl_iterator out);
dl_api dl_integer dl_drop(dl_iterator left, dl_iterator right, dl_natural count);

dl_api dl_ptr dl_remove(dl_iterator left, dl_iterator right, dl_filter predicate, dl_ptr out);
dl_api dl_ptr dl_remove_reverse(dl_iterator left, dl_iterator right, dl_filter predicate, dl_ptr out);

dl_api dl_integer dl_remove_all(dl_iterator left, dl_iterator right, dl_filter predicate, dl_iterator out);
dl_api dl_integer dl_remove_count(dl_iterator left, dl_iterator right, dl_natural count, dl_iterator out);

dl_api dl_bool dl_quick_sort(dl_iterator left, dl_iterator right, dl_comparator compare);

#endif /* DL_IMPLEMENTATION */

#endif
