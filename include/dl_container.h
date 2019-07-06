/*
 * This entire file is a testament to the travesty of C's limitations.
 * What is being done with switch statements at run time could be better done with static analysis
 * at compile time. Alternativey, a structure with function pointers could be used, but the differences
 * in behaviour at runtime are negligible; while at compile-time there is greater ambiguity. It would be
 * better if traits/generics were available to C, so this logic could be done at compile time.
 *
 */

#ifndef DL_CONTAINER_H
#define DL_CONTAINER_H 1

#include "dl_core.h"
#include "dl_vector.h"
#include "dl_linked_list.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*****************************************************************************
   **  Generic Interface
   ****************************************************************************/

  typedef enum
  {
    DL_CONTAINER_TYPE_NONE,
    DL_CONTAINER_TYPE_VECTOR,
    DL_CONTAINER_TYPE_LINKED_LIST,
    DL_CONTAINER_TYPE_SORTED_LIST,
    DL_CONTAINER_TYPE_QUEUE,
    DL_CONTAINER_TYPE_SET
  } dl_container_type;

  typedef enum
  {
    DL_CONTAINER_TRAIT_RANDOM_ACCESS = 1,
    DL_CONTAINER_TRAIT_RANDOM_MUTATE = 2,
    DL_CONTAINER_TRAIT_SORTED = 4,
    DL_CONTAINER_TRAIT_SET = 8
  } dl_container_trait;

  typedef struct
  {
    dl_natural traits;
    dl_ptr (*init)(dl_ptr, dl_natural, dl_natural);
    dl_ptr (*init_array)(dl_ptr, dl_byte *, dl_natural, dl_natural);
    dl_ptr (*copy)(dl_ptr, dl_ptr);
  } dl_container_descriptor;

  typedef struct
  {
    dl_container_type type;
    union {
      dl_linked_list list;
      dl_vector vector;
    } data;
  } dl_container;

  typedef struct
  {
    dl_container *container;
    union {
      dl_integer index;
      dl_linked_list_node *node;
    } data;
  } dl_iterator;

  dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity);
  dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count);
  dl_api dl_container *dl_container_copy(dl_container *target, dl_container *source);
  dl_api void dl_destroy_container(dl_container *target);

  dl_api dl_natural dl_container_element_size(const dl_container *container);
  dl_api dl_natural dl_container_length(const dl_container *container);
  dl_api dl_bool dl_container_is_empty(const dl_container *container);

  dl_api dl_iterator dl_container_index(const dl_container *container, dl_natural position);
  dl_api dl_iterator dl_container_first(const dl_container *target);
  dl_api dl_iterator dl_container_last(const dl_container *target);

  dl_api dl_ptr dl_container_push(dl_container *v, dl_ptr value);
  dl_api dl_ptr dl_container_pop(dl_container *v, dl_ptr out);

  dl_api dl_natural dl_container_traits(const dl_container *v);

#ifdef __cplusplus
}
#endif

#if DL_IMPLEMENTATION

#include "dl_iterator.h"

/* TODO: use lookup tables instead of switches */

/*******************************************************************************
 ** Generic Interface
 ******************************************************************************/

dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity)
{
  if (dl_safety(target == NULL || element_size == 0 || capacity == 0))
    return NULL;

  target->type = type;
  switch (type)
  {
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

dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count)
{
  if (dl_safety(target == NULL || element_size == 0 || count == 0 || array_data == NULL))
    return NULL;

  target->type = DL_CONTAINER_TYPE_VECTOR;
  if (NULL == dl_init_vector_array(&target->data.vector, array_data, element_size, count))
    return NULL;

  return target;
}

dl_api dl_container *dl_container_copy(dl_container *target, dl_container *source)
{
  if (dl_safety(source == NULL || target == NULL))
    return NULL;

  if (NULL == dl_init_container(target, source->type, dl_container_element_size(source), dl_container_length(source)))
    return NULL;

  switch (target.container->type)
  {
    default:
      return NULL;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_copy((dl_linked_list *)target.container, (dl_linked_list *)source.container);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_copy((dl_vector *)target.container, (dl_vector *)source.container);
  }
}

dl_api void dl_destroy_container(dl_container *target)
{
  if (dl_safety(target == NULL))
    return;

  switch (target->type)
  {
    default:
      break;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      dl_destroy_linked_list(&target->data.list);
      break;
    case DL_CONTAINER_TYPE_VECTOR:
      dl_destroy_vector(&target->data.vector);
      break;
  }

  target->type = DL_CONTAINER_TYPE_NONE;
}

dl_api dl_natural dl_container_length(const dl_container *target)
{
  if (dl_safety(target == NULL))
    return 0;

  switch (target->type)
  {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_length(&target->data.list);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_capacity(&target->data.vector);
  }
}

dl_api dl_bool dl_container_is_empty(const dl_container *target)
{
  if (dl_safety(target == NULL))
    return true;

  switch (target->type)
  {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_is_empty(&target->data.list);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_is_empty(&target->data.vector);
  }
}

dl_api dl_natural dl_container_element_size(const dl_container *target)
{
  if (dl_safety(target == NULL))
    return 0;

  switch (target->type)
  {
    default:
      return 0;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return target->data.list.element_size;
    case DL_CONTAINER_TYPE_VECTOR:
      return target->data.vector.element_size;
  }
}

dl_api dl_iterator dl_container_index(const dl_container *target, dl_natural position)
{
  dl_iterator iter;

  if (dl_safety(target == NULL))
    return dl_make_invalid_iterator();

  switch (target->type)
  {
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

dl_api dl_iterator dl_container_first(const dl_container *target)
{
  dl_iterator iter;

  if (dl_safety(target == NULL))
    return dl_make_invalid_iterator();

  switch (target->type)
  {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = (dl_container *)target;
      iter.data.node = target->data.list.first;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_safety(target->data.vector.array == NULL))
        return dl_make_invalid_iterator();
      iter.container = (dl_container *)target;
      iter.data.index = 0;
      return iter;
  }
}

dl_api dl_iterator dl_container_last(const dl_container *target)
{
  dl_iterator iter;

  if (dl_safety(target == NULL))
    return dl_make_invalid_iterator();

  switch (target->type)
  {
    default:
      return dl_make_invalid_iterator();
    case DL_CONTAINER_TYPE_LINKED_LIST:
      iter.container = (dl_container *)target;
      iter.data.node = target->data.list.last;
      return iter;
    case DL_CONTAINER_TYPE_VECTOR:
      if (dl_safety(target->data.vector.array == NULL))
        return dl_make_invalid_iterator();
      iter.container = (dl_container *)target;
      iter.data.index = target->data.vector.capacity - 1;
      return iter;
  }
}

dl_api dl_ptr dl_container_push(dl_container *c, dl_ptr value)
{
  if (dl_safety(c == NULL))
    return false;

  switch (c->type)
  {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_push(&c->data.list, value);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_push(&c->data.vector, value);
  }
}

dl_api dl_ptr dl_container_pop(dl_container *c, dl_ptr out)
{
  if (dl_safety(c == NULL))
    return false;

  switch (c->type)
  {
    default:
      return false;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      return dl_linked_list_pop(&c->data.list, out);
    case DL_CONTAINER_TYPE_VECTOR:
      return dl_vector_pop(&c->data.vector, out);
  }
}

dl_api dl_natural dl_container_traits(const dl_container *c)
{
  dl_natural traits;

  traits = 0;

  switch (c->type)
  {
    default:
      break;
    case DL_CONTAINER_TYPE_LINKED_LIST:
      traits |= DL_CONTAINER_TRAIT_RANDOM_MUTATE;
    case DL_CONTAINER_TYPE_VECTOR:
      traits |= DL_CONTAINER_TRAIT_RANDOM_ACCESS;
      break;
  }

  return traits;
}

#endif /* DL_IMPLEMENTATION */

#endif
