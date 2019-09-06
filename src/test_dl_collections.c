#include "dl.h"

#if DL_IS_ATLEAST_C99

typedef struct {
  const char *type_name;
  dl_container_interface *interface;
  dl_natural traits;
} _container_info;

_container_info infos[] = {
  {"vector", &dl_vector_container_interface, DL_CONTAINER_TRAIT_RANDOM_ACCESS},
  {"linked list", &dl_linked_list_container_interface, DL_CONTAINER_TRAIT_RANDOM_INSERT}};
dl_natural info_count = 2;

dl_bool test_dl_make_container() {
  dl_container *c;
  dl_natural idx;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, 37, 128), "Make %s container failed.", infos[idx].type_name))
      return false;
    dl_destroy_container(c);
  }

  return true;
}

dl_bool test_dl_container_element_size() {
  dl_container *c;
  dl_natural idx;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, 37, 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    if (!dl_check(37 == dl_container_element_size(c), "Expected %s element size to be %i, was %i.", infos[idx].type_name, 37, dl_container_element_size(c)))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_length() {
  dl_container *c;
  dl_natural idx, idx2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2) {
      if (!dl_check(idx2 == dl_container_length(c), "Expected %s length to be %i, was %i.", infos[idx].type_name, idx2, dl_container_length(c)))
        goto error;
      dl_container_push(c, &idx2);
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_is_empty() {
  dl_container *c;
  dl_natural idx;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    if (!dl_check(dl_container_is_empty(c), "Expected %s to be empty", infos[idx].type_name))
      goto error;

    dl_container_push(c, &idx);

    if (!dl_check(!dl_container_is_empty(c), "Expected %s not to be empty", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_index() {
  dl_container *c;
  dl_natural idx, idx2;
  dl_iterator iter;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter = dl_container_index(c, 1);
    if (!dl_check(dl_iterator_is_valid(iter), "Expected %s iterator to be valid.", infos[idx].type_name))
      goto error;

    idx2 = dl_iterator_index(iter);
    if (!dl_check(idx2 == 1, "Expected %s iterator to point at position $i.", infos[idx].type_name, 1))
      goto error;

    iter = dl_container_index(c, 11);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s iterator to be invalid.", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_first() {
  dl_container *c;
  dl_natural idx, idx2;
  dl_iterator iter;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter = dl_container_first(c);
    if (!dl_check(0 == dl_iterator_index(iter), "Expected %s first index to be 0", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_last() {
  dl_container *c;
  dl_natural idx, idx2;
  dl_iterator iter;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter = dl_container_last(c);
    if (!dl_check(9 == dl_iterator_index(iter), "Expected %s last index to be 9", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_push() {
  dl_container *c;
  dl_natural idx, idx2, val;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 0; idx2 < 10; ++idx2) {
      dl_iterator_get(dl_container_index(c, idx2), &val);
      if (!dl_check(idx2 == val, "Expected %s index %i to be %i not %i.", infos[idx].type_name, idx2, idx2, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_pop() {
  dl_container *c;
  dl_natural idx, idx2, val;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 10; idx2 > 0; --idx2) {
      dl_container_pop(c, &val);
      if (!dl_check(idx2 - 1 == val, "Expected %s index %i to be %i not %i.", infos[idx].type_name, 10 - idx2, idx2 - 1, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_traits() {
  dl_container *c;
  dl_natural idx;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    if (!dl_check(dl_container_traits(c) == infos[idx].traits, "Expected %s traits to be %i, not %i.", infos[idx].type_name, infos[idx].traits, dl_container_traits(c)))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_make_invalid_iterator() {
  dl_container *c;
  dl_natural idx;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    if (!dl_check(!dl_iterator_is_valid(dl_make_invalid_iterator(c)), "Expected %s iterator to not be valid.", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_is_valid() {
  dl_container *c;
  dl_natural idx, idx2;
  dl_iterator iter;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter = dl_container_index(c, 1);
    if (!dl_check(dl_iterator_is_valid(iter), "Expected %s iterator to be valid.", infos[idx].type_name))
      goto error;

    iter = dl_container_index(c, 11);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s iterator to be invalid.", infos[idx].type_name))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_get() {
  dl_container *c;
  dl_natural idx, idx2, val;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 0; idx2 < 10; ++idx2) {
      dl_iterator_get(dl_container_index(c, idx2), &val);
      if (!dl_check(idx2 == val, "Expected %s index %i to be %i not %i.", infos[idx].type_name, idx2, idx2, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_ref() {
  dl_container *c;
  dl_natural idx, idx2, val;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 0; idx2 < 10; ++idx2) {
      val = *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx2));
      if (!dl_check(idx2 == val, "Expected %s index %i to be %i not %i.", infos[idx].type_name, idx2, idx2, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_set() {
  dl_container *c;
  dl_natural idx, idx2, val, val2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 0; idx2 < 10; ++idx2) {
      val = -idx2;
      dl_iterator_set(dl_container_index(c, idx2), &val);
      val2 = *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx2));
      if (!dl_check(val == val2, "Expected %s index %i to be %i not %i.", infos[idx].type_name, idx2, val, val2))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_swap() {
  dl_container *c;
  dl_natural idx, idx2, val;
  dl_iterator iter1, iter2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (idx2 = 0; idx2 < 5; ++idx2) {
      iter1 = dl_container_index(c, idx2);
      iter2 = dl_container_index(c, 9 - idx2);
      if (!dl_check(dl_iterator_swap(iter1, iter2), "Expected %s iterator swap to work.", infos[idx].type_name))
        goto error;
    }

    for (idx2 = 0; idx2 < 10; ++idx2) {
      val = *(dl_natural *)dl_iterator_ref(dl_container_index(c, 9 - idx2));
      if (!dl_check(idx2 == val, "Expected %s iterator index %i to be %i not %i.", infos[idx].type_name, 9 - idx2, idx2, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_insert() {
  dl_container *c;
  dl_natural idx, idx2, val, val2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    val = 0;
    dl_container_push(c, &val);

    for (idx2 = 1; idx2 < 10; ++idx2)
      dl_iterator_insert(dl_container_first(c), &idx2);

    if (!dl_check(10 == dl_container_length(c), "Expected %s container length to be 10, not %i.", infos[idx].type_name, dl_container_length(c)))
      goto error;

    for (idx2 = 0; idx2 < 10; ++idx2) {
      val = *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx2));
      val2 = 9 - idx2;
      if (!dl_check(val2 == val, "Expected %s iterator index %i to be %i not %i.", infos[idx].type_name, idx2, val2, val))
        goto error;
    }

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("%i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_iterator_remove() {
  return false;
}

dl_bool test_dl_iterator_next() {
  return false;
}

dl_bool test_dl_iterator_prev() {
  return false;
}

dl_bool test_dl_iterator_next_ref() {
  return false;
}

dl_bool test_dl_iterator_prev_ref() {
  return false;
}

dl_bool test_dl_iterator_equal() {
  return false;
}

dl_bool test_dl_iterator_compare() {
  return false;
}

dl_bool test_dl_iterator_index() {
  return false;
}

#endif
