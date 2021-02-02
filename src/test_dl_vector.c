#include "dl.h"

#if DL_IS_ATLEAST_C99 && DL_USE_MALLOC

dl_bool test_dl_init_vector() {
  dl_vector *v;
  if (!dl_check(v = dl_make_vector(sizeof(dl_natural), 32), "Expected dl_init_vector to return non-NULL"))
    return false;
  dl_destroy_vector(v);
  return true;
}

dl_bool test_dl_vector_push_pop() {
  dl_vector *v;
  dl_natural popped, values[2] = {0xDEADBEEF, 0xFFFFFFFF};
  dl_integer idx;

  if (!dl_check(v = dl_make_vector(sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  if (!dl_check(dl_vector_capacity(v) == 1, "Expected capacity to be 1, not %i", dl_vector_capacity(v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_length(v) == 0, "Expected length to be 0, not %i", dl_vector_length(v)))
    goto push_pop_fail;

  for (idx = 0; idx < 2; ++idx) {
    if (!dl_check(dl_vector_push(v, &values[idx]), "Expected to be able to push value %#010x", values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_vector_length(v) == idx + 1, "Expected length to be %i, not %i", idx + 1, dl_vector_length(v)))
      goto push_pop_fail;
  }

  for (idx = 1; idx >= 0; --idx) {
    if (!dl_check(dl_vector_pop(v, &popped), "Expected to be able to pop value"))
      goto push_pop_fail;
    if (!dl_check(popped == values[idx], "Expected %#010x == %#010x", popped, values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_vector_length(v) == idx, "Expected length to be %i", idx))
      goto push_pop_fail;
  }

  if (!dl_check(dl_vector_capacity(v) == 1 + DL_VECTOR_CAPACITY_GROWTH, "Expected capacity to be %i, not %i", 1 + DL_VECTOR_CAPACITY_GROWTH, dl_vector_capacity(v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_length(v) == 0, "Expected length to be 0, not %i", dl_vector_length(v)))
    goto push_pop_fail;

  if (!dl_check(dl_vector_is_empty(v), "Expected vector to be empty"))
    goto push_pop_fail;

  dl_destroy_vector(v);
  return true;
push_pop_fail:
  dl_destroy_vector(v);
  return false;
}

dl_bool test_dl_vector_grow() {
  dl_vector *v;

  if (!dl_check(v = dl_make_vector(sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  if (!dl_check(dl_vector_capacity(v) == 1, "Expected capacity to be 1, not %i", dl_vector_length(v)))
    goto grow_fail;

  if (!dl_check(dl_vector_grow(v, 16), "Expected vector to grow"))
    goto grow_fail;

  if (!dl_check(dl_vector_capacity(v) == 17, "Expected vector capacity to be 17, not %i", dl_vector_capacity(v)))
    goto grow_fail;

  if (!dl_check(!dl_vector_grow(v, DL_NATURAL_MAX), "Expected vector capacity not to grow to %i", dl_vector_capacity(v)))
    goto grow_fail;

  dl_destroy_vector(v);
  return true;
grow_fail:
  dl_destroy_vector(v);
  return false;
}

dl_bool test_dl_vector_get_set_ref() {
  dl_vector *v;
  dl_natural value, idx;
  dl_natural values[4] = {0xDEADBEEF, 0xFFFFFFFF, 0xFEEDADAD, 0xACEDAD69};
  dl_natural new_values[4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};

  if (!dl_check(v = dl_make_vector(sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  for (idx = 0; idx < 4; ++idx)
    dl_vector_push(v, &values[idx]);

  for (idx = 0; idx < 4; ++idx) {
    if (!dl_check(NULL != dl_vector_get(v, idx, &value), "Expected get to work"))
      goto get_set_ref_fail;

    if (!dl_check(value == values[idx], "Expected %#010x == %#010x", value, values[idx]))
      goto get_set_ref_fail;

    if (!dl_check(NULL != dl_vector_set(v, idx, &new_values[idx]), "Expected set to work"))
      goto get_set_ref_fail;

    value = ((dl_natural *)v->array)[idx];
    if (!dl_check(value == new_values[idx], "Expected %#010x == %#010x", value, new_values[idx]))
      goto get_set_ref_fail;

    if (!dl_check(new_values[idx] == *(dl_natural *)dl_vector_ref(v, idx), "Expected %#010x == %#010x", new_values[idx], *(dl_natural *)dl_vector_ref(v, idx)))
      goto get_set_ref_fail;
  }

  dl_destroy_vector(v);
  return true;
get_set_ref_fail:
  dl_destroy_vector(v);
  return false;
}

dl_bool test_dl_vector_insert_remove() {
  dl_vector *v;
  dl_natural values[4] = {0xDEADBEEF, 0xFFFFFFFF, 0xFEEDADAD, 0xACEDAD69};
  dl_integer idx;

  if (!dl_check(v = dl_make_vector(sizeof(dl_natural), 1), "Expected dl_init_vector to return non-NULL"))
    return false;

  for (idx = 3; idx >= 0; --idx) {
    if (!dl_check(0 == dl_vector_insert(v, 0, &values[idx]), "Expected insert to work"))
      goto insert_remove_fail;

    if (!dl_check(-1 == dl_vector_insert(v, DL_NATURAL_MAX, &values[idx]), "Expected insert to fail"))
      goto insert_remove_fail;

    if (!dl_check(4 - idx == dl_vector_length(v), "Expected length to be %i not %i", 4 - idx, dl_vector_length(v)))
      goto insert_remove_fail;

    if (!dl_check(values[idx] == *(dl_natural *)dl_vector_ref(v, 0), "Expected %#010x == %#010x", values[idx], *(dl_natural *)dl_vector_ref(v, 0)))
      goto insert_remove_fail;

    if (idx < 3 && !dl_check(values[idx + 1] == *(dl_natural *)dl_vector_ref(v, 1), "Expected %#010x == %#010x", &values[idx + 1], *(dl_natural *)dl_vector_ref(v, 1)))
      goto insert_remove_fail;
  }

  for (idx = 0; idx <= 3; ++idx) {
    if (!dl_check(dl_vector_remove(v, 0), "Expected remove to work"))
      goto insert_remove_fail;

    if (!dl_check(!dl_vector_remove(v, DL_NATURAL_MAX), "Expected remove to fail"))
      goto insert_remove_fail;

    if (idx < 3 && !dl_check(values[idx + 1] == *(dl_natural *)dl_vector_ref(v, 0), "Expected %#010x == %#010x", &values[idx + 1], *(dl_natural *)dl_vector_ref(v, 0)))
      goto insert_remove_fail;

    if (!dl_check(dl_vector_length(v) == 4 - idx - 1, "Expected length to be %i, not %i", 4 - idx - 1, dl_vector_length(v)))
      goto insert_remove_fail;
  }

  dl_destroy_vector(v);
  return true;
insert_remove_fail:
  dl_destroy_vector(v);
  return false;
}

#endif
