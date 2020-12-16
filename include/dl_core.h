#ifndef DL_CORE_H
#define DL_CORE_H 1

#include <stddef.h>

/***************************************
 * Configuration
 **************************************/

#ifndef DL_USE_EXTENSIONS
#define DL_USE_EXTENSIONS 1
#endif

#ifndef DL_USE_LOGGING
#define DL_USE_LOGGING 1
#endif

#ifndef DL_USE_SAFETY_CHECKS
#define DL_USE_SAFETY_CHECKS 0
#endif

#ifndef DL_USE_MALLOC
#define DL_USE_MALLOC 1
#endif

/***************************************
 * Environment
 **************************************/

#if defined(__STDC__)
#if __STDC_VERSION__ >= 201112L
#define DL_IS_C11 1
#define DL_IS_C99 0
#define DL_IS_C90 0
#define DL_IS_C89 0
#elif __STDC_VERSION__ >= 199901L
#define DL_IS_C11 0
#define DL_IS_C99 1
#define DL_IS_C90 0
#define DL_IS_C89 0
#elif __STDC_VERSION__ >= 199409L
#define DL_IS_C11 0
#define DL_IS_C99 0
#define DL_IS_C90 1
#define DL_IS_C89 0
#else
#define DL_IS_C11 0
#define DL_IS_C99 0
#define DL_IS_C90 0
#define DL_IS_C89 1
#endif
#else
#error "C version not determined."
#define DL_IS_C11 0
#define DL_IS_C99 0
#define DL_IS_C90 0
#define DL_IS_C89 0
#endif

#if defined(__cplusplus)
#if __cplusplus <= 199711L
#define DL_IS_CPP98 1
#define DL_IS_CPP11 0
#define DL_IS_CPP14 0
#define DL_IS_CPP17 0
#elif __cplusplus <= 201103L
#define DL_IS_CPP98 0
#define DL_IS_CPP11 1
#define DL_IS_CPP14 0
#define DL_IS_CPP17 0
#elif __cplusplus <= 201402L
#define DL_IS_CPP98 0
#define DL_IS_CPP11 0
#define DL_IS_CPP14 1
#define DL_IS_CPP17 0
#elif __cplusplus <= 201703L
#define DL_IS_CPP98 0
#define DL_IS_CPP11 0
#define DL_IS_CPP14 0
#define DL_IS_CPP17 1
#endif
#else
#define DL_IS_CPP98 0
#define DL_IS_CPP11 0
#define DL_IS_CPP14 0
#define DL_IS_CPP17 0
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

#if !DL_IS_ATLEAST_C99
#undef DL_USE_LOGGING
#define DL_USE_LOGGING 0
#endif

#if _WIN32 || _WIN64
#define DL_IS_WINDOWS 1
#else
#define DL_IS_WINDOWS 0
#endif

#if __linux__ || __linux || linux
#define DL_IS_LINUX 1
#else
#define DL_IS_LINUX 0
#endif

#if __APPLE__ || __MACH__
#define DL_IS_APPLE 1
#else
#define DL_IS_APPLE 0
#endif

#if _MSC_VER
#define DL_IS_MSC 1
#else
#define DL_IS_MSC 0
#endif

#if __GNUC__ && !__clang__ && !__MINGW32__
#define DL_IS_GNUC 1
#else
#define DL_IS_GNUC 0
#endif

#if __clang__
#define DL_IS_CLANG 1
#else
#define DL_IS_CLANG 0
#endif

#if __MINGW32__
#define DL_IS_MINGW 1
#else
#define DL_IS_MINGW 0
#endif

#if defined(NDEBUG) || defined(_DEBUG)
#define DL_IS_DEBUG 1
#else
#define DL_IS_DEBUG 0
#endif

#ifndef dl_inline
#define dl_inline
#if DL_IS_ATLEAST_C99 && (DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW)
#undef dl_inline
#define dl_inline inline __attribute__((__always_inline__))
#endif
#if DL_IS_MSC
#undef dl_inline
#define dl_inline __forceinline
#endif
#endif

#ifndef dl_api
#if !DL_IS_CPP
#if defined(DL_IMPLEMENTATION)
#define dl_api extern
#else
#define dl_api extern
#endif
#else
#if DL_IS_MSC
#define dl_api __declspec(dllexport)
#else
#if defined(DL_IMPLEMENTATION)
#define dl_api
#else
#define dl_api extern
#endif
#endif
#endif
#endif

#ifndef dl_likely
#define dl_likely(x) !!(x)
#if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#undef dl_likely
#define dl_likely(x) __builtin_expect(!!(x), 1)
#endif
#endif

#ifndef dl_unlikely
#define dl_unlikely(x) !!(x)
#if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#undef dl_unlikely
#define dl_unlikely(x) __builtin_expect(!!(x), 0)
#endif
#endif

#if defined(DL_USE_SAFETY_CHECKS) && DL_USE_SAFETY_CHECKS
#define dl_safety(x) dl_unlikely(x)
#else
#define dl_safety(x) (1 == 0)
#endif

#define dl_unused(x) (void)(x)

typedef void *dl_ptr;
typedef signed int dl_integer;
typedef float dl_real;
typedef unsigned char dl_byte;
typedef unsigned int dl_natural;

#if !DL_IS_ATLEAST_C99 && !defined(__cplusplus)
typedef enum { false,
               true } dl_bool;
#else
typedef _Bool dl_bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif

#define DL_INTEGER_MAX 2147483647
#define DL_INTEGER_MIN -2147483648

#define DL_NATURAL_MAX 4294967295U
#define DL_NATURAL_MIN 0

#define DL_REAL_MAX 3.402823e+38f
#define DL_REAL_MIN 1.175494e-38f

#ifndef NULL
#define NULL 0
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif
