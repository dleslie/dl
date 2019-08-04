#include "dl.h"

#if DL_IS_ATLEAST_C99

dl_bool test_dl_init_vector() {
  dl_vector v;
  if (!dl_check(dl_init_vector(&v, sizeof(dl_natural), 32), "Expected dl_init_vector to return non-NULL"))
    return false;
  dl_destroy_vector(&v);
  return true;
}

dl_bool test_dl_vector_push_pop() {
  dl_vector v;
  dl_natural popped, values[2] = {0xDEADBEEF, 0xFFFFFFFF};
  dl_integer idx;

  if (!dl_check(dl_init_vector(&v, sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  if (!dl_check(dl_vector_capacity(&v) == 1, "Expected capacity to be 1, not %i", dl_vector_capacity(&v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_length(&v) == 0, "Expected length to be 0, not %i", dl_vector_length(&v)))
    goto push_pop_fail;

  for (idx = 0; idx < 2; ++idx) {
    if (!dl_check(dl_vector_push(&v, &values[idx]), "Expected to be able to push value %#010X", values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_vector_length(&v) == idx + 1, "Expected length to be %i, not %i", idx + 1, dl_vector_length(&v)))
      goto push_pop_fail;
  }

  for (idx = 1; idx >= 0; --idx) {
    if (!dl_check(dl_vector_pop(&v, &popped), "Expected to be able to pop value"))
      goto push_pop_fail;
    if (!dl_check(popped == values[idx], "Expected %#010X == %#010X", popped, values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_vector_length(&v) == idx, "Expected length to be %i", idx))
      goto push_pop_fail;
  }

  if (!dl_check(dl_vector_capacity(&v) == 2, "Expected capacity to be 2, not %i", dl_vector_capacity(&v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_length(&v) == 0, "Expected length to be 0, not %i", dl_vector_length(&v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_is_empty(&v), "Expected vector to be empty"))
    goto push_pop_fail;

  dl_destroy_vector(&v);
  return true;
push_pop_fail:
  dl_destroy_vector(&v);
  return false;
}

/* grow */
dl_bool test_dl_vector_grow() {
  dl_vector v;

  if (!dl_check(dl_init_vector(&v, sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  if (!dl_check(dl_vector_capacity(&v) == 1, "Expected capacity to be 1, not %i", dl_vector_length(&v)))
    goto grow_fail;

  if (!dl_check(dl_vector_grow(&v, 16), "Expected vector to grow"))
    goto grow_fail;

  if (!dl_check(dl_vector_capacity(&v) == 17, "Expected vector capacity to be 17, not %i", dl_vector_capacity(&v)))
    goto grow_fail;

  if (!dl_check(!dl_vector_grow(&v, DL_NATURAL_MAX), "Expected vector capacity not to grow to %i", dl_vector_capacity(&v)))
    goto grow_fail;

  dl_destroy_vector(&v);
  return true;
grow_fail:
  dl_destroy_vector(&v);
  return false;
}
/* get, set, ref, ref_array */
/* insert, remove */
#endif
