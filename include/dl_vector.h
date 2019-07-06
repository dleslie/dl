#ifndef DL_VECTOR_H
#define DL_VECTOR_H 1

#include "dl_core.h"



#ifdef __cplusplus
extern "C" {
#endif

  /*****************************************************************************
   **  Vectors
   ****************************************************************************/

  typedef struct {
    dl_natural element_size;
    dl_natural capacity;
    dl_natural length;
    dl_byte *array;
    dl_bool should_free;
  } dl_vector;

  dl_api dl_vector *dl_init_vector(dl_vector *target, dl_natural element_size, dl_natural capacity);
  dl_api dl_vector *dl_init_vector_custom(dl_vector *target, dl_natural element_size, dl_natural capacity);

  dl_api dl_vector *dl_init_vector_array(dl_vector *target, dl_byte *data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_vector_copy(dl_vector *target, dl_natural target_offset_index, const dl_vector *original, dl_natural original_offset_index, dl_natural count);

  dl_api void dl_destroy_vector(dl_vector *target);

  dl_api dl_natural dl_vector_capacity(const dl_vector *v);
  dl_api dl_natural dl_vector_length(const dl_vector *v);
  dl_api dl_bool dl_vector_is_empty(const dl_vector *v);
    
  dl_api dl_bool dl_vector_grow(dl_vector *v, dl_natural amount);

  dl_api dl_ptr dl_vector_get(const dl_vector *v, dl_natural index, dl_ptr out);
  dl_api dl_ptr dl_vector_ref(const dl_vector *v, dl_natural index);
  dl_api dl_ptr dl_vector_set(dl_vector *v, dl_natural index, dl_ptr value);

  dl_api dl_bool dl_vector_swap(dl_vector *v, dl_natural index1, dl_natural index2);
  dl_api dl_natural dl_vector_ref_array(dl_vector *v, dl_natural index, dl_ptr *out);

  dl_api dl_integer dl_vector_insert(dl_vector *v, dl_natural position, dl_ptr value);
  dl_api dl_bool dl_vector_remove(dl_vector *v, dl_natural position);

  dl_api dl_ptr dl_vector_push(dl_vector *v, dl_ptr value);
  dl_api dl_ptr dl_vector_pop(dl_vector *v, dl_ptr out);

#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

/*****************************************************************************
 **  Vectors
 ****************************************************************************/

dl_api dl_vector *dl_init_vector(dl_vector *target, dl_natural element_size, dl_natural capacity)
{
  return dl_init_vector_custom(target, element_size, capacity);
}

dl_api dl_vector *dl_init_vector_custom(dl_vector *target, dl_natural element_size, dl_natural capacity)
{
  
  if (target == NULL)
    return NULL;

  target->element_size = element_size;
  target->array = NULL;
  target->should_free = false;
  target->capacity = 0;

  if (dl_unlikely(target->element_size == 0 || capacity == 0))
    return NULL;

  target->array = (dl_byte *)DL_ALLOC(capacity * element_size);
  if (dl_unlikely(target->array == NULL))
  {
    target->capacity = 0;
    return NULL;
  }
  target->capacity = capacity;
  target->should_free = true;

  return target;
}

dl_api dl_vector *dl_init_vector_array(dl_vector *target, dl_byte *data, dl_natural element_size, dl_natural count)
{
  if (dl_safety(target == NULL || data == NULL))
    return NULL;

  target->element_size = element_size;
  target->capacity = 0;
  target->array = data;
  target->should_free = false;

  return target;
}

dl_api void dl_destroy_vector(dl_vector *target)
{
  if (target == NULL)
    return;

  if (target->should_free)
    DL_FREE((dl_ptr)target->array);

  target->array = NULL;
  target->capacity = target->length = 0;
}

dl_api dl_inline dl_natural dl_vector_capacity(const dl_vector *v)
{
  if (dl_safety(v == NULL))
    return 0;

  return v->capacity;
}

dl_api dl_inline dl_natural dl_vector_length(const dl_vector *v)
{
  if (dl_safety(v == NULL))
    return 0;

  return v->length;
}

dl_api dl_inline dl_bool dl_vector_is_empty(const dl_vector *v)
{
  if (dl_safety(v == NULL))
    return true;
  return v->length > 0;
}

dl_api dl_ptr dl_vector_get(const dl_vector *v, dl_natural index, dl_ptr out)
{
  dl_ptr ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(out, ref, v->element_size);
}

dl_api dl_ptr dl_vector_set(dl_vector *v, dl_natural index, dl_ptr value)
{
  dl_ptr ref = dl_vector_ref(v, index);

  return ref == NULL ? NULL : dl_memory_copy(ref, value, v->element_size);
}

dl_api dl_ptr dl_vector_ref(const dl_vector *v, dl_natural index)
{
  if (dl_safety(v == NULL))
    return NULL;

  if (index >= v->length)
    return NULL;
  return &v->array[index * v->element_size];
}

dl_api dl_bool dl_vector_grow(dl_vector *v, dl_natural amount)
{
  dl_byte *new_array;
  dl_natural new_capacity;
  
  if (dl_safety(v == NULL))
    return false;
  
  new_capacity = (v->capacity + amount);
  new_array = (dl_byte *)DL_ALLOC(new_capacity * v->element_size);
  if (dl_unlikely(new_array == NULL))
    return false;

  dl_memory_copy((dl_ptr)new_array, (dl_ptr)v->array, v->capacity * v->element_size);
  
  if (v->should_free)
    DL_FREE(v->array);
  v->should_free = true;
  v->array = new_array;
  v->capacity = new_capacity;

  return true;
}

dl_api dl_bool dl_vector_swap(dl_vector *v, dl_natural index1, dl_natural index2)
{
  dl_ptr left, right;
  
  if (dl_safety(v == NULL))
    return false;

  if (dl_unlikely(index1 == index2))
    return true;

  if (dl_unlikely(!(left = dl_vector_ref(v, index1)) || !(right = dl_vector_ref(v, index2))))
    return false;

  dl_memory_swap(left, right, v->element_size);

  return true;
}

dl_api dl_natural dl_vector_copy(dl_vector *target, dl_natural target_offset_index, const dl_vector *original, dl_natural original_offset_index, dl_natural count)
{
  dl_natural target_window, original_window, actual_count;
  
  if (dl_safety(target == NULL || original == NULL || target->element_size != original->element_size))
    return 0;
  if (dl_unlikely(target->capacity <= target_offset_index))
    return 0;
  if (dl_unlikely(original->length <= original_offset_index))
    return 0;

  target_window = target->capacity - target_offset_index;
  original_window = original->length - original_offset_index;
  actual_count = dl_min(dl_min(count, original_window), target_window);
  
  if (dl_memory_copy((dl_ptr)&target->array[target_offset_index], (dl_ptr)&original->array[original_offset_index], target->element_size * actual_count) != NULL)
    return actual_count;
  return 0;
}

dl_api dl_natural dl_vector_ref_array(dl_vector *v, dl_natural index, dl_ptr *out)
{
  dl_natural count;
  
  if (dl_safety(v == NULL || out == NULL))
    return 0;
  if (dl_safety(v->length <= index))
    return 0;

  count = v->length - index;
  *out = (dl_ptr)&v->array[index * v->element_size];

  return count;
}

dl_api dl_integer dl_vector_insert(dl_vector *v, dl_natural position, dl_ptr value)
{
  if (dl_safety(v == NULL || value == NULL))
    return -1;

  if (v->length == v->capacity)
  {
    dl_vector new_vec;
  
    if (NULL == dl_init_vector(&new_vec, v->element_size, 1 + dl_vector_capacity(v)))
      return -1;

    if (position > 0 && position < v->capacity)
    {
      if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * position)
          || (NULL != value && NULL == dl_vector_set(&new_vec, position, value))
          || NULL == dl_memory_copy(&new_vec.array[new_vec.element_size * (position +1)],
                                    &v->array[new_vec.element_size * position],
                                    dl_vector_capacity(&new_vec) - position))
      {
        dl_destroy_vector(&new_vec);
        return-1;
      }
    }
    else {
      if (NULL == dl_memory_copy(new_vec.array, v->array, new_vec.element_size * new_vec.capacity)
          || (NULL != value && NULL == dl_vector_set(&new_vec, v->capacity, value)))
      {
        dl_destroy_vector(&new_vec);
        return-1;
      }
      position = v->capacity;
    }

    dl_destroy_vector(v);
    *v = new_vec;
  }
  else 
  {
    dl_integer idx;

    for (idx = v->length; idx > position; --idx) 
      dl_vector_set(v, idx, dl_vector_ref(v, idx - 1));

    if (NULL != value)
      dl_vector_set(v, position, value);
  }

  return position;
}

dl_api dl_bool dl_vector_remove(dl_vector *v, dl_natural position)
{
  dl_integer idx;

  if (dl_safety(v == NULL) || v->length == 0)
    return false;

  for (idx = position; idx < v->length - 1; ++idx)
    dl_vector_set(v, idx, dl_vector_ref(v, idx + 1));
  v->length--;

  return true;
}

dl_api dl_ptr dl_vector_push(dl_vector *v, dl_ptr value)
{
  dl_ptr ref;
  if (dl_safety(v == NULL))
    return NULL;

  if (v->length == v->capacity && !dl_vector_grow(v, 1))
    return NULL;

  ref = dl_vector_set(v, v->length, value);
  v->length++;

  return ref;
}

dl_api dl_ptr dl_vector_pop(dl_vector *v, dl_ptr out)
{
  dl_ptr ref;

  if (dl_safety(v == NULL) || v->length == 0)
    return NULL;

  if (out != NULL)
    ref = dl_vector_get(v, v->length - 1, out);
  else
    ref = dl_vector_ref(v, v->length - 1);

  v->length--;

  return ref;
}



#endif /* DL_IMPLEMENTATION */

#endif
