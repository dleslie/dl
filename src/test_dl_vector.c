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

  if (!dl_check(dl_init_vector(&v, sizeof(dl_natural), 32), "Expected dl_init_vector to return non-NULL"))
    return false;

  for (idx = 0; idx < 2; ++idx) {
    if (!dl_check(dl_vector_push(&v, &values[idx]), "Expected to be able to push value %#010X", values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_vector_length(&v) == idx + 1, "Expected length to be %i", idx + 1))
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

  dl_destroy_vector(&v);
  return true;
push_pop_fail:
  dl_destroy_vector(&v);
  return false;
}
#endif
