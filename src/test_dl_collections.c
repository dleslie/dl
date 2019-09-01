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
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_container_index() {
  return false;
}

dl_bool test_dl_container_first() {
  return false;
}

dl_bool test_dl_container_last() {
  return false;
}

dl_bool test_dl_container_push() {
  return false;
}

dl_bool test_dl_container_pop() {
  return false;
}

dl_bool test_dl_container_traits() {
  return false;
}

dl_bool test_dl_make_invalid_iterator() {
  return false;
}

dl_bool test_dl_iterator_is_valid() {
  return false;
}

dl_bool test_dl_iterator_get() {
  return false;
}
dl_bool test_dl_iterator_ref() {
  return false;
}
dl_bool test_dl_iterator_set() {
  return false;
}

dl_bool test_dl_iterator_swap() {
  return false;
}

dl_bool test_dl_iterator_insert() {
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
