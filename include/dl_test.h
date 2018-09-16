#ifndef DL_TEST_H
#define DL_TEST_H

#include "dl.h"
#include "dl_logging.h"

#if !DL_IS_ATLEAST_C99
#  error "DL_TEST is not available for C90 or lower."
#endif

#ifdef __cplusplus
extern "C" {
#endif


  
  dl_integer dl_test_run(dl_bool (**tests)(), const char **names, dl_integer count);
  dl_integer dl_test_count(dl_bool (**tests)(), dl_integer max);

#define dl_check(predicate, ...) ((predicate) ? true : DL_ERROR(__VA_ARGS__) && false)
#define dl_not_implemented() dl_check(false, "Test is not implemented.")
#define dl_begin_test_suite(name)						\
  void _test_suite_##name(dl_integer *out_count, dl_integer *out_passed) {	\
  dl_bool (*tests[256])();							\
  const char *test_names[256];							\
  dl_integer current = 0;							\
  if (out_count == NULL || out_passed == NULL)					\
    return;									\
  dl_memory_set(tests, 0, sizeof(dl_bool (*)()) * 256);
#define dl_end_test_suite							\
  *out_count = dl_test_count(tests, 256);					\
  *out_passed = dl_test_run(tests, test_names, *out_count);			\
}
#define dl_declare_test(test_name)						\
  dl_bool test_name();								\
  tests[current] = test_name;							\
  test_names[current] = #test_name;						\
  current++;
#define dl_run_test_suite(name)							\
  do {										\
    dl_integer count, passed;							\
    DL_TEST("<<"#name ">>");							\
    _test_suite_##name(&count, &passed);					\
    DL_TEST("<<" #name ">> %i/%i", passed, count);				\
  } while (0)

#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

dl_integer dl_test_run(dl_bool (**tests)(), const char **names, dl_integer count) {
  dl_integer i, passed = 0;

  for (i = 0; i < count; ++i) {
    DL_TEST("[%3i/%3i] %s", i + 1, count, names[i]);
    if (tests[i]())
      passed++;
  }

  return passed;
}

dl_integer dl_test_count(dl_bool (**tests)(), dl_integer max) {
  dl_integer idx;
  
  for (idx = 0; idx < max && tests[idx] != NULL; ++idx);
  return idx;
}

#endif /* DL_IMPLEMENTATION */

#endif
