#ifndef DL_H
#define DL_H 1

/***************************************
 * Configuration
 **************************************/

#ifndef DL_USE_EXTENSIONS
# define DL_USE_EXTENSIONS 0
#endif

#ifndef DL_USE_SAFETY_CHECKS
# define DL_USE_SAFETY_CHECKS 0
#endif

#if !defined(DL_ALLOC)
# include <malloc.h>
# define DL_ALLOC malloc
#endif

#if !defined(DL_FREE)
# include <malloc.h>
# define DL_FREE free
#endif


/***************************************
 * Environment
 **************************************/

#if defined(__STDC__)
# if __STDC_VERSION__ >= 201112L
#   define DL_IS_C11 1
#   define DL_IS_C99 0
#   define DL_IS_C90 0
#   define DL_IS_C89 0
# elif __STDC_VERSION__ >= 199901L
#   define DL_IS_C11 0
#   define DL_IS_C99 1
#   define DL_IS_C90 0
#   define DL_IS_C89 0
# elif __STDC_VERSION__ >= 199409L
#   define DL_IS_C11 0
#   define DL_IS_C99 0
#   define DL_IS_C90 1
#   define DL_IS_C89 0
# else
#   define DL_IS_C11 0
#   define DL_IS_C99 0
#   define DL_IS_C90 0
#   define DL_IS_C89 1
# endif
#else
# error "C version not determined."
# define DL_IS_C11 0
# define DL_IS_C99 0
# define DL_IS_C90 0
# define DL_IS_C89 0
#endif

#if defined(__cplusplus)
# if __cplusplus <= 199711L
#   define DL_IS_CPP98 1
#   define DL_IS_CPP11 0
#   define DL_IS_CPP14 0
#   define DL_IS_CPP17 0
# elif __cplusplus <= 201103L
#   define DL_IS_CPP98 0
#   define DL_IS_CPP11 1
#   define DL_IS_CPP14 0
#   define DL_IS_CPP17 0
# elif __cplusplus <= 201402L
#   define DL_IS_CPP98 0
#   define DL_IS_CPP11 0
#   define DL_IS_CPP14 1
#   define DL_IS_CPP17 0
# elif __cplusplus <= 201703L
#   define DL_IS_CPP98 0
#   define DL_IS_CPP11 0
#   define DL_IS_CPP14 0
#   define DL_IS_CPP17 1
#endif
#else
# define DL_IS_CPP98 0
# define DL_IS_CPP11 0
# define DL_IS_CPP14 0
# define DL_IS_CPP17 0
#endif

#define DL_IS_ATLEAST_C89 (DL_IS_C11 || DL_IS_C99 || DL_IS_C90 || DL_IS_C89)
#define DL_IS_ATLEAST_C90 (DL_IS_C11 || DL_IS_C99 || DL_IS_C90)
#define DL_IS_ATLEAST_C99 (DL_IS_C11 || DL_IS_C99)
#define DL_IS_ATLEAST_C11 (DL_IS_C11)

#define DL_IS_CPP (DL_IS_CPP98 || DL_IS_CPP11 || DL_IS_CPP14 || DL_IS_CPP17)
#define DL_IS_ATLEAST_CPP98 DL_IS_CPP
#define DL_IS_ATLEAST_CPP11 (DL_IS_CPP11 || DL_IS_CPP14 || DL_IS_CPP17)
#define DL_IS_ATLEAST_CPP14 (DL_IS_CPP14 || DL_IS_CPP17)
#define DL_IS_ATLEAST_CPP17 (DL_IS_CPP17)



#if _WIN32 || _WIN64
# define DL_IS_WINDOWS 1
#else
# define DL_IS_WINDOWS 0
#endif

#if __linux__ || __linux || linux
# define DL_IS_LINUX 1
#else
# define DL_IS_LINUX 0
#endif

#if __APPLE__ || __MACH__
# define DL_IS_APPLE 1
#else
# define DL_IS_APPLE 0
#endif

#if _MSC_VER
# define DL_IS_MSC 1
#else
# define DL_IS_MSC 0
#endif

#if __GNUC__ && !__clang__ && !__MINGW32__
# define DL_IS_GNUC 1
#else
# define DL_IS_GNUC 0
#endif

#if __clang__
# define DL_IS_CLANG 1
#else
# define DL_IS_CLANG 0
#endif

#if __MINGW32__
# define DL_IS_MINGW 1
#else
# define DL_IS_MINGW 0
#endif

#if defined(NDEBUG) || defined(_DEBUG)
# define DL_IS_DEBUG 1
#else
# define DL_IS_DEBUG 0
#endif

#ifndef DL_IMPLEMENTATION
# define DL_IMPLEMENTATION 0
#endif



#ifndef dl_inline
# define dl_inline
# if DL_IS_ATLEAST_C99 && (DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW)
#   undef dl_inline
#   define dl_inline inline __attribute__((__always_inline__))
# endif
# if DL_IS_MSC
#   undef dl_inline
#   define dl_inline __forceinline
# endif
#endif

#ifndef dl_api
# if !DL_IS_CPP
#   if DL_IMPLEMENTATION
#     define dl_api extern
#   else
#     define dl_api extern
#   endif
# else
#   if DL_IS_MSC
#     define dl_api __declspec(dllexport)
#   else
#     if DL_IMPLEMENTATION
#       define dl_api 
#     else
#       define dl_api extern
#     endif
#   endif
# endif
#endif

#ifndef dl_restrict
# if DL_IS_ATLEAST_C99 && !DL_IS_CPP
#   define dl_restrict restrict
# elif DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   define dl_restrict __restrict__
# elif DL_IS_MSC
#   define dl_restrict __restrict
# else
#   define dl_restrict
# endif
#endif

#ifndef dl_likely
# define dl_likely(x) !!(x)
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_likely
#   define dl_likely(x) __builtin_expect(!!(x), 1)
# endif
#endif

#ifndef dl_unlikely
# define dl_unlikely(x) !!(x)
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_unlikely
#   define dl_unlikely(x) __builtin_expect(!!(x), 0)
# endif
#endif

#if DL_USE_SAFETY_CHECKS
# define dl_safety(x) (dl_unlikely(x) ? DL_ERROR("Safety triggered") || 1 : 0)
#else
# define dl_safety(x) (1 == 0)
#endif



typedef void* dl_any;
typedef signed int dl_integer;
typedef float dl_real;
typedef unsigned char dl_byte;
typedef unsigned int dl_natural;
typedef dl_byte dl_bool;

#define DL_INTEGER_MAX 2147483647
#define DL_INTEGER_MIN -2147483648

#define DL_NATURAL_MAX 4294967295
#define DL_NATURAL_MIN 0

#define DL_REAL_MAX 3.402823e+38f
#define DL_REAL_MIN 1.175494e-38f

#define DL_DEFAULT_SLICE_LENGTH 512

#ifndef true
# define true 1
#endif

#ifndef false
# define false 0
#endif

#ifndef NULL
# define NULL 0
#endif



#if defined(__cplusplus)
extern "C" {
#endif

  /*****************************************************************************
   **  Memory Tools
   ****************************************************************************/

  dl_api dl_any dl_memory_swap(dl_any target, dl_any source, dl_natural dl_bytes);
  dl_api dl_any dl_memory_copy(dl_any target, dl_any source, dl_natural dl_bytes);
  dl_api dl_any dl_memory_set(dl_any target, dl_byte val, dl_natural dl_bytes);

  typedef struct {
    dl_integer (*func)(dl_any data, const dl_any value);
    dl_any data;
  } dl_filter;

  typedef struct {
    dl_any (*func)(dl_any data, dl_any value);
    dl_any data;
  } dl_handler;

  typedef struct {
    dl_any (*func)(dl_any data, dl_any item, const dl_any left);
    dl_any data;
  } dl_folder;

  typedef struct {
    dl_integer (*func)(dl_any data, const dl_any left, const dl_any right);
    dl_any data;
  } dl_comparator;

  typedef struct {
    dl_any (*func)(dl_any data, const dl_any left, const dl_any right, dl_any out);
    dl_any data;
  } dl_zipper;

  typedef struct {
    dl_any (*func)(dl_any data, const dl_any left, dl_any out);
    dl_any data;
  } dl_converter;

#if defined(__cplusplus)
}
#endif



/*****************************************************************************
 **  IMPLEMENTATION
 ****************************************************************************/

#if DL_IMPLEMENTATION

/*****************************************************************************
 **  Memory Tools
 ****************************************************************************/

dl_api dl_any dl_memory_swap(dl_any left, dl_any right, dl_natural dl_bytes) {
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

  if (byte_count > 0)
  {
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

dl_api dl_any dl_memory_copy(dl_any left, dl_any right, dl_natural dl_bytes) {
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

  if (dl_unlikely(byte_count > 0))
  {
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

dl_any dl_memory_set(dl_any left, dl_byte val, dl_natural dl_bytes) {
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

  if (dl_unlikely(byte_count > 0))
  {
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

