#ifndef DL_H
#define DL_H 1

/***************************************
 * Configuration
 **************************************/

#ifndef DL_USE_EXTENSIONS
# define DL_USE_EXTENSIONS 1
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
# define dl_safety(x) dl_unlikely(x)
#else
# define dl_safety(x) (1 == 0)
#endif

#define dl_unused(x) (void)(x)



#include <inttypes.h>

typedef void* dl_any;
typedef int64_t dl_integer;
typedef uint64_t dl_natural;
typedef float dl_real;
typedef uint8_t dl_byte;
typedef uint16_t dl_short;
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

dl_api dl_any dl_memory_swap(dl_any left, dl_any right, dl_natural byte_count) {
  switch (byte_count) {
  case sizeof(uint8_t):
    {
      uint8_t t;
      t = *(uint8_t *)left;
      *(uint8_t *)left = *(uint8_t *)right;
      *(uint8_t *)right = t;
    }
    break;
  case sizeof(uint16_t):
    {
      uint16_t t;
      t = *(uint16_t *)left;
      *(uint16_t *)left = *(uint16_t *)right;
      *(uint16_t *)right = t;
    }
    break;    
  case sizeof(uint32_t):
    {
      uint32_t t;
      t = *(uint32_t *)left;
      *(uint32_t *)left = *(uint32_t *)right;
      *(uint32_t *)right = t;
    }
    break;    
  case sizeof(uint64_t):
    {
      uint64_t t;
      t = *(uint64_t *)left;
      *(uint64_t *)left = *(uint64_t *)right;
      *(uint64_t *)right = t;
    }
    break;    
  default:
    {
      size_t sz_count, excess_byte_count, *sz_left, *sz_right, sz_temp;

      sz_count = byte_count / sizeof(size_t);
      excess_byte_count = byte_count % sizeof(size_t);
  
      sz_left = (size_t *)left;
      sz_right = (size_t *)right;

      for (; sz_count > 0; --sz_count) {
	sz_temp = *sz_left;
	*sz_left = *sz_right;
	*sz_right = sz_temp;
	++sz_left;
	++sz_right;
      }

      if (excess_byte_count > 0) {
	dl_byte *byte_left, *byte_right, byte_temp;
	byte_left = (dl_byte *)sz_left;
	byte_right = (dl_byte *)sz_right;

	for (; excess_byte_count > 0; --excess_byte_count) {
	  byte_temp = *byte_left;
	  *byte_left = *byte_right;
	  *byte_right = byte_temp;
	  ++byte_left;
	  ++byte_right;
	}
      }
    }
  }

  return left;
}

dl_api dl_any dl_memory_copy(dl_any left, dl_any right, dl_natural byte_count) {
  switch (byte_count) {
  case sizeof(uint8_t):
    {
      *(uint8_t *)left = *(uint8_t *)right;
    }
    break;
  case sizeof(uint16_t):
    {
      *(uint16_t *)left = *(uint16_t *)right;
    }
    break;    
  case sizeof(uint32_t):
    {
      *(uint32_t *)left = *(uint32_t *)right;
    }
    break;    
  case sizeof(uint64_t):
    {
      *(uint64_t *)left = *(uint64_t *)right;
    }
    break;
  default:
    {
      size_t sz_count, excess_byte_count, *sz_left, *sz_right;
  
      sz_count = byte_count / sizeof(size_t);
      excess_byte_count = byte_count % sizeof(size_t);

      sz_left = (size_t *)left;
      sz_right = (size_t *)right;

      for (; sz_count > 0; --sz_count) {
	*sz_left = *sz_right;
	++sz_left;
	++sz_right;
      }

      if (dl_unlikely(excess_byte_count > 0)) {
	dl_byte *byte_left, *byte_right;
	byte_left = (dl_byte *)sz_left;
	byte_right = (dl_byte *)sz_right;

	for (; excess_byte_count > 0; --excess_byte_count) {
	  *byte_left = *byte_right;
	  ++byte_left;
	  ++byte_right;
	}
      }
    }
  }
  return left;
}

dl_any dl_memory_set(dl_any left, dl_byte val, dl_natural byte_count) {
  switch (byte_count) {
  case sizeof(uint8_t):
    {
      *(uint8_t *)left = val;
    }
    break;
  case sizeof(uint16_t):
    {
      uint8_t *l = (uint8_t *)left;
      l[0] = val;
      l[1] = val;
    }
    break;    
  case sizeof(uint32_t):
    {
      uint8_t *l = (uint8_t *)left;
      l[0] = val;
      l[1] = val;
      l[2] = val;
      l[3] = val;
    }
    break;    
  case sizeof(uint64_t):
    {
      uint8_t *l = (uint8_t *)left;
      l[0] = val;
      l[1] = val;
      l[2] = val;
      l[3] = val;
      l[4] = val;
      l[5] = val;
      l[6] = val;
      l[7] = val;
    }
    break;
  default:
    {
      size_t *sz_left, sz_count, excess_byte_count, sz_val, shift;
  
      sz_left = (size_t *)left;
      sz_count = byte_count / sizeof(size_t);
      excess_byte_count = byte_count % sizeof(size_t);

      sz_val = val;
  
      for (shift = 1; shift < sizeof(size_t); ++shift)
	sz_val |= (val << shift);

      for (; sz_count > 0; --sz_count) {
	*(size_t *)sz_left = sz_val;
	++sz_left;
      }

      if (dl_unlikely(excess_byte_count > 0)) {
	dl_byte *byte_left;

	byte_left = (dl_byte *)sz_left;
	
	for (; excess_byte_count > 0; --excess_byte_count) {
	  *(dl_byte *)byte_left = val;
	  ++byte_left;
	}
      }
    }
  }
  return left;
}
  
#endif /* DL_IMPLEMENTATION */

#endif

