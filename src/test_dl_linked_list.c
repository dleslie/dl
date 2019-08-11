#include "dl.h"

#if DL_IS_ATLEAST_C99

dl_bool test_dl_init_linked_list() {
  dl_linked_list *l;
  if (!dl_check(l = dl_make_linked_list(sizeof(dl_natural), 32), "Expected dl_init_linked_list to return non-NULL"))
    return false;
  dl_destroy_linked_list(l);
  return true;
}

dl_bool test_dl_linked_list_push_pop() {
  dl_linked_list *l;
  dl_natural popped, values[2] = {0xDEADBEEF, 0xFFFFFFFF};
  dl_integer idx;

  if (!dl_check(l = dl_make_linked_list(sizeof(dl_natural), 1), "Expected dl_init_linked_list to return non-NULL"))
    return false;

  if (!dl_check(dl_linked_list_length(l) == 0, "Expected length to be 0, not %i", dl_linked_list_length(l)))
    goto push_pop_fail;

  for (idx = 0; idx < 2; ++idx) {
    if (!dl_check(dl_linked_list_push(l, &values[idx]), "Expected to be able to push value %#010x", values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_linked_list_length(l) == idx + 1, "Expected length to be %i, not %i", idx + 1, dl_linked_list_length(l)))
      goto push_pop_fail;
  }

  for (idx = 1; idx >= 0; --idx) {
    if (!dl_check(dl_linked_list_pop(l, &popped), "Expected to be able to pop value"))
      goto push_pop_fail;
    if (!dl_check(popped == values[idx], "Expected %#010x == %#010x", popped, values[idx]))
      goto push_pop_fail;
    if (!dl_check(dl_linked_list_length(l) == idx, "Expected length to be %i", idx))
      goto push_pop_fail;
  }

  if (!dl_check(dl_linked_list_length(l) == 0, "Expected length to be 0, not %i", dl_linked_list_length(l)))
    goto push_pop_fail;

  if (!dl_check(dl_linked_list_is_empty(l), "Expected linked_list to be empty"))
    goto push_pop_fail;

  dl_destroy_linked_list(l);
  return true;
push_pop_fail:
  dl_destroy_linked_list(l);
  return false;
}

dl_bool test_dl_linked_list_get_set_ref() {
  dl_linked_list *l;
  dl_linked_list_node *n;
  dl_natural value, idx;
  dl_natural values[4] = {0xDEADBEEF, 0xFFFFFFFF, 0xFEEDADAD, 0xACEDAD69};
  dl_natural new_values[4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};

  if (!dl_check(l = dl_make_linked_list(sizeof(dl_natural), 1), "Expected dl_init_linked_list to return non-NULL"))
    return false;

  for (idx = 0; idx < 4; ++idx)
    dl_linked_list_push(l, &values[idx]);

  for (idx = 0; idx < 4; ++idx) {
    n = dl_linked_list_index(l, idx);
    if (!dl_check(NULL != dl_linked_list_get(l, n, &value), "Expected get to work"))
      goto get_set_ref_fail;

    if (!dl_check(value == values[idx], "Expected %#010x == %#010x", value, values[idx]))
      goto get_set_ref_fail;

    if (!dl_check(NULL != dl_linked_list_set(l, n, &new_values[idx]), "Expected set to work"))
      goto get_set_ref_fail;

    value = *((dl_natural *)DL_LINKED_LIST_DATA(n));
    if (!dl_check(value == new_values[idx], "Expected %#010x == %#010x", value, new_values[idx]))
      goto get_set_ref_fail;

    if (!dl_check(new_values[idx] == *(dl_natural *)dl_linked_list_ref(l, n), "Expected %#010x == %#010x", new_values[idx], *(dl_natural *)dl_linked_list_ref(l, n)))
      goto get_set_ref_fail;
  }

  dl_destroy_linked_list(l);
  return true;
get_set_ref_fail:
  dl_destroy_linked_list(l);
  return false;
}

dl_bool test_dl_linked_list_insert_remove() {
  dl_linked_list *l;
  dl_natural values[4] = {0xDEADBEEF, 0xFFFFFFFF, 0xFEEDADAD, 0xACEDAD69};
  dl_integer idx;

  if (!dl_check(l = dl_make_linked_list(sizeof(dl_natural), 1), "Expected dl_init_linked_list to return non-NULL"))
    return false;

  for (idx = 3; idx >= 0; --idx) {
    if (!dl_check(NULL != dl_linked_list_insert(l, dl_linked_list_index(l, 0), &values[idx]), "Expected insert to work"))
      goto insert_remove_fail;

    if (!dl_check(4 - idx == dl_linked_list_length(l), "Expected length to be %i not %i", 4 - idx, dl_linked_list_length(l)))
      goto insert_remove_fail;

    if (!dl_check(values[idx] == *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 0)), "Expected %#010x == %#010x", values[idx], *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 0))))
      goto insert_remove_fail;

    if (idx < 3 && !dl_check(values[idx + 1] == *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 1)), "Expected %#010x == %#010x", values[idx + 1], *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 1))))
      goto insert_remove_fail;
  }

  for (idx = 0; idx <= 3; ++idx) {
    if (!dl_check(dl_linked_list_remove(l, dl_linked_list_index(l, 0)), "Expected remove to work"))
      goto insert_remove_fail;

    if (!dl_check(!dl_linked_list_remove(l, dl_linked_list_index(l, DL_NATURAL_MAX)), "Expected remove to fail"))
      goto insert_remove_fail;

    if (idx < 3 && !dl_check(values[idx + 1] == *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 0)), "Expected %#010x == %#010x", values[idx + 1], *(dl_natural *)dl_linked_list_ref(l, dl_linked_list_index(l, 0))))
      goto insert_remove_fail;

    if (!dl_check(dl_linked_list_length(l) == 4 - idx - 1, "Expected length to be %i, not %i", 4 - idx - 1, dl_linked_list_length(l)))
      goto insert_remove_fail;
  }

  dl_destroy_linked_list(l);
  return true;
insert_remove_fail:
  dl_destroy_linked_list(l);
  return false;
}

#endif
