#ifndef DL_VECTOR_H
#define DL_VECTOR_H 1

#include "dl_core.h"
#include "dl_memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DL_VECTOR_CAPACITY_GROWTH
#define DL_VECTOR_CAPACITY_GROWTH 64
#endif

typedef struct {
  dl_natural element_size;
  dl_natural capacity;
  dl_natural length;
  dl_byte *array;
} dl_vector;

dl_api dl_vector *dl_make_vector(dl_natural element_size, dl_natural capacity);

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

#if defined(DL_IMPLEMENTATION)

/*****************************************************************************
 **  Vectors
 ****************************************************************************/

dl_api dl_vector *dl_make_vector(dl_natural element_size, dl_natural capacity) {
  dl_vector *target;

  if (dl_unlikely(NULL == (target = dl_alloc(sizeof(dl_vector)))))
    return NULL;

  target->element_size = element_size;
  target->array = NULL;
  target->capacity = 0;
  target->length = 0;

  if (dl_unlikely(target->element_size == 0 || capacity == 0))
    return NULL;

  target->array = (dl_byte *)dl_alloc(capacity * element_size);
  if (dl_unlikely(target->array == NULL)) {
    dl_free(target);
    return NULL;
  }
  target->capacity = capacity;

  return target;
}

dl_api void dl_destroy_vector(dl_vector *target) {
  if (dl_safety(target == NULL))
    return;

  if (target->array != NULL) {
    dl_free((dl_ptr)target->array);
    target->array = NULL;
  }

  dl_free(target);
}

dl_api dl_inline dl_natural dl_vector_capacity(const dl_vector *v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->capacity;
}

dl_api dl_inline dl_natural dl_vector_length(const dl_vector *v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->length;
}

dl_api dl_inline dl_bool dl_vector_is_empty(const dl_vector *v) {
  if (dl_safety(v == NULL))
    return true;
  return v->length == 0;
}

dl_api dl_ptr dl_vector_get(const dl_vector *v, dl_natural index, dl_ptr out) {
  dl_ptr ref = dl_vector_ref(v, index);

  return ref == NULL ? (dl_ptr)NULL : dl_memory_copy(out, ref, v->element_size);
}

dl_api dl_ptr dl_vector_set(dl_vector *v, dl_natural index, dl_ptr value) {
  dl_ptr ref = dl_vector_ref(v, index);

  return ref == NULL ? (dl_ptr)NULL : dl_memory_copy(ref, value, v->element_size);
}

dl_api dl_ptr dl_vector_ref(const dl_vector *v, dl_natural index) {
  if (dl_safety(v == NULL))
    return NULL;

  if (index >= v->length)
    return NULL;
  return &v->array[index * v->element_size];
}

dl_api dl_bool dl_vector_grow(dl_vector *v, dl_natural amount) {
  dl_byte *new_array;
  dl_natural new_capacity;

  if (dl_safety(v == NULL || v->array == NULL))
    return false;

  if (v->capacity + amount < v->capacity)
    return false;

  new_capacity = v->capacity + amount;
  new_array = (dl_byte *)dl_alloc(new_capacity * v->element_size);
  if (dl_unlikely(new_array == NULL))
    return false;

  if (v->array != NULL) {
    dl_memory_copy((dl_ptr)new_array, (dl_ptr)v->array, v->length * v->element_size);
    dl_free(v->array);
  }

  v->array = new_array;
  v->capacity = new_capacity;

  return true;
}

dl_api dl_bool dl_vector_swap(dl_vector *v, dl_natural index1, dl_natural index2) {
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

dl_api dl_natural dl_vector_ref_array(dl_vector *v, dl_natural index, dl_ptr *out) {
  dl_natural count;

  if (dl_safety(v == NULL || out == NULL))
    return 0;
  if (dl_safety(v->length <= index))
    return 0;

  count = v->length - index;
  *out = (dl_ptr)&v->array[index * v->element_size];

  return count;
}

dl_api dl_integer dl_vector_insert(dl_vector *v, dl_natural position, dl_ptr value) {
  dl_natural idx;

  if (dl_safety(v == NULL || value == NULL) || position > v->length)
    return -1;

  if (v->length >= v->capacity && !dl_vector_grow(v, DL_VECTOR_CAPACITY_GROWTH))
    return -1;

  v->length++;
  for (idx = v->length; idx > position; --idx)
    dl_vector_set(v, idx, dl_vector_ref(v, idx - 1));
  if (NULL != value)
    dl_vector_set(v, position, value);

  return position;
}

dl_api dl_bool dl_vector_remove(dl_vector *v, dl_natural position) {
  dl_natural idx;

  if (dl_safety(v == NULL) || v->length == 0 || position >= v->length)
    return false;

  for (idx = position; idx < v->length - 1; ++idx)
    dl_vector_set(v, idx, dl_vector_ref(v, idx + 1));
  v->length--;

  return true;
}

dl_api dl_ptr dl_vector_push(dl_vector *v, dl_ptr value) {
  dl_ptr ref;
  if (dl_safety(v == NULL))
    return NULL;

  if (v->length == v->capacity && !dl_vector_grow(v, DL_VECTOR_CAPACITY_GROWTH))
    return NULL;

  v->length++;
  ref = dl_vector_set(v, v->length - 1, value);

  return ref;
}

dl_api dl_ptr dl_vector_pop(dl_vector *v, dl_ptr out) {
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
