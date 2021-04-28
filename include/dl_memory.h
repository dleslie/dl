#ifndef DL_MEMORY_H
#define DL_MEMORY_H 1

#include "dl_core.h"

#if defined(__cplusplus)
extern "C" {
#endif

dl_api dl_ptr dl_memory_swap(dl_ptr target, dl_ptr source, dl_natural dl_bytes);
dl_api dl_ptr dl_memory_copy(dl_ptr target, dl_ptr source, dl_natural dl_bytes);
dl_api dl_ptr dl_memory_set(dl_ptr target, dl_byte val, dl_natural dl_bytes);

typedef void *(*dl_memory_allocator)(size_t sz);
typedef void (*dl_memory_releaser)(void *ptr);

struct dl_memory_manager;
struct dl_memory_manager {
  dl_memory_allocator alloc;
  dl_memory_releaser free;
  struct dl_memory_manager *_next;
};
typedef struct dl_memory_manager dl_memory_manager;

extern void *(*dl_alloc)(size_t sz);
extern void (*dl_free)(void *ptr);

dl_api dl_ptr dl_push_memory_manager(dl_memory_manager *manager);
dl_api dl_ptr dl_pop_memory_manager();
extern

#if defined(__cplusplus)
}
#endif

/*****************************************************************************
 **  IMPLEMENTATION
 ****************************************************************************/

#if defined(DL_IMPLEMENTATION)

#if defined(DL_USE_MALLOC)
#include <malloc.h>
dl_memory_manager root_allocator = {(dl_memory_allocator)malloc, (dl_memory_releaser)free, NULL};
void *(*dl_alloc)(size_t) = malloc;
void (*dl_free)(void *) = free;
#else
void *__dl_alloc(size_t sz) { return 0; }
void __dl_free(void *location) {}
dl_memory_manager root_allocator = {(dl_memory_allocator)__dl_alloc, (dl_memory_releaser)__dl_free, NULL};
void *(*dl_alloc)(size_t) = __dl_alloc;
void (*dl_free)(void *) = __dl_free;
#endif

dl_memory_manager *current_allocator = &root_allocator;

dl_api dl_ptr dl_push_memory_manager(dl_memory_manager *manager) {
  if (manager == NULL) return NULL;
  manager->_next = current_allocator;
  current_allocator = manager;
  dl_alloc = manager->alloc;
  dl_free = manager->free;
  return (dl_api)current_allocator;
}

dl_api dl_ptr dl_pop_memory_manager() {
  dl_ptr popped;
  if (current_allocator->_next == NULL) return NULL;
  popped = current_allocator;
  current_allocator = current_allocator->_next;
  dl_alloc = current_allocator->alloc;
  dl_free = current_allocator->free;
  return popped;
}

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
