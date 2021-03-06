#include "dl.h"

#if DL_IS_ATLEAST_C99

dl_bool my_passing_test_function() {
  return true;
}

dl_bool my_failing_test_function() {
  return false;
}

dl_bool test_test_count_tests() {
  dl_bool (*predicates[3])();
  dl_memory_set(predicates, 0, sizeof(predicates));
  predicates[0] = &my_passing_test_function;
  predicates[1] = &my_passing_test_function;

  return dl_check(dl_test_count(predicates, 3) == 2, "Expected the number of tests to be 2");
}

dl_bool test_test_run_tests() {
  dl_bool (*passing_predicates[2])();
  dl_bool (*failing_predicates[2])();
  const char *passing_names[2];
  const char *failing_names[2];
  failing_names[0] = "IGNORE_first_should_fail";
  failing_names[1] = "IGNORE_second_should_pass";
  passing_names[0] = "IGNORE_first_should_pass";
  passing_names[1] = "IGNORE_second_should_pass";
  passing_predicates[0] = my_passing_test_function;
  passing_predicates[1] = my_passing_test_function;
  failing_predicates[0] = my_failing_test_function;
  failing_predicates[1] = my_passing_test_function;

  return dl_check(dl_test_run(passing_predicates, passing_names, 2) == 2, "Expected the number of passing tests to be 2") && dl_check(dl_test_run(failing_predicates, failing_names, 2) == 1, "Expected the number of passing tests to be 1");
}

#endif