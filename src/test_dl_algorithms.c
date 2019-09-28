#include "dl.h"

#if DL_IS_ATLEAST_C99
#include "test_containers.h"

dl_integer _integer_filter(dl_ptr data, const dl_ptr value) {
  dl_integer left, right;
  left = *(dl_integer *)data;
  right = *(dl_integer *)value;
  return left - right;
}

dl_integer _even_integer_filter(dl_ptr data, const dl_ptr value) {
  return (*(dl_integer *)value % 2) == 0;
}

dl_ptr _push_handler(dl_ptr data, dl_ptr value) {
  return dl_container_push((dl_container *)data, value);
}

dl_ptr _fold_handler(dl_ptr data, dl_ptr item, dl_ptr left) {
  DL_INFO("%i %s %i", *(dl_integer *)item, (*(dl_bool *)data ? "*" : "+"), *(dl_integer *)left);
  *(dl_integer *)item = *(dl_bool *)data ? (*(dl_integer *)item * *(dl_integer *)left) : (*(dl_integer *)item + *(dl_integer *)left);
  *(dl_bool *)data = !*(dl_bool *)data;
  return item;
}

dl_bool test_dl_count() {
  dl_container *c;
  dl_natural idx, idx2, cnt;
  dl_iterator iter1, iter2;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_natural), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    iter1 = dl_container_index(c, 3);
    iter2 = dl_container_index(c, 7);

    cnt = dl_count(iter1, iter2);
    if (!dl_check(5 == cnt, "Expected %s count from 3 to 7 to be 5, not %i.", infos[idx].type_name, cnt))
      goto error;

    cnt = dl_count(iter1, dl_make_invalid_iterator(c));
    if (!dl_check(0 == cnt, "Expected %s count with invalid iterator to be 0, not %i.", infos[idx].type_name, cnt))
      goto error;

    cnt = dl_count(dl_make_invalid_iterator(c), iter2);
    if (!dl_check(0 == cnt, "Expected %s count with invalid iterator to be 0, not %i.", infos[idx].type_name, cnt))
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
  dl_container *c;
  dl_integer idx, idx2, value;
  dl_iterator iter;
  dl_filter filter = dl_make_filter(&value, _integer_filter);

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (value = 0; value < 10; ++value) {
      iter = dl_find(dl_container_first(c), dl_container_last(c), filter);
      if (!dl_check(dl_iterator_is_valid(iter) && value == *(dl_integer *)dl_iterator_ref(iter), "Expected %s to contain %i.", infos[idx].type_name, value))
        goto error;
    }

    value = -1;
    iter = dl_find(dl_container_first(c), dl_container_last(c), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to contain %i.", infos[idx].type_name, value))
      goto error;

    value = 0;
    iter = dl_find(dl_make_invalid_iterator(), dl_container_last(c), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to work with invalid left.", infos[idx].type_name))
      goto error;

    iter = dl_find(dl_container_first(c), dl_make_invalid_iterator(), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to work with invalid right.", infos[idx].type_name))
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

dl_bool test_dl_find_reverse() {
  dl_container *c;
  dl_integer idx, idx2, value;
  dl_iterator iter;
  dl_filter filter = dl_make_filter(&value, _integer_filter);

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    for (value = 0; value < 10; ++value) {
      iter = dl_find_reverse(dl_container_first(c), dl_container_last(c), filter);
      if (!dl_check(dl_iterator_is_valid(iter) && value == *(dl_integer *)dl_iterator_ref(iter), "Expected %s to contain %i.", infos[idx].type_name, value))
        goto error;
    }

    value = -1;
    iter = dl_find_reverse(dl_container_first(c), dl_container_last(c), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to contain %i.", infos[idx].type_name, value))
      goto error;

    value = 0;
    iter = dl_find_reverse(dl_make_invalid_iterator(), dl_container_last(c), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to work with invalid left.", infos[idx].type_name))
      goto error;

    iter = dl_find_reverse(dl_container_first(c), dl_make_invalid_iterator(), filter);
    if (!dl_check(!dl_iterator_is_valid(iter), "Expected %s not to work with invalid right.", infos[idx].type_name))
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

dl_bool test_dl_where() {
  dl_container *c = NULL;
  dl_container *c2 = NULL;
  dl_integer idx, idx2, count;
  dl_filter filter = dl_make_filter(NULL, _even_integer_filter);
  dl_handler out;

  for (idx = 0; idx < info_count; ++idx) {
    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    if (!dl_check(c2 = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name)) {
      dl_destroy_container(c);
      return false;
    }

    out = dl_make_handler(c2, _push_handler);

    count = dl_where(dl_container_first(c), dl_container_last(c), filter, out);

    if (!dl_check(count == 5, "Expected %s where to produce 5 results, not %i", infos[idx].type_name, count))
      goto error;

    dl_destroy_container(c);
    dl_destroy_container(c2);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("C1 %i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  for (idx = 0; idx < dl_container_length(c2); ++idx) {
    DL_INFO("C2 %i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c2, idx)));
  }
  dl_destroy_container(c);
  dl_destroy_container(c2);
  return false;
}

dl_bool test_dl_foldl() {
  dl_container *c = NULL;
  dl_integer idx, idx2, outcome, desired;
  dl_bool truth = true;
  dl_folder folder = dl_make_folder(&truth, _fold_handler);

  desired = ((((((((((0 * 0) + 1) * 2) + 3) * 4) + 5) * 6) + 7) * 8) + 9);

  for (idx = 0; idx < info_count; ++idx) {
    outcome = 0;

    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    if (!dl_check(NULL != dl_foldl(dl_container_first(c), dl_container_last(c), &outcome, folder), "Expected %s foldl to succeed.", infos[idx].type_name))
      goto error;

    if (!dl_check(desired == outcome, "Expected %s foldl to produce %i not %i.", infos[idx].type_name, desired, outcome))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("C1 %i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
  return false;
}

dl_bool test_dl_foldr() {
  dl_container *c = NULL;
  dl_integer idx, idx2, outcome, desired;
  dl_bool truth = true;
  dl_folder folder = dl_make_folder(&truth, _fold_handler);

  desired = ((((((((((0 * 9) + 8) * 7) + 6) * 5) + 4) * 3) + 2) * 1) + 0);

  for (idx = 0; idx < info_count; ++idx) {
    outcome = 0;

    if (!dl_check(c = dl_make_container(infos[idx].interface, sizeof(dl_integer), 128), "Make %s container failed.", infos[idx].type_name))
      return false;

    for (idx2 = 0; idx2 < 10; ++idx2)
      dl_container_push(c, &idx2);

    if (!dl_check(NULL != dl_foldr(dl_container_first(c), dl_container_last(c), &outcome, folder), "Expected %s foldr to succeed.", infos[idx].type_name))
      goto error;

    if (!dl_check(desired == outcome, "Expected %s foldr to produce %i not %i.", infos[idx].type_name, desired, outcome))
      goto error;

    dl_destroy_container(c);
  }

  return true;
error:
  for (idx = 0; idx < dl_container_length(c); ++idx) {
    DL_INFO("C1 %i: %i", idx, *(dl_natural *)dl_iterator_ref(dl_container_index(c, idx)));
  }
  dl_destroy_container(c);
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