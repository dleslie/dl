#ifndef DL_H
#define DL_H 1



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



#ifndef DL_USE_MALLOC
# define DL_USE_MALLOC 1
#endif

#ifndef DL_USE_SAFETY_CHECKS
# define DL_USE_SAFETY_CHECKS DL_IS_DEBUG
#endif

#ifndef DL_USE_LEFT_HANDED
# define DL_USE_LEFT_HANDED 0
#endif

#ifndef DL_USE_LOGGING
# if DL_IS_ATLEAST_C99
#   define DL_USE_LOGGING 1
# else
#   define DL_USE_LOGGING 0
# endif
#endif

#ifndef DL_USE_TEST
# if DL_IS_ATLEAST_C99
#   define DL_USE_TEST 1
# else
#   define DL_USE_TEST 0
# endif
#endif

#ifndef DL_USE_MATH
# define DL_USE_MATH 1
#endif

#ifndef DL_USE_TWEEN
# define DL_USE_TWEEN 1
#endif

#ifndef DL_USE_CONTAINERS
# define DL_USE_CONTAINERS 1
#endif

#ifndef DL_USE_TIME
# define DL_USE_TIME 1
#endif

#ifndef DL_IMPLEMENTATION
# define DL_IMPLEMENTATION 0
#endif

#ifndef DL_BEZIER_DEGREE
# define DL_BEZIER_DEGREE 4
#endif



#if DL_USE_LOGGING
# include <stdio.h>
# include <stdarg.h>
#endif

#if DL_USE_TIME
# if DL_IS_LINUX || DL_IS_APPLE
#   define DL_HAS_TIME 1
#   include <time.h>
#   include <sys/time.h>
# endif
# if DL_IS_WINDOWS
#   define DL_HAS_TIME 1
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <stdint.h>
# endif
# ifndef DL_HAS_TIME
#   define DL_HAS_TIME 0
# endif
#endif

#if DL_USE_MATH
# include <math.h>
#endif

#if DL_USE_MALLOC
# include <malloc.h>
#endif



#ifndef dl_inline
# define dl_inline  
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_inline
#   define dl_inline inline __attribute__((__always_inline__))
# endif
# if DL_IS_MSC
#   undef dl_inline
#   define dl_inline __forceinline
# endif
#endif

#ifndef dl_api
# define dl_api  
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_api
#   define dl_api
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
typedef signed long int dl_integer;
typedef float dl_real;
typedef unsigned char dl_byte;
typedef unsigned long int dl_natural;
typedef dl_byte dl_bool;

#define DL_INTEGER_MAX 0x7fffffffL
#define DL_INTEGER_MIN (-DL_INTEGER_MAX - 1L)

#define DL_NATURAL_MAX 0xffffffffUL
#define DL_NATURAL_MIN 0

#define DL_REAL_MAX 3.402823e+38f
#define DL_REAL_MIN 1.175494e-38f

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
   **  Tools
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



  /*****************************************************************************
   **  Logging
   ****************************************************************************/

#if DL_USE_LOGGING
# if !DL_IS_ATLEAST_C99
#   error "DL_USE_LOGGING not available for C90 or lower"
#   undef DL_USE_LOGGING
#   define DL_USE_LOGGING 0
# endif
  
  typedef enum {
    DL_LOG_INFO = 1,
    DL_LOG_WARNING = 2,
    DL_LOG_ERROR = 3,
    DL_LOG_TEST = 4,
    DL_LOG_MESSAGE = 5
  } dl_log_channel;

  extern dl_natural (*dl_active_log_handler)(dl_log_channel, const char *dl_restrict, dl_natural, const char *dl_restrict, const char *dl_restrict);

  dl_natural dl_log_message(dl_log_channel ch, const char *dl_restrict file, dl_natural line, const char *dl_restrict function, const char *dl_restrict fmt, ...);

# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MSC || DL_IS_MINGW
#   define DL_INFO(...) dl_log_message(DL_LOG_INFO, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_WARN(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_ERROR(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_TEST(...) dl_log_message(DL_LOG_TEST, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_MSG(...) dl_log_message(DL_LOG_MESSAGE, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
# else
#   define DL_INFO(...) dl_log_message(DL_LOG_INFO, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_WARN(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_ERROR(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_TEST(...) dl_log_message(DL_LOG_TEST, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_MSG(...) dl_log_message(DL_LOG_MESSAGE, __FILE__, __LINE__, "", ## __VA_ARGS__)
# endif
#else
# if DL_IS_ATLEAST_C99
#   define DL_INFO(...)
#   define DL_WARN(...)
#   define DL_ERROR(...)
#   define DL_TEST(...)
#   define DL_MSG(...)
# endif
#endif /* DL_USE_LOGGING */



  /*****************************************************************************
   **  Testing
   ****************************************************************************/

#if DL_USE_TEST
# if !DL_IS_ATLEAST_C99
#   error "DL_USE_TEST is not available for C90 or lower"
#   undef DL_USE_TEST
#   define DL_USE_TEST 0
# endif
    
  dl_integer dl_test_run(dl_bool (**tests)(), const char **names, dl_integer count);
  dl_integer dl_test_count(dl_bool (**tests)(), dl_integer max);

# define dl_check(predicate, ...) ((predicate) ? true : DL_ERROR(__VA_ARGS__) && false)
# define dl_not_implemented() dl_check(false, "Test is not implemented.")
# define DL_BEGIN_TEST_SUITE(name) \
  void _test_suite_##name(dl_integer *out_count, dl_integer *out_passed) {\
    dl_bool (*tests[256])();\
    const char *test_names[256];\
    dl_integer current = 0;\
    if (out_count == NULL || out_passed == NULL)\
      return;\
    dl_memory_set(tests, 0, sizeof(dl_bool (*)()) * 256);
# define DL_END_TEST_SUITE \
    *out_count = dl_test_count(tests, 256);\
    *out_passed = dl_test_run(tests, test_names, *out_count);\
  }
# define DL_DECLARE_TEST(test_name) \
  dl_bool test_name();\
  tests[current] = test_name;\
  test_names[current] = #test_name;\
  current++;
# define DL_RUN_TEST_SUITE(name)\
  do {\
    dl_integer count, passed;\
    DL_TEST("<<"#name ">>");\
    _test_suite_##name(&count, &passed);\
    DL_TEST("<<" #name ">> %i/%i", passed, count);\
  } while (0)
#endif /* DL_USE_TEST */



  /*****************************************************************************
   **  Math
   ****************************************************************************/

#if DL_USE_MATH
  extern const dl_real DL_PI;
  extern const dl_real DL_INV_PI;
  extern const dl_real DL_E;
  extern const dl_real DL_INV_E;
  extern const dl_real DL_EPSILON;
  
# if DL_IS_ATLEAST_C90 && (DL_IS_GNUC || DL_IS_CLANG)
#   define dl_max(a,b) ({\
      __auto_type _a = (a);\
      __auto_type _b = (b);\
      _a > _b ? _a : _b;\
     })
#   define dl_min(a,b) ({\
      __auto_type _a = (a);\
      __auto_type _b = (b);\
      _a < _b ? _a : _b;\
     })
#   define dl_clamp(x, i, j) ({\
      __auto_type _i = (i);\
      __auto_type _j = (j);\
      __auto_type _x = (x);\
      dl_max(dl_min(_j, _x), _i);\
    })
#   define dl_clamp01(x) dl_clamp(x, 0, 1)
# else
#   define dl_min(x, y) ((x) <= (y) ? (x) : (y))
#   define dl_max(x, y) ((x) >= (y) ? (x) : (y))
#   define dl_clamp(x, a, b) dl_max(dl_min(b, x), a)
#   define dl_clamp01(x) dl_clamp(x, 0, 1)
# endif

  dl_api dl_bool dl_approximately_equal(dl_real a, dl_real b, dl_real epsilon);
  dl_api dl_integer dl_floor_to_integer(dl_real n);
  dl_api dl_integer dl_ceil_to_integer(dl_real n);
  dl_api dl_integer dl_round_to_integer(dl_real n);
  dl_api dl_real dl_rationalize(dl_real value, dl_natural decimal_points);

  dl_api dl_real dl_degree_to_radian(dl_real degree);
  dl_api dl_real dl_radian_to_degree(dl_real radian);
  dl_api dl_integer dl_factorial(dl_integer n);
  
# define dl_sqrt(v) sqrt(v)
# define dl_cos(v) cos(v)
# define dl_sin(v) sin(v)
# define dl_tan(v) tan(v)
# define dl_acos(v) acos(v)
# define dl_asin(v) asin(v)
# define dl_atan(v) atan(v)
# define dl_pow(a, b) pow((a), (b))
# define dl_exp(v) exp(v)
# define dl_floor(v) floor(v)
# define dl_ceil(v) ceil(v)
# define dl_abs(v) ((v) > 0 ? (v) : -(v))

# if !DL_IS_ATLEAST_C99
#   define dl_hypot(a, b) dl_sqrt((a) * (a) + (b) * (b))
#   define dl_round(a) dl_floor(a)
#else
#   define dl_hypot(a, b) hypot((a), (b))
#   define dl_round(a) roundf(a)
#endif

  typedef struct {
    dl_integer m, a, c;
    dl_integer seed;
  } dl_random_state;

  dl_api dl_random_state *dl_init_random(dl_random_state *state, dl_integer seed);
  dl_api dl_random_state *dl_init_random_custom(dl_random_state *state, dl_integer m, dl_integer a, dl_integer c, dl_integer seed);
  dl_api dl_random_state *dl_init_random_time(dl_random_state *state);

  dl_api dl_integer dl_random_integer(dl_random_state *state, dl_integer max);
  dl_api dl_integer dl_random_integer_range(dl_random_state *state, dl_integer min, dl_integer max);
  dl_api dl_real dl_random_degree(dl_random_state *state);
  dl_api dl_real dl_random_radian(dl_random_state *state);
  dl_api dl_real dl_random_real(dl_random_state *state, dl_real max);
  dl_api dl_real dl_random_real_range(dl_random_state *state, dl_real min, dl_real max);

  
  
  typedef struct {
    dl_real x, y;
  } dl_vec2;

  extern const dl_vec2 dl_vec2_up;
  extern const dl_vec2 dl_vec2_right;
  extern const dl_vec2 dl_vec2_down;
  extern const dl_vec2 dl_vec2_left;
  extern const dl_vec2 dl_vec2_zero;
  extern const dl_vec2 dl_vec2_one;

  dl_api dl_vec2 *dl_init_vec2(dl_vec2 * dl_restrict v, dl_real x, dl_real y);

  dl_api dl_vec2 *dl_vec2_add(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_sub(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_vec2 *dl_restrict out);

  dl_api dl_vec2 *dl_vec2_mul_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_div_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_add_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_sub_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out);

  dl_api dl_vec2 *dl_vec2_normalize(const dl_vec2 *dl_restrict left, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_negate(const dl_vec2 *dl_restrict left, dl_vec2 *dl_restrict out);

  dl_api dl_real dl_vec2_dot(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right);
  dl_api dl_bool dl_vec2_approximately_equal(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_real epsilon);
  dl_api dl_real dl_vec2_square_magnitude(const dl_vec2 *dl_restrict left);
  dl_api dl_real dl_vec2_magnitude(const dl_vec2 *dl_restrict left);

  dl_api dl_vec2 *dl_vec2_reflect(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict normal, dl_vec2 *dl_restrict out);
  dl_api dl_vec2 *dl_vec2_refract(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict normal, float eta, dl_vec2 *dl_restrict out);


  
  typedef dl_vec2 dl_point2;
  extern const dl_point2 dl_point2_zero;
  extern const dl_point2 dl_point2_one;

  dl_api dl_point2 *dl_init_point2(dl_point2 * dl_restrict p, dl_real x, dl_real y);

  dl_api dl_point2 *dl_point2_add(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_point2_sub(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_point2 *dl_restrict out);

  dl_api dl_point2 *dl_point2_negate(const dl_point2 *dl_restrict left, dl_point2 *dl_restrict out);

  dl_api dl_point2 *dl_point2_mul_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_point2_div_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_point2_add_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_point2_sub_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out);

  dl_api dl_bool dl_point2_approximately_equal(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_real epsilon);

  dl_api dl_real dl_point2_line_orientation(const dl_point2 *dl_restrict point, const dl_point2 *dl_restrict line_a, const dl_point2 *dl_restrict line_b);
  

  
  typedef struct {
    dl_real x, y, z, w;
  } dl_vec4;

  typedef dl_vec4 dl_point3;
  typedef dl_vec4 dl_vec3;

  dl_api dl_vec4 *dl_init_vec4(dl_vec4 * dl_restrict v, dl_real x, dl_real y, dl_real z, dl_real w);

  dl_api dl_vec4 *dl_vec4_add(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out);
  dl_api dl_vec4 *dl_vec4_sub(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out);

  dl_api dl_real dl_vec4_dot(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right);
  dl_api dl_vec4 *dl_vec4_normalize(const dl_vec4 *dl_restrict left, dl_vec4 *dl_restrict out);
  dl_api dl_vec4 *dl_vec4_negate(const dl_vec4 *dl_restrict left, dl_vec4 *dl_restrict out);
  dl_api dl_real dl_vec4_square_magnitude(const dl_vec4 *dl_restrict left);
  dl_api dl_real dl_vec4_magnitude(const dl_vec4 *dl_restrict left);
  dl_api dl_bool dl_vec4_approximately_equal(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_real epsilon);

  dl_api dl_vec4 *dl_vec4_mul_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out);
  dl_api dl_vec4 *dl_vec4_div_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out);
  dl_api dl_vec4 *dl_vec4_add_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out);
  dl_api dl_vec4 *dl_vec4_sub_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out);


  
  extern const dl_point3 dl_point3_zero;
  extern const dl_point3 dl_point3_one;

  dl_api dl_point3 *dl_init_point3(dl_point3 * dl_restrict p, dl_real x, dl_real y, dl_real z);

  dl_api dl_point3 *dl_point3_add(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_point3_sub(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out);

  dl_api dl_point3 *dl_point3_mul_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_point3_div_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_point3_add_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_point3_sub_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out);

  dl_api dl_point3 *dl_point3_rotate(const dl_point3 *dl_restrict left, const dl_vec3 *axis, const dl_real angle, dl_point3 *dl_restrict out);

  dl_api dl_point3 *dl_point3_negate(const dl_point3 *dl_restrict left, dl_point3 *dl_restrict out);

  dl_api dl_bool dl_point3_approximately_equal(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict out, dl_real epsilon);

  /* dl_real point3_triangle_orientation(const dl_point3 *dl_restrict point, const dl_point3 *dl_restrict triangle_a, const dl_point3 *dl_restrict triangle_b, const dl_point3 *dl_restrict triangle_c); */



  extern const dl_vec3 dl_vec3_up;
  extern const dl_vec3 dl_vec3_right;
  extern const dl_vec3 dl_vec3_forward;
  extern const dl_vec3 dl_vec3_down;
  extern const dl_vec3 dl_vec3_left;
  extern const dl_vec3 dl_vec3_backward;
  extern const dl_vec3 dl_vec3_zero;
  extern const dl_vec3 dl_vec3_one;

  dl_api dl_vec3 *dl_init_vec3(dl_vec3 * dl_restrict v, dl_real x, dl_real y, dl_real z);

  dl_api dl_vec3 *dl_vec3_reflect(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);
  dl_api dl_vec3 *dl_vec3_refract(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, float eta, dl_vec3 *dl_restrict out);

  dl_api dl_vec3 *dl_vec3_add(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);
  dl_api dl_vec3 *dl_vec3_sub(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);

  dl_api dl_vec3 *dl_vec3_cross(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);
  dl_api dl_real dl_vec3_dot(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict out);

  dl_api dl_vec3 *dl_vec3_mul_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out);
  dl_api dl_vec3 *dl_vec3_div_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out);
  dl_api dl_vec3 *dl_vec3_add_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out);
  dl_api dl_vec3 *dl_vec3_sub_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out);

  dl_api dl_vec3 *dl_vec3_rotate(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict axis, const dl_real angle, dl_vec3 *dl_restrict out);

  dl_api dl_vec3 *dl_vec3_normalize(const dl_vec3 *dl_restrict left, dl_vec3 *dl_restrict out);
  dl_api dl_bool dl_vec3_normalized(const dl_vec3 *dl_restrict left);
  dl_api dl_vec3 *dl_vec3_negate(const dl_vec3 *dl_restrict left, dl_vec3 *dl_restrict out);

  dl_api dl_bool dl_vec3_approximately_equal(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict out, dl_real epsilon);
  dl_api dl_real dl_vec3_square_magnitude(const dl_vec3 *dl_restrict left);
  dl_api dl_real dl_vec3_magnitude(const dl_vec3 *dl_restrict left);


  
  typedef union {
    dl_real vals[16];
    dl_real ary[4][4];
    dl_vec4 cols[4];
  } dl_mat4;
  
  extern const dl_mat4 dl_mat4_identity;

  dl_api dl_mat4 *dl_init_mat4(dl_mat4 * dl_restrict m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3);

  dl_api dl_mat4 *dl_init_mat4_cols(dl_mat4 * dl_restrict m, const dl_vec4 *dl_restrict column_a, const dl_vec4 *dl_restrict column_b, const dl_vec4 *dl_restrict column_c, const dl_vec4 *dl_restrict column_d);

  dl_api dl_mat4 *dl_init_mat4_translate(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z);
  dl_api dl_mat4 *dl_init_mat4_rotate_x(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *dl_init_mat4_rotate_y(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *dl_init_mat4_rotate_z(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *dl_init_mat4_rotate(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict axis, dl_real radians);
  dl_api dl_mat4 *dl_init_mat4_scale(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z);

  dl_api dl_mat4 *dl_init_mat4_look_at(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict origin, const dl_vec3 *dl_restrict target, const dl_vec3 *dl_restrict up);
  dl_api dl_mat4 *dl_init_mat4_orthographic(dl_mat4 * dl_restrict m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far);
  dl_api dl_mat4 *dl_init_mat4_perspective(dl_mat4 * dl_restrict m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far);

  dl_api dl_mat4 *dl_mat4_add(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *dl_mat4_sub(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *dl_mat4_mul(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);

  dl_api dl_vec4 *dl_mat4_mul_vec4(const dl_mat4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out);
  dl_api dl_vec3 *dl_mat4_mul_vec3(const dl_mat4 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);
  dl_api dl_point3 *dl_mat4_mul_point3(const dl_mat4 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out);

  dl_api dl_mat4 *dl_mat4_transpose(const dl_mat4 *dl_restrict left, dl_mat4 *dl_restrict out);

  dl_api dl_mat4 *dl_mat4_mul_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *dl_mat4_div_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *dl_mat4_add_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *dl_mat4_sub_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);

  dl_api dl_bool dl_mat4_approximately_equal(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_real epsilon);

  
  
  /* typedef dl_vec4 dl_quat; */

  /* dl_quat *dl_init_quat(dl_quat *dl_restrict q, dl_real x, dl_real y, dl_real z, dl_real w); */
  /* dl_quat *dl_init_quat_axis_angle(dl_quat *dl_restrict q, const dl_vec3 *dl_restrict axis, dl_real angle); */
  /* dl_quat *dl_init_quat_euler_angles(dl_quat *dl_restrict q, const dl_vec3 *dl_restrict angles); */
  /* dl_quat *dl_init_quat_identity(dl_quat *dl_restrict q); */

  /* dl_quat *dl_quat_add(const dl_quat *dl_restrict left, const quat *dl_restrict right, quat *dl_restrict out); */
  /* dl_quat *dl_quat_sub(const dl_quat *dl_restrict left, const quat *dl_restrict right, quat *dl_restrict out); */
  /* dl_quat *dl_uat_mul(const dl_quat *dl_restrict left, const quat *dl_restrict right, quat *dl_restrict out); */
  /* dl_quat *dl_quat_div(const dl_quat *dl_restrict left, const quat *dl_restrict right, quat *dl_restrict out); */

  /* dl_quat *dl_quat_mul_scalar(const dl_quat *dl_restrict left, dl_real scalar, quat *dl_restrict out); */
  /* dl_quat *dl_quat_div_scalar(const dl_quat *dl_restrict left, dl_real scalar, quat *dl_restrict out); */

  /* dl_real qdl_uat_dot(const dl_quat *dl_restrict left, const quat *dl_restrict right); */
  /* dl_real dl_quat_magnitude(const dl_quat *dl_restrict q); */

  /* dl_quat *dl_quat_normalize(const quat *dl_restrict q, dl_quat *dl_restrict out); */
  /* dl_quat *dl_quat_conjugate(const quat *dl_restrict q, dl_quat *dl_restrict out); */
  /* dl_quat *dl_quat_inverse(const quat *dl_restrict q, dl_quat *dl_restrict out); */

  /* dl_bool dl_quat_axis_angle(const dl_quat *dl_restrict q, dl_vec3 *dl_restrict out_axis, dl_real *out_angle); */
  /* dl_quat *dl_quat_euler_angles(const dl_quat *dl_restrict q, dl_vec3 *dl_restrict out); */

  /* dl_vec3 *dl_vec3_rotate_quat(const dl_vec3 *dl_restrict left, const quat *dl_restrict q, dl_vec3 *dl_restrict out); */

  /* dl_mat4 *dl_quat_to_mat4(const dl_quat *dl_restrict q, dl_mat4 *dl_restrict out); */
  /* dl_quat *dl_mat4_to_quat(const dl_mat4 *dl_restrict m, dl_quat *dl_restrict out); */

#endif /* DL_USE_MATH */
  


  /*****************************************************************************
   **  Tweening
   ****************************************************************************/

#if DL_USE_TWEEN

  typedef enum {
    DL_EASE_IN,
    DL_EASE_OUT,
    DL_EASE_INOUT
  } dl_ease_direction;

  typedef dl_real(*dl_easing_function)(dl_ease_direction d, dl_real p);

  dl_api dl_real dl_tween(dl_easing_function ease, dl_ease_direction direction, dl_real percent);

  dl_api dl_real dl_ease_linear(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_quadratic(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_cubic(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_quartic(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_quintic(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_sinusoidal(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_exponential(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_circular(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_elastic(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_elastic_tunable(dl_ease_direction d, dl_real p, dl_real a, dl_real k);
  dl_api dl_real dl_ease_back(dl_ease_direction d, dl_real p);
  dl_api dl_real dl_ease_back_tunable(dl_ease_direction d, dl_real p, dl_real s, dl_real t);
  dl_api dl_real dl_ease_bounce(dl_ease_direction d, dl_real p);

  typedef dl_real *(*dl_selector_function)(const dl_real *dl_restrict values, dl_natural length, dl_real percent, dl_real *out);

  dl_api dl_real *dl_interpolate(const dl_selector_function select, const dl_real *dl_restrict values, dl_natural length, dl_real percent, dl_real *dl_restrict out);

  dl_api dl_real *dl_select_linear(const dl_real *dl_restrict v, dl_natural l, dl_real p, dl_real *dl_restrict out);
  dl_api dl_real *dl_select_catmullrom(const dl_real *dl_restrict v, dl_natural l, dl_real p, dl_real *dl_restrict out);

  typedef dl_point2 *(*dl_selector_function_point2)(const dl_point2 *dl_restrict values, dl_natural length, dl_real percent, dl_point2 *dl_restrict out);

  dl_api dl_point2 *dl_interpolate_point2(const dl_selector_function_point2 select, const dl_point2 *dl_restrict values, dl_natural length, dl_real percent, dl_point2 *dl_restrict out);

  dl_api dl_point2 *dl_select_linear_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_select_bezier_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out);
  dl_api dl_point2 *dl_select_catmullrom_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out);

  typedef dl_point3 *(*dl_selector_function_point3)(const dl_point3 *dl_restrict values, dl_natural length, dl_real percent, dl_point3 *dl_restrict out);

  dl_api dl_point3 *dl_interpolate_point3(const dl_selector_function_point3 select, const dl_point3 *dl_restrict values, dl_natural length, dl_real percent, dl_point3 *dl_restrict out);

  dl_api dl_point3 *dl_select_linear_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_select_bezier_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out);
  dl_api dl_point3 *dl_select_catmullrom_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out);



  dl_api dl_integer *dl_lerp_integer(dl_integer a, dl_integer b, dl_real p, dl_integer *dl_restrict out);
  dl_api dl_real *dl_lerp_real(dl_real a, dl_real b, dl_real p, dl_real *dl_restrict out);
  dl_api dl_point2 *dl_lerp_point2(const dl_point2 *dl_restrict a, const dl_point2 *dl_restrict b, dl_real p, dl_point2 *dl_restrict out);
  dl_api dl_point3 *dl_lerp_point3(const dl_point3 *dl_restrict a, const dl_point3 *dl_restrict b, dl_real p, dl_point3 *dl_restrict out);
  dl_api dl_vec2 *dl_lerp_vec2(const dl_vec2 *dl_restrict a, const dl_vec2 *dl_restrict b, dl_real p, dl_vec2 *dl_restrict out);
  dl_api dl_vec3 *dl_lerp_vec3(const dl_vec3 *dl_restrict a, const dl_vec3 *dl_restrict b, dl_real p, dl_vec3 *dl_restrict out);



#endif /* DL_USE_TWEEN */
  
  /*****************************************************************************
   **  Vectors
   ****************************************************************************/

#if DL_USE_CONTAINERS
  
  typedef struct {
    dl_natural element_size;
    dl_natural slice_length;
    dl_any (*alloc)(dl_natural count, dl_natural element_size);
    void (*free)(dl_any data);
  } dl_vector_settings;

  typedef struct {
    dl_vector_settings settings;
    dl_integer slice_count;

    union {
      dl_byte **slices;
      dl_byte *array;
    } data;
  } dl_vector;

  extern dl_vector_settings default_vector_settings;

  dl_api dl_vector *dl_init_vector(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity);
  dl_api dl_vector *dl_init_vector_custom(dl_vector * dl_restrict target, dl_vector_settings *dl_restrict settings, dl_natural capacity);

  dl_api dl_vector *dl_init_vector_array(dl_vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_vector_copy(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_vector * dl_restrict original);
  dl_api dl_natural dl_vector_copy_array(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void dl_destroy_vector(dl_vector * dl_restrict target, const dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural dl_vector_capacity(const dl_vector * dl_restrict v);

  dl_api dl_bool dl_vector_grow(dl_vector * dl_restrict v);
  dl_api dl_bool dl_vector_shrink(dl_vector * dl_restrict v, dl_handler *deconstruct_entry);
  dl_api dl_bool dl_vector_resize(dl_vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry);

  dl_api dl_any dl_vector_get(const dl_vector * dl_restrict v, dl_natural index, dl_any out);
  dl_api const dl_any dl_vector_ref(const dl_vector * dl_restrict v, dl_natural index);
  dl_api dl_any dl_vector_set(dl_vector * dl_restrict v, dl_natural index, dl_any value);

  dl_api dl_bool dl_vector_swap(dl_vector * dl_restrict v, dl_natural index1, dl_natural index2);
  dl_api dl_natural dl_vector_ref_array(dl_vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out);



  /*****************************************************************************
   **  Linked Lists
   ****************************************************************************/

  typedef struct {
    dl_natural element_size;
    dl_natural cache_length;
    dl_any (*alloc)(dl_natural count, dl_natural element_size);
    void (*free)(dl_any data);
  } dl_linked_list_settings;

  struct dl_linked_list_node {
    struct dl_linked_list_node *next;
    struct dl_linked_list_node *previous;
  };

#define DL_LINKED_LIST_HEADER_SIZE (sizeof(struct dl_linked_list_node))
#define DL_LINKED_LIST_DATA(element) ((dl_any)&((struct dl_linked_list_node *)element)[1])

  typedef struct {
    struct dl_linked_list_node *first;
    struct dl_linked_list_node *last;
    struct dl_linked_list_node *free;

    dl_linked_list_settings settings;
    dl_vector node_cache;
  } dl_linked_list;

  extern dl_linked_list_settings default_linked_list_settings;

  dl_api dl_linked_list *dl_init_linked_list(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length);
  dl_api dl_linked_list *dl_init_linked_list_custom(dl_linked_list * dl_restrict target, dl_linked_list_settings *dl_restrict settings);

  dl_api dl_natural dl_linked_list_copy(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_linked_list *dl_restrict original);
  dl_api dl_natural dl_linked_list_copy_array(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void dl_destroy_linked_list(dl_linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural dl_linked_list_capacity(const dl_linked_list * dl_restrict list);
  dl_api dl_natural dl_linked_list_length(const dl_linked_list * dl_restrict list);

  dl_api dl_bool dl_linked_list_grow(dl_linked_list * dl_restrict list);
  dl_api dl_bool dl_linked_list_shrink(dl_linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry);
  dl_api dl_bool dl_linked_list_resize(dl_linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_any dl_linked_list_get(const dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any out);
  dl_api const dl_any dl_linked_list_ref(const struct dl_linked_list_node *dl_restrict position);
  dl_api dl_any dl_linked_list_set(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any value);
  dl_api struct dl_linked_list_node *dl_linked_list_index(dl_linked_list * dl_restrict list, dl_natural position);

  dl_api struct dl_linked_list_node *dl_linked_list_add(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_any value);
  dl_api dl_any dl_linked_list_remove(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict position, dl_any out);
  dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_natural count, dl_handler *dl_restrict destruct_entry);
  dl_api dl_bool dl_linked_list_destroy(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position, dl_handler *deconstruct_entry);

  dl_api dl_bool dl_linked_list_swap(dl_linked_list * dl_restrict list, struct dl_linked_list_node *dl_restrict position1, struct dl_linked_list_node *dl_restrict position2, dl_bool data);



  /*****************************************************************************
   **  Collections
   ****************************************************************************/


  typedef union {
    struct {
      dl_natural index;
    } dl_vector;
    struct {
      struct dl_linked_list_node *node;
    } dl_linked_list;
  } dl_iterator;

  enum {
    /* Unsorted */
    DL_COLLECTION_TYPE_LIST,
    DL_COLLECTION_TYPE_QUEUE,
    /* Sorted */
    DL_COLLECTION_TYPE_SORTED_LIST,
    DL_COLLECTION_TYPE_SET
  };
  typedef dl_byte dl_collection_type;

  enum {
    DL_STORAGE_TYPE_VECTOR,
    DL_STORAGE_TYPE_LINKED_LIST
  };
  typedef dl_byte dl_storage_type;  

  struct dl_collection_dispatch_functions;
  typedef struct {
    dl_collection_type type;
    dl_storage_type storage;
    
    dl_comparator comparer;
    dl_handler deconstruct_entry;
    dl_natural capacity;
    dl_natural element_size;

    struct dl_collection_dispatch_functions *functions;
  } dl_collection_settings;

  extern dl_collection_settings default_vector_collection_settings;
  extern dl_collection_settings default_linked_list_collection_settings;

  typedef struct {
    dl_collection_settings settings;

    union {
      struct {
        dl_vector container;
        dl_natural index[2];
      } dl_vector;
      struct {
        dl_linked_list container;
      } dl_linked_list;
    } data;
  } dl_collection;

  struct dl_collection_dispatch_functions {
    dl_integer (*_dl_iterator_compare)(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right);
    dl_bool (*_dl_iterator_is_valid)(const dl_collection *dl_restrict col, dl_iterator index);
    dl_iterator (*_dl_make_invalid_dl_iterator)(const dl_collection *dl_restrict col);

    dl_any (*_collection_push_start)(dl_collection *dl_restrict col, dl_iterator *iter);
    dl_bool (*_collection_is_empty)(const dl_collection *dl_restrict col);
    dl_any (*_collection_pop)(const dl_collection *dl_restrict col, dl_any out);
    dl_bool (*_collection_pop_destroy)(dl_collection *dl_restrict col);
    dl_bool (*_collection_pop_forget)(dl_collection *dl_restrict col);
    dl_integer (*_collection_ref_array)(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array);
    dl_any (*_collection_ref)(dl_collection *dl_restrict col, dl_iterator iter);
    dl_bool (*_collection_swap)(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b);
    dl_any (*_collection_get)(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out);
    dl_any (*_collection_set)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value);
    dl_iterator (*_collection_index)(dl_collection *dl_restrict col, dl_natural index);
    void (*_collection_next)(const dl_collection *dl_restrict col, dl_iterator *iter);
    void (*_collection_prev)(const dl_collection *dl_restrict col, dl_iterator *iter);
    dl_integer (*_collection_count)(const dl_collection *dl_restrict col);
    dl_iterator (*_collection_begin)(const dl_collection *dl_restrict col);
    dl_iterator (*_collection_end)(const dl_collection *dl_restrict col);
    const dl_any (*_collection_search_region)(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter);
    dl_bool (*_collection_destroy_at)(dl_collection *dl_restrict col, dl_iterator *iter, dl_handler *destructor);
    dl_any (*_collection_remove_at)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out);
    dl_integer (*_collection_destroy_range)(dl_collection *dl_restrict col, dl_iterator *iter, dl_natural count);
    dl_bool (*_collection_insert)(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item);
  };
  
  dl_api dl_integer dl_iterator_compare(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_iterator_equal(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_iterator_is_valid(const dl_collection *dl_restrict col, dl_iterator index);
  dl_api dl_iterator dl_make_invalid_dl_iterator(const dl_collection *dl_restrict col);

  dl_api dl_collection *dl_init_collection(dl_collection *dl_restrict col, dl_collection_type type, dl_storage_type storage, dl_comparator *dl_restrict compare, dl_handler *dl_restrict destructor, dl_natural element_size);
  dl_api dl_collection *dl_init_collection_custom(dl_collection *dl_restrict col, dl_collection_settings *dl_restrict settings, dl_natural element_size);

  dl_api dl_collection *dl_init_collection_array(dl_collection *dl_restrict col, dl_collection_type type, dl_comparator *dl_restrict comp, dl_handler *dl_restrict destruct_entry, dl_byte * data, dl_natural element_size, dl_natural count);

  dl_api dl_natural dl_collection_element_size(const dl_collection *dl_restrict col);

  dl_api void dl_destroy_collection(dl_collection *dl_restrict col);

  dl_api dl_bool dl_collection_is_set(const dl_collection *dl_restrict col);
  dl_api dl_bool dl_collection_is_sorted(const dl_collection * dl_restrict col);
  dl_api dl_bool dl_collection_is_queue(const dl_collection * dl_restrict col);
  dl_api dl_bool dl_collection_is_vector(const dl_collection * dl_restrict col);
  dl_api dl_bool dl_collection_is_linked_list(const dl_collection * dl_restrict col);

  dl_api dl_integer dl_collection_copy(const dl_collection *dl_restrict original, dl_collection *dl_restrict target);
  dl_api dl_integer dl_collection_copy_array(const dl_any data, dl_natural count, dl_collection *dl_restrict target);

  dl_api dl_iterator dl_collection_begin(const dl_collection *dl_restrict col);
  dl_api dl_iterator dl_collection_end(const dl_collection *dl_restrict col);

  dl_api dl_any dl_collection_pop(dl_collection *dl_restrict col, dl_any out);
  dl_api dl_bool dl_collection_pop_forget(dl_collection *dl_restrict col);
  dl_api dl_bool dl_collection_pop_destroy(dl_collection *dl_restrict col);

  dl_api dl_any dl_collection_push(dl_collection *dl_restrict col, const dl_any value);
  dl_api dl_any dl_collection_push_index(dl_collection *dl_restrict col, const dl_any value, dl_iterator *out_index);
  dl_api dl_any dl_collection_push_start(dl_collection *dl_restrict col, dl_iterator *iter);
  dl_api dl_any dl_collection_push_finish(dl_collection *dl_restrict col, dl_iterator *iter);

  dl_api const dl_any dl_collection_begin_ref(const dl_collection *dl_restrict col, dl_iterator *iter);
  dl_api const dl_any dl_collection_end_ref(const dl_collection *dl_restrict col, dl_iterator *iter);

  dl_api dl_iterator dl_collection_index(dl_collection *dl_restrict col, dl_natural index);
  dl_api dl_iterator dl_collection_index_of(const dl_collection *dl_restrict col, const dl_any item);

  dl_api const dl_any dl_collection_next(const dl_collection *dl_restrict col, dl_iterator *iter);
  dl_api const dl_any dl_collection_prev(const dl_collection *dl_restrict col, dl_iterator *iter);

  dl_api dl_any dl_collection_find(const dl_collection *dl_restrict col, dl_filter *dl_restrict predicate, dl_iterator *iter);
  dl_api dl_any dl_collection_find_last(const dl_collection *dl_restrict col, dl_filter *dl_restrict predicate, dl_iterator *iter);

  dl_api dl_any dl_collection_foldl(const dl_collection *dl_restrict col, dl_any initial, dl_folder *func);
  dl_api dl_any dl_collection_foldr(const dl_collection *dl_restrict col, dl_any initial, dl_folder *func);
  dl_api const dl_any dl_collection_search(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *iter);
  dl_api const dl_any dl_collection_search_region(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter);

  dl_api dl_any dl_collection_get(const dl_collection *dl_restrict col, dl_iterator index, dl_any out);
  dl_api dl_any dl_collection_insert(dl_collection *dl_restrict col, dl_iterator *index, dl_any item);
  dl_api const dl_any dl_collection_peek(const dl_collection *dl_restrict col);
  dl_api const dl_any dl_collection_ref(const dl_collection *dl_restrict col, dl_iterator iter);
  dl_api dl_any dl_collection_remove_at(dl_collection *dl_restrict col, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_remove_first(dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_remove_last(dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *index, dl_any out);
  dl_api dl_any dl_collection_set(dl_collection *dl_restrict col, dl_iterator *index, dl_any value);

  dl_api dl_bool dl_collection_all(const dl_collection *dl_restrict col, dl_filter *func);
  dl_api dl_bool dl_collection_any(const dl_collection *dl_restrict col, dl_filter *func);
  dl_api dl_bool dl_collection_contains(const dl_collection *dl_restrict col, const dl_any item);
  dl_api dl_bool dl_collection_is_empty(const dl_collection *dl_restrict col);

  dl_api dl_integer dl_collection_map(const dl_collection *dl_restrict col, dl_converter *func, dl_collection *out);
  dl_api dl_integer dl_collection_zip(const dl_collection *dl_restrict col1, const dl_collection *dl_restrict col2, dl_zipper *zip, dl_collection *out);

  dl_api dl_bool dl_collection_forget_at(dl_collection *dl_restrict col, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_at(dl_collection *dl_restrict col, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_first(dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_bool dl_collection_destroy_last(dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *iter);
  dl_api dl_bool dl_collection_swap(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b);
  dl_api dl_bool dl_collection_quick_sort(dl_collection *dl_restrict col, dl_comparator *compare);
  dl_api dl_bool dl_collection_quick_sort_region(dl_collection *dl_restrict col, dl_comparator *compare, dl_iterator left, dl_iterator right);
  dl_api dl_bool dl_collection_sort(dl_collection *dl_restrict col);

  dl_api void dl_collection_clear(dl_collection *dl_restrict col);

  dl_api dl_integer dl_collection_count(const dl_collection *dl_restrict col);
  dl_api dl_integer dl_collection_find_all(const dl_collection *dl_restrict col, dl_filter *predicate, dl_collection *out);

  dl_api dl_integer dl_collection_take(dl_collection *dl_restrict col, dl_natural count, dl_collection *out);
  dl_api dl_integer dl_collection_destroy_all(dl_collection *dl_restrict col, dl_filter *predicate);
  dl_api dl_integer dl_collection_destroy_range(dl_collection *dl_restrict col, dl_iterator *index, dl_natural count);
  dl_api dl_integer dl_collection_drop(dl_collection *dl_restrict col, dl_natural count);
  dl_api dl_integer dl_collection_ref_array(dl_collection *dl_restrict col, dl_iterator index, dl_any *out_array);
  dl_api dl_integer dl_collection_remove_all(dl_collection *dl_restrict col, dl_filter *predicate, dl_collection *out);
  dl_api dl_integer dl_collection_remove_range(dl_collection *dl_restrict col, dl_iterator *index, dl_natural count, dl_collection *out);

#endif /* DL_USE_CONTAINERS */

#if defined(__cplusplus)
}
#endif



/*****************************************************************************
 **  IMPLEMENTATION
 ****************************************************************************/

#if DL_IMPLEMENTATION


/*****************************************************************************
 **  Logging
 ****************************************************************************/

#if DL_USE_LOGGING

dl_natural _default_log_handler(dl_log_channel ch, const char *dl_restrict file, dl_natural line, const char *dl_restrict function, const char *dl_restrict msg) {
  char time_buf[20];

#if DL_HAS_TIME
# if DL_IS_MSC
  struct tm ltime;
  time_t t = time(NULL);
  localtime_s(&ltime, &t);
  strftime(time_buf, sizeof(time_buf), "%F %T", &ltime);
# else
  time_t t = time(NULL);
  strftime(time_buf, sizeof(time_buf), "%F %T", localtime(&t));
# endif
#else
  time_buf[0] = (char)0;
#endif

  switch (ch) {
  case DL_LOG_INFO:
    fprintf(stdout, "%s:%lu:%s\n[LOG %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_WARNING:
    fprintf(stdout, "%s:%lu:%s\n[WRN %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_ERROR:
    fprintf(stderr, "%s:%lu:%s\n[ERR %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_TEST:
    fprintf(stdout, "[TST %s] %s\n", time_buf, msg);
    return ch;
  case DL_LOG_MESSAGE:
    fprintf(stdout, "[MSG %s] %s\n", time_buf, msg);
    return ch;
  default:
    return 0;
  }
}

dl_natural (*dl_active_log_handler)(dl_log_channel ch, const char *dl_restrict, dl_natural, const char *dl_restrict, const char *dl_restrict) = _default_log_handler;

dl_natural dl_log_message(dl_log_channel ch, const char *dl_restrict file, dl_natural line, const char *dl_restrict function, const char *dl_restrict fmt, ...) {
  char buf[256];
  va_list args1, args2;
  va_start(args1, fmt);
  va_copy(args2, args1);
  dl_natural count = 1 + vsnprintf(NULL, 0, fmt, args1);
  count = count > 254 ? 254 : count;
  va_end(args1);
  vsnprintf(buf, count, fmt, args2);
  buf[count + 1] = 0;
  va_end(args2);

  return dl_active_log_handler(ch, file, line, function, buf);
}

#endif /* DL_USE_LOGGING */



/*****************************************************************************
 **  Tests
 ****************************************************************************/

#if DL_USE_TEST

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

#endif /* DL_USE_TEST */



/*****************************************************************************
 **  Math
 ****************************************************************************/

#if DL_USE_MATH

#ifndef DL_PI
const dl_real DL_PI = 3.14159265359f;
#endif
#ifndef DL_E
const dl_real DL_E = 2.71828182846f;
#endif

#ifndef DL_INV_PI
const dl_real DL_INV_PI = 0.318309886185f;
#endif
#ifndef DL_INV_E
const dl_real DL_INV_E = 0.367879441171f;
#endif
#ifndef DL_EPSILON
const dl_real DL_EPSILON = 0.001f;
#endif

const dl_mat4 dl_mat4_identity = { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
const dl_point2 dl_point2_zero = { 0, 0 };
const dl_point2 dl_point2_one = { 1, 1 };
const dl_vec2 dl_vec2_up = { 0, 1 };
const dl_vec2 dl_vec2_right = { 1, 0 };
const dl_vec3 dl_vec3_up = { 0, 1, 0, 0 };
const dl_vec3 dl_vec3_right = { 1, 0, 0, 0 };

const dl_vec2 dl_vec2_down = { 0, -1 };
const dl_vec2 dl_vec2_left = { -1, 0 };
const dl_vec3 dl_vec3_down = { 0, -1, 0, 0 };
const dl_vec3 dl_vec3_left = { -1, 0, 0, 0 };

#if DL_USE_LEFT_HANDED
const dl_vec3 dl_vec3_forward = { 0, 0, 1, 0 };
const dl_vec3 dl_vec3_backward = { 0, 0, -1, 0 };
#else
const dl_vec3 dl_vec3_forward = { 0, 0, -1, 0 };
const dl_vec3 dl_vec3_backward = { 0, 0, 1, 0 };
#endif

const dl_vec2 dl_vec2_zero = { 0, 0 };
const dl_vec3 dl_vec3_zero = { 0, 0, 0, 0 };
const dl_point3 dl_point3_zero = { 0, 0, 0, 1 };

const dl_vec2 dl_vec2_one = { 1, 1 };
const dl_vec3 dl_vec3_one = { 1, 1, 1, 0 };
const dl_point3 dl_point3_one = { 1, 1, 1, 1 };

dl_api dl_bool dl_approximately_equal(dl_real a, dl_real b, dl_real epsilon) {
  return dl_abs(a - b) < epsilon;
}

dl_api dl_integer dl_floor_to_integer(dl_real n) {
  return (dl_integer)dl_floor(n);
}

dl_api dl_integer dl_ceil_to_integer(dl_real n) {
  return (dl_integer)dl_ceil(n);
}

dl_api dl_integer dl_round_to_integer(dl_real n) {
#if !DL_IS_ATLEAST_C99
  dl_real floored = dl_floor(n);
  dl_real frac = n - floored;
  return frac > 0.5 ? (dl_integer)(floored + 1) : (dl_integer)floored;
#else
  return (dl_integer)dl_round(n);
#endif
}

dl_api dl_real dl_rationalize(dl_real value, dl_natural decimal_points) {
#if !DL_IS_ATLEAST_C99
  dl_real d = (dl_real)pow(10, (dl_real)decimal_points);
  dl_real floored = floor(value);
  dl_real frac = value - floored;
  dl_real n = frac > 0.5 ? (floored + 1) : floored;
  value = (value - n) * d;
  floored = floor(value);
  frac = value - floored;
  value = frac > 0.5 ? (floored + 1) : floored;
  return n + (value / d);
#else  
  dl_real d = (dl_real)dl_pow(10, (dl_real)decimal_points);
  dl_real n = (dl_real)dl_round(value);
  return n + (roundf((value - n) * d) / d);
#endif
}

dl_api dl_real dl_degree_to_radian(dl_real degree) {
  static const dl_real factor = 0.01745329252f;
  return degree * factor;
}

dl_api dl_real dl_radian_to_degree(dl_real radian) {
  static const dl_real factor = 57.2957795131f;
  return radian * factor;
}

dl_integer _factorial_cache[13] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600 };

dl_api dl_integer dl_factorial(dl_integer n) {
  dl_integer i;
  if (dl_likely((dl_natural)n < sizeof(_factorial_cache)))
    return _factorial_cache[n];
  if (dl_unlikely(n < 0))
    return 1;
  for (i = n - 1; i > 0; --i) {
    n = n * i;
  }
  return n;
}



dl_api dl_random_state *dl_init_random(dl_random_state *state, dl_integer seed) {
  return dl_init_random_custom(state, DL_INTEGER_MAX, 1103515245, 12345, seed);
}

dl_api dl_random_state *dl_init_random_custom(dl_random_state *state, dl_integer m, dl_integer a, dl_integer c, dl_integer seed) {
  if (dl_safety(state == NULL))
    return NULL;

  state->m = dl_abs(m);
  state->a = dl_abs(a);
  state->c = dl_abs(c);
  state->seed = (a * seed + c) % m;

  return state;
}

dl_api dl_real dl_random_degree(dl_random_state *state) {
  return dl_random_real_range(state, 0, 360);
}

dl_api dl_real dl_random_radian(dl_random_state *state) {
  return dl_random_real_range(state, 0, 2 * DL_PI);
}

dl_api dl_real dl_random_real(dl_random_state *state, dl_real max) {
  if (dl_safety(state == NULL))
    return 0;

  state->seed = (state->a * state->seed + state->c) % state->m;

  return fabs((dl_real)state->seed / (dl_real)state->m) * max;
}

dl_api dl_integer dl_random_integer(dl_random_state *state, dl_integer max) {
  if (dl_safety(state == NULL))
    return 0;

  state->seed = (state->a * state->seed + state->c) % state->m;

  return dl_abs(state->seed) % max;
}

dl_api dl_integer dl_random_integer_range(dl_random_state *state, dl_integer min, dl_integer max) {
  return dl_random_integer(state, max - min) + min;
}

dl_api dl_real dl_random_real_range(dl_random_state *state, dl_real min, dl_real max) {
  return dl_random_real(state, max - min) + min;
}

#if DL_IS_WINDOWS

dl_api dl_random_state *dl_init_random_time(dl_random_state *state) {
  static const uint64_t epoch = ((uint64_t)116444736000000000ULL);
  
  SYSTEMTIME system_time;
  FILETIME file_time;
  uint64_t time;
  long tv_sec, tv_usec;

  GetSystemTime(&system_time);
  SystemTimeToFileTime(&system_time, &file_time);
  time = ((uint64_t)file_time.dwLowDateTime);
  time += ((uint64_t)file_time.dwHighDateTime) << 32;

  tv_sec = (long)((time - epoch) / 10000000L);
  tv_usec = (long)(system_time.wMilliseconds * 1000);

  return dl_init_random(state, tv_usec + tv_sec);
}

#endif

#if (DL_IS_LINUX || DL_IS_APPLE) && DL_IS_ATLEAST_C99

dl_api dl_random_state *dl_init_random_time(dl_random_state *state) {
  struct timeval t1;
  gettimeofday(&t1, NULL);
  return dl_init_random(state, t1.tv_usec + t1.tv_sec);
}

#else
dl_api dl_random_state *dl_init_random_time(dl_random_state *state) {
  return dl_init_random(state, 0);
}
#endif



dl_api dl_vec2 *dl_init_vec2(dl_vec2 * dl_restrict v, dl_real x, dl_real y) {
  if (dl_safety(v == NULL))
    return NULL;

  v->x = x;
  v->y = y;
  
  return v;
}

dl_api dl_vec2 *dl_vec2_add(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;

  return out;
}

dl_api dl_vec2 *dl_vec2_sub(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;

  return out;
}

dl_api dl_vec2 *dl_vec2_mul_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;

  return out;
}

dl_api dl_vec2 *dl_vec2_div_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out) {
  return dl_vec2_mul_scalar(left, 1.0f / scalar, out);
}

dl_api dl_vec2 *dl_vec2_add_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;

  return out;
}

dl_api dl_vec2 *dl_vec2_sub_scalar(const dl_vec2 *dl_restrict left, dl_real scalar, dl_vec2 *dl_restrict out) {
  return dl_vec2_add_scalar(left, -scalar, out);
}

dl_api dl_vec2 *dl_vec2_normalize(const dl_vec2 *dl_restrict left, dl_vec2 *dl_restrict out) {
  dl_real inv_magnitude;
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  inv_magnitude = 1.0f / dl_hypot(left->x, left->y);
  dl_vec2_mul_scalar(left, inv_magnitude, out);

  return out;
}

dl_api dl_vec2 *dl_vec2_negate(const dl_vec2 *dl_restrict left, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;

  return out;
}

dl_api dl_real dl_vec2_dot(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  return left->x * right->x + left->y * right->y;
}

dl_api dl_bool dl_vec2_approximately_equal(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict right, dl_real epsilon) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  return fabs(left->x - right->x) < epsilon && fabs(left->y - right->y) < epsilon;
}

dl_api dl_real dl_vec2_square_magnitude(const dl_vec2 *dl_restrict left) {
  if (dl_safety(left == NULL))
    return 0;
  return left->x * left->x + left->y * left->y;
}

dl_api dl_real dl_vec2_magnitude(const dl_vec2 *dl_restrict left) {
  if (dl_safety(left == NULL))
    return 0;
  
  return dl_hypot(left->x, left->y);
}

dl_api dl_vec2 *dl_vec2_reflect(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict normal, dl_vec2 *dl_restrict out) {
  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;
  
  return dl_vec2_sub(left, dl_vec2_mul_scalar(normal, dl_vec2_dot(normal, left) * 2.0f, out), out);
}

dl_api dl_vec2 *dl_vec2_refract(const dl_vec2 *dl_restrict left, const dl_vec2 *dl_restrict normal, float eta, dl_vec2 *dl_restrict out) {
  float dot, k;
  dl_vec2 t;

  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  dot = dl_vec2_dot(normal, left);
  k = 1.0f - (eta * eta) * (1.0f - (dot * dot));
  if (dl_unlikely(k < 0.0f)) {
    *out = dl_vec2_zero;
    return out;
  }

  return dl_vec2_sub(dl_vec2_mul_scalar(left, eta, &t), dl_vec2_mul_scalar(normal, eta * dot * dl_sqrt(k), out), out);
}



dl_api dl_point2 *dl_init_point2(dl_point2 * dl_restrict p, dl_real x, dl_real y) {
  return dl_init_vec2((dl_vec2 *)p, x, y);
}

dl_api dl_point2 *dl_point2_add(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_point2 *dl_restrict out) {
  return dl_vec2_add(left, right, out);
}

dl_api dl_point2 *dl_point2_sub(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_point2 *dl_restrict out) {
  return dl_vec2_sub(left, right, out);
}

dl_api dl_point2 *dl_point2_mul_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out) {
  return dl_vec2_mul_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_div_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out) {
  return dl_vec2_div_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_add_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out) {
  return dl_vec2_add_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_sub_scalar(const dl_point2 *dl_restrict left, dl_real scalar, dl_point2 *dl_restrict out) {
  return dl_vec2_sub_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_negate(const dl_point2 *dl_restrict left, dl_point2 *dl_restrict out) {
  return dl_vec2_negate(left, out);
}

dl_api dl_bool dl_point2_approximately_equal(const dl_point2 *dl_restrict left, const dl_point2 *dl_restrict right, dl_real epsilon) {
  return dl_vec2_approximately_equal(left, right, epsilon);
}

dl_api dl_real dl_point2_line_orientation(const dl_point2 *dl_restrict point, const dl_point2 *dl_restrict line_a, const dl_point2 *dl_restrict line_b) {
  dl_real rise, run, m, b;

  if (dl_safety(point == NULL || line_a == NULL || line_b == NULL))
    return 0;
  
  rise = line_b->y - line_a->y;
  run = line_b->x - line_a->x;
  m = rise / run;
  b = -(m * line_a->x - line_a->y);

  return m * point->x + b - point->y;
}



dl_api dl_vec4 *dl_init_vec4(dl_vec4 * dl_restrict v, dl_real x, dl_real y, dl_real z, dl_real w) {
  if (dl_safety(v == NULL))
    return NULL;
  
  v->x = x;
  v->y = y;
  v->z = z;
  v->w = w;

  return v;
}

dl_api dl_vec4 *dl_vec4_add(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = left->w + right->w;

  return out;
}

dl_api dl_vec4 *dl_vec4_sub(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = left->w - right->w;

  return out;
}

dl_api dl_real dl_vec4_dot(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  
  return left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
}

dl_api dl_vec4 *dl_vec4_normalize(const dl_vec4 *dl_restrict left, dl_vec4 *dl_restrict out) {
  return dl_vec4_div_scalar(left, dl_vec4_magnitude(left), out);
}

dl_api dl_vec4 *dl_vec4_negate(const dl_vec4 *dl_restrict left, dl_vec4 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = -left->w;

  return out;
}

dl_api dl_real dl_vec4_square_magnitude(const dl_vec4 *dl_restrict left) {
  if (dl_safety(left == NULL))
    return 0;
  
  return (left->x * left->x)
    + (left->y * left->y)
    + (left->z * left->z)
    + (left->w * left->w);
}

dl_api dl_real dl_vec4_magnitude(const dl_vec4 *dl_restrict left) {
  return dl_sqrt(dl_vec4_square_magnitude(left));
}

dl_api dl_bool dl_vec4_approximately_equal(const dl_vec4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_real epsilon) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  
  return fabs(left->x - right->x) < epsilon
    && fabs(left->y - right->y) < epsilon
    && fabs(left->z - right->z) < epsilon
    && fabs(left->w - right->w) < epsilon;
}

dl_api dl_vec4 *dl_vec4_mul_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = left->w * scalar;

  return out;
}

dl_api dl_vec4 *dl_vec4_div_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *dl_restrict out) {
  return dl_vec4_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_vec4 *dl_vec4_add_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = left->w + scalar;

  return out;
}

dl_api dl_vec4 *dl_vec4_sub_scalar(const dl_vec4 *dl_restrict left, dl_real scalar, dl_vec4 *dl_restrict out) {
  return dl_vec4_add_scalar(left, -scalar, out);
}



dl_api dl_point3 *dl_init_point3(dl_point3 * dl_restrict p, dl_real x, dl_real y, dl_real z) {
  return dl_init_vec4(p, x, y, z, 1);
}

dl_api dl_point3 *dl_point3_add(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_sub(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_mul_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_div_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out) {
  return dl_point3_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_point3 *dl_point3_add_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_sub_scalar(const dl_point3 *dl_restrict left, dl_real scalar, dl_point3 *dl_restrict out) {
  return dl_point3_add_scalar(left, -scalar, out);
}

dl_api dl_point3 *dl_point3_negate(const dl_point3 *dl_restrict left, dl_point3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 1;

  return out;
}

dl_api dl_bool dl_point3_approximately_equal(const dl_point3 *dl_restrict left, const dl_point3 *dl_restrict right, dl_real epsilon) {
  return dl_vec4_approximately_equal(left, right, epsilon);
}

dl_api dl_point3 *dl_point3_rotate(const dl_point3 *dl_restrict left, const dl_vec3 *dl_restrict axis, const dl_real angle, dl_point3 *dl_restrict out) {
  dl_mat4 mat;

  return dl_mat4_mul_point3(dl_init_mat4_rotate(&mat, axis, angle), left, out);
}



dl_api dl_vec3 *dl_init_vec3(dl_vec3 * dl_restrict v, dl_real x, dl_real y, dl_real z) {
  return dl_init_vec4(v, x, y, z, 0);
}

dl_api dl_vec3 *dl_vec3_add(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_sub(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_cross(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out) {
  dl_real orig_x, orig_y;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  /* In case right was passed as out, as well. */
  orig_x = right->x;
  orig_y = right->y;
  
  out->x = left->y * right->z - left->z * orig_y;
  out->y = left->z * orig_x - left->x * right->z;
  out->z = left->x * orig_y - left->y * orig_x;
  out->w = 0;

  return out;
}

dl_api dl_real dl_vec3_dot(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right) {
  return left->x * right->x + left->y * right->y + left->z * right->z;
}

dl_api dl_vec3 *dl_vec3_mul_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_div_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out) {
  return dl_vec3_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_vec3 *dl_vec3_add_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_sub_scalar(const dl_vec3 *dl_restrict left, dl_real scalar, dl_vec3 *dl_restrict out) {
  return dl_vec3_add_scalar(left, -scalar, out);
}

dl_api dl_vec3 *dl_vec3_normalize(const dl_vec3 *dl_restrict left, dl_vec3 *dl_restrict out) {
  return dl_vec3_div_scalar(left, dl_vec3_magnitude(left), out);
}

dl_api dl_bool dl_vec3_normalized(const dl_vec3 *dl_restrict left) {
  return dl_approximately_equal(fabs(left->x) + fabs(left->y) + fabs(left->z), 1.0, DL_EPSILON);
}

dl_api dl_vec3 *dl_vec3_negate(const dl_vec3 *dl_restrict left, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 0;

  return out;
}

dl_api dl_bool dl_vec3_approximately_equal(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_real epsilon) {
  return dl_vec4_approximately_equal(left, right, epsilon);
}

dl_api dl_real dl_vec3_square_magnitude(const dl_vec3 *dl_restrict left) {
  if (dl_safety(left == NULL))
    return 0;

  return left->x * left->x
    + left->y * left->y
    + left->z * left->z;
}

dl_api dl_real dl_vec3_magnitude(const dl_vec3 *dl_restrict left) {
  return dl_sqrt(dl_vec3_square_magnitude(left));
}

dl_api dl_vec3 *dl_vec3_rotate(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict axis, const dl_real angle, dl_vec3 *dl_restrict out) {
  dl_mat4 mat;

  return dl_mat4_mul_vec3(dl_init_mat4_rotate(&mat, axis, angle), left, out);
}

dl_api dl_vec3 *dl_vec3_reflect(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict normal, dl_vec3 *dl_restrict out) {
  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  return dl_vec3_sub(left, dl_vec3_mul_scalar(normal, dl_vec3_dot(normal, left) * 2.0, out), out);
}

dl_api dl_vec3 *dl_vec3_refract(const dl_vec3 *dl_restrict left, const dl_vec3 *dl_restrict normal, float eta, dl_vec3 *dl_restrict out) {
  float dot, k;
  dl_vec3 t;

  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;
  
  dot = dl_vec3_dot(normal, left);
  k = 1.0 - (eta * eta) * (1.0 - (dot * dot));
  if (dl_unlikely(k < 0.0)) {
    *out = dl_vec3_zero;
    return out;
  }

  return dl_vec3_sub(dl_vec3_mul_scalar(left, eta, &t), dl_vec3_mul_scalar(normal, eta * dot * dl_sqrt(k), out), out);
}



dl_api dl_mat4 *dl_init_mat4(dl_mat4 * dl_restrict m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3) {
  if (dl_safety(m == NULL))
    return NULL;
  
  m->ary[0][0] = c0r0;
  m->ary[0][1] = c0r1;
  m->ary[0][2] = c0r2;
  m->ary[0][3] = c0r3;
  m->ary[1][0] = c1r0;
  m->ary[1][1] = c1r1;
  m->ary[1][2] = c1r2;
  m->ary[1][3] = c1r3;
  m->ary[2][0] = c2r0;
  m->ary[2][1] = c2r1;
  m->ary[2][2] = c2r2;
  m->ary[2][3] = c2r3;
  m->ary[3][0] = c3r0;
  m->ary[3][1] = c3r1;
  m->ary[3][2] = c3r2;
  m->ary[3][3] = c3r3;

  return m;
}

dl_api dl_mat4 *dl_init_mat4_cols(dl_mat4 * dl_restrict m, const dl_vec4 *dl_restrict column_a, const dl_vec4 *dl_restrict column_b, const dl_vec4 *dl_restrict column_c, const dl_vec4 *dl_restrict column_d) {
  if (dl_safety(m == NULL || column_a == NULL || column_b == NULL || column_c == NULL || column_d == NULL))
    return NULL;
  
  *(dl_vec4 *)(&m->ary[0]) = *column_a;
  *(dl_vec4 *)(&m->ary[1]) = *column_b;
  *(dl_vec4 *)(&m->ary[2]) = *column_c;
  *(dl_vec4 *)(&m->ary[3]) = *column_d;

  return m;
}

dl_api dl_mat4 *dl_mat4_add(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + right->ary[i][j];

  return out;
}

dl_api dl_mat4 *dl_mat4_sub(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] - right->ary[i][j];

  return out;
}

dl_api dl_mat4 *dl_mat4_mul(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
  dl_vec4 *A, *B;
  dl_mat4 temp;

  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  /* The copy prevents errors arising from right/left and out pointing at the same data. */
  temp = *right;
  A = (dl_vec4 *)&left->ary;
  B = (dl_vec4 *)&temp.ary;

  out->ary[0][0] = A[0].x * B[0].x + A[1].x * B[0].y + A[2].x * B[0].z + A[3].x * B[0].w;
  out->ary[0][1] = A[0].y * B[0].x + A[1].y * B[0].y + A[2].y * B[0].z + A[3].y * B[0].w;
  out->ary[0][2] = A[0].z * B[0].x + A[1].z * B[0].y + A[2].z * B[0].z + A[3].z * B[0].w;
  out->ary[0][3] = A[0].w * B[0].x + A[1].w * B[0].y + A[2].w * B[0].z + A[3].w * B[0].w;

  out->ary[1][0] = A[0].x * B[1].x + A[1].x * B[1].y + A[2].x * B[1].z + A[3].x * B[1].w;
  out->ary[1][1] = A[0].y * B[1].x + A[1].y * B[1].y + A[2].y * B[1].z + A[3].y * B[1].w;
  out->ary[1][2] = A[0].z * B[1].x + A[1].z * B[1].y + A[2].z * B[1].z + A[3].z * B[1].w;
  out->ary[1][3] = A[0].w * B[1].x + A[1].w * B[1].y + A[2].w * B[1].z + A[3].w * B[1].w;

  out->ary[2][0] = A[0].x * B[2].x + A[1].x * B[2].y + A[2].x * B[2].z + A[3].x * B[2].w;
  out->ary[2][1] = A[0].y * B[2].x + A[1].y * B[2].y + A[2].y * B[2].z + A[3].y * B[2].w;
  out->ary[2][2] = A[0].z * B[2].x + A[1].z * B[2].y + A[2].z * B[2].z + A[3].z * B[2].w;
  out->ary[2][3] = A[0].w * B[2].x + A[1].w * B[2].y + A[2].w * B[2].z + A[3].w * B[2].w;

  out->ary[3][0] = A[0].x * B[3].x + A[1].x * B[3].y + A[2].x * B[3].z + A[3].x * B[3].w;
  out->ary[3][1] = A[0].y * B[3].x + A[1].y * B[3].y + A[2].y * B[3].z + A[3].y * B[3].w;
  out->ary[3][2] = A[0].z * B[3].x + A[1].z * B[3].y + A[2].z * B[3].z + A[3].z * B[3].w;
  out->ary[3][3] = A[0].w * B[3].x + A[1].w * B[3].y + A[2].w * B[3].z + A[3].w * B[3].w;

  return out;
}

dl_bool dl_mat4_approximately_equal(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_real epsilon) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL))
    return false;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (dl_abs(left->ary[i][j] - right->ary[i][j]) > epsilon)
	return false;
  
  return true;
}

dl_api dl_vec4 *dl_mat4_mul_vec4(const dl_mat4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out) {
  dl_vec4 v;

  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  v = *right;
  out->x = left->ary[0][0] * v.x + left->ary[1][0] * v.y + left->ary[2][0] * v.z + left->ary[3][0] * v.w;
  out->y = left->ary[0][1] * v.x + left->ary[1][1] * v.y + left->ary[2][1] * v.z + left->ary[3][1] * v.w;
  out->z = left->ary[0][2] * v.x + left->ary[1][2] * v.y + left->ary[2][2] * v.z + left->ary[3][2] * v.w;
  out->w = left->ary[0][3] * v.x + left->ary[1][3] * v.y + left->ary[2][3] * v.z + left->ary[3][3] * v.w;

  return out;
}

dl_api dl_vec3 *dl_mat4_mul_vec3(const dl_mat4 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out) {
  dl_vec3 v;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  v = *right;
  out->x = left->ary[0][0] * v.x + left->ary[1][0] * v.y + left->ary[2][0] * v.z + left->ary[3][0] * v.w;
  out->y = left->ary[0][1] * v.x + left->ary[1][1] * v.y + left->ary[2][1] * v.z + left->ary[3][1] * v.w;
  out->z = left->ary[0][2] * v.x + left->ary[1][2] * v.y + left->ary[2][2] * v.z + left->ary[3][2] * v.w;
  out->w = 0.0;

  return out;
}

dl_api dl_point3 *dl_mat4_mul_point3(const dl_mat4 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out) {
  dl_point3 p;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  p = *right;
  out->x = left->ary[0][0] * p.x + left->ary[1][0] * p.y + left->ary[2][0] * p.z + left->ary[3][0] * p.w;
  out->y = left->ary[0][1] * p.x + left->ary[1][1] * p.y + left->ary[2][1] * p.z + left->ary[3][1] * p.w;
  out->z = left->ary[0][2] * p.x + left->ary[1][2] * p.y + left->ary[2][2] * p.z + left->ary[3][2] * p.w;
  out->w = 1.0;

  return out;
}

dl_api dl_mat4 *dl_mat4_transpose(const dl_mat4 *dl_restrict left, dl_mat4 *dl_restrict out) {
  dl_real v[4][4];
  dl_natural a_idx, b_idx;

  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  for (a_idx = 0; a_idx < 4; ++a_idx)
    for (b_idx = 0; b_idx < 4; ++b_idx)
      v[a_idx][b_idx] = left->ary[a_idx][b_idx];
  for (a_idx = 0; a_idx < 4; ++a_idx)
    for (b_idx = 0; b_idx < 4; ++b_idx)
      out->ary[b_idx][a_idx] = v[a_idx][b_idx];

  return out;
}

dl_api dl_mat4 *dl_mat4_mul_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  dl_integer i,j;
  
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] * scalar;

  return out;
}

dl_api dl_mat4 *dl_mat4_div_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  return dl_mat4_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_mat4 *dl_mat4_add_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + scalar;

  return out;
}

dl_api dl_mat4 *dl_mat4_sub_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  return dl_mat4_add_scalar(left, -scalar, out);
}

dl_api dl_mat4 *dl_init_mat4_translate(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;
  
  *m = dl_mat4_identity;
  m->ary[3][0] = x;
  m->ary[3][1] = y;
  m->ary[3][2] = z;

  return m;
}

dl_api dl_mat4 *dl_init_mat4_rotate_x(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);

  return dl_init_mat4(m,
		   1,  0,  0, 0,
		   0,  c, -s, 0,
		   0,  s,  c, 0,
		   0,  0,  0, 1);
#else
# if !DL_IS_ATLEAST_C99
  c = (dl_real)cos(radians);
  s = (dl_real)sin(radians);
# else
  c = dl_cos(radians);
  s = dl_sin(radians);
# endif

  return dl_init_mat4(m,
		   1, 0,  0, 0,
		   0, c,  s, 0,
		   0, -s, c, 0,
		   0, 0,  0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_rotate_y(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;
  
#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);
  
  return dl_init_mat4(m,
		   c,  0, s, 0,
		   0,  1, 0, 0,
		   -s, 0, c, 0,
		   0,  0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
  
  return dl_init_mat4(m,
		   c, 0, -s, 0,
		   0, 1,  0, 0,
		   s, 0,  c, 0,
		   0, 0,  0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_rotate_z(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

  c = dl_cos(radians);
  s = dl_sin(radians);
  
  return dl_init_mat4(m,
		   c,  s, 0, 0,
		   -s, c, 0, 0,
		   0,  0, 1, 0,
		   0,  0, 0, 1);
}

dl_api dl_mat4 *dl_init_mat4_rotate(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict a, dl_real radians) {
  dl_vec3 i, t;
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;
  
  if (dl_unlikely(dl_vec3_approximately_equal(a, &dl_vec3_zero, DL_EPSILON))) {
    *m = dl_mat4_identity;
    return m;
  }
  
  if (dl_unlikely(!dl_vec3_normalized(a)))
    dl_vec3_normalize(a, &t);
  else
    t = *a;

#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);
  dl_vec3_mul_scalar(&t, 1.0 - c, &i);  

  return dl_init_mat4(m,
		   c + i.x * t.x,          i.x * t.y - s * t.z,    -(-i.x * t.z - s * t.y), 0,
		   i.y * t.x + s * t.z,    c + i.y * t.y,          -(-i.y * t.z + s * t.x), 0,
		   -(i.z * t.x + s * t.y), -(i.z * t.y - s * t.x), c + i.z * t.z,          0,
		   0, 0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
  dl_vec3_mul_scalar(&t, 1.0 - c, &i);  

  return dl_init_mat4(m,
		   c + i.x * t.x,       i.x * t.y - s * t.z, -i.x * t.z - s * t.y, 0,
		   i.y * t.x + s * t.z, c + i.y * t.y,       -i.y * t.z + s * t.x, 0,
		   i.z * t.x + s * t.y, i.z * t.y - s * t.x, c + i.z * t.z,       0,
		   0, 0, 0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_scale(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;
  
  *m = dl_mat4_identity;
  m->ary[0][0] = x;
  m->ary[1][1] = y;
  m->ary[2][2] = z;

  return m;
}

dl_api dl_mat4 *dl_init_mat4_look_at(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict origin, const dl_vec3 *dl_restrict target, const dl_vec3 *dl_restrict up) {
  dl_vec3 zaxis, yaxis, xaxis, temp;

  if (dl_safety(m == NULL || origin == NULL || target == NULL || up == NULL))
    return NULL;

  dl_vec3_sub(origin, target, &temp);
  dl_vec3_normalize(&temp, &zaxis);
  dl_vec3_cross(up, &zaxis, &temp);
  dl_vec3_normalize(&temp, &xaxis);
  dl_vec3_cross(&zaxis, &xaxis, &yaxis);

  return dl_init_mat4(m,
		   xaxis.x, yaxis.x, zaxis.x, 0,
		   xaxis.y, yaxis.y, zaxis.y, 0,
		   xaxis.z, yaxis.z, zaxis.z, 0,
		   -dl_vec3_dot(&xaxis, origin), -dl_vec3_dot(&yaxis, origin), -dl_vec3_dot(&zaxis, origin), 1);
}

dl_api dl_mat4 *dl_init_mat4_orthographic(dl_mat4 * dl_restrict m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far) {
  dl_real inv_width, inv_height, inv_depth;

  if (dl_safety(m == NULL))
    return NULL;

  inv_width = 1.0 / (x_right - x_left);
  inv_height = 1.0 / (yop - y_bottom);
  inv_depth = 1.0 / (z_far - z_near);

  return dl_init_mat4(m,
		   2 * inv_width, 0, 0, 0,
		   0, 2 * inv_height, 0, 0,
		   0, 0, 0, -2 * inv_depth,
		   -((x_right + x_left) * inv_width), -((yop + y_bottom) * inv_height), -((z_far + z_near) * inv_depth), 1);
}

dl_api dl_mat4 *dl_init_mat4_perspective(dl_mat4 * dl_restrict m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far) {
  dl_real half_fov, invan_fov, neg_depth, inv_neg_depth;

  if (dl_safety(m == NULL))
    return NULL;

  half_fov = vertical_fov * 0.5f;
  invan_fov = 1.0 / dl_tan(half_fov);
  neg_depth = z_near - z_far;
  inv_neg_depth = 1.0 / neg_depth;

  *m = dl_mat4_identity;
  m->ary[0][0] = (1.0 / aspect_ratio) * invan_fov;
  m->ary[1][1] = invan_fov;
  m->ary[2][2] = (-neg_depth) * inv_neg_depth;
  m->ary[2][3] = 1.0;
  m->ary[3][2] = (2.0 * z_far * z_near) * inv_neg_depth;
  m->ary[3][3] = 0.0;

  return m;
}

#endif /* DL_USE_MATH */



/*****************************************************************************
 **  Tweening
 ****************************************************************************/

#if DL_USE_TWEEN
  
dl_api dl_real dl_tween(dl_easing_function ease, dl_ease_direction direction, dl_real percent) {
  percent = dl_clamp01(percent);
  return ease(direction, percent);
}

dl_api dl_real dl_ease_linear(dl_ease_direction d, dl_real p) {
  return p;
}

dl_api dl_real dl_ease_quadratic(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return p * p;
  case DL_EASE_OUT:
    return p * (2.0 - p);
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * p * p;

    p = p - 1.0;
    return -0.5f * ((p * (p - 2.0)) - 1.0);
  }
  return 0;
}

dl_api dl_real dl_ease_cubic(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return (p * p * p);
  case DL_EASE_OUT:
    p = p - 1.0;
    return 1.0 + (p * p * p);
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p);

    p = p - 2.0;
    return 0.5f * (2.0 + (p * p * p));
  }
  return 0;
}

dl_api dl_real dl_ease_quartic(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return (p * p * p * p);
  case DL_EASE_OUT:
    p = p - 1.0;
    return 1.0 - (p * p * p * p);
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p * p);

    p = p - 2.0;
    return -0.5f * ((p * p * p * p) - 2.0);
  }
  return 0;
}

dl_api dl_real dl_ease_quintic(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return (p * p * p * p * p);
  case DL_EASE_OUT:
    p = p - 1.0;
    return 1.0 + (p * p * p * p * p);
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p * p * p);

    p = p - 2.0;
    return 0.5f * (2.0 + (p * p * p * p * p));
  }
  return 0;
}

dl_api dl_real dl_ease_sinusoidal(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return 1.0 - dl_cos(p * DL_PI * 0.5f);
  case DL_EASE_OUT:
    return dl_sin(p * DL_PI * 0.5f);
  case DL_EASE_INOUT:
    return 0.5f * (1 - dl_cos(DL_PI * p));
  }
  return 0;
}

dl_api dl_real dl_ease_exponential(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return dl_pow(1024.0, (p - 1.0));
  case DL_EASE_OUT:
    return 1.0 - dl_pow(2.0, -10.0 * p);
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * dl_pow(1024.0, (p - 1.0));
    return 0.5f * (2.0 - dl_pow(2.0, -10.0 * (p - 1.0)));
  }
  return 0;
}

dl_api dl_real dl_ease_circular(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return 1.0 - dl_sqrt(1.0 - (p * p));
  case DL_EASE_OUT:
    p = p - 1.0;
    return dl_sqrt(1.0 - (p * p));
  case DL_EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return -0.5f * (dl_sqrt(1.0 - (p * p)) - 1.0);

    p = p - 2.0;
    return 0.5f * (dl_sqrt(1.0 - (p * p)) + 1.0);
  }
  return 0;
}

dl_api dl_real dl_ease_elastic(dl_ease_direction d, dl_real p) {
  static const dl_real a = 1.0;
  static const dl_real k = 0.4f;

  return dl_ease_elastic_tunable(d, p, a, k);
}

dl_api dl_real dl_ease_elastic_tunable(dl_ease_direction d, dl_real p, dl_real a, dl_real k) {
  dl_real invk = 1.0 / k;
  dl_real s = a < 1.0 ? (k * 0.25f) : (k * dl_asin(1.0 / a) * 0.5f * DL_INV_PI);

  switch (d) {
  case DL_EASE_IN:
    p = p - 1.0;
    return -(a * dl_pow(2.0, 10.0 * p) * dl_sin((p - s) * 2.0 * DL_PI * invk));
  case DL_EASE_OUT:
    return 1.0 + (a * dl_pow(2.0, -10.0 * p) * dl_sin((p - s) * 2.0 * DL_PI * invk));
  case DL_EASE_INOUT:
    p = p * 2.0;
    if (p < 1)
      return -0.5f * a * dl_pow(2.0, 10.0 * (p - 1.0)) * dl_sin((p - 1 - s) * 2.0 * DL_PI * invk);
    return 1.0 + (0.5f * a * dl_pow(2.0, -10.0 * (p - 1.0)) * dl_sin((p - 1 - s) * 2.0 * DL_PI * invk));
  }
  return 0;
}

dl_api dl_real dl_ease_back(dl_ease_direction d, dl_real p) {
  static const dl_real s = 1.70158f;
  static const dl_real t = 1.525f;

  return dl_ease_back_tunable(d, p, s, t);
}

dl_api dl_real dl_ease_back_tunable(dl_ease_direction d, dl_real p, dl_real s, dl_real t) {
  switch (d) {
  case DL_EASE_IN:
    return (p * p) * ((p * (s + 1)) - s);
  case DL_EASE_OUT:
    p = p - 1;
    return 1 + ((p * p) * (s + (p * (s + 1))));
  case DL_EASE_INOUT:
    s = s * t;
    p = 2.0 * p;

    if (p < 1)
      return 0.5f * ((p * p) * ((s + 1) * p - s));

    p = p - 2.0;
    return 0.5f * (2.0 + ((p * p) * (s + (p * (s + 1)))));
  }
  return 0;
}

dl_api dl_real _ease_bounce_out(dl_real p) {
  if (p < 0.3636)
    return 7.5625f * p * p;
  if (p < 0.7273) {
    p = p - 0.5455f;
    return 0.75f + (7.5625f * p * p);
  }
  if (p < 0.9091) {
    p = p - 0.8182f;
    return 0.9375f + (7.5625f * p * p);
  }
  p = p - 0.9545f;
  return 0.9844f + (7.5625f * p * p);
}

dl_api dl_real _ease_bounce_in(dl_real p) {
  return 1 - _ease_bounce_out(1 - p);
}

dl_real dl_ease_bounce(dl_ease_direction d, dl_real p) {
  switch (d) {
  case DL_EASE_IN:
    return _ease_bounce_in(p);
  case DL_EASE_OUT:
    return _ease_bounce_out(p);
  case DL_EASE_INOUT:
    if (p < 0.5)
      return 0.5f * _ease_bounce_in(p * 2.0);
    return 0.5f + (0.5f * _ease_bounce_out((p * 2.0) - 1.0));
  }
  return 0;
}



dl_real *dl_interpolate(const dl_selector_function select, const dl_real *dl_restrict values, dl_natural length, dl_real percent, dl_real *out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_real *dl_select_linear(const dl_real *dl_restrict v, dl_natural l, dl_real p, dl_real *dl_restrict out) {
  dl_natural max_idx, idx, next_idx;
  dl_real scaled_p;
  
  if (dl_safety(v ==  NULL || out == NULL))
    return NULL;
  
  max_idx = l - 1;
  scaled_p = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(scaled_p);
  next_idx = idx + 1;
  
  if (dl_unlikely(next_idx > max_idx)) {
    *out = v[max_idx];
    return out;
  }

  return dl_lerp_real(v[idx], v[next_idx], (scaled_p - (dl_real)idx), out);
}

dl_real *dl_select_catmullrom(const dl_real *dl_restrict v, dl_natural l, dl_real p, dl_real *dl_restrict out) {
  dl_natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  dl_real target, t, v0, v1, t2, t3;
  
  if (dl_safety(v == NULL || out == NULL))
    return NULL;
  
  max_idx = l - 1;
  target = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(target);

  a_idx = dl_unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = dl_unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = dl_unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (dl_real)idx;

  v0 = 0.5f * (v[c_idx] - v[a_idx]);
  v1 = 0.5f * (v[d_idx] - v[b_idx]);
  t2 = t * t;
  t3 = t2 * t;

  *out = v[b_idx]
    + (t * v0)
    + (t2 * (-v1 - (2 * v0) + (3 * v[c_idx]) - (3 * v[b_idx])))
    + (t3 * (v1 + v0 + (2 * v[b_idx]) + (-2 * v[c_idx])));

  return out;
}



dl_point2 *dl_interpolate_point2(const dl_selector_function_point2 select, const dl_point2 *dl_restrict values, dl_natural length, dl_real percent, dl_point2 *dl_restrict out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_point2 *dl_select_linear_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out) {
  dl_natural max_idx, idx, next_idx;
  dl_real scaled_p;
  
  max_idx = l - 1;
  scaled_p = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(scaled_p);
  next_idx = idx + 1;

  if (dl_unlikely(next_idx > max_idx)) {
    *out = v[max_idx];
    return out;
  }

  return dl_lerp_point2(&v[idx], &v[next_idx], (scaled_p - (dl_real)idx), out);
}

dl_point2 *dl_select_bezier_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out) {
  dl_natural max_idx, idx, degree;
  dl_real target;
  dl_point2 t1, t2, compute_v[DL_BEZIER_DEGREE + 1];
  dl_integer i, j, desired_idx;
  
  max_idx = l - 1;
  degree = dl_clamp(DL_BEZIER_DEGREE, 1, max_idx);
  target = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(target);

  for (i = 0; i < degree + 1; ++i) {
    desired_idx = idx + i;
    desired_idx = dl_clamp(desired_idx, 0, max_idx);
    compute_v[i] = v[desired_idx];
  }
    
  for (i = 1; i <= degree; ++i)
    for (j = 0; j <= degree - i; ++j)
      dl_point2_add(
        dl_point2_mul_scalar(&compute_v[j], 1.0 - p, &t1),
		    dl_point2_mul_scalar(&compute_v[j + 1], p, &t2),
        &compute_v[j]);

  *out = compute_v[0];
  return out;
}

dl_point2 *dl_select_catmullrom_point2(const dl_point2 *dl_restrict v, dl_natural l, dl_real p, dl_point2 *dl_restrict out) {
  dl_natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  dl_real target, t, t2, t3;
  dl_point2 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;
  
  max_idx = l - 1;
  target = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(target);

  a_idx = dl_unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = dl_unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = dl_unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (dl_real)idx;
  t2 = t * t;
  t3 = t2 * t;

  dl_point2_mul_scalar(dl_point2_sub(&v[c_idx], &v[a_idx], &v0), 0.5f, &v0);
  dl_point2_mul_scalar(dl_point2_sub(&v[d_idx], &v[b_idx], &v1), 0.5f, &v1);

  dl_point2_mul_scalar(&v[b_idx], 3, &threev1);
  dl_point2_mul_scalar(&v[c_idx], 3, &threev2);
  dl_point2_mul_scalar(&v0, 2, &twov0);
  dl_point2_mul_scalar(&v[c_idx], -2, &ntwov2);
  dl_point2_mul_scalar(&v[b_idx], 2, &twov1);

  /* a = v[b_idx]; */
  dl_point2_mul_scalar(&v0, t, &b);
  dl_point2_mul_scalar(dl_point2_sub(dl_point2_add(dl_point2_sub(dl_point2_negate(&v1, &c), &twov0, &c), &threev2, &c), &threev1, &c), t2, &c);
  dl_point2_mul_scalar(dl_point2_add(&v1, dl_point2_add(&v0, dl_point2_add(&twov1, &ntwov2, &d), &d), &d), t3, &d);

  return dl_point2_add(&v[b_idx], dl_point2_add(&b, dl_point2_add(&c, &d, &v0), &v0), out);
}



dl_point3 *dl_interpolate_point3(const dl_selector_function_point3 select, const dl_point3 *dl_restrict values, dl_natural length, dl_real percent, dl_point3 *dl_restrict out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_point3 *dl_select_linear_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out) {
  dl_natural max_idx, idx, next_idx;
  dl_real scaled_p;
  
  max_idx = l - 1;
  scaled_p = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(scaled_p);
  next_idx = idx + 1;

  if (dl_unlikely(next_idx > max_idx)) {
    *out = v[max_idx];
    return out;
  }

  return dl_lerp_point3(&v[idx], &v[next_idx], (scaled_p - (dl_real)idx), out);
}

dl_point3 *dl_select_bezier_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out) {
  dl_natural max_idx, idx;
  dl_real target;
  dl_point3 temp[2], compute_v[DL_BEZIER_DEGREE + 1];
  dl_integer i, j, desired_idx;
  
  max_idx = l - 1;
  target = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(target);

  for (i = 0; i < DL_BEZIER_DEGREE + 1; ++i) {
    desired_idx = idx + i;
    desired_idx = dl_clamp(desired_idx, 0, max_idx);
    compute_v[i] = v[desired_idx];
  }
    
  for (i = 1; i <= DL_BEZIER_DEGREE; ++i)
    for (j = 0; j <= DL_BEZIER_DEGREE - i; ++j)
      dl_point3_add(dl_point3_mul_scalar(&compute_v[j], 1.0 - p, &temp[0]), dl_point3_mul_scalar(&compute_v[j + 1], p, &temp[1]), &compute_v[j]);

  *out = compute_v[0];
  return out;
}

dl_point3 *dl_select_catmullrom_point3(const dl_point3 *dl_restrict v, dl_natural l, dl_real p, dl_point3 *dl_restrict out) {
  dl_natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  dl_real target, t, t2, t3;
  dl_point3 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;
  
  max_idx = l - 1;
  target = (dl_real)max_idx * p;
  idx = (dl_natural)dl_floor(target);

  a_idx = dl_unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = dl_unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = dl_unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (dl_real)idx;
  t2 = t * t;
  t3 = t2 * t;

  dl_point3_mul_scalar(dl_point3_sub(&v[c_idx], &v[a_idx], &v0), 0.5f, &v0);
  dl_point3_mul_scalar(dl_point3_sub(&v[d_idx], &v[b_idx], &v1), 0.5f, &v1);

  dl_point3_mul_scalar(&v[b_idx], 3, &threev1);
  dl_point3_mul_scalar(&v[c_idx], 3, &threev2);
  dl_point3_mul_scalar(&v0, 2, &twov0);
  dl_point3_mul_scalar(&v[c_idx], -2, &ntwov2);
  dl_point3_mul_scalar(&v[b_idx], 2, &twov1);

  /* a = v[b_idx]; */
  dl_point3_mul_scalar(&v0, t, &b);
  dl_point3_mul_scalar(dl_point3_sub(dl_point3_add(dl_point3_sub(dl_point3_negate(&v1, &c), &twov0, &c), &threev2, &c), &threev1, &c), t2, &c);
  dl_point3_mul_scalar(dl_point3_add(&v1, dl_point3_add(&v0, dl_point3_add(&twov1, &ntwov2, &d), &d), &d), t3, &d);

  return dl_point3_add(&v[b_idx], dl_point3_add(&b, dl_point3_add(&c, &d, &v0), &v0), out);
}



dl_api dl_integer *dl_lerp_integer(dl_integer a, dl_integer b, dl_real p, dl_integer *out) {
  if (dl_safety(out == NULL))
    return NULL;
  
  *out = b - a;
  *out = (dl_integer)((dl_real)*out * p);
  *out += a;

  return out;
}

dl_api dl_real *dl_lerp_real(dl_real a, dl_real b, dl_real p, dl_real *out) {
  if (dl_safety(out == NULL))
    return NULL;

  *out = b - a;
  *out *= p;
  *out += a;

  return out;
}

dl_api dl_point2 *dl_lerp_point2(const dl_point2 *dl_restrict a, const dl_point2 *dl_restrict b, dl_real p, dl_point2 *dl_restrict out) {
  return dl_point2_add(dl_point2_mul_scalar(dl_point2_sub(b, a, out), p, out), a, out);
}

dl_api dl_point3 *dl_lerp_point3(const dl_point3 *dl_restrict a, const dl_point3 *dl_restrict b, dl_real p, dl_point3 *dl_restrict out) {
  return dl_point3_add(dl_point3_mul_scalar(dl_point3_sub(b, a, out), p, out), a, out);
}

dl_api dl_vec2 *dl_lerp_vec2(const dl_vec2 *dl_restrict a, const dl_vec2 *dl_restrict b, dl_real p, dl_vec2 *dl_restrict out) {
  return dl_vec2_add(dl_vec2_mul_scalar(dl_vec2_sub(b, a, out), p, out), a, out);
}

dl_api dl_vec3 *dl_lerp_vec3(const dl_vec3 *dl_restrict a, const dl_vec3 *dl_restrict b, dl_real p, dl_vec3 *dl_restrict out) {
  return dl_vec3_add(dl_vec3_mul_scalar(dl_vec3_sub(b, a, out), p, out), a, out);
}

#endif /* DL_USE_TWEEN */
  


/*****************************************************************************
 **  Tools
 ****************************************************************************/

#if DL_USE_MALLOC
#if DL_IS_GNUC
dl_any _default_alloc(dl_natural count, dl_natural element_size) {
  return (dl_any)memalign(sizeof(dl_any), count * element_size);
}
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  _default_alloc,				\
  (void(*)(dl_any))free
#else
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  (dl_any (*)(dl_natural, dl_natural))calloc,		\
  (void (*)(dl_any))free
#endif
#else
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  (dl_any (*)(dl_natural, dl_natural))NULL,		\
  (void (*)(dl_any))NULL
#endif

dl_api dl_any dl_memory_swap(dl_any left, dl_any right, dl_natural dl_bytes) {

  dl_natural nat_count, byte_count, *nat_left, *nat_right, nat_temp;
  dl_byte *byte_left, *byte_right, byte_temp;
  
  nat_count = dl_bytes / sizeof(dl_natural);
  byte_count = dl_bytes - (nat_count * sizeof(dl_natural));

  nat_left = (dl_natural *)left;
  nat_right = (dl_natural *)right;

  for (; nat_count > 0; --nat_count) {
    nat_temp = *nat_left;
    *nat_left = *nat_right;
    *nat_right = nat_temp;
    ++nat_left;
    ++nat_right;
  }

  byte_left = (dl_byte *)nat_left;
  byte_right = (dl_byte *)nat_right;

  for (; byte_count > 0; --byte_count) {
    byte_temp = *byte_left;
    *byte_left = *byte_right;
    *byte_right = byte_temp;
    ++byte_left;
    ++byte_right;
  }

  return left;
}

dl_api dl_any dl_memory_copy(dl_any left, dl_any right, dl_natural dl_bytes) {
  dl_natural nat_count, byte_count, *nat_left, *nat_right;
  dl_byte *byte_left, *byte_right;
  
  nat_count = dl_bytes / sizeof(dl_natural);
  byte_count = dl_bytes - (nat_count * sizeof(dl_natural));

  nat_left = (dl_natural *)left;
  nat_right = (dl_natural *)right;

  for (; nat_count > 0; --nat_count) {
    *nat_left = *nat_right;
    ++nat_left;
    ++nat_right;
  }

  byte_left = (dl_byte *)nat_left;
  byte_right = (dl_byte *)nat_right;

  for (; byte_count > 0; --byte_count) {
    *byte_left = *byte_right;
    ++byte_left;
    ++byte_right;
  }

  return left;
}

dl_any dl_memory_set(dl_any left, dl_byte val, dl_natural dl_bytes) {
  dl_natural *nat_left, nat_count, byte_count, nat_val, shift;
  dl_byte *byte_left;
  
  nat_left = (dl_natural *)left;
  nat_count = dl_bytes / sizeof(dl_natural);
  byte_count = dl_bytes - (nat_count * sizeof(dl_natural));

  nat_val = val;
  
  for (shift = 1; shift < sizeof(dl_natural); ++shift)
    nat_val |= (val << shift);

  for (; nat_count > 0; --nat_count) {
    *(dl_natural *)nat_left = nat_val;
    ++nat_left;
  }

  byte_left = (dl_byte *)nat_left;

  for (; byte_count > 0; --byte_count) {
    *(dl_byte *)byte_left = val;
    ++byte_left;
  }

  return left;
}



/*****************************************************************************
 **  Collections
 ****************************************************************************/

#if DL_USE_CONTAINERS

dl_integer _default_compare_8(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned char *)left - *(unsigned char *)right);
}

dl_integer _default_compare_16(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned int *)left - *(unsigned int *)right);
}

dl_integer _default_compare_32(dl_any data, dl_any left, dl_any right) {
  return (dl_integer)(*(unsigned long int *)left - *(unsigned long int *)right);
}

dl_integer _default_compare_64(dl_any data, dl_any left, dl_any right) {
#if DL_IS_ATLEAST_C99
  return (dl_integer)(*(unsigned long long int *)left - *(unsigned long long int *)right);
#else
  return 0;
#endif
}

dl_integer _default_compare_any(dl_any data, dl_any left, dl_any right) {
  return 0;
}

typedef struct {
  dl_comparator c;
  dl_any item;
} _collection_sorted_list_predicate_data;

dl_integer _collection_sorted_list_predicate_func(dl_any data, dl_any value) {
  _collection_sorted_list_predicate_data *d = (_collection_sorted_list_predicate_data *)data;
  return d->c.func(d->c.data, value, d->item);
}

/*****************************************************************************
 **  Vectors
 ****************************************************************************/

dl_vector_settings default_vector_settings = {
  0, 32,
  DECLARE_ALLOC_MEMBERS(alloc, free)
};

dl_api dl_vector *dl_init_vector(dl_vector * dl_restrict target, dl_natural element_size, dl_natural capacity) {
  dl_vector_settings settings = default_vector_settings;
  settings.element_size = element_size;
  return dl_init_vector_custom(target, &settings, capacity);
}

dl_api dl_vector *dl_init_vector_custom(dl_vector * dl_restrict target, dl_vector_settings *dl_restrict settings, dl_natural capacity) {
  dl_real dl_real_count;
  dl_natural slice_count, idx;
  
  if (target == NULL || settings == NULL)
    return NULL;

  target->settings = *settings;
  target->slice_count = -1;
  target->data.array = 0;

  if (settings->alloc == NULL || settings->free == NULL)
    return NULL;

  if (settings->element_size < 1)
    return NULL;

  settings->slice_length = settings->slice_length < 1 ? default_vector_settings.slice_length : settings->slice_length;

  dl_real_count = ((dl_real)capacity / (dl_real)settings->slice_length);
  slice_count = (dl_natural)dl_real_count;
  if (dl_real_count > (dl_real)(dl_natural)dl_real_count)
    slice_count++;

  target->slice_count = slice_count < 1 ? 1 : slice_count;

  target->data.slices = (dl_byte **)target->settings.alloc(target->slice_count, sizeof(dl_byte *));
  if (dl_unlikely(target->data.slices == NULL))
    return NULL;

  for (idx = 0; idx < target->slice_count; ++idx) {
    target->data.slices[idx] = (dl_byte *)target->settings.alloc(target->settings.slice_length, target->settings.element_size);

    if (dl_unlikely(target->data.slices[idx] == NULL))
      break;
  }

  if (dl_unlikely(idx < slice_count)) {
    for (--idx; idx != 0; --idx)
      target->settings.free((dl_any)target->data.slices[idx]);
    target->settings.free((dl_any)target->data.slices);

    return NULL;
  }

  return target;
}

dl_api dl_vector *dl_init_vector_array(dl_vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count) {
  if (dl_safety(target == NULL || data == NULL))
    return NULL;

  target->settings.alloc = NULL;
  target->settings.free = NULL;
  target->settings.element_size = element_size;
  target->settings.slice_length = count;
  target->slice_count = 0;

  target->data.slices = (dl_byte **)data;

  return target;
}

dl_api void dl_destroy_vector(dl_vector * dl_restrict target, const dl_handler *deconstruct_entry) {
  dl_any entry;
  dl_natural slice_idx, idx;
  
  if (target == NULL || target->settings.free == NULL)
    return;

  if (target->data.slices != NULL) {
    for (slice_idx = 0; slice_idx < target->slice_count; ++slice_idx) {
      if (deconstruct_entry != NULL) {
        for (idx = 0; idx < target->settings.slice_length; ++idx) {
          entry = &target->data.slices[slice_idx][idx * target->settings.element_size];
	        deconstruct_entry->func(deconstruct_entry->data, entry);
        }
      }

      target->settings.free((dl_any)target->data.slices[slice_idx]);
    }

    target->settings.free((dl_any)target->data.slices);
  }
}

dl_api dl_natural dl_vector_capacity(const dl_vector * dl_restrict v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->slice_count > 0 ? v->slice_count * v->settings.slice_length : v->settings.slice_length;
}

dl_api dl_any dl_vector_get(const dl_vector * dl_restrict v, dl_natural index, dl_any out) {
  dl_natural slice, slice_index;
  dl_byte *target_slice;
  
  if (dl_safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (dl_unlikely(index >= v->settings.slice_length))
      return NULL;

    dl_memory_copy(out, (void *)&v->data.array[index * v->settings.element_size], v->settings.element_size);
  }
  else if (v->slice_count > 0) {
    slice = index / v->settings.slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    dl_memory_copy(out, (void *)&target_slice[slice_index * v->settings.element_size], v->settings.element_size);
  }
  else
    return NULL;

  return out;
}

dl_api const dl_any dl_vector_ref(const dl_vector * dl_restrict v, dl_natural index) {
  dl_natural slice, slice_index;
  dl_byte *target_slice;
  
  if (dl_safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (dl_unlikely(index >= v->settings.slice_length))
      return NULL;

    return &v->data.array[index * v->settings.element_size];
  }
  else if (v->slice_count > 0) {
    slice = index / v->settings.slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    return &target_slice[slice_index * v->settings.element_size];
  }
  else
    return NULL;
}

dl_api dl_any dl_vector_set(dl_vector * dl_restrict v, dl_natural index, dl_any value) {
  dl_natural base_index, slice, slice_index;
  dl_byte *source, *target;
  
  if (dl_safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (dl_unlikely(index >= v->settings.slice_length))
      return NULL;

    base_index = index * v->settings.element_size;
    source = (dl_byte *)value;
    target = v->data.array;

    return dl_memory_copy((void *)&target[base_index], (void *)source, v->settings.element_size);
  }
  else if (v->slice_count > 0) {
    slice = index / v->settings.slice_length;
    if (slice >= v->slice_count)
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    base_index = slice_index * v->settings.element_size;
    source = (dl_byte *)value;

    return dl_memory_copy((void *)&v->data.slices[slice][base_index], (void *)source, v->settings.element_size);
  }
  else
    return NULL;
}

dl_api dl_bool dl_vector_grow(dl_vector * dl_restrict v) {
  dl_byte **new_slices, *new_slice, **existing_slices;
  dl_natural idx;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->settings.free == NULL || v->settings.alloc == NULL)
    return false;

  new_slices = (dl_byte **)v->settings.alloc(v->slice_count + 1, sizeof(dl_byte *));
  if (dl_unlikely(new_slices == NULL))
    return false;

  new_slice = v->settings.alloc(v->settings.slice_length, v->settings.element_size);
  if (dl_unlikely(new_slice == NULL)) {
    v->settings.free((dl_any)new_slices);
    return false;
  }

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->data.slices[v->slice_count] = new_slice;

  v->settings.free((dl_any)existing_slices);

  v->slice_count++;

  return true;
}

dl_api dl_bool dl_vector_swap(dl_vector * dl_restrict v, dl_natural index1, dl_natural index2) {
  dl_any left, right;
  
  if (dl_safety(v == NULL))
    return false;

  if (index1 == index2)
    return true;

  left = dl_vector_ref(v, index1);
  if (dl_unlikely(left == NULL))
    return false;

  right = dl_vector_ref(v, index2);
  if (dl_unlikely(right == NULL))
    return false;

  dl_memory_swap(left, right, v->settings.element_size);

  return true;
}

dl_api dl_bool dl_vector_shrink(dl_vector * dl_restrict v, dl_handler *dl_restrict deconstruct_entry) {
  dl_byte **new_slices, **existing_slices;
  dl_natural idx;
  dl_any entry;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->slice_count <= 1 || v->settings.free == NULL || v->settings.alloc == NULL)
    return false;

  new_slices = (dl_byte **)v->settings.alloc(v->slice_count - 1, sizeof(dl_byte *));
  if (dl_unlikely(new_slices == NULL))
    return false;

  if (deconstruct_entry != NULL) {
    for (idx = 0; idx < v->settings.slice_length; ++idx) {
      entry = &v->data.slices[v->slice_count - 1][idx * v->settings.element_size];
      deconstruct_entry->func(deconstruct_entry->data, entry);
    }
  }

  v->settings.free((dl_any)v->data.slices[v->slice_count - 1]);

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count - 1; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->slice_count--;

  v->settings.free((dl_any)existing_slices);

  return true;
}

dl_api dl_bool dl_vector_resize(dl_vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry) {
  dl_natural current_capacity, new_slice_count, slice_idx, slice_count, item_idx;
  dl_real needed;
  dl_integer needed_count;
  dl_byte **new_slices, *slice;
  dl_any item;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->settings.free == NULL || v->settings.alloc == NULL || v->slice_count < 1 || minimum_capacity < 1)
    return false;

  current_capacity = dl_vector_capacity(v);
  needed = (dl_real)((dl_integer)minimum_capacity - (dl_integer)current_capacity) / (dl_real)v->settings.slice_length;

  needed_count = needed < 0 ? (dl_integer)needed : (dl_integer)needed + 1;

  if (dl_likely(needed_count != 0)) {
    new_slice_count = v->slice_count + needed_count;
    new_slices = (dl_byte **)v->settings.alloc(new_slice_count, sizeof(dl_byte **));
    if (dl_unlikely(new_slices == NULL))
      return false;

    slice_count = new_slice_count < v->slice_count ? new_slice_count : v->slice_count;

    for (slice_idx = 0; slice_idx < slice_count; ++slice_idx)
      new_slices[slice_idx] = v->data.slices[slice_idx];

    /* Shrinking */
    if (needed_count < 0) {
      for (; slice_idx < v->slice_count; ++slice_idx) {
        slice = v->data.slices[slice_idx];
        if (deconstruct_entry != NULL)
          for (item_idx = 0; item_idx < v->settings.slice_length; ++item_idx) {
            item = &slice[item_idx * v->settings.element_size];
	    deconstruct_entry->func(deconstruct_entry->data, item);
          }

        v->settings.free((dl_any)slice);
      }
    }
    /* Growing */
    else {
      for (; slice_idx < new_slice_count; ++slice_idx) {
        new_slices[slice_idx] = v->settings.alloc(v->settings.slice_length, v->settings.element_size);
        if (dl_unlikely(new_slices[slice_idx] == NULL))
          break;
      }
      if (dl_unlikely(slice_idx != new_slice_count)) {
        for (slice_idx--; slice_idx >= v->slice_count; --slice_idx)
          v->settings.free((dl_any)new_slices[slice_idx]);
        v->settings.free((dl_any)new_slices);
        return false;
      }
    }

    v->settings.free((dl_any)v->data.slices);
    v->data.slices = new_slices;
    v->slice_count = new_slice_count;
  }

  return true;
}

dl_api dl_natural dl_vector_copy_array(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *data, dl_natural count) {
  dl_vector source;
  if (dl_unlikely(!dl_init_vector_array(&source, (dl_byte *)data, target->settings.element_size, count)))
    return 0;

  return dl_vector_copy(target, target_offset_index, &source);
}

dl_api dl_natural dl_vector_copy(dl_vector * dl_restrict target, dl_natural target_offset_index, const dl_vector * dl_restrict original) {
  dl_natural original_capacity, element_size, target_length, original_length, target_remainder, original_remainder, total_remainder, target_slice_idx, original_slice_idx, min_remainder, count_to_copy;
  dl_byte *target_slice, *original_slice;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (dl_unlikely(original->settings.element_size != target->settings.element_size))
    return 0;

  /* target is too small */
  original_capacity = dl_vector_capacity(original);
  if (dl_unlikely(dl_vector_capacity(target) - target_offset_index < original_capacity))
    return 0;

  element_size = original->settings.element_size;

  target_length = target->settings.slice_length * element_size;
  original_length = original->settings.slice_length * element_size;

  target_remainder = target_length - ((target_offset_index % target->settings.slice_length) * element_size);
  original_remainder = original_length;

  total_remainder = original_capacity * element_size;

  target_slice_idx = target_offset_index / target->settings.slice_length;
  original_slice_idx = 0;

  target_slice = target->slice_count == 0 ? target->data.array : target->data.slices[target_slice_idx];
  original_slice = original->slice_count == 0 ? original->data.array : original->data.slices[0];

  while (total_remainder != 0) {
    if (target_remainder == 0) {
      ++target_slice_idx;
      target_slice = target->data.slices[target_slice_idx];
      target_remainder = target_length;
    }
    if (original_remainder == 0) {
      ++original_slice_idx;
      original_slice = original->data.slices[original_slice_idx];
      original_remainder = original_length;
    }

    min_remainder = target_remainder < original_remainder ? target_remainder : original_remainder;
    count_to_copy = total_remainder < min_remainder ? total_remainder : min_remainder;

    dl_memory_copy(&target_slice[target_length - target_remainder], &original_slice[original_length - original_remainder], count_to_copy);

    total_remainder -= count_to_copy;
    target_remainder -= count_to_copy;
    original_remainder -= count_to_copy;
  }

  return original_capacity;
}

dl_api dl_natural dl_vector_ref_array(dl_vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out) {
  dl_natural last_idx, slice, slice_index;
  
  if (dl_safety(v == NULL))
    return 0;

  last_idx = v->settings.slice_length - 1;

  if (v->slice_count == 0 && index < last_idx) {
    *((dl_byte **)out) = &v->data.array[index];
    return last_idx - index;
  }
  else {
    slice = index / v->settings.slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return 0;

    slice_index = index - (slice * v->settings.slice_length);
    *((dl_byte **)out) = &v->data.slices[slice][slice_index];
    return last_idx - slice_index;
  }
}



/*****************************************************************************
 **  Linked Lists
 ****************************************************************************/

dl_linked_list_settings default_linked_list_settings = {
  32, 32,
  DECLARE_ALLOC_MEMBERS(alloc, free)
};

dl_api struct dl_linked_list_node *_linked_list_node_alloc(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict after) {
  struct dl_linked_list_node *node;
  
  if (list->free == NULL)
    return NULL;

  node = list->free;
  list->free = node->next;
  if (list->free != NULL)
    list->free->previous = NULL;

  if (after == NULL) {
    node->previous = NULL;
    if (list->first != NULL) {
      node->next = list->first;
      node->next->previous = node;
      list->first = node;
    }
    else {
      list->first = list->last = node;
      node->next = NULL;
    }
  }
  else {
    node->previous = after;
    node->next = after->next;
    after->next = node;
    if (node->next == NULL)
      list->last = node;
    else
      node->next->previous = node;
  }

  return node;
}

dl_api void _linked_list_node_free(dl_linked_list * dl_restrict list, struct dl_linked_list_node * dl_restrict node) {
  if (list->first == node)
    list->first = node->next;
  if (node->next != NULL)
    node->next->previous = node->previous;
  if (node->previous != NULL)
    node->previous->next = node->next;
  if (list->last == node)
    list->last = node->previous;

  node->next = list->free;
  if (node->next != NULL)
    node->next->previous = node;
  node->previous = NULL;
  list->free = node;
}

dl_api void _linked_list_node_detach_free(dl_linked_list *dl_restrict list, struct dl_linked_list_node *e) {
  if (list->free == e)
    list->free = e->next;
  if (e->next != NULL)
    e->next->previous = e->previous;
  if (e->previous != NULL)
    e->previous->next = e->next;
  e->next = e->previous = NULL;
}

typedef struct {
  dl_handler *original_destructor;
  dl_linked_list *list;
} _linked_list_node_deconstructor_data;

dl_api dl_any _linked_list_node_deconstructor(dl_any data, dl_any element) {
  _linked_list_node_deconstructor_data *d;
  struct dl_linked_list_node *f, *e, *new_node;
  dl_handler *destruct;

  d = (_linked_list_node_deconstructor_data *)data;
  e = (struct dl_linked_list_node *)element;

  /* Is it in the free list? */
  for (f = d->list->free; f != NULL; f = f->next)
    if (f == e) {
      _linked_list_node_detach_free(d->list, e);
      return NULL;
    }

  /* Swap from free list */
  new_node = _linked_list_node_alloc(d->list, e->previous);
  if (!new_node) {
    /* No free nodes, destroy it. */
    if (d->original_destructor != NULL && d->original_destructor->func != NULL) {
      destruct = d->original_destructor;
      destruct->func(destruct->data, DL_LINKED_LIST_DATA(e));
    }
  }
  else
    dl_linked_list_swap(d->list, e, new_node, true);

  _linked_list_node_free(d->list, e);
  _linked_list_node_detach_free(d->list, e);

  return NULL;
}

dl_api dl_linked_list *dl_init_linked_list(dl_linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length) {
  dl_linked_list_settings settings;
  
  if (dl_safety(target == NULL || element_size == 0))
    return false;

  settings = default_linked_list_settings;
  settings.element_size = element_size;
  settings.cache_length = cache_length;

  return dl_init_linked_list_custom(target, &settings);
}

dl_api dl_linked_list *_linked_list_cache_grow(dl_linked_list * dl_restrict target) {
  dl_linked_list_settings settings;
  dl_vector *v;
  dl_natural zero, length, idx;
  struct dl_linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return NULL;

  settings = target->settings;

  if (settings.cache_length == 0)
    return NULL;

  v = &target->node_cache;

  if (target->free == NULL && target->first == NULL) {
    zero = 0;
    if (!dl_init_vector(v, DL_LINKED_LIST_HEADER_SIZE + settings.element_size, settings.cache_length))
      return NULL;
  }
  else {
    zero = dl_vector_capacity(v);
    if (!dl_vector_grow(v))
      return NULL;
  }

  length = dl_vector_capacity(v);

  for (idx = zero; idx < length; ++idx) {
    node = (struct dl_linked_list_node *)dl_vector_ref(v, idx);
    dl_memory_set(node, 0, DL_LINKED_LIST_HEADER_SIZE);
    _linked_list_node_free(target, node);
  }

  return target;
}

dl_api dl_linked_list *dl_init_linked_list_custom(dl_linked_list * dl_restrict target, dl_linked_list_settings *dl_restrict settings) {
  if (dl_safety(target == NULL || settings.element_size < 1))
    return NULL;

  target->first = target->last = target->free = NULL;
  target->settings = *settings;

  return _linked_list_cache_grow(target);
}

dl_api dl_natural dl_linked_list_copy(dl_linked_list * dl_restrict target, struct dl_linked_list_node *dl_restrict target_position, const dl_linked_list * dl_restrict original) {
  struct dl_linked_list_node *source_node, *next;
  dl_natural count;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  source_node = original->first;
  count = 0;

  while (source_node != NULL) {
    next = dl_linked_list_add(target, target_position, DL_LINKED_LIST_DATA(source_node));
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    source_node = source_node->next;
    ++count;
  }

  return count;
}

dl_api dl_natural dl_linked_list_copy_array(dl_linked_list * dl_restrict target, struct dl_linked_list_node *target_position, const dl_byte *data, dl_natural length) {
  dl_natural count, byte_size, idx;
  struct dl_linked_list_node *next;
  
  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  count = 0;
  byte_size = target->settings.element_size;

  for (idx = 0; idx < length; ++idx) {
    next = dl_linked_list_add(target, target_position, (dl_byte *)&data[idx * byte_size]);
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    ++count;
  }

  return count;
}

dl_api void dl_destroy_linked_list(dl_linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    node = target->first;
    while (node != NULL) {
      deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(node));
      node = node->previous;
    }
  }

  dl_destroy_vector(&target->node_cache, NULL);

  dl_memory_set(target, 0, sizeof(dl_linked_list));
}

dl_api dl_natural dl_linked_list_capacity(const dl_linked_list * dl_restrict list) {
  if (dl_safety(list == NULL))
    return 0;

  return dl_vector_capacity(&list->node_cache);
}

dl_api dl_natural dl_linked_list_length(const dl_linked_list * dl_restrict list) {
  dl_natural length;
  struct dl_linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return 0;

  length = 0;
  node = list->first;
  while (node != NULL) {
    node = node->next;
    ++length;
  }

  return length;
}

dl_api dl_bool dl_linked_list_grow(dl_linked_list * dl_restrict list) {
  return _linked_list_cache_grow(list) != NULL;
}

dl_api dl_bool dl_linked_list_shrink(dl_linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;

  return dl_vector_shrink(&list->node_cache, &destructor);
}

dl_api dl_bool dl_linked_list_resize(dl_linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;
  
  return dl_vector_resize(&list->node_cache, minimum_capacity, &destructor);
}

dl_api dl_any dl_linked_list_get(const dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->settings.element_size);
}

dl_api const dl_any dl_linked_list_ref(const struct dl_linked_list_node *position) {
  if (dl_safety(position == NULL))
    return NULL;

  return (const dl_any)DL_LINKED_LIST_DATA(position);
}

dl_api dl_any dl_linked_list_set(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any value) {
  if (dl_safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return dl_memory_copy(DL_LINKED_LIST_DATA(position), value, list->settings.element_size);
}

dl_api struct dl_linked_list_node *dl_linked_list_add(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any value) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return NULL;

  if (list->free == NULL)
    return NULL;

  node = _linked_list_node_alloc(list, position);
  if (dl_unlikely(value != NULL && !dl_memory_copy(DL_LINKED_LIST_DATA(node), value, list->settings.element_size))) {
    _linked_list_node_free(list, node);
    return NULL;
  }

  return node;
}

dl_api dl_any dl_linked_list_remove(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL))
    return NULL;

  if (dl_unlikely(!dl_memory_copy(out, DL_LINKED_LIST_DATA(position), list->settings.element_size)))
    return NULL;

  _linked_list_node_free(list, position);

  return out;
}

dl_api dl_bool dl_linked_list_destroy(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_handler *deconstruct_entry) {
  if (dl_safety(list == NULL || position == NULL))
    return false;
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, DL_LINKED_LIST_DATA(position));

  _linked_list_node_free(list, position);

  return true;
}

dl_api dl_natural dl_linked_list_destroy_range(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position, dl_natural count, dl_handler *deconstruct_entry) {
  dl_natural removed;
  struct dl_linked_list_node *next;
  
  if (dl_safety(list == NULL || position == NULL || count == 0))
    return 0;

  removed = 0;
  while (removed < count) {
    next = position->next;
    if (!dl_linked_list_destroy(list, position, deconstruct_entry))
      break;

    position = next;
    ++removed;
  }

  return removed;
}

dl_api dl_bool dl_linked_list_swap(dl_linked_list * dl_restrict list, struct dl_linked_list_node *position1, struct dl_linked_list_node *position2, dl_bool data) {
  struct dl_linked_list_node *t;
  
  if (dl_safety(list == NULL || position1 == NULL || position2 == NULL))
    return false;

  /* It's complicated because it is a double-linked list, and we have to be careful not to create a cycle. */
  if (position1 == position2->previous) {
    if (position1->previous != NULL)
      position1->previous->next = position2;
    position2->previous = position1->previous;
    position1->previous = position2;
    position1->next = position2->next;
    position2->next = position1;
    if (position1->next != NULL)
      position1->next->previous = position1;
  }
  else if (position2 == position1->previous) {
    if (position2->previous != NULL)
      position2->previous->next = position1;
    position1->previous = position2->previous;
    position2->previous = position1;
    position2->next = position1->next;
    position1->next = position2;
    if (position2->next != NULL)
      position2->next->previous = position2;
  }
  else {
    if (position1->previous != NULL)
      position1->previous->next = position2;
    if (position1->next != NULL)
      position1->next->previous = position2;
    if (position2->previous != NULL)
      position2->previous->next = position1;
    if (position2->next != NULL)
      position2->next->previous = position1;

    t = position1->next;
    position1->next = position2->next;
    position2->next = t;

    t = position1->previous;
    position1->previous = position2->previous;
    position2->previous = t;
  }

  if (list->first == position1)
    list->first = position2;
  else if (list->first == position2)
    list->first = position1;
  if (list->last == position1)
    list->last = position2;
  else if (list->last == position2)
    list->last = position1;

  if (data)
    dl_memory_swap(DL_LINKED_LIST_DATA(position1), DL_LINKED_LIST_DATA(position2), list->settings.element_size);


  return true;
}

dl_api struct dl_linked_list_node *dl_linked_list_index(dl_linked_list * dl_restrict list, dl_natural position) {
  struct dl_linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return NULL;

  node = list->first;
  while (node != NULL && position > 0) {
    node = node->next;
    --position;
  }

  if (position == 0)
    return node;
  return NULL;
}



/*****************************************************************************
 **  Iterators
 ****************************************************************************/

dl_api dl_integer dl_iterator_compare(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right) {
  if (!dl_iterator_is_valid(col, left) && !dl_iterator_is_valid(col, right))
    return 0;

  if (dl_iterator_is_valid(col, left) && !dl_iterator_is_valid(col, right))
    return 1;

  if (!dl_iterator_is_valid(col, left) && dl_iterator_is_valid(col, right))
    return -1;

  return col->settings.functions->_dl_iterator_compare(col, left, right);
}

dl_api dl_bool dl_iterator_equal(const dl_collection *dl_restrict col, const dl_iterator left, const dl_iterator right) {
  return 0 == dl_iterator_compare(col, left, right);
}

dl_api dl_bool dl_iterator_is_valid(const dl_collection *dl_restrict col, const dl_iterator index) {
  if (dl_safety(col == NULL))
    return false;

  return col->settings.functions->_dl_iterator_is_valid(col, index);
}

dl_api dl_iterator dl_make_invalid_dl_iterator(const dl_collection *dl_restrict col) {
  return col->settings.functions->_dl_make_invalid_dl_iterator(col);
}



/*****************************************************************************
 **  Container Dependent
 ****************************************************************************/

dl_api void _force_collection_properties(dl_collection *dl_restrict col) {
  dl_iterator current, next;
  dl_any ref;
  
  if (dl_collection_is_sorted(col))
    dl_collection_sort(col);
  if (dl_collection_is_set(col)) {
    for (ref = dl_collection_begin_ref(col, &current); ref != NULL; ref = dl_collection_next(col, &current)) {
      next = current;
      if (!dl_collection_next(col, &next))
        break;
      while (dl_iterator_is_valid(col, next) && 0 == col->settings.comparer.func(col->settings.comparer.data, ref, dl_collection_ref(col, next)))
        dl_collection_destroy_at(col, &next);
    }
  }
}

dl_api dl_any dl_collection_push_start(dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(col == NULL))
    return NULL;

  return col->settings.functions->_collection_push_start(col, iter);
}

dl_api dl_any dl_collection_push_finish(dl_collection *dl_restrict col, dl_iterator *iter) {
  dl_comparator *comp;
  dl_bool is_set;
  dl_any ref, prev_ref;
  dl_iterator prev;
  dl_integer c;

  if (dl_safety(col == NULL || iter == NULL || !dl_iterator_is_valid(col, *iter)))
    return NULL;

  if (dl_collection_is_sorted(col)) {
    comp = &col->settings.comparer;
    is_set = dl_collection_is_set(col);

    while (dl_iterator_is_valid(col, *iter)) {
      ref = dl_collection_ref(col, *iter);
      if (ref == NULL)
        break;

      prev = *iter;
      prev_ref = dl_collection_prev(col, &prev);

      if (prev_ref == NULL)
        break;

      c = comp->func(comp->data, prev_ref, ref);

      if (c < 0)
        break;

      if (is_set && c == 0) {
        dl_collection_destroy_at(col, iter);
        return NULL;
      }

      dl_collection_swap(col, &prev, iter);
    }
  }

  return dl_collection_ref(col, *iter);
}

dl_api void _vector_queue_roll_slices(dl_collection *dl_restrict col) {
  dl_byte *slice_left, *slice_first;
  dl_natural last_idx, slice_idx;
  
  if (col->data.dl_vector.index[0] == col->data.dl_vector.index[1]) {
    col->data.dl_vector.index[0] = col->data.dl_vector.index[1] = 0;
    return;
  }

  if (col->data.dl_vector.container.slice_count <= 1)
    return;

  if (col->data.dl_vector.index[0] < col->data.dl_vector.container.settings.slice_length)
    return;

  slice_left = col->data.dl_vector.container.data.slices[0];
  slice_first = slice_left;

  last_idx = col->data.dl_vector.container.slice_count - 1;

  for (slice_idx = 0; slice_idx < last_idx; ++slice_idx)
    col->data.dl_vector.container.data.slices[slice_idx] = col->data.dl_vector.container.data.slices[slice_idx + 1];

  col->data.dl_vector.container.data.slices[last_idx] = slice_first;

  col->data.dl_vector.index[0] -= col->data.dl_vector.container.settings.slice_length;
  col->data.dl_vector.index[1] -= col->data.dl_vector.container.settings.slice_length;
}

dl_api dl_bool dl_collection_is_empty(const dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return true;

  return col->settings.functions->_collection_is_empty(col);
}

dl_api dl_any dl_collection_pop(dl_collection *dl_restrict col, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_pop(col, out);
}

dl_api dl_bool dl_collection_pop_destroy(dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_destroy(col);
}

dl_api dl_bool dl_collection_pop_forget(dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_forget(col);
}

dl_api dl_integer dl_collection_ref_array(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array) {  
  if (dl_safety(col == NULL || out_array == NULL))
    return 0;

  if (!dl_iterator_is_valid(col, iter))
    return 0;

  return col->settings.functions->_collection_ref_array(col, iter, out_array);
}

dl_api const dl_any dl_collection_ref(const dl_collection *dl_restrict col, dl_iterator iter) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_ref((dl_collection *dl_restrict )col, iter);
}

dl_api dl_bool dl_collection_swap(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b) {
  if (dl_safety(col == NULL || iter_a == NULL || iter_b == NULL))
    return false;

  if (!dl_iterator_is_valid(col, *iter_a) || !dl_iterator_is_valid(col, *iter_b))
    return false;

  return col->settings.functions->_collection_swap(col, iter_a, iter_b);
}

dl_any dl_collection_get(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_get(col, iter, out);
}

dl_any dl_collection_set(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!dl_iterator_is_valid(col, *iter))
    return NULL;

  if (dl_collection_is_set(col) && dl_collection_contains(col, value))
    return NULL;

  if (dl_collection_is_sorted(col)) {
    dl_collection_forget_at(col, iter);
    return dl_collection_push_index(col, value, iter);
  }

  return col->settings.functions->_collection_set(col, iter, value);
}

dl_api dl_iterator dl_collection_index(dl_collection *dl_restrict col, dl_natural index) {
  if (dl_safety(col == NULL || (dl_integer)index >= dl_collection_count(col)))
    return dl_collection_end(col);

  return col->settings.functions->_collection_index(col, index);
}

dl_api const dl_any dl_collection_next(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL)) {
    *iter = dl_make_invalid_dl_iterator(col);
    return NULL;
  }

  if (!dl_iterator_is_valid(col, *iter)) {
    *iter = dl_make_invalid_dl_iterator(col);
    return NULL;
  }

  col->settings.functions->_collection_next(col, iter);

  return dl_collection_ref(col, *iter);
}

dl_api const dl_any dl_collection_prev(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  col->settings.functions->_collection_prev(col, iter);
  
  return dl_collection_ref(col, *iter);
}

dl_api dl_integer dl_collection_count(const dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return 0;

  return col->settings.functions->_collection_count(col);
}

dl_api dl_iterator dl_collection_begin(const dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return dl_make_invalid_dl_iterator(col);

  return col->settings.functions->_collection_begin(col);
}

dl_api dl_iterator dl_collection_end(const dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return dl_make_invalid_dl_iterator(col);

  return col->settings.functions->_collection_end(col);
}

const dl_any _collection_search_region_vector(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  dl_iterator iter_next;
  
  if (left.dl_vector.index >= right.dl_vector.index)
    return NULL;

  iter->dl_vector.index = ((right.dl_vector.index - left.dl_vector.index) >> 1) + left.dl_vector.index;

  if (dl_unlikely(!(ref = dl_vector_ref(&col->data.dl_vector.container, iter->dl_vector.index))))
    return NULL;

  outcome = predicate->func(predicate->data, ref);
  
  if (outcome == 0)
    return ref;
  else if (outcome > 0)
    return _collection_search_region_vector(col, predicate, left, *iter, iter);
  else {
    iter_next.dl_vector.index = iter->dl_vector.index + 1;
    return _collection_search_region_vector(col, predicate, iter_next, right, iter);
  }
}

const dl_any _collection_linear_search(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  
  ref = dl_collection_ref(col, left);
  *iter = left;
  while (!dl_iterator_equal(col, *iter, right) && ref != NULL) {
    outcome = predicate->func(predicate->data, ref);
    if (outcome == 0)
      return ref;
    ref = dl_collection_next(col, iter);
  }

  return NULL;
}

const dl_any dl_collection_search_region(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  if (dl_safety(col == NULL || predicate == NULL || iter == NULL))
    return NULL;

  return col->settings.functions->_collection_search_region(col, predicate, left, right, iter);
}

const dl_any dl_collection_search(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator *iter) {
  return dl_collection_search_region(col, predicate, dl_collection_begin(col), dl_collection_end(col), iter);
}

dl_api dl_bool dl_collection_destroy_at(dl_collection *dl_restrict col, dl_iterator *iter) {
  dl_handler *destructor;
  
  if (dl_safety(col == NULL || !dl_iterator_is_valid(col, *iter)))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  destructor = col->settings.deconstruct_entry.func == NULL ? NULL : &col->settings.deconstruct_entry;

  return col->settings.functions->_collection_destroy_at(col, iter, destructor);
}

dl_api dl_bool dl_collection_forget_at(dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(col == NULL))
    return false;

  if (dl_collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_destroy_at(col, iter, NULL);
}

dl_api dl_any dl_collection_remove_at(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_remove_at(col, iter, out);
}

dl_api dl_integer dl_collection_destroy_range(dl_collection *dl_restrict col, dl_iterator *index, dl_natural count) {
  if (dl_safety(col == NULL || !dl_iterator_is_valid(col, *index)))
    return 0;

  return col->settings.functions->_collection_destroy_range(col, index, count);
}

dl_any dl_collection_insert(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item) {
  if (dl_safety(col == NULL || !dl_iterator_is_valid(col, *position)))
    return NULL;

  if (dl_collection_is_set(col) && dl_collection_contains(col, item))
    return NULL;

  if (dl_collection_is_sorted(col))
    return dl_collection_push_index(col, item, position);

  if (col->settings.functions->_collection_insert(col, position, item))
    return dl_collection_ref(col, *position);
  return NULL;
}

dl_api dl_natural dl_collection_element_size(const dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return 0;

  return col->settings.element_size;
}



/*****************************************************************************
 **  Container Independent
 ****************************************************************************/

dl_api dl_bool dl_collection_is_vector(const dl_collection * dl_restrict col) {
  return col != NULL && col->settings.storage == DL_STORAGE_TYPE_VECTOR;
}

dl_api dl_bool dl_collection_is_linked_list(const dl_collection * dl_restrict col) {
  return col != NULL && col->settings.storage == DL_STORAGE_TYPE_LINKED_LIST;
}

dl_api dl_bool dl_collection_is_queue(const dl_collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_QUEUE);
}

dl_api dl_bool dl_collection_is_set(const dl_collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_SET);
}

dl_api dl_bool dl_collection_is_sorted(const dl_collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == DL_COLLECTION_TYPE_SORTED_LIST
     || col->settings.type == DL_COLLECTION_TYPE_SET);
}

dl_api const dl_any dl_collection_begin_ref(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = dl_collection_begin(col);
  return dl_collection_ref(col, *iter);
}

dl_api const dl_any dl_collection_end_ref(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = dl_collection_end(col);
  return dl_collection_prev(col, iter);
}

dl_api dl_bool dl_collection_sort(dl_collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  return dl_collection_quick_sort(col, &col->settings.comparer);
}

dl_api void dl_collection_clear(dl_collection *dl_restrict col) {
  while (dl_collection_pop_destroy(col));
}

dl_api const dl_any dl_collection_peek(const dl_collection *dl_restrict col) {
  dl_iterator iter;
  
  if (dl_safety(col == NULL))
    return NULL;

  if (dl_collection_is_empty(col))
    return NULL;

  if (dl_collection_is_queue(col)) {
    iter = dl_collection_begin(col);

    return dl_collection_ref(col, iter);
  }
  else {
    iter = dl_collection_end(col);
    return dl_collection_prev(col, &iter);
  }
}

dl_api dl_iterator dl_collection_index_of(const dl_collection *dl_restrict col, const dl_any item) {
  dl_iterator bad, iter, index;
  dl_any candidate;
  _collection_sorted_list_predicate_data data;
  dl_filter predicate;
  
  bad = dl_make_invalid_dl_iterator(col);

  if (dl_safety(col == NULL))
    return bad;

  if (dl_collection_is_empty(col))
    return bad;

  if (dl_collection_is_sorted(col)) {
    data.c = col->settings.comparer;
    data.item = item;

    predicate.func = _collection_sorted_list_predicate_func;
    predicate.data = &data;

    iter = dl_collection_begin(col);
    return NULL != dl_collection_search(col, &predicate, &iter) ? iter : bad;
  }
  else {
    index = dl_collection_begin(col);
    for (candidate = dl_collection_ref(col, index); candidate != NULL; candidate = dl_collection_next(col, &index)) {
      if (0 == col->settings.comparer.func(col->settings.comparer.data, item, candidate))
        return index;
    }
    return bad;
  }
}

dl_api dl_bool dl_collection_all(const dl_collection *dl_restrict col, dl_filter *dl_restrict f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
    if (!f->func(f->data, item))
      return false;

  return true;
}

dl_api dl_bool dl_collection_any(const dl_collection *dl_restrict col, dl_filter *dl_restrict f) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index);
       item != NULL;
       item = dl_collection_next(col, &index))
    if (f->func(f->data, item))
      return true;

  return false;
}

dl_api dl_integer dl_collection_drop(dl_collection *dl_restrict col, dl_natural count) {
  dl_natural removed = 0;

  if (dl_safety(col == NULL))
    return 0;

  for (; count > 0; --count, ++removed)
    if (!dl_collection_pop_destroy(col))
      return removed;

  return removed;
}

dl_api dl_integer dl_collection_map(const dl_collection *dl_restrict col, dl_converter *func, dl_collection *out) {
  dl_iterator iter, new_iter;
  dl_integer count;
  dl_any new_ref;
  dl_any ref;
  
  if (dl_safety(col == NULL || func == NULL || out == NULL))
    return 0;

  count = 0;
  for (ref = dl_collection_begin_ref(col, &iter); ref != NULL; ref = dl_collection_next(col, &iter)) {
    new_ref = dl_collection_push_start(out, &new_iter);
    if (!dl_iterator_is_valid(out, new_iter))
      return -count;

    func->func(func->data, ref, new_ref);

    dl_collection_push_finish(out, &new_iter);
    ++count;
  }

  return count;
}

dl_api dl_any dl_collection_foldl(const dl_collection *dl_restrict col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_any dl_collection_foldr(const dl_collection *dl_restrict col, dl_any initial, dl_folder *func) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = dl_collection_end(col);

  while ((item = dl_collection_prev(col, &index)))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_integer dl_collection_zip(const dl_collection *dl_restrict col1, const dl_collection *dl_restrict col2, dl_zipper *zip, dl_collection *out) {
  dl_iterator iter1, iter2, new_iter;
  dl_integer added;
  dl_any ref1, ref2, new_ref;
  
  if (dl_safety(col1 == NULL || col2 == NULL || zip == NULL || out == NULL))
    return 0;

  added = 0;
  for (ref1 = dl_collection_begin_ref(col1, &iter1), ref2 = dl_collection_begin_ref(col2, &iter2);
       ref1 != NULL && ref2 != NULL;
       ref1 = dl_collection_next(col1, &iter1), ref2 = dl_collection_next(col2, &iter2)) {
    new_ref = dl_collection_push_start(out, &new_iter);
    if (!dl_iterator_is_valid(out, new_iter))
      return -added;

    zip->func(zip->data, ref1, ref2, new_ref);

    dl_collection_push_finish(out, &new_iter);
    ++added;
  }

  return added;
}

dl_api dl_integer dl_collection_take(dl_collection *dl_restrict col, dl_natural count, dl_collection *out) {
  dl_any ref;
  dl_natural added;
  if (dl_safety(col == NULL || out == NULL))
    return 0;

  added = 0;
  while ((ref = dl_collection_peek(col)) && count > added) {
    if (!(ref = dl_collection_push(out, ref)))
      break;
    dl_collection_pop_forget(col);
    ++added;
  }

  return added;
}

dl_api dl_any dl_collection_find(const dl_collection *dl_restrict col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  for (item = dl_collection_ref(col, *index); item != NULL; item = dl_collection_next(col, index))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_any dl_collection_find_last(const dl_collection *dl_restrict col, dl_filter *f, dl_iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  while ((item = dl_collection_prev(col, index)))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_integer dl_collection_find_all(const dl_collection *dl_restrict col, dl_filter *f, dl_collection *out) {
  dl_natural start_count;
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || out == NULL))
    return 0;

  start_count = dl_collection_count(out);

  index = dl_collection_begin(col);
  for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index)) {
    if (f->func(f->data, item) && !dl_collection_push(out, item))
      break;
  }

  return dl_collection_count(out) - start_count;
}

dl_api dl_any dl_collection_remove_first(dl_collection *dl_restrict col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_collection_find(col, f, iter))
    return NULL;

  return dl_collection_remove_at(col, iter, out);
}

dl_api dl_any dl_collection_remove_last(dl_collection *dl_restrict col, dl_filter *f, dl_iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!dl_collection_find_last(col, f, iter))
    return NULL;

  return dl_collection_remove_at(col, iter, out);
}

dl_api dl_bool dl_collection_destroy_first(dl_collection *dl_restrict col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_collection_find(col, f, index))
    return false;

  return dl_collection_destroy_at(col, index);
}

dl_api dl_bool dl_collection_destroy_last(dl_collection *dl_restrict col, dl_filter *f, dl_iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!dl_collection_find_last(col, f, index))
    return false;

  return dl_collection_destroy_at(col, index);
}

dl_bool dl_collection_quick_sort_region(dl_collection *dl_restrict col, dl_comparator *compare, dl_iterator left, dl_iterator right) {
  dl_iterator pivot, leftwall, iter;
  dl_any pivot_ref, iter_ref;
  
  if (dl_safety(col == NULL || col->settings.comparer.func == NULL))
    return false;

  if (dl_iterator_equal(col, left, right)
      || (!dl_iterator_is_valid(col, left)
	  && !dl_iterator_is_valid(col, right)))
    return true;

  /* Partition */

  pivot = left;
  leftwall = left;
  iter = left;

  pivot_ref = dl_collection_ref(col, pivot);

  for (iter_ref = dl_collection_next(col, &iter);
       iter_ref != NULL && !dl_iterator_equal(col, iter, right);
       iter_ref = dl_collection_next(col, &iter)) {
    if (compare->func(compare->data, iter_ref, pivot_ref) < 0) {
      if (!dl_collection_swap(col, &iter, &leftwall))
        return false;
      if (dl_iterator_equal(col, pivot, leftwall)) {
        pivot = iter;
        pivot_ref = dl_collection_ref(col, pivot);
      }
      if (!dl_collection_next(col, &leftwall))
        return false;
    }
  }

  if (!dl_collection_swap(col, &pivot, &leftwall))
    return false;

  /* Recurse */

  if (!dl_collection_quick_sort_region(col, compare, left, leftwall))
    return false;

  if (!dl_collection_next(col, &leftwall))
    return true;

  return dl_collection_quick_sort_region(col, compare, leftwall, right);
}

dl_api dl_bool dl_collection_quick_sort(dl_collection *dl_restrict col, dl_comparator *compare) {
  return dl_collection_quick_sort_region(col, compare, dl_collection_begin(col), dl_collection_end(col));
}

dl_api dl_integer dl_collection_destroy_all(dl_collection *dl_restrict col, dl_filter *f) {
  dl_natural count;
  dl_any item;
  dl_iterator iter;
  
  if (dl_safety(col == NULL || f == NULL))
    return 0;

  if (dl_collection_is_empty(col))
    return 0;

  count = 0;
  iter = dl_collection_begin(col);

  while ((item = dl_collection_ref(col, iter))) {
    if (f->func(f->data, item)) {
      dl_collection_destroy_at(col, &iter);
      count++;
    }
    else
      dl_collection_next(col, &iter);
  }

  return count;
}

dl_api dl_bool dl_collection_contains(const dl_collection *dl_restrict col, dl_any item) {
  dl_iterator iter;
  iter= dl_collection_index_of(col, item);
  return dl_iterator_is_valid(col, iter);
}

dl_api dl_any dl_collection_push(dl_collection *dl_restrict col, dl_any value) {
  dl_iterator iter;
  return dl_collection_push_index(col, value, &iter);
}

dl_api dl_any dl_collection_push_index(dl_collection *dl_restrict col, dl_any value, dl_iterator *out_index) {
  dl_any ref;
  
  ref = dl_collection_push_start(col, out_index);
  if (!dl_iterator_is_valid(col, *out_index))
    return NULL;

  dl_memory_copy(ref, value, dl_collection_element_size(col));
  return dl_collection_push_finish(col, out_index);
}

dl_api dl_integer dl_collection_remove_range(dl_collection *dl_restrict col, dl_iterator *index, dl_natural count, dl_collection *out) {
  dl_natural total;
  dl_iterator new_iter;
  dl_any ref;
  
  if (dl_safety(col == NULL || out == NULL || index == NULL || !dl_iterator_is_valid(col, *index)))
    return 0;

  total = 0;
  while (total < count && dl_iterator_is_valid(col, *index)) {
    ref = dl_collection_push_start(out, &new_iter);
    if (!ref)
      break;

    if (!dl_collection_remove_at(col, index, ref)) {
      dl_collection_forget_at(col, &new_iter);
      break;
    }
    else
      dl_collection_push_finish(out, &new_iter);
    ++total;
  }

  return total;
}


dl_api dl_integer dl_collection_remove_all(dl_collection *dl_restrict col, dl_filter *f, dl_collection *out) {
  dl_iterator index, new_iter;
  dl_natural total;
  dl_any ref, new_ref;
  
  if (dl_safety(col == NULL || out == NULL || f == NULL))
    return 0;

  total = 0;
  for (ref = dl_collection_begin_ref(col, &index); dl_iterator_is_valid(col, index);) {
    if (f->func(f->data, ref)) {
      new_ref = dl_collection_push_start(out, &new_iter);
      if (!ref)
        break;

      if (!dl_collection_remove_at(col, &index, new_ref)) {
        dl_collection_forget_at(col, &new_iter);
        break;
      }
      else
        dl_collection_push_finish(out, &new_iter);

      ref = dl_collection_ref(col, index);
      ++total;
    }
    else
      ref = dl_collection_next(col, &index);
  }

  return total;
}

dl_api dl_integer dl_collection_copy(const dl_collection *dl_restrict original, dl_collection *dl_restrict target) {
  dl_iterator iter, new_iter;
  dl_integer count;
  dl_any ref, new_ref;
  dl_bool is_set;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  is_set = dl_collection_is_set(target);

  count = 0;
  for (ref = dl_collection_begin_ref(original, &iter); ref != NULL; ref = dl_collection_next(original, &iter)) {
    if (is_set && dl_collection_contains(target, ref))
      continue;

    new_ref = dl_collection_push_start(target, &new_iter);
    if (!new_ref) {
      count = -count;
      break;
    }

    ref = dl_collection_ref(original, iter);

    dl_memory_copy(new_ref, ref, dl_collection_element_size(original));
    dl_collection_push_finish(target, &new_iter);

    ++count;
  }

  return count;
}

dl_api dl_integer dl_collection_copy_array(const dl_any data, dl_natural count, dl_collection *dl_restrict target) {
  dl_collection source;

  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (dl_unlikely(!dl_init_collection_array(&source, DL_COLLECTION_TYPE_LIST, NULL, NULL, data, dl_collection_element_size(target), count)))
    return 0;

  return dl_collection_copy(&source, target);
}



/*****************************************************************************
 **  Container Dispatchers
 ****************************************************************************/

dl_api dl_integer _vector_dl_iterator_compare(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right) {
  return left.dl_vector.index - right.dl_vector.index;
}

dl_api dl_integer _linked_list_dl_iterator_compare(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right) {
  return (dl_integer)(left.dl_linked_list.node - right.dl_linked_list.node);
}

dl_api dl_bool _vector_dl_iterator_is_valid(const dl_collection *dl_restrict col, dl_iterator index) {
  return index.dl_vector.index < col->data.dl_vector.index[1]
    && index.dl_vector.index >= col->data.dl_vector.index[0];
}

dl_api dl_bool _linked_list_dl_iterator_is_valid(const dl_collection *dl_restrict col, dl_iterator index) {
  return index.dl_linked_list.node != NULL;
}

dl_api dl_iterator _vector_dl_make_invalid_dl_iterator(const dl_collection *dl_restrict col) {
  dl_iterator bad;
  bad.dl_vector.index = DL_NATURAL_MAX;
  return bad;
}

dl_api dl_iterator _linked_list_dl_make_invalid_dl_iterator(const dl_collection *dl_restrict col) {
  dl_iterator bad;
  bad.dl_linked_list.node = NULL;

  return bad;
}

dl_api dl_any _vector_collection_push_start(dl_collection *dl_restrict col, dl_iterator *iter) {
  if (col->data.dl_vector.index[1] >= dl_vector_capacity(&col->data.dl_vector.container)
      && !dl_vector_grow(&col->data.dl_vector.container))
    return NULL;

  iter->dl_vector.index = col->data.dl_vector.index[1];
  col->data.dl_vector.index[1]++;

  return dl_vector_ref(&col->data.dl_vector.container, iter->dl_vector.index);
}

dl_api dl_any _linked_list_collection_push_start(dl_collection *dl_restrict col, dl_iterator *iter) {
  dl_linked_list *list;
  
  list = &col->data.dl_linked_list.container;
  if ((iter->dl_linked_list.node = dl_linked_list_add(list, col->data.dl_linked_list.container.last, NULL)))
    return dl_linked_list_ref(iter->dl_linked_list.node);
  else {
    if (dl_linked_list_grow(list) && (iter->dl_linked_list.node = dl_linked_list_add(list, col->data.dl_linked_list.container.last, NULL)))
      return dl_linked_list_ref(iter->dl_linked_list.node);
    return NULL;
  }
}

dl_api dl_bool _vector_collection_is_empty(const dl_collection *dl_restrict col) {
  return col->data.dl_vector.index[0] == col->data.dl_vector.index[1];
}

dl_api dl_bool _linked_list_collection_is_empty(const dl_collection *dl_restrict col) {
  return col->data.dl_linked_list.container.first == NULL;
}

dl_api dl_any _vector_collection_pop(dl_collection *dl_restrict col, dl_any out) {
  dl_vector *v;
  dl_natural idx;

  if (dl_collection_is_queue(col)) {
    v = &col->data.dl_vector.container;
    idx = col->data.dl_vector.index[0];

    if (dl_unlikely(!dl_vector_get(v, idx, out)))
      return NULL;

    col->data.dl_vector.index[0]++;

    _vector_queue_roll_slices(col);

    return out;
  }
  else {
    v = &col->data.dl_vector.container;
    idx = dl_collection_end(col).dl_vector.index - 1;

    col->data.dl_vector.index[1] = idx;
    return dl_vector_get(v, idx, out);
  }
}

dl_api dl_any _linked_list_collection_pop(dl_collection * dl_restrict col, dl_any out) {
  dl_linked_list *list;
  struct dl_linked_list_node *position;

  list = &col->data.dl_linked_list.container;
  
  if (dl_collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return dl_linked_list_remove(list, position, out);
}

dl_api dl_bool _vector_collection_pop_destroy(dl_collection *dl_restrict col) {
  dl_natural index;
  dl_any ref;
  
  if (dl_collection_is_queue(col)) {
    index = col->data.dl_vector.index[0];

    if (col->settings.deconstruct_entry.func != NULL) {
      if (!(ref = dl_vector_ref(&col->data.dl_vector.container, index)))
        return false;

      col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
    }

    col->data.dl_vector.index[0]++;

    _vector_queue_roll_slices(col);

    return true;
  }
  else {
    index = dl_collection_end(col).dl_vector.index - 1;

    if (col->settings.deconstruct_entry.func != NULL) {
      if (!(ref = dl_vector_ref(&col->data.dl_vector.container, index)))
        return false;

      col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
    }

    col->data.dl_vector.index[1] = index;

    return true;
  }
}

dl_api dl_bool _linked_list_collection_pop_destroy(dl_collection *dl_restrict col) {
  dl_linked_list *list;
  struct dl_linked_list_node *position;

  list = &col->data.dl_linked_list.container;
  
  if (dl_collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return dl_linked_list_destroy(list, position, &col->settings.deconstruct_entry);
}

dl_api dl_bool _vector_collection_pop_forget(dl_collection *dl_restrict col) {
  dl_natural index;
  
  if (dl_collection_is_queue(col)) {
    col->data.dl_vector.index[0]++;

    _vector_queue_roll_slices(col);

    return true;
  }
  else {
    index = dl_collection_end(col).dl_vector.index - 1;

    col->data.dl_vector.index[1] = index;

    return true;
  }
}

dl_api dl_bool _linked_list_collection_pop_forget(dl_collection *dl_restrict col) {
  dl_linked_list *list;
  struct dl_linked_list_node *position;

  list = &col->data.dl_linked_list.container;
  
  if (dl_collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return dl_linked_list_destroy(list, position, NULL);
}

dl_api dl_integer _vector_collection_ref_array(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array) {
  dl_natural count, limit;
    
  count = dl_vector_ref_array(&col->data.dl_vector.container, iter.dl_vector.index, out_array);
  limit = col->data.dl_vector.index[1] - iter.dl_vector.index;

  return count < limit ? count : limit;
}

dl_api dl_integer _linked_list_collection_ref_array(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array) {
  out_array[0] = dl_linked_list_ref(iter.dl_linked_list.node);
  return 1;
}

dl_api const dl_any _vector_collection_ref(const dl_collection *dl_restrict col, dl_iterator iter) {
  return dl_vector_ref(&col->data.dl_vector.container, iter.dl_vector.index);
}

dl_api const dl_any _linked_list_collection_ref(const dl_collection *dl_restrict col, dl_iterator iter) {
  return dl_linked_list_ref(iter.dl_linked_list.node);
}

dl_api dl_bool _vector_collection_swap(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b) {
  dl_iterator t;

  if (!dl_vector_swap(&col->data.dl_vector.container, iter_a->dl_vector.index, iter_b->dl_vector.index))
    return false;
  t = *iter_a;
  *iter_a = *iter_b;
  *iter_b = t;
  return true;
}

dl_api dl_bool _linked_list_collection_swap(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b) {
  if (!dl_linked_list_swap(&col->data.dl_linked_list.container, iter_a->dl_linked_list.node, iter_b->dl_linked_list.node, false))
    return false;
  return true;
}

dl_any _vector_collection_get(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out) {
  return dl_vector_get(&col->data.dl_vector.container, iter.dl_vector.index, out);
}

dl_any _linked_list_collection_get(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out) {
  return dl_linked_list_get(&col->data.dl_linked_list.container, iter.dl_linked_list.node, out);
}

dl_any _vector_collection_set(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value) {
  dl_any ref;

  if ((ref = dl_vector_set(&col->data.dl_vector.container, iter->dl_vector.index, value)))
    return dl_collection_ref(col, *iter);
  return NULL;
}

dl_any _linked_list_collection_set(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value) {
  dl_any ref;

  if ((ref = dl_linked_list_set(&col->data.dl_linked_list.container, iter->dl_linked_list.node, value)))
    return dl_collection_ref(col, *iter);
  return NULL;
}

dl_api dl_iterator _vector_collection_index(dl_collection *dl_restrict col, dl_natural index) {
  dl_iterator iter;
  iter.dl_vector.index = index;
  return iter;
}

dl_api dl_iterator _linked_list_collection_index(dl_collection *dl_restrict col, dl_natural index) {
  dl_iterator iter;
  iter.dl_linked_list.node = dl_linked_list_index(&col->data.dl_linked_list.container, index);
  return iter;
}

dl_api void _vector_collection_next(const dl_collection *dl_restrict col, dl_iterator *iter) {
  iter->dl_vector.index++;
}

dl_api void _linked_list_collection_next(const dl_collection *dl_restrict col, dl_iterator *iter) {
  iter->dl_linked_list.node = iter->dl_linked_list.node->next;
}

dl_api void _vector_collection_prev(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (!dl_iterator_is_valid(col, *iter))
    iter->dl_vector.index = col->data.dl_vector.index[1] - 1;
  else if (iter->dl_vector.index > col->data.dl_vector.index[0])
    iter->dl_vector.index--;
  else
    *iter = dl_make_invalid_dl_iterator(col);
}

dl_api void _linked_list_collection_prev(const dl_collection *dl_restrict col, dl_iterator *iter) {
  if (!dl_iterator_is_valid(col, *iter))
    iter->dl_linked_list.node = col->data.dl_linked_list.container.last;
  else if (iter->dl_linked_list.node != col->data.dl_linked_list.container.first)
    iter->dl_linked_list.node = iter->dl_linked_list.node->previous;
  else
    *iter = dl_make_invalid_dl_iterator(col);
}

dl_api dl_integer _vector_collection_count(const dl_collection *dl_restrict col) {
  return col->data.dl_vector.index[1] - col->data.dl_vector.index[0];
}

dl_api dl_integer _linked_list_collection_count(const dl_collection *dl_restrict col) {
  return dl_linked_list_length(&col->data.dl_linked_list.container);
}

dl_api dl_iterator _vector_collection_begin(const dl_collection *dl_restrict col) {
  dl_iterator iter;
  iter.dl_vector.index = col->data.dl_vector.index[0];
  return iter;
}

dl_api dl_iterator _linked_list_collection_begin(const dl_collection *dl_restrict col) {
  dl_iterator iter;
  iter.dl_linked_list.node = col->data.dl_linked_list.container.first;
  return iter;
}

dl_api dl_iterator _vector_collection_end(const dl_collection *dl_restrict col) {
  dl_iterator iter;
  iter.dl_vector.index = col->data.dl_vector.index[1];
  return iter;
}

dl_api dl_iterator _linked_list_collection_end(const dl_collection *dl_restrict col) {
  dl_iterator iter;
  iter.dl_linked_list.node = NULL;
  return iter;
}

dl_api const dl_any _vector_collection_search_region(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  if (dl_collection_is_sorted(col))
    return _collection_search_region_vector(col, predicate, left, right, iter);
  return _collection_linear_search(col, predicate, left, right, iter);
}

dl_api const dl_any _linked_list_collection_search_region(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter) {
  return _collection_linear_search(col, predicate, left, right, iter);
}

dl_api dl_bool _vector_collection_destroy_at(dl_collection *dl_restrict col, dl_iterator *iter, dl_handler *destructor) {
  dl_vector *v;
  dl_natural index, swap_index;
  dl_any item;
  
  v = &col->data.dl_vector.container;
  index = iter->dl_vector.index;

  if (destructor != NULL) {
    if (dl_unlikely(!(item = dl_vector_ref(v, index))))
      return false;
    destructor->func(destructor->data, item);
  }

  if (dl_collection_count(col) > 1)
    for (swap_index = col->data.dl_vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
      dl_vector_swap(v, swap_index, index);
  col->data.dl_vector.index[1]--;

  return true;
}

dl_api dl_bool _linked_list_collection_destroy_at(dl_collection *dl_restrict col, dl_iterator *iter, dl_handler *destructor) {
  struct dl_linked_list_node *next;
  next = iter->dl_linked_list.node->next;
  if (dl_linked_list_destroy(&col->data.dl_linked_list.container, iter->dl_linked_list.node, destructor)) {
    iter->dl_linked_list.node = next;
    return true;
  }
  return false;
}
  
dl_api dl_any _vector_collection_remove_at(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out) {
  dl_vector *v;
  dl_natural index, swap_index;
  
  v = &col->data.dl_vector.container;
  index = iter->dl_vector.index;

  if (!(out = dl_vector_get(v, index, out)))
    return NULL;

  if (dl_collection_count(col) > 1)
    for (swap_index = col->data.dl_vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
      dl_vector_swap(v, swap_index, index);
  col->data.dl_vector.index[1]--;

  return out;
}

dl_api dl_any _linked_list_collection_remove_at(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out) {
  struct dl_linked_list_node *next;
  
  next = iter->dl_linked_list.node->next;
  if ((out = dl_linked_list_remove(&col->data.dl_linked_list.container, iter->dl_linked_list.node, out))) {
    iter->dl_linked_list.node = next;
    return out;
  }

  return NULL;
}

  
dl_api dl_integer _vector_collection_destroy_range(dl_collection *dl_restrict col, dl_iterator *iter, dl_natural count) {
  dl_natural index, start, total;
  dl_handler destruct;
  dl_vector *v;
  dl_any item;
  
  index = iter->dl_vector.index;
  start = index;
  v = &col->data.dl_vector.container;

  total = 0;
  destruct = col->settings.deconstruct_entry;

  for (item = dl_vector_ref(v, index);
       item != NULL && total < count;
       ++index, item = dl_vector_ref(v, index), ++total)
    if (destruct.func != NULL)
      destruct.func(destruct.data, item);

  if (total != 0) {
    do {
      dl_vector_swap(v, start, index);
      ++start;
      ++index;
    } while (NULL != dl_vector_ref(v, index));
  }

  col->data.dl_vector.index[1] -= total;

  return total;
}
  
dl_api dl_integer _linked_list_collection_destroy_range(dl_collection *dl_restrict col, dl_iterator *index, dl_natural count) {
  struct dl_linked_list_node *next;
  dl_natural idx;

  next = index->dl_linked_list.node;
  for (idx = 0; idx < count && next != NULL; ++idx)
    next = next->next;
  count = dl_linked_list_destroy_range(&col->data.dl_linked_list.container, index->dl_linked_list.node, count, &col->settings.deconstruct_entry);
  index->dl_linked_list.node = next;
  return count;
}

dl_bool _vector_collection_insert(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item) {
  dl_iterator index, next;
  
  index = dl_collection_end(col);

  if (!dl_collection_push(col, item))
    return false;

  while (!dl_iterator_equal(col, index, *position)) {
    next = index;
    dl_collection_prev(col, &next);
    dl_collection_swap(col, &next, &index);
  }

  return true;
}

dl_bool _linked_list_collection_insert(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item) {
  struct dl_linked_list_node *node;
  dl_linked_list *list;

  node = position->dl_linked_list.node;
  list = &col->data.dl_linked_list.container;

  position->dl_linked_list.node = dl_linked_list_add(list, node, item);

  return true;
}



/*****************************************************************************
 **  Init/Destroy Collections
 ****************************************************************************/

struct dl_collection_dispatch_functions default_vector_dl_collection_dispatch_functions = {
  (dl_integer (*)(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right))_vector_dl_iterator_compare,
  (dl_bool (*)(const dl_collection *dl_restrict col, dl_iterator index))_vector_dl_iterator_is_valid,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_vector_dl_make_invalid_dl_iterator,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter))_vector_collection_push_start,
  (dl_bool (*)(const dl_collection *dl_restrict col))_vector_collection_is_empty,
  (dl_any (*)(const dl_collection *dl_restrict col, dl_any out))_vector_collection_pop,
  (dl_bool (*)(dl_collection *dl_restrict col))_vector_collection_pop_destroy,
  (dl_bool (*)(dl_collection *dl_restrict col))_vector_collection_pop_forget,
  (dl_integer (*)(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array))_vector_collection_ref_array,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator iter))_vector_collection_ref,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b))_vector_collection_swap,
  (dl_any (*)(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out))_vector_collection_get,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value))_vector_collection_set,
  (dl_iterator (*)(dl_collection *dl_restrict col, dl_natural index))_vector_collection_index,
  (void (*)(const dl_collection *dl_restrict col, dl_iterator *iter))_vector_collection_next,
  (void (*)(const dl_collection *dl_restrict col, dl_iterator *iter))_vector_collection_prev,
  (dl_integer (*)(const dl_collection *dl_restrict col))_vector_collection_count,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_vector_collection_begin,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_vector_collection_end,
  (const dl_any (*)(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter))_vector_collection_search_region,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_handler *destructor))_vector_collection_destroy_at,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out))_vector_collection_remove_at,
  (dl_integer (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_natural count))_vector_collection_destroy_range,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item))_vector_collection_insert
};

struct dl_collection_dispatch_functions default_linked_list_dl_collection_dispatch_functions = {
  (dl_integer (*)(const dl_collection *dl_restrict col, dl_iterator left, dl_iterator right))_linked_list_dl_iterator_compare,
  (dl_bool (*)(const dl_collection *dl_restrict col, dl_iterator index))_linked_list_dl_iterator_is_valid,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_linked_list_dl_make_invalid_dl_iterator,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter))_linked_list_collection_push_start,
  (dl_bool (*)(const dl_collection *dl_restrict col))_linked_list_collection_is_empty,
  (dl_any (*)(const dl_collection *dl_restrict col, dl_any out))_linked_list_collection_pop,
  (dl_bool (*)(dl_collection *dl_restrict col))_linked_list_collection_pop_destroy,
  (dl_bool (*)(dl_collection *dl_restrict col))_linked_list_collection_pop_forget,
  (dl_integer (*)(dl_collection *dl_restrict col, dl_iterator iter, dl_any *out_array))_linked_list_collection_ref_array,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator iter))_linked_list_collection_ref,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *iter_a, dl_iterator *iter_b))_linked_list_collection_swap,
  (dl_any (*)(const dl_collection *dl_restrict col, dl_iterator iter, dl_any out))_linked_list_collection_get,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any value))_linked_list_collection_set,
  (dl_iterator (*)(dl_collection *dl_restrict col, dl_natural index))_linked_list_collection_index,
  (void (*)(const dl_collection *dl_restrict col, dl_iterator *iter))_linked_list_collection_next,
  (void (*)(const dl_collection *dl_restrict col, dl_iterator *iter))_linked_list_collection_prev,
  (dl_integer (*)(const dl_collection *dl_restrict col))_linked_list_collection_count,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_linked_list_collection_begin,
  (dl_iterator (*)(const dl_collection *dl_restrict col))_linked_list_collection_end,
  (const dl_any (*)(const dl_collection *dl_restrict col, dl_filter *predicate, dl_iterator left, dl_iterator right, dl_iterator *iter))_linked_list_collection_search_region,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_handler *destructor))_linked_list_collection_destroy_at,
  (dl_any (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_any out))_linked_list_collection_remove_at,
  (dl_integer (*)(dl_collection *dl_restrict col, dl_iterator *iter, dl_natural count))_linked_list_collection_destroy_range,
  (dl_bool (*)(dl_collection *dl_restrict col, dl_iterator *dl_restrict position, dl_any item))_linked_list_collection_insert
};

dl_collection_settings default_vector_collection_settings = {
  DL_COLLECTION_TYPE_LIST,
  DL_STORAGE_TYPE_VECTOR,
  {0},
  {0},
  32,
  sizeof(dl_byte),
  &default_vector_dl_collection_dispatch_functions
};

dl_collection_settings default_linked_list_collection_settings = {
  DL_COLLECTION_TYPE_LIST,
  DL_STORAGE_TYPE_LINKED_LIST,
  {0},
  {0},
  32,
  sizeof(dl_byte),
  &default_linked_list_dl_collection_dispatch_functions
};

void _check_init_collection(dl_collection *dl_restrict col, dl_collection_settings *dl_restrict settings, dl_natural count) {
  col->settings = *settings;
  
  if (col->settings.comparer.func == NULL) {
    switch (col->settings.element_size) {
    case 1:
      col->settings.comparer.func = _default_compare_8;
      break;
    case 2:
      col->settings.comparer.func = _default_compare_16;
      break;
    case 4:
      col->settings.comparer.func = _default_compare_32;
      break;
    case 8:
      col->settings.comparer.func = _default_compare_64;
      break;
    default:
      col->settings.comparer.func = _default_compare_any;
      break;
    }
  }

  if (col->settings.storage == DL_STORAGE_TYPE_VECTOR) {
    col->data.dl_vector.index[0] = 0;
    col->data.dl_vector.index[1] = count;
  }
}

dl_api dl_collection *dl_init_collection(dl_collection *dl_restrict col, dl_collection_type type, dl_storage_type storage, dl_comparator *compare, dl_handler *destructor, dl_natural element_size) {
  dl_collection_settings settings;
  
  if (dl_safety(col == NULL || element_size < 1))
    return NULL;

  switch (storage) {
  case DL_STORAGE_TYPE_VECTOR: {
    settings = default_vector_collection_settings;
    break;
  }
  case DL_STORAGE_TYPE_LINKED_LIST: {
    settings = default_linked_list_collection_settings;
    break;
  }
  default:
    return NULL;
  }

  settings.type = type;
  settings.element_size = element_size;
  
  if (destructor != NULL)
    settings.deconstruct_entry = *destructor;
  else
    settings.deconstruct_entry.func = NULL;
  if (compare != NULL)
    settings.comparer = *compare;
  else
    settings.comparer.func = NULL;

  _check_init_collection(col, &settings, 0);
  
  switch (settings.storage) {
  case DL_STORAGE_TYPE_VECTOR: {
    if (!dl_init_vector(&col->data.dl_vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  case DL_STORAGE_TYPE_LINKED_LIST: {
    if (!dl_init_linked_list(&col->data.dl_linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  default:
    return NULL;
  }
  
  _force_collection_properties(col);

  return col;
}

dl_api dl_collection *dl_init_collection_custom(dl_collection *dl_restrict col, dl_collection_settings *dl_restrict settings, dl_natural element_size) {
  if (dl_safety(col == NULL))
    return NULL;

  _check_init_collection(col, settings, 0);

  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_VECTOR:
    if (!dl_init_vector(&col->data.dl_vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  case DL_STORAGE_TYPE_LINKED_LIST:
    if (!dl_init_linked_list(&col->data.dl_linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  default:
    return NULL;
  }

  _force_collection_properties(col);

  return col;
}

dl_api dl_collection *dl_init_collection_array(dl_collection *dl_restrict col, dl_collection_type type, dl_comparator *comp, dl_handler *deconstruct_entry, dl_byte *data, dl_natural element_size, dl_natural count) {
  dl_collection_settings settings;
  
  if (dl_safety(col == NULL || data == NULL))
    return NULL;

  if (!dl_init_vector_array(&col->data.dl_vector.container, data, element_size, count))
    return NULL;

  settings = default_vector_collection_settings;
  settings.storage = DL_STORAGE_TYPE_VECTOR;
  settings.type = type;
  settings.element_size = element_size;
  
  if (comp != NULL)
    settings.comparer = *comp;
  else
    settings.comparer.func = NULL;
  if (deconstruct_entry != NULL)
    settings.deconstruct_entry = *deconstruct_entry;
  else
    settings.deconstruct_entry.func = NULL;

  _check_init_collection(col, &settings, count);

  _force_collection_properties(col);

  return col;
}

dl_api void dl_destroy_collection(dl_collection *dl_restrict col) {
  dl_iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL))
    return;

  switch (col->settings.storage) {
  case DL_STORAGE_TYPE_VECTOR:
    if (col->settings.deconstruct_entry.func != NULL) {
      index = dl_collection_begin(col);
      for (item = dl_collection_ref(col, index); item != NULL; item = dl_collection_next(col, &index))
	col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, item);
    }

    dl_destroy_vector(&col->data.dl_vector.container, NULL);
    break;
  case DL_STORAGE_TYPE_LINKED_LIST:
    dl_destroy_linked_list(&col->data.dl_linked_list.container, &col->settings.deconstruct_entry);
    break;
  }
}


#endif /* DL_USE_CONTAINERS */
  
#endif /* DL_IMPLEMENTATION */

#endif

