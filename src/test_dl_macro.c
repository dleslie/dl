#include "dl.h"

#if DL_IS_ATLEAST_C99

dl_bool test_dl_swap() {
  dl_integer ca, cb;
  ca = 1;
  cb = 2;

  DL_SWAP(ca, cb);

  return dl_check(ca == 2 && cb == 1, "Expected values to swap, got: (1, 2) => (%i, %i)", ca, cb);
}

dl_bool test_dl_count_args() {
  dl_integer count;

  count = DL_COUNT_ARGS(1, 2, 3, 4, 5, 6, 7);

  return dl_check(7 == count, "Expected to have 7 arguments, found %i.", count);
}

dl_bool test_dl_argcar() {
  dl_integer first;

  first = DL_ARGCAR(1, 2, 3, 4, 5, 6, 7);

  return dl_check(1 == first, "Expected to receive head of arguments, instead found %i.", first);
}

dl_bool test_dl_argcdr() {
  dl_integer idx;
  dl_integer rest[] = {DL_ARGMAP(DL_COMMASPLICE, DL_ARGCDR(1, 2, 3, 4, 5, 6, 7))};

  if (!dl_check(sizeof(rest) == 6 * sizeof(dl_integer), "Expected sizeof rest to be %i, instead was %i.", 6 * sizeof(dl_integer), sizeof(rest)))
    return false;

  for (idx = 0; idx < 6; ++idx) {
    if (!dl_check(rest[idx] == idx + 2, "Expected rest index %i to be %i, was %i.", idx, idx + 2, rest[idx]))
      return false;
  }

  return true;
}

#define DL_ADD1(X) X + 1,

dl_bool test_dl_argmap() {
  dl_integer idx;
  dl_integer data[] = {DL_ARGMAP(DL_ADD1, DL_ARGLIST(1, 2, 3, 4, 5, 6, 7))};

  for (idx = 0; idx < 7; ++idx)
    if (!dl_check(data[idx] == idx + 2, "Expected list index %i to be %i, was %i.", idx, idx + 2, data[idx]))
      return false;

  return true;
}

#undef DL_ADD1

dl_bool test_dl_stringify() {
  const char *text = DL_STRINGIFY(Hi);

  return dl_check(text[0] == 'H' && text[1] == 'i' && text[2] == '\0', "Failed to stringify");
}

dl_bool test_dl_define_enum() {
  DL_DEFINE_ENUM(letters, a, b);

  if (!dl_check(a == 0, "Expected a to be defined as 0, was %i.", a))
    return false;

  if (!dl_check(b == 1, "Expected b to be defined as 0, was %i.", b))
    return false;

  if (!dl_check(letters_count == 2, "Expected count to be defined as 2, was %i.", letters_count))
    return false;

  if (!dl_check(letters_strings[a][0] == 'a', "Expected strings[a] to be a, was %c.", letters_strings[a][0]))
    return false;

  if (!dl_check(letters_strings[b][0] == 'b', "Expected strings[b] to be b, was %c.", letters_strings[b][0]))
    return false;

  if (!dl_check(letters_strings[letters_count][0] == '2', "Expected strings[letters_count] to be 2, was %c.", letters_strings[letters_count][0]))
    return false;

  return true;
}

#endif
