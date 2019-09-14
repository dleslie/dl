#include "dl.h"

#if DL_IS_ATLEAST_C99
#include "test_containers.h"

dl_bool test_dl_count() {
  dl_container *c;
  dl_natural idx, idx2, cnt;
  dl_iterator iter1, iter2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter1 = dl_container_index(c, 2);
    iter2 = dl_container_index(c, 7);

    cnt = dl_count(iter1, iter2);
    if (!dl_check(5 == cnt, "Expected %s count between 2 and 7 to be 5, not %i.", infos[idx].type_name, cnt))
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

dl_bool test_dl_find() {
  return false;
}

dl_bool test_dl_find_reverse() {
  return false;
}

dl_bool test_dl_find_all() {
  return false;
}

dl_bool test_dl_foldl() {
  return false;
}

dl_bool test_dl_foldr() {
  return false;
}

dl_bool test_dl_all() {
  return false;
}

dl_bool test_dl_any() {
  return false;
}

dl_bool test_dl_map() {
  return false;
}

dl_bool test_dl_zip() {
  return false;
}

dl_bool test_dl_take() {
  return false;
}

dl_bool test_dl_drop() {
  return false;
}

dl_bool test_dl_remove() {
  return false;
}

dl_bool test_dl_remove_reverse() {
  return false;
}

dl_bool test_dl_remove_all() {
  return false;
}

dl_bool test_dl_remove_count() {
  return false;
}

dl_bool test_dl_quick_sort() {
  return false;
}

dl_bool test_dl_make_handler() {
  return false;
}

dl_bool test_dl_make_zipper() {
  return false;
}

dl_bool test_dl_make_filter() {
  return false;
}

dl_bool test_dl_make_folder() {
  return false;
}

dl_bool test_dl_make_comparator() {
  return false;
}

#endif
