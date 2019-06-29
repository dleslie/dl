#ifndef DL_ITERATOR_H
#define DL_ITERATOR_H 1

#include "dl.h"
#include "dl_container.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*****************************************************************************
   **  Iterator Interface
   ****************************************************************************/

  typedef struct
  {
    dl_container *container;
    union {
      dl_integer index;
      dl_linked_list_node *node;
    } data;
  } dl_iterator;

  dl_api dl_iterator dl_make_invalid_iterator();

  dl_api dl_bool dl_iterator_is_valid(dl_iterator iter);

  dl_api dl_natural dl_iterator_copy(dl_iterator target, const dl_iterator original, dl_natural count);

  dl_api dl_ptr dl_iterator_get(const dl_iterator position, dl_ptr out);
  dl_api dl_ptr dl_iterator_ref(dl_iterator position);
  dl_api dl_ptr dl_iterator_set(dl_iterator position, dl_ptr value);

  dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2);

  dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_ptr value);
  dl_api dl_ptr dl_iterator_remove(dl_iterator position, dl_ptr out);
  dl_api dl_bool dl_iterator_destroy(dl_iterator position, dl_handler *deconstruct_entry);

  dl_api dl_iterator dl_iterator_next(dl_iterator target);
  dl_api dl_iterator dl_iterator_prev(dl_iterator target);
  dl_api dl_iterator dl_iterator_next_ref(dl_iterator target, dl_ptr *out);
  dl_api dl_iterator dl_iterator_prev_ref(dl_iterator target, dl_ptr *out);

  dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right);
  dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right);
  dl_api dl_integer dl_iterator_index(dl_iterator iter);

  dl_api dl_container *dl_iterator_container(dl_iterator target);

#ifdef __cplusplus
}
#endif

#if DL_IMPLEMENTATION

/*******************************************************************************
 ** Iterator Interface
 ******************************************************************************/
dl_api dl_container *dl_iterator_container(dl_iterator target)
{
  return target.container;
}

dl_api dl_iterator dl_make_invalid_iterator()
{
  dl_iterator iter;
  iter.container = NULL;
  iter.data.index = 0;
  iter.data.node = NULL;
  return iter;
}

dl_api dl_bool dl_iterator_is_valid(dl_iterator iter)
{
  return iter.container != NULL && iter.container->data.container != NULL && ((iter.container->type == DL_CONTAINER_TYPE_LINKED_LIST && iter.data.node != NULL) || (iter.container->type == DL_CONTAINER_TYPE_VECTOR && iter.data.index >= 0 && iter.data.index < dl_vector_capacity(&iter.container->data.vector)));
}

dl_api dl_ptr dl_iterator_get(const dl_iterator target, dl_ptr out)
{
  if (dl_safety(!dl_iterator_is_valid(target) || out != NULL))
    return NULL;

  switch (target.container->type)
  {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_get((dl_linked_list *)target.container, target.data.node, out);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_get((dl_vector *)target.container, target.data.index, out);
  }
}

dl_api dl_ptr dl_iterator_ref(dl_iterator target)
{
  if (dl_safety(!dl_iterator_is_valid(target)))
    return NULL;

  switch (target.container->type)
  {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_ref(target.data.node);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_ref((dl_vector *)target.container, target.data.index);
  }
}

dl_api dl_ptr dl_iterator_set(dl_iterator target, dl_ptr value)
{
  if (dl_safety(!dl_iterator_is_valid(target) || value == NULL))
    return NULL;

  switch (target.container->type)
  {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_set((dl_linked_list *)target.container, target.data.node, value);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_set((dl_vector *)target.container, target.data.index, value);
  }
}

dl_api dl_bool dl_iterator_swap(dl_iterator position1, dl_iterator position2)
{
  if (dl_safety(!dl_iterator_is_valid(position1) || !dl_iterator_is_valid(position2) || position2.container != position1.container))
    return false;

  switch (position1.container->type)
  {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_swap((dl_linked_list *)position1.container, position1.data.node, position2.data.node, false);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_swap((dl_vector *)position1.container, position1.data.index, position2.data.index);
  }
}

dl_api dl_iterator dl_iterator_insert(dl_iterator position, dl_ptr value)
{
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(position)) || value == NULL)
    return dl_make_invalid_iterator();

  switch (position.container->type)
  {
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

dl_api dl_ptr dl_iterator_remove(dl_iterator position, dl_ptr out)
{
  if (dl_safety(!dl_iterator_is_valid(position) || out == NULL))
    return NULL;

  switch (position.container->type)
  {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_remove((dl_linked_list *)position.container, position.data.node, out);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_remove((dl_vector *)position.container, position.data.index, out);
  }
}

dl_api dl_bool dl_iterator_destroy(dl_iterator position, dl_handler *deconstruct_entry)
{
  if (dl_safety(!dl_iterator_is_valid(position)))
    return false;

  switch (position.container->type)
  {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_destroy((dl_linked_list *)position.container, position.data.node, deconstruct_entry);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_destroy((dl_vector *)position.container, position.data.index, deconstruct_entry);
  }
}

dl_api dl_iterator dl_iterator_next(dl_iterator target)
{
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  switch (target.container->type)
  {
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

dl_api dl_iterator dl_iterator_prev(dl_iterator target)
{
  dl_iterator iter;

  if (dl_safety(!dl_iterator_is_valid(target)))
    return dl_make_invalid_iterator();

  switch (target.container->type)
  {
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

dl_api dl_iterator dl_iterator_next_ref(dl_iterator iter, dl_ptr *out)
{
  dl_iterator next;
  next = dl_iterator_next(iter);
  *out = dl_iterator_ref(next);
  return next;
}

dl_api dl_iterator dl_iterator_prev_ref(dl_iterator iter, dl_ptr *out)
{
  dl_iterator prev;
  prev = dl_iterator_prev(iter);
  *out = dl_iterator_ref(prev);
  return prev;
}

dl_api dl_bool dl_iterator_equal(dl_iterator left, dl_iterator right)
{
  dl_bool leftValid, rightValid;

  leftValid = dl_iterator_is_valid(left);
  rightValid = dl_iterator_is_valid(right);

  return (leftValid == rightValid) && (!leftValid || (left.container == right.container && ((left.container->type = DL_CONTAINER_TYPE_LINKED_LIST && left.data.node == right.data.node) || (left.container->type = DL_CONTAINER_TYPE_VECTOR && left.data.index == right.data.index))));
}

dl_api dl_integer dl_iterator_compare(dl_iterator left, dl_iterator right)
{
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

dl_api dl_integer dl_iterator_index(dl_iterator iter)
{
  if (dl_safety(!dl_iterator_is_valid(iter)))
    return -1;

  switch (iter.container->type)
  {
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
