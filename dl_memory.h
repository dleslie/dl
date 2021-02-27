#ifndef DL_MEMORY_H
#define DL_MEMORY_H 1

#include "dl_core.h"

#if defined(__cplusplus)
extern "C" {
#endif

dl_api dl_ptr dl_memory_swap(dl_ptr target, dl_ptr source, dl_natural dl_bytes);
dl_api dl_ptr dl_memory_copy(dl_ptr target, dl_ptr source, dl_natural dl_bytes);
dl_api dl_ptr dl_memory_set(dl_ptr target, dl_byte val, dl_natural dl_bytes);

extern void *(*dl_alloc)(size_t sz);
extern void (*dl_free)(void *ptr);

#if defined(__cplusplus)
}
#endif

/*****************************************************************************
 **  IMPLEMENTATION
 ****************************************************************************/

#if defined(DL_IMPLEMENTATION)

#if defined(DL_USE_MALLOC)
#include <malloc.h>
void *(*dl_alloc)(size_t) = malloc;
void (*dl_free)(void *) = free;
#else
void *dl_alloc(size_t sz) { return 0; }
void dl_free(void *location) {}
#endif

dl_api dl_ptr dl_memory_swap(dl_ptr left, dl_ptr right, dl_natural dl_bytes) {
  size_t sz_count, byte_count, *sz_left, *sz_right, sz_temp;

  sz_count = dl_bytes / sizeof(size_t);
  byte_count = dl_bytes - (sz_count * sizeof(size_t));

  sz_left = (size_t *)left;
  sz_right = (size_t *)right;

  for (; sz_count > 0; --sz_count) {
    sz_temp = *sz_left;
    *sz_left = *sz_right;
    *sz_right = sz_temp;
    ++sz_left;
    ++sz_right;
  }

  if (byte_count > 0) {
    dl_byte *byte_left, *byte_right, byte_temp;
    byte_left = (dl_byte *)sz_left;
    byte_right = (dl_byte *)sz_right;

    for (; byte_count > 0; --byte_count) {
      byte_temp = *byte_left;
      *byte_left = *byte_right;
      *byte_right = byte_temp;
      ++byte_left;
      ++byte_right;
    }
  }

  return left;
}

dl_api dl_ptr dl_memory_copy(dl_ptr left, dl_ptr right, dl_natural dl_bytes) {
  size_t sz_count, byte_count, *sz_left, *sz_right;

  sz_count = dl_bytes / sizeof(size_t);
  byte_count = dl_bytes - (sz_count * sizeof(size_t));

  sz_left = (size_t *)left;
  sz_right = (size_t *)right;

  for (; sz_count > 0; --sz_count) {
    *sz_left = *sz_right;
    ++sz_left;
    ++sz_right;
  }

  if (dl_unlikely(byte_count > 0)) {
    dl_byte *byte_left, *byte_right;
    byte_left = (dl_byte *)sz_left;
    byte_right = (dl_byte *)sz_right;

    for (; byte_count > 0; --byte_count) {
      *byte_left = *byte_right;
      ++byte_left;
      ++byte_right;
    }
  }

  return left;
}

dl_ptr dl_memory_set(dl_ptr left, dl_byte val, dl_natural dl_bytes) {
  size_t *sz_left, sz_count, byte_count, sz_val, shift;

  sz_left = (size_t *)left;
  sz_count = dl_bytes / sizeof(size_t);
  byte_count = dl_bytes - (sz_count * sizeof(size_t));

  sz_val = val;

  for (shift = 1; shift < sizeof(size_t); ++shift)
    sz_val |= (val << shift);

  for (; sz_count > 0; --sz_count) {
    *(size_t *)sz_left = sz_val;
    ++sz_left;
  }

  if (dl_unlikely(byte_count > 0)) {
    dl_byte *byte_left;

    byte_left = (dl_byte *)sz_left;

    for (; byte_count > 0; --byte_count) {
      *(dl_byte *)byte_left = val;
      ++byte_left;
    }
  }

  return left;
}

#endif /* DL_IMPLEMENTATION */

#endif
