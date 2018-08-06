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

#define DL_IS_ATLEAST_C89 (DL_IS_C11 || DL_IS_C99 || DL_IS_C90 || DL_IS_C89)
#define DL_IS_ATLEAST_C90 (DL_IS_C11 || DL_IS_C99 || DL_IS_C90)
#define DL_IS_ATLEAST_C99 (DL_IS_C11 || DL_IS_C99)
#define DL_IS_ATLEAST_C11 (DL_IS_C11)



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
# if DL_IS_ATLEAST_C99
#   include <tgmath.h>
# else
#   include <math.h>
# endif
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
#endif

#ifndef dl_api
# define dl_api  
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_api
#   define dl_api
# endif
#endif

#ifndef dl_restrict
# define dl_restrict
# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MINGW
#   undef dl_restrict
#   define dl_restrict __restrict__
# endif
# if DL_IS_MSC
#   undef dl_restrict
#   define dl_restrict __restrict
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
# if DL_USE_LOGGING
#   define dl_safety(x) (dl_unlikely(x) ? DL_ERROR("Safety triggered") || 1 : 0)
# else
#   define dl_safety(x) (dl_unlikely(x) ? 1 : 0)
# endif
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
  
# if DL_IS_GNUC || DL_IS_CLANG
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
  
  extern const dl_mat4 mat4_identity;

  dl_api dl_mat4 *init_mat4(dl_mat4 * dl_restrict m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3);

  dl_api dl_mat4 *init_mat4_cols(dl_mat4 * dl_restrict m, const dl_vec4 *dl_restrict column_a, const dl_vec4 *dl_restrict column_b, const dl_vec4 *dl_restrict column_c, const dl_vec4 *dl_restrict column_d);

  dl_api dl_mat4 *init_mat4_translate(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z);
  dl_api dl_mat4 *init_mat4_rotate_x(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *init_mat4_rotate_y(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *init_mat4_rotate_z(dl_mat4 * dl_restrict m, dl_real radians);
  dl_api dl_mat4 *init_mat4_rotate(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict axis, dl_real radians);
  dl_api dl_mat4 *init_mat4_scale(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z);

  dl_api dl_mat4 *init_mat4_look_at(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict origin, const dl_vec3 *dl_restrict target, const dl_vec3 *dl_restrict up);
  dl_api dl_mat4 *init_mat4_orthographic(dl_mat4 * dl_restrict m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far);
  dl_api dl_mat4 *init_mat4_perspective(dl_mat4 * dl_restrict m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far);

  dl_api dl_mat4 *mat4_add(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *mat4_sub(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *mat4_mul(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out);

  dl_api dl_vec4 *mat4_mul_vec4(const dl_mat4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out);
  dl_api dl_vec3 *mat4_mul_vec3(const dl_mat4 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out);
  dl_api dl_point3 *mat4_mul_point3(const dl_mat4 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out);

  dl_api dl_mat4 *mat4_transpose(const dl_mat4 *dl_restrict left, dl_mat4 *dl_restrict out);

  dl_api dl_mat4 *mat4_mul_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *mat4_div_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *mat4_add_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);
  dl_api dl_mat4 *mat4_sub_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out);

  dl_api dl_bool mat4_approximately_equal(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_real epsilon);

  
  
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
  } vector_settings;

  typedef struct {
    vector_settings settings;
    dl_natural slice_count;

    union {
      dl_byte **slices;
      dl_byte *array;
    } data;
  } vector;

  extern vector_settings default_vector_settings;

  dl_api vector *init_vector(vector * dl_restrict target, dl_natural element_size, dl_natural capacity);
  dl_api vector *init_vector_custom(vector * dl_restrict target, vector_settings settings, dl_natural capacity);

  dl_api vector *init_vector_array(vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count);

  dl_api dl_natural vector_copy(vector * dl_restrict target, dl_natural target_offset_index, const vector * dl_restrict original);
  dl_api dl_natural vector_copy_array(vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void destroy_vector(vector * dl_restrict target, const dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural vector_capacity(const vector * dl_restrict v);

  dl_api dl_bool vector_grow(vector * dl_restrict v);
  dl_api dl_bool vector_shrink(vector * dl_restrict v, dl_handler *deconstruct_entry);
  dl_api dl_bool vector_resize(vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry);

  dl_api dl_any vector_get(const vector * dl_restrict v, dl_natural index, dl_any out);
  dl_api const dl_any vector_ref(const vector * dl_restrict v, dl_natural index);
  dl_api dl_any vector_set(vector * dl_restrict v, dl_natural index, dl_any value);

  dl_api dl_bool vector_swap(vector * dl_restrict v, dl_natural index1, dl_natural index2);
  dl_api dl_natural vector_ref_array(vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out);



  /*****************************************************************************
   **  Linked Lists
   ****************************************************************************/

  typedef struct {
    dl_natural element_size;
    dl_natural cache_length;
    dl_any (*alloc)(dl_natural count, dl_natural element_size);
    void (*free)(dl_any data);
  } linked_list_settings;

  struct linked_list_node {
    struct linked_list_node *next;
    struct linked_list_node *previous;
  };

#define LINKED_LIST_HEADER_SIZE (sizeof(struct linked_list_node))
#define LINKED_LIST_DATA(element) ((dl_any)&((struct linked_list_node *)element)[1])

  typedef struct {
    struct linked_list_node *first;
    struct linked_list_node *last;
    struct linked_list_node *free;

    linked_list_settings settings;
    vector node_cache;
  } linked_list;

  extern linked_list_settings default_linked_list_settings;

  dl_api linked_list *init_linked_list(linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length);
  dl_api linked_list *init_linked_list_custom(linked_list * dl_restrict target, linked_list_settings settings);

  dl_api dl_natural linked_list_copy(linked_list * dl_restrict target, struct linked_list_node *target_position, const linked_list *dl_restrict original);
  dl_api dl_natural linked_list_copy_array(linked_list * dl_restrict target, struct linked_list_node *target_position, const dl_byte *dl_restrict data, dl_natural count);

  dl_api void destroy_linked_list(linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_natural linked_list_capacity(const linked_list * dl_restrict list);
  dl_api dl_natural linked_list_length(const linked_list * dl_restrict list);

  dl_api dl_bool linked_list_grow(linked_list * dl_restrict list);
  dl_api dl_bool linked_list_shrink(linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry);
  dl_api dl_bool linked_list_resize(linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry);

  dl_api dl_any linked_list_get(const linked_list * dl_restrict list, struct linked_list_node *dl_restrict position, dl_any out);
  dl_api const dl_any linked_list_ref(const struct linked_list_node *dl_restrict position);
  dl_api dl_any linked_list_set(linked_list * dl_restrict list, struct linked_list_node *dl_restrict position, dl_any value);
  dl_api struct linked_list_node *linked_list_index(linked_list * dl_restrict list, dl_natural position);

  dl_api struct linked_list_node *linked_list_add(linked_list * dl_restrict list, struct linked_list_node *dl_restrict position, dl_any value);
  dl_api dl_any linked_list_remove(linked_list * dl_restrict list, struct linked_list_node * dl_restrict position, dl_any out);
  dl_api dl_natural linked_list_destroy_range(linked_list * dl_restrict list, struct linked_list_node *dl_restrict position, dl_natural count, dl_handler *dl_restrict destruct_entry);
  dl_api dl_bool linked_list_destroy(linked_list * dl_restrict list, struct linked_list_node *dl_restrict position, dl_handler *deconstruct_entry);

  dl_api dl_bool linked_list_swap(linked_list * dl_restrict list, struct linked_list_node *dl_restrict position1, struct linked_list_node *dl_restrict position2, dl_bool data);



  /*****************************************************************************
   **  Collections
   ****************************************************************************/


  typedef union {
    struct {
      dl_natural index;
    } vector;
    struct {
      struct linked_list_node *node;
    } linked_list;
  } iterator;

  enum {
    /* Unsorted */
    COLLECTION_TYPE_LIST,
    COLLECTION_TYPE_QUEUE,
    /* Sorted */
    COLLECTION_TYPE_SORTED_LIST,
    COLLECTION_TYPE_SET
  };
  typedef dl_byte collection_type;

  enum {
    STORAGE_TYPE_VECTOR,
    STORAGE_TYPE_LINKED_LIST
  };
  typedef dl_byte storage_type;  

  struct collection_dispatch_functions;
  typedef struct {
    collection_type type;
    storage_type storage;
    
    dl_comparator comparer;
    dl_handler deconstruct_entry;
    dl_natural capacity;
    dl_natural element_size;

    struct collection_dispatch_functions *functions;
  } collection_settings;

  extern collection_settings default_vector_collection_settings;
  extern collection_settings default_linked_list_collection_settings;

  typedef struct {
    collection_settings settings;

    union {
      struct {
        vector container;
        dl_natural index[2];
      } vector;
      struct {
        linked_list container;
      } linked_list;
    } data;
  } collection;

  struct collection_dispatch_functions {
    dl_integer (*_iterator_compare)(const collection *dl_restrict col, iterator left, iterator right);
    dl_bool (*_iterator_is_valid)(const collection *dl_restrict col, iterator index);
    iterator (*_make_invalid_iterator)(const collection *dl_restrict col);

    dl_any (*_collection_push_start)(collection *dl_restrict col, iterator *iter);
    dl_bool (*_collection_is_empty)(const collection *dl_restrict col);
    dl_any (*_collection_pop)(const collection *dl_restrict col, dl_any out);
    dl_bool (*_collection_pop_destroy)(collection *dl_restrict col);
    dl_bool (*_collection_pop_forget)(collection *dl_restrict col);
    dl_integer (*_collection_ref_array)(collection *dl_restrict col, iterator iter, dl_any *out_array);
    dl_any (*_collection_ref)(collection *dl_restrict col, iterator iter);
    dl_bool (*_collection_swap)(collection *dl_restrict col, iterator *iter_a, iterator *iter_b);
    dl_any (*_collection_get)(const collection *dl_restrict col, iterator iter, dl_any out);
    dl_any (*_collection_set)(collection *dl_restrict col, iterator *iter, dl_any value);
    iterator (*_collection_index)(collection *dl_restrict col, dl_natural index);
    void (*_collection_next)(const collection *dl_restrict col, iterator *iter);
    void (*_collection_prev)(const collection *dl_restrict col, iterator *iter);
    dl_integer (*_collection_count)(const collection *dl_restrict col);
    iterator (*_collection_begin)(const collection *dl_restrict col);
    iterator (*_collection_end)(const collection *dl_restrict col);
    const dl_any (*_collection_search_region)(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter);
    dl_bool (*_collection_destroy_at)(collection *dl_restrict col, iterator *iter, dl_handler *destructor);
    dl_any (*_collection_remove_at)(collection *dl_restrict col, iterator *iter, dl_any out);
    dl_integer (*_collection_destroy_range)(collection *dl_restrict col, iterator *iter, dl_natural count);
    dl_bool (*_collection_insert)(collection *dl_restrict col, iterator *dl_restrict position, dl_any item);
  };
  
  dl_api dl_integer iterator_compare(const collection *dl_restrict col, iterator left, iterator right);
  dl_api dl_bool iterator_equal(const collection *dl_restrict col, iterator left, iterator right);
  dl_api dl_bool iterator_is_valid(const collection *dl_restrict col, iterator index);
  dl_api iterator make_invalid_iterator(const collection *dl_restrict col);

  dl_api collection *init_collection(collection *dl_restrict col, collection_type type, storage_type storage, dl_comparator *dl_restrict compare, dl_handler *dl_restrict destructor, dl_natural element_size);
  dl_api collection *init_collection_custom(collection *dl_restrict col, collection_settings settings, dl_natural element_size);

  dl_api collection *init_collection_array(collection *dl_restrict col, collection_type type, dl_comparator *dl_restrict comp, dl_handler *dl_restrict destruct_entry, dl_byte * data, dl_natural element_size, dl_natural count);

  dl_api dl_natural collection_element_size(const collection *dl_restrict col);

  dl_api void destroy_collection(collection *dl_restrict col);

  dl_api dl_bool collection_is_set(const collection *dl_restrict col);
  dl_api dl_bool collection_is_sorted(const collection * dl_restrict col);
  dl_api dl_bool collection_is_queue(const collection * dl_restrict col);
  dl_api dl_bool collection_is_vector(const collection * dl_restrict col);
  dl_api dl_bool collection_is_linked_list(const collection * dl_restrict col);

  dl_api dl_integer collection_copy(const collection *dl_restrict original, collection *dl_restrict target);
  dl_api dl_integer collection_copy_array(const dl_any data, dl_natural count, collection *dl_restrict target);

  dl_api iterator collection_begin(const collection *dl_restrict col);
  dl_api iterator collection_end(const collection *dl_restrict col);

  dl_api dl_any collection_pop(collection *dl_restrict col, dl_any out);
  dl_api dl_bool collection_pop_forget(collection *dl_restrict col);
  dl_api dl_bool collection_pop_destroy(collection *dl_restrict col);

  dl_api dl_any collection_push(collection *dl_restrict col, const dl_any value);
  dl_api dl_any collection_push_index(collection *dl_restrict col, const dl_any value, iterator *out_index);
  dl_api dl_any collection_push_start(collection *dl_restrict col, iterator *iter);
  dl_api dl_any collection_push_finish(collection *dl_restrict col, iterator *iter);

  dl_api const dl_any collection_begin_ref(const collection *dl_restrict col, iterator *iter);
  dl_api const dl_any collection_end_ref(const collection *dl_restrict col, iterator *iter);

  dl_api iterator collection_index(collection *dl_restrict col, dl_natural index);
  dl_api iterator collection_index_of(const collection *dl_restrict col, const dl_any item);

  dl_api const dl_any collection_next(const collection *dl_restrict col, iterator *iter);
  dl_api const dl_any collection_prev(const collection *dl_restrict col, iterator *iter);

  dl_api dl_any collection_find(const collection *dl_restrict col, dl_filter *dl_restrict predicate, iterator *iter);
  dl_api dl_any collection_find_last(const collection *dl_restrict col, dl_filter *dl_restrict predicate, iterator *iter);

  dl_api dl_any collection_foldl(const collection *dl_restrict col, dl_any initial, dl_folder *func);
  dl_api dl_any collection_foldr(const collection *dl_restrict col, dl_any initial, dl_folder *func);
  dl_api const dl_any collection_search(const collection *dl_restrict col, dl_filter *predicate, iterator *iter);
  dl_api const dl_any collection_search_region(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter);

  dl_api dl_any collection_get(const collection *dl_restrict col, iterator index, dl_any out);
  dl_api dl_any collection_insert(collection *dl_restrict col, iterator *index, dl_any item);
  dl_api const dl_any collection_peek(const collection *dl_restrict col);
  dl_api const dl_any collection_ref(const collection *dl_restrict col, iterator iter);
  dl_api dl_any collection_remove_at(collection *dl_restrict col, iterator *index, dl_any out);
  dl_api dl_any collection_remove_first(collection *dl_restrict col, dl_filter *predicate, iterator *index, dl_any out);
  dl_api dl_any collection_remove_last(collection *dl_restrict col, dl_filter *predicate, iterator *index, dl_any out);
  dl_api dl_any collection_set(collection *dl_restrict col, iterator *index, dl_any value);

  dl_api dl_bool collection_all(const collection *dl_restrict col, dl_filter *func);
  dl_api dl_bool collection_any(const collection *dl_restrict col, dl_filter *func);
  dl_api dl_bool collection_contains(const collection *dl_restrict col, const dl_any item);
  dl_api dl_bool collection_is_empty(const collection *dl_restrict col);

  dl_api dl_integer collection_map(const collection *dl_restrict col, dl_converter *func, collection *out);
  dl_api dl_integer collection_zip(const collection *dl_restrict col1, const collection *dl_restrict col2, dl_zipper *zip, collection *out);

  dl_api dl_bool collection_forget_at(collection *dl_restrict col, iterator *iter);
  dl_api dl_bool collection_destroy_at(collection *dl_restrict col, iterator *iter);
  dl_api dl_bool collection_destroy_first(collection *dl_restrict col, dl_filter *predicate, iterator *iter);
  dl_api dl_bool collection_destroy_last(collection *dl_restrict col, dl_filter *predicate, iterator *iter);
  dl_api dl_bool collection_swap(collection *dl_restrict col, iterator *iter_a, iterator *iter_b);
  dl_api dl_bool collection_quick_sort(collection *dl_restrict col, dl_comparator *compare);
  dl_api dl_bool collection_quick_sort_region(collection *dl_restrict col, dl_comparator *compare, iterator left, iterator right);
  dl_api dl_bool collection_sort(collection *dl_restrict col);

  dl_api void collection_clear(collection *dl_restrict col);

  dl_api dl_integer collection_count(const collection *dl_restrict col);
  dl_api dl_integer collection_find_all(const collection *dl_restrict col, dl_filter *predicate, collection *out);

  dl_api dl_integer collection_take(collection *dl_restrict col, dl_natural count, collection *out);
  dl_api dl_integer collection_destroy_all(collection *dl_restrict col, dl_filter *predicate);
  dl_api dl_integer collection_destroy_range(collection *dl_restrict col, iterator *index, dl_natural count);
  dl_api dl_integer collection_drop(collection *dl_restrict col, dl_natural count);
  dl_api dl_integer collection_ref_array(collection *dl_restrict col, iterator index, dl_any *out_array);
  dl_api dl_integer collection_remove_all(collection *dl_restrict col, dl_filter *predicate, collection *out);
  dl_api dl_integer collection_remove_range(collection *dl_restrict col, iterator *index, dl_natural count, collection *out);

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
#ifndef M_E
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

const dl_mat4 mat4_identity = { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
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
#warning dl_init_random_time will use uninitialized pointer value instead of time
dl_api dl_random_state *dl_init_random_time(dl_random_state *state) {
  any value;
  return dl_init_random(state, (dl_integer)value);
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

  return mat4_mul_point3(init_mat4_rotate(&mat, axis, angle), left, out);
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

  return mat4_mul_vec3(init_mat4_rotate(&mat, axis, angle), left, out);
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



dl_api dl_mat4 *init_mat4(dl_mat4 * dl_restrict m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3) {
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

dl_api dl_mat4 *init_mat4_cols(dl_mat4 * dl_restrict m, const dl_vec4 *dl_restrict column_a, const dl_vec4 *dl_restrict column_b, const dl_vec4 *dl_restrict column_c, const dl_vec4 *dl_restrict column_d) {
  if (dl_safety(m == NULL || column_a == NULL || column_b == NULL || column_c == NULL || column_d == NULL))
    return NULL;
  
  *(dl_vec4 *)(&m->ary[0]) = *column_a;
  *(dl_vec4 *)(&m->ary[1]) = *column_b;
  *(dl_vec4 *)(&m->ary[2]) = *column_c;
  *(dl_vec4 *)(&m->ary[3]) = *column_d;

  return m;
}

dl_api dl_mat4 *mat4_add(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + right->ary[i][j];

  return out;
}

dl_api dl_mat4 *mat4_sub(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] - right->ary[i][j];

  return out;
}

dl_api dl_mat4 *mat4_mul(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_mat4 *dl_restrict out) {
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

dl_bool mat4_approximately_equal(const dl_mat4 *dl_restrict left, const dl_mat4 *dl_restrict right, dl_real epsilon) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || right == NULL))
    return false;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (dl_abs(left->ary[i][j] - right->ary[i][j]) > epsilon)
	return false;
  
  return true;
}

dl_api dl_vec4 *mat4_mul_vec4(const dl_mat4 *dl_restrict left, const dl_vec4 *dl_restrict right, dl_vec4 *dl_restrict out) {
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

dl_api dl_vec3 *mat4_mul_vec3(const dl_mat4 *dl_restrict left, const dl_vec3 *dl_restrict right, dl_vec3 *dl_restrict out) {
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

dl_api dl_point3 *mat4_mul_point3(const dl_mat4 *dl_restrict left, const dl_point3 *dl_restrict right, dl_point3 *dl_restrict out) {
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

dl_api dl_mat4 *mat4_transpose(const dl_mat4 *dl_restrict left, dl_mat4 *dl_restrict out) {
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

dl_api dl_mat4 *mat4_mul_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  dl_integer i,j;
  
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] * scalar;

  return out;
}

dl_api dl_mat4 *mat4_div_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  return mat4_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_mat4 *mat4_add_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  dl_integer i, j;
  
  if (dl_safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + scalar;

  return out;
}

dl_api dl_mat4 *mat4_sub_scalar(const dl_mat4 *dl_restrict left, dl_real scalar, dl_mat4 *dl_restrict out) {
  return mat4_add_scalar(left, -scalar, out);
}

dl_api dl_mat4 *init_mat4_translate(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;
  
  *m = mat4_identity;
  m->ary[3][0] = x;
  m->ary[3][1] = y;
  m->ary[3][2] = z;

  return m;
}

dl_api dl_mat4 *init_mat4_rotate_x(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);

  return init_mat4(m,
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

  return init_mat4(m,
		   1, 0,  0, 0,
		   0, c,  s, 0,
		   0, -s, c, 0,
		   0, 0,  0, 1);
#endif
}

dl_api dl_mat4 *init_mat4_rotate_y(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;
  
#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);
  
  return init_mat4(m,
		   c,  0, s, 0,
		   0,  1, 0, 0,
		   -s, 0, c, 0,
		   0,  0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
  
  return init_mat4(m,
		   c, 0, -s, 0,
		   0, 1,  0, 0,
		   s, 0,  c, 0,
		   0, 0,  0, 1);
#endif
}

dl_api dl_mat4 *init_mat4_rotate_z(dl_mat4 * dl_restrict m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

  c = dl_cos(radians);
  s = dl_sin(radians);
  
  return init_mat4(m,
		   c,  s, 0, 0,
		   -s, c, 0, 0,
		   0,  0, 1, 0,
		   0,  0, 0, 1);
}

dl_api dl_mat4 *init_mat4_rotate(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict a, dl_real radians) {
  dl_vec3 i, t;
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;
  
  if (dl_unlikely(dl_vec3_approximately_equal(a, &dl_vec3_zero, DL_EPSILON))) {
    *m = mat4_identity;
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

  return init_mat4(m,
		   c + i.x * t.x,          i.x * t.y - s * t.z,    -(-i.x * t.z - s * t.y), 0,
		   i.y * t.x + s * t.z,    c + i.y * t.y,          -(-i.y * t.z + s * t.x), 0,
		   -(i.z * t.x + s * t.y), -(i.z * t.y - s * t.x), c + i.z * t.z,          0,
		   0, 0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
  dl_vec3_mul_scalar(&t, 1.0 - c, &i);  

  return init_mat4(m,
		   c + i.x * t.x,       i.x * t.y - s * t.z, -i.x * t.z - s * t.y, 0,
		   i.y * t.x + s * t.z, c + i.y * t.y,       -i.y * t.z + s * t.x, 0,
		   i.z * t.x + s * t.y, i.z * t.y - s * t.x, c + i.z * t.z,       0,
		   0, 0, 0, 1);
#endif
}

dl_api dl_mat4 *init_mat4_scale(dl_mat4 * dl_restrict m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;
  
  *m = mat4_identity;
  m->ary[0][0] = x;
  m->ary[1][1] = y;
  m->ary[2][2] = z;

  return m;
}

dl_api dl_mat4 *init_mat4_look_at(dl_mat4 * dl_restrict m, const dl_vec3 *dl_restrict origin, const dl_vec3 *dl_restrict target, const dl_vec3 *dl_restrict up) {
  dl_vec3 zaxis, yaxis, xaxis, temp;

  if (dl_safety(m == NULL || origin == NULL || target == NULL || up == NULL))
    return NULL;

  dl_vec3_sub(origin, target, &temp);
  dl_vec3_normalize(&temp, &zaxis);
  dl_vec3_cross(up, &zaxis, &temp);
  dl_vec3_normalize(&temp, &xaxis);
  dl_vec3_cross(&zaxis, &xaxis, &yaxis);

  return init_mat4(m,
		   xaxis.x, yaxis.x, zaxis.x, 0,
		   xaxis.y, yaxis.y, zaxis.y, 0,
		   xaxis.z, yaxis.z, zaxis.z, 0,
		   -dl_vec3_dot(&xaxis, origin), -dl_vec3_dot(&yaxis, origin), -dl_vec3_dot(&zaxis, origin), 1);
}

dl_api dl_mat4 *init_mat4_orthographic(dl_mat4 * dl_restrict m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far) {
  dl_real inv_width, inv_height, inv_depth;

  if (dl_safety(m == NULL))
    return NULL;

  inv_width = 1.0 / (x_right - x_left);
  inv_height = 1.0 / (yop - y_bottom);
  inv_depth = 1.0 / (z_far - z_near);

  return init_mat4(m,
		   2 * inv_width, 0, 0, 0,
		   0, 2 * inv_height, 0, 0,
		   0, 0, 0, -2 * inv_depth,
		   -((x_right + x_left) * inv_width), -((yop + y_bottom) * inv_height), -((z_far + z_near) * inv_depth), 1);
}

dl_api dl_mat4 *init_mat4_perspective(dl_mat4 * dl_restrict m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far) {
  dl_real half_fov, invan_fov, neg_depth, inv_neg_depth;

  if (dl_safety(m == NULL))
    return NULL;

  half_fov = vertical_fov * 0.5f;
  invan_fov = 1.0 / dl_tan(half_fov);
  neg_depth = z_near - z_far;
  inv_neg_depth = 1.0 / neg_depth;

  *m = mat4_identity;
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

vector_settings default_vector_settings = {
  0, 32,
  DECLARE_ALLOC_MEMBERS(alloc, free)
};

dl_api vector *init_vector(vector * dl_restrict target, dl_natural element_size, dl_natural capacity) {
  vector_settings settings = default_vector_settings;
  settings.element_size = element_size;
  return init_vector_custom(target, settings, capacity);
}

dl_api vector *init_vector_custom(vector * dl_restrict target, vector_settings settings, dl_natural capacity) {
  dl_real dl_real_count;
  dl_natural slice_count, idx;
  
  if (dl_safety(target == NULL || settings.alloc == NULL || settings.free == NULL))
    return NULL;

  if (settings.element_size < 1)
    return NULL;

  settings.slice_length = settings.slice_length < 1 ? default_vector_settings.slice_length : settings.slice_length;

  dl_real_count = ((dl_real)capacity / (dl_real)settings.slice_length);
  slice_count = (dl_natural)dl_real_count;
  if (dl_real_count > (dl_real)(dl_natural)dl_real_count)
    slice_count++;

  target->slice_count = slice_count < 1 ? 1 : slice_count;

  target->settings = settings;

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

dl_api vector *init_vector_array(vector * dl_restrict target, dl_byte *data, dl_natural element_size, dl_natural count) {
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

dl_api void destroy_vector(vector * dl_restrict target, const dl_handler *deconstruct_entry) {
  dl_any entry;
  dl_natural slice_idx, idx;
  
  if (dl_safety(target == NULL) || target->settings.free == NULL)
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

dl_api dl_natural vector_capacity(const vector * dl_restrict v) {
  if (dl_safety(v == NULL))
    return 0;

  return v->slice_count > 0 ? v->slice_count * v->settings.slice_length : v->settings.slice_length;
}

dl_api dl_any vector_get(const vector * dl_restrict v, dl_natural index, dl_any out) {
  dl_natural slice, slice_index;
  dl_byte *target_slice;
  
  if (dl_safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (dl_unlikely(index >= v->settings.slice_length))
      return NULL;

    dl_memory_copy(out, (void *)&v->data.array[index * v->settings.element_size], v->settings.element_size);
  }
  else {
    slice = index / v->settings.slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    dl_memory_copy(out, (void *)&target_slice[slice_index * v->settings.element_size], v->settings.element_size);
  }

  return out;
}

dl_api const dl_any vector_ref(const vector * dl_restrict v, dl_natural index) {
  dl_natural slice, slice_index;
  dl_byte *target_slice;
  
  if (dl_safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (dl_unlikely(index >= v->settings.slice_length))
      return NULL;

    return &v->data.array[index * v->settings.element_size];
  }
  else {
    slice = index / v->settings.slice_length;
    if (dl_unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    return &target_slice[slice_index * v->settings.element_size];
  }
}

dl_api dl_any vector_set(vector * dl_restrict v, dl_natural index, dl_any value) {
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
  else {
    slice = index / v->settings.slice_length;
    if (slice >= v->slice_count)
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    base_index = slice_index * v->settings.element_size;
    source = (dl_byte *)value;

    return dl_memory_copy((void *)&v->data.slices[slice][base_index], (void *)source, v->settings.element_size);
  }
}

dl_api dl_bool vector_grow(vector * dl_restrict v) {
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

dl_api dl_bool vector_swap(vector * dl_restrict v, dl_natural index1, dl_natural index2) {
  dl_any left, right;
  
  if (dl_safety(v == NULL))
    return false;

  if (index1 == index2)
    return true;

  left = vector_ref(v, index1);
  if (dl_unlikely(left == NULL))
    return false;

  right = vector_ref(v, index2);
  if (dl_unlikely(right == NULL))
    return false;

  dl_memory_swap(left, right, v->settings.element_size);

  return true;
}

dl_api dl_bool vector_shrink(vector * dl_restrict v, dl_handler *dl_restrict deconstruct_entry) {
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

dl_api dl_bool vector_resize(vector * dl_restrict v, dl_natural minimum_capacity, dl_handler *deconstruct_entry) {
  dl_natural current_capacity, new_slice_count, slice_idx, slice_count, item_idx;
  dl_real needed;
  dl_integer needed_count;
  dl_byte **new_slices, *slice;
  dl_any item;
  
  if (dl_safety(v == NULL))
    return false;

  if (v->settings.free == NULL || v->settings.alloc == NULL || v->slice_count < 1 || minimum_capacity < 1)
    return false;

  current_capacity = vector_capacity(v);
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

dl_api dl_natural vector_copy_array(vector * dl_restrict target, dl_natural target_offset_index, const dl_byte *data, dl_natural count) {
  vector source;
  if (dl_unlikely(!init_vector_array(&source, (dl_byte *)data, target->settings.element_size, count)))
    return 0;

  return vector_copy(target, target_offset_index, &source);
}

dl_api dl_natural vector_copy(vector * dl_restrict target, dl_natural target_offset_index, const vector * dl_restrict original) {
  dl_natural original_capacity, element_size, target_length, original_length, target_remainder, original_remainder, total_remainder, target_slice_idx, original_slice_idx, min_remainder, count_to_copy;
  dl_byte *target_slice, *original_slice;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (dl_unlikely(original->settings.element_size != target->settings.element_size))
    return 0;

  /* target is too small */
  original_capacity = vector_capacity(original);
  if (dl_unlikely(vector_capacity(target) - target_offset_index < original_capacity))
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

dl_api dl_natural vector_ref_array(vector * dl_restrict v, dl_natural index, dl_any *dl_restrict out) {
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

linked_list_settings default_linked_list_settings = {
  32, 32,
  DECLARE_ALLOC_MEMBERS(alloc, free)
};

dl_api struct linked_list_node *_linked_list_node_alloc(linked_list * dl_restrict list, struct linked_list_node * dl_restrict after) {
  struct linked_list_node *node;
  
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

dl_api void _linked_list_node_free(linked_list * dl_restrict list, struct linked_list_node * dl_restrict node) {
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

dl_api void _linked_list_node_detach_free(linked_list *dl_restrict list, struct linked_list_node *e) {
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
  linked_list *list;
} _linked_list_node_deconstructor_data;

dl_api dl_any _linked_list_node_deconstructor(dl_any data, dl_any element) {
  _linked_list_node_deconstructor_data *d;
  struct linked_list_node *f, *e, *new_node;
  dl_handler *destruct;

  d = (_linked_list_node_deconstructor_data *)data;
  e = (struct linked_list_node *)element;

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
      destruct->func(destruct->data, LINKED_LIST_DATA(e));
    }
  }
  else
    linked_list_swap(d->list, e, new_node, true);

  _linked_list_node_free(d->list, e);
  _linked_list_node_detach_free(d->list, e);

  return NULL;
}

dl_api linked_list *init_linked_list(linked_list * dl_restrict target, dl_natural element_size, dl_natural cache_length) {
  linked_list_settings settings;
  
  if (dl_safety(target == NULL || element_size == 0))
    return false;

  settings = default_linked_list_settings;
  settings.element_size = element_size;
  settings.cache_length = cache_length;

  return init_linked_list_custom(target, settings);
}

dl_api linked_list *_linked_list_cache_grow(linked_list * dl_restrict target) {
  linked_list_settings settings;
  vector *v;
  dl_natural zero, length, idx;
  struct linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return NULL;

  settings = target->settings;

  if (settings.cache_length == 0)
    return NULL;

  v = &target->node_cache;

  if (target->free == NULL && target->first == NULL) {
    zero = 0;
    if (!init_vector(v, LINKED_LIST_HEADER_SIZE + settings.element_size, settings.cache_length))
      return NULL;
  }
  else {
    zero = vector_capacity(v);
    if (!vector_grow(v))
      return NULL;
  }

  length = vector_capacity(v);

  for (idx = zero; idx < length; ++idx) {
    node = (struct linked_list_node *)vector_ref(v, idx);
    dl_memory_set(node, 0, LINKED_LIST_HEADER_SIZE);
    _linked_list_node_free(target, node);
  }

  return target;
}

dl_api linked_list *init_linked_list_custom(linked_list * dl_restrict target, linked_list_settings settings) {
  if (dl_safety(target == NULL || settings.element_size < 1))
    return NULL;

  target->first = target->last = target->free = NULL;
  target->settings = settings;

  return _linked_list_cache_grow(target);
}

dl_api dl_natural linked_list_copy(linked_list * dl_restrict target, struct linked_list_node *dl_restrict target_position, const linked_list * dl_restrict original) {
  struct linked_list_node *source_node, *next;
  dl_natural count;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  source_node = original->first;
  count = 0;

  while (source_node != NULL) {
    next = linked_list_add(target, target_position, LINKED_LIST_DATA(source_node));
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    source_node = source_node->next;
    ++count;
  }

  return count;
}

dl_api dl_natural linked_list_copy_array(linked_list * dl_restrict target, struct linked_list_node *target_position, const dl_byte *data, dl_natural length) {
  dl_natural count, byte_size, idx;
  struct linked_list_node *next;
  
  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  count = 0;
  byte_size = target->settings.element_size;

  for (idx = 0; idx < length; ++idx) {
    next = linked_list_add(target, target_position, (dl_byte *)&data[idx * byte_size]);
    if (dl_unlikely(next == NULL))
      break;

    target_position = next;
    ++count;
  }

  return count;
}

dl_api void destroy_linked_list(linked_list * dl_restrict target, dl_handler *dl_restrict deconstruct_entry) {
  struct linked_list_node *node;
  
  if (dl_safety(target == NULL))
    return;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    node = target->first;
    while (node != NULL) {
      deconstruct_entry->func(deconstruct_entry->data, LINKED_LIST_DATA(node));
      node = node->previous;
    }
  }

  destroy_vector(&target->node_cache, NULL);

  dl_memory_set(target, 0, sizeof(linked_list));
}

dl_api dl_natural linked_list_capacity(const linked_list * dl_restrict list) {
  if (dl_safety(list == NULL))
    return 0;

  return vector_capacity(&list->node_cache);
}

dl_api dl_natural linked_list_length(const linked_list * dl_restrict list) {
  dl_natural length;
  struct linked_list_node *node;
  
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

dl_api dl_bool linked_list_grow(linked_list * dl_restrict list) {
  return _linked_list_cache_grow(list) != NULL;
}

dl_api dl_bool linked_list_shrink(linked_list * dl_restrict list, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;

  return vector_shrink(&list->node_cache, &destructor);
}

dl_api dl_bool linked_list_resize(linked_list * dl_restrict list, dl_natural minimum_capacity, dl_handler *dl_restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  dl_handler destructor;
  
  if (dl_safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;
  
  return vector_resize(&list->node_cache, minimum_capacity, &destructor);
}

dl_api dl_any linked_list_get(const linked_list * dl_restrict list, struct linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return dl_memory_copy(out, LINKED_LIST_DATA(position), list->settings.element_size);
}

dl_api const dl_any linked_list_ref(const struct linked_list_node *position) {
  if (dl_safety(position == NULL))
    return NULL;

  return (const dl_any)LINKED_LIST_DATA(position);
}

dl_api dl_any linked_list_set(linked_list * dl_restrict list, struct linked_list_node *position, dl_any value) {
  if (dl_safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return dl_memory_copy(LINKED_LIST_DATA(position), value, list->settings.element_size);
}

dl_api struct linked_list_node *linked_list_add(linked_list * dl_restrict list, struct linked_list_node *position, dl_any value) {
  struct linked_list_node *node;
  
  if (dl_safety(list == NULL))
    return NULL;

  if (list->free == NULL)
    return NULL;

  node = _linked_list_node_alloc(list, position);
  if (dl_unlikely(value != NULL && !dl_memory_copy(LINKED_LIST_DATA(node), value, list->settings.element_size))) {
    _linked_list_node_free(list, node);
    return NULL;
  }

  return node;
}

dl_api dl_any linked_list_remove(linked_list * dl_restrict list, struct linked_list_node *position, dl_any out) {
  if (dl_safety(list == NULL || position == NULL))
    return NULL;

  if (dl_unlikely(!dl_memory_copy(out, LINKED_LIST_DATA(position), list->settings.element_size)))
    return NULL;

  _linked_list_node_free(list, position);

  return out;
}

dl_api dl_bool linked_list_destroy(linked_list * dl_restrict list, struct linked_list_node *position, dl_handler *deconstruct_entry) {
  if (dl_safety(list == NULL || position == NULL))
    return false;
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, LINKED_LIST_DATA(position));

  _linked_list_node_free(list, position);

  return true;
}

dl_api dl_natural linked_list_destroy_range(linked_list * dl_restrict list, struct linked_list_node *position, dl_natural count, dl_handler *deconstruct_entry) {
  dl_natural removed;
  struct linked_list_node *next;
  
  if (dl_safety(list == NULL || position == NULL || count == 0))
    return 0;

  removed = 0;
  while (removed < count) {
    next = position->next;
    if (!linked_list_destroy(list, position, deconstruct_entry))
      break;

    position = next;
    ++removed;
  }

  return removed;
}

dl_api dl_bool linked_list_swap(linked_list * dl_restrict list, struct linked_list_node *position1, struct linked_list_node *position2, dl_bool data) {
  struct linked_list_node *t;
  
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
    dl_memory_swap(LINKED_LIST_DATA(position1), LINKED_LIST_DATA(position2), list->settings.element_size);


  return true;
}

dl_api struct linked_list_node *linked_list_index(linked_list * dl_restrict list, dl_natural position) {
  struct linked_list_node *node;
  
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

dl_api dl_integer iterator_compare(const collection *dl_restrict col, iterator left, iterator right) {
  if (!iterator_is_valid(col, left) && !iterator_is_valid(col, right))
    return 0;

  if (iterator_is_valid(col, left) && !iterator_is_valid(col, right))
    return 1;

  if (!iterator_is_valid(col, left) && iterator_is_valid(col, right))
    return -1;

  return col->settings.functions->_iterator_compare(col, left, right);
}

dl_api dl_bool iterator_equal(const collection *dl_restrict col, const iterator left, const iterator right) {
  return 0 == iterator_compare(col, left, right);
}

dl_api dl_bool iterator_is_valid(const collection *dl_restrict col, const iterator index) {
  if (dl_safety(col == NULL))
    return false;

  return col->settings.functions->_iterator_is_valid(col, index);
}

dl_api iterator make_invalid_iterator(const collection *dl_restrict col) {
  return col->settings.functions->_make_invalid_iterator(col);
}



/*****************************************************************************
 **  Container Dependent
 ****************************************************************************/

dl_api void _force_collection_properties(collection *dl_restrict col) {
  iterator current, next;
  dl_any ref;
  
  if (collection_is_sorted(col))
    collection_sort(col);
  if (collection_is_set(col)) {
    for (ref = collection_begin_ref(col, &current); ref != NULL; ref = collection_next(col, &current)) {
      next = current;
      if (!collection_next(col, &next))
        break;
      while (iterator_is_valid(col, next) && 0 == col->settings.comparer.func(col->settings.comparer.data, ref, collection_ref(col, next)))
        collection_destroy_at(col, &next);
    }
  }
}

dl_api dl_any collection_push_start(collection *dl_restrict col, iterator *iter) {
  if (dl_safety(col == NULL))
    return NULL;

  return col->settings.functions->_collection_push_start(col, iter);
}

dl_api dl_any collection_push_finish(collection *dl_restrict col, iterator *iter) {
  dl_comparator *comp;
  dl_bool is_set;
  dl_any ref, prev_ref;
  iterator prev;
  dl_integer c;

  if (dl_safety(col == NULL || iter == NULL || !iterator_is_valid(col, *iter)))
    return NULL;

  if (collection_is_sorted(col)) {
    comp = &col->settings.comparer;
    is_set = collection_is_set(col);

    while (iterator_is_valid(col, *iter)) {
      ref = collection_ref(col, *iter);
      if (ref == NULL)
        break;

      prev = *iter;
      prev_ref = collection_prev(col, &prev);

      if (prev_ref == NULL)
        break;

      c = comp->func(comp->data, prev_ref, ref);

      if (c < 0)
        break;

      if (is_set && c == 0) {
        collection_destroy_at(col, iter);
        return NULL;
      }

      collection_swap(col, &prev, iter);
    }
  }

  return collection_ref(col, *iter);
}

dl_api void _vector_queue_roll_slices(collection *dl_restrict col) {
  dl_byte *slice_left, *slice_first;
  dl_natural last_idx, slice_idx;
  
  if (col->data.vector.index[0] == col->data.vector.index[1]) {
    col->data.vector.index[0] = col->data.vector.index[1] = 0;
    return;
  }

  if (col->data.vector.container.slice_count <= 1)
    return;

  if (col->data.vector.index[0] < col->data.vector.container.settings.slice_length)
    return;

  slice_left = col->data.vector.container.data.slices[0];
  slice_first = slice_left;

  last_idx = col->data.vector.container.slice_count - 1;

  for (slice_idx = 0; slice_idx < last_idx; ++slice_idx)
    col->data.vector.container.data.slices[slice_idx] = col->data.vector.container.data.slices[slice_idx + 1];

  col->data.vector.container.data.slices[last_idx] = slice_first;

  col->data.vector.index[0] -= col->data.vector.container.settings.slice_length;
  col->data.vector.index[1] -= col->data.vector.container.settings.slice_length;
}

dl_api dl_bool collection_is_empty(const collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return true;

  return col->settings.functions->_collection_is_empty(col);
}

dl_api dl_any collection_pop(collection *dl_restrict col, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_pop(col, out);
}

dl_api dl_bool collection_pop_destroy(collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_destroy(col);
}

dl_api dl_bool collection_pop_forget(collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_forget(col);
}

dl_api dl_integer collection_ref_array(collection *dl_restrict col, iterator iter, dl_any *out_array) {  
  if (dl_safety(col == NULL || out_array == NULL))
    return 0;

  if (!iterator_is_valid(col, iter))
    return 0;

  return col->settings.functions->_collection_ref_array(col, iter, out_array);
}

dl_api const dl_any collection_ref(const collection *dl_restrict col, iterator iter) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_ref((collection *dl_restrict )col, iter);
}

dl_api dl_bool collection_swap(collection *dl_restrict col, iterator *iter_a, iterator *iter_b) {
  if (dl_safety(col == NULL || iter_a == NULL || iter_b == NULL))
    return false;

  if (!iterator_is_valid(col, *iter_a) || !iterator_is_valid(col, *iter_b))
    return false;

  return col->settings.functions->_collection_swap(col, iter_a, iter_b);
}

dl_any collection_get(const collection *dl_restrict col, iterator iter, dl_any out) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_get(col, iter, out);
}

dl_any collection_set(collection *dl_restrict col, iterator *iter, dl_any value) {
  if (dl_safety(col == NULL))
    return NULL;

  if (!iterator_is_valid(col, *iter))
    return NULL;

  if (collection_is_set(col) && collection_contains(col, value))
    return NULL;

  if (collection_is_sorted(col)) {
    collection_forget_at(col, iter);
    return collection_push_index(col, value, iter);
  }

  return col->settings.functions->_collection_set(col, iter, value);
}

dl_api iterator collection_index(collection *dl_restrict col, dl_natural index) {
  if (dl_safety(col == NULL || (dl_integer)index >= collection_count(col)))
    return collection_end(col);

  return col->settings.functions->_collection_index(col, index);
}

dl_api const dl_any collection_next(const collection *dl_restrict col, iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL)) {
    *iter = make_invalid_iterator(col);
    return NULL;
  }

  if (!iterator_is_valid(col, *iter)) {
    *iter = make_invalid_iterator(col);
    return NULL;
  }

  col->settings.functions->_collection_next(col, iter);

  return collection_ref(col, *iter);
}

dl_api const dl_any collection_prev(const collection *dl_restrict col, iterator *iter) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  col->settings.functions->_collection_prev(col, iter);
  
  return collection_ref(col, *iter);
}

dl_api dl_integer collection_count(const collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return 0;

  return col->settings.functions->_collection_count(col);
}

dl_api iterator collection_begin(const collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return make_invalid_iterator(col);

  return col->settings.functions->_collection_begin(col);
}

dl_api iterator collection_end(const collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return make_invalid_iterator(col);

  return col->settings.functions->_collection_end(col);
}

const dl_any _collection_search_region_vector(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  iterator iter_next;
  
  if (left.vector.index >= right.vector.index)
    return NULL;

  iter->vector.index = ((right.vector.index - left.vector.index) >> 1) + left.vector.index;

  if (dl_unlikely(!(ref = vector_ref(&col->data.vector.container, iter->vector.index))))
    return NULL;

  outcome = predicate->func(predicate->data, ref);
  
  if (outcome == 0)
    return ref;
  else if (outcome > 0)
    return _collection_search_region_vector(col, predicate, left, *iter, iter);
  else {
    iter_next.vector.index = iter->vector.index + 1;
    return _collection_search_region_vector(col, predicate, iter_next, right, iter);
  }
}

const dl_any _collection_linear_search(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter) {
  dl_any ref;
  dl_integer outcome;
  
  ref = collection_ref(col, left);
  *iter = left;
  while (!iterator_equal(col, *iter, right) && ref != NULL) {
    outcome = predicate->func(predicate->data, ref);
    if (outcome == 0)
      return ref;
    ref = collection_next(col, iter);
  }

  return NULL;
}

const dl_any collection_search_region(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter) {
  if (dl_safety(col == NULL || predicate == NULL || iter == NULL))
    return NULL;

  return col->settings.functions->_collection_search_region(col, predicate, left, right, iter);
}

const dl_any collection_search(const collection *dl_restrict col, dl_filter *predicate, iterator *iter) {
  return collection_search_region(col, predicate, collection_begin(col), collection_end(col), iter);
}

dl_api dl_bool collection_destroy_at(collection *dl_restrict col, iterator *iter) {
  dl_handler *destructor;
  
  if (dl_safety(col == NULL || !iterator_is_valid(col, *iter)))
    return false;

  if (collection_is_empty(col))
    return false;

  destructor = col->settings.deconstruct_entry.func == NULL ? NULL : &col->settings.deconstruct_entry;

  return col->settings.functions->_collection_destroy_at(col, iter, destructor);
}

dl_api dl_bool collection_forget_at(collection *dl_restrict col, iterator *iter) {
  if (dl_safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_destroy_at(col, iter, NULL);
}

dl_api dl_any collection_remove_at(collection *dl_restrict col, iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || out == NULL))
    return NULL;

  if (collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_remove_at(col, iter, out);
}

dl_api dl_integer collection_destroy_range(collection *dl_restrict col, iterator *index, dl_natural count) {
  if (dl_safety(col == NULL || !iterator_is_valid(col, *index)))
    return 0;

  return col->settings.functions->_collection_destroy_range(col, index, count);
}

dl_any collection_insert(collection *dl_restrict col, iterator *dl_restrict position, dl_any item) {
  if (dl_safety(col == NULL || !iterator_is_valid(col, *position)))
    return NULL;

  if (collection_is_set(col) && collection_contains(col, item))
    return NULL;

  if (collection_is_sorted(col))
    return collection_push_index(col, item, position);

  if (col->settings.functions->_collection_insert(col, position, item))
    return collection_ref(col, *position);
  return NULL;
}

dl_api dl_natural collection_element_size(const collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return 0;

  return col->settings.element_size;
}



/*****************************************************************************
 **  Container Independent
 ****************************************************************************/

dl_api dl_bool collection_is_vector(const collection * dl_restrict col) {
  return col != NULL && col->settings.storage == STORAGE_TYPE_VECTOR;
}

dl_api dl_bool collection_is_linked_list(const collection * dl_restrict col) {
  return col != NULL && col->settings.storage == STORAGE_TYPE_LINKED_LIST;
}

dl_api dl_bool collection_is_queue(const collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_QUEUE);
}

dl_api dl_bool collection_is_set(const collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_SET);
}

dl_api dl_bool collection_is_sorted(const collection *dl_restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_SORTED_LIST
     || col->settings.type == COLLECTION_TYPE_SET);
}

dl_api const dl_any collection_begin_ref(const collection *dl_restrict col, iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = collection_begin(col);
  return collection_ref(col, *iter);
}

dl_api const dl_any collection_end_ref(const collection *dl_restrict col, iterator *iter) {
  if (dl_safety(iter == NULL || col == NULL))
    return NULL;

  *iter = collection_end(col);
  return collection_prev(col, iter);
}

dl_api dl_bool collection_sort(collection *dl_restrict col) {
  if (dl_safety(col == NULL))
    return false;

  return collection_quick_sort(col, &col->settings.comparer);
}

dl_api void collection_clear(collection *dl_restrict col) {
  while (collection_pop_destroy(col));
}

dl_api const dl_any collection_peek(const collection *dl_restrict col) {
  iterator iter;
  
  if (dl_safety(col == NULL))
    return NULL;

  if (collection_is_empty(col))
    return NULL;

  if (collection_is_queue(col)) {
    iter = collection_begin(col);

    return collection_ref(col, iter);
  }
  else {
    iter = collection_end(col);
    return collection_prev(col, &iter);
  }
}

dl_api iterator collection_index_of(const collection *dl_restrict col, const dl_any item) {
  iterator bad, iter, index;
  dl_any candidate;
  _collection_sorted_list_predicate_data data;
  dl_filter predicate;
  
  bad = make_invalid_iterator(col);

  if (dl_safety(col == NULL))
    return bad;

  if (collection_is_empty(col))
    return bad;

  if (collection_is_sorted(col)) {
    data.c = col->settings.comparer;
    data.item = item;

    predicate.func = _collection_sorted_list_predicate_func;
    predicate.data = &data;

    iter = collection_begin(col);
    return NULL != collection_search(col, &predicate, &iter) ? iter : bad;
  }
  else {
    index = collection_begin(col);
    for (candidate = collection_ref(col, index); candidate != NULL; candidate = collection_next(col, &index)) {
      if (0 == col->settings.comparer.func(col->settings.comparer.data, item, candidate))
        return index;
    }
    return bad;
  }
}

dl_api dl_bool collection_all(const collection *dl_restrict col, dl_filter *dl_restrict f) {
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index))
    if (!f->func(f->data, item))
      return false;

  return true;
}

dl_api dl_bool collection_any(const collection *dl_restrict col, dl_filter *dl_restrict f) {
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL))
    return false;

  index = collection_begin(col);
  for (item = collection_ref(col, index);
       item != NULL;
       item = collection_next(col, &index))
    if (f->func(f->data, item))
      return true;

  return false;
}

dl_api dl_integer collection_drop(collection *dl_restrict col, dl_natural count) {
  dl_natural removed = 0;

  if (dl_safety(col == NULL))
    return 0;

  for (; count > 0; --count, ++removed)
    if (!collection_pop_destroy(col))
      return removed;

  return removed;
}

dl_api dl_integer collection_map(const collection *dl_restrict col, dl_converter *func, collection *out) {
  iterator iter, new_iter;
  dl_integer count;
  dl_any new_ref;
  dl_any ref;
  
  if (dl_safety(col == NULL || func == NULL || out == NULL))
    return 0;

  count = 0;
  for (ref = collection_begin_ref(col, &iter); ref != NULL; ref = collection_next(col, &iter)) {
    new_ref = collection_push_start(out, &new_iter);
    if (!iterator_is_valid(out, new_iter))
      return -count;

    func->func(func->data, ref, new_ref);

    collection_push_finish(out, &new_iter);
    ++count;
  }

  return count;
}

dl_api dl_any collection_foldl(const collection *dl_restrict col, dl_any initial, dl_folder *func) {
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_any collection_foldr(const collection *dl_restrict col, dl_any initial, dl_folder *func) {
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || func == NULL))
    return NULL;

  index = collection_end(col);

  while ((item = collection_prev(col, &index)))
    func->func(func->data, initial, item);

  return initial;
}

dl_api dl_integer collection_zip(const collection *dl_restrict col1, const collection *dl_restrict col2, dl_zipper *zip, collection *out) {
  iterator iter1, iter2, new_iter;
  dl_integer added;
  dl_any ref1, ref2, new_ref;
  
  if (dl_safety(col1 == NULL || col2 == NULL || zip == NULL || out == NULL))
    return 0;

  added = 0;
  for (ref1 = collection_begin_ref(col1, &iter1), ref2 = collection_begin_ref(col2, &iter2);
       ref1 != NULL && ref2 != NULL;
       ref1 = collection_next(col1, &iter1), ref2 = collection_next(col2, &iter2)) {
    new_ref = collection_push_start(out, &new_iter);
    if (!iterator_is_valid(out, new_iter))
      return -added;

    zip->func(zip->data, ref1, ref2, new_ref);

    collection_push_finish(out, &new_iter);
    ++added;
  }

  return added;
}

dl_api dl_integer collection_take(collection *dl_restrict col, dl_natural count, collection *out) {
  dl_any ref;
  dl_natural added;
  if (dl_safety(col == NULL || out == NULL))
    return 0;

  added = 0;
  while ((ref = collection_peek(col)) && count > added) {
    if (!(ref = collection_push(out, ref)))
      break;
    collection_pop_forget(col);
    ++added;
  }

  return added;
}

dl_api dl_any collection_find(const collection *dl_restrict col, dl_filter *f, iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  for (item = collection_ref(col, *index); item != NULL; item = collection_next(col, index))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_any collection_find_last(const collection *dl_restrict col, dl_filter *f, iterator *index) {
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  while ((item = collection_prev(col, index)))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

dl_api dl_integer collection_find_all(const collection *dl_restrict col, dl_filter *f, collection *out) {
  dl_natural start_count;
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL || f == NULL || out == NULL))
    return 0;

  start_count = collection_count(out);

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index)) {
    if (f->func(f->data, item) && !collection_push(out, item))
      break;
  }

  return collection_count(out) - start_count;
}

dl_api dl_any collection_remove_first(collection *dl_restrict col, dl_filter *f, iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!collection_find(col, f, iter))
    return NULL;

  return collection_remove_at(col, iter, out);
}

dl_api dl_any collection_remove_last(collection *dl_restrict col, dl_filter *f, iterator *iter, dl_any out) {
  if (dl_safety(col == NULL || iter == NULL))
    return NULL;

  if (!collection_find_last(col, f, iter))
    return NULL;

  return collection_remove_at(col, iter, out);
}

dl_api dl_bool collection_destroy_first(collection *dl_restrict col, dl_filter *f, iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!collection_find(col, f, index))
    return false;

  return collection_destroy_at(col, index);
}

dl_api dl_bool collection_destroy_last(collection *dl_restrict col, dl_filter *f, iterator *index) {
  if (dl_safety(col == NULL || index == NULL))
    return false;

  if (!collection_find_last(col, f, index))
    return false;

  return collection_destroy_at(col, index);
}

dl_bool collection_quick_sort_region(collection *dl_restrict col, dl_comparator *compare, iterator left, iterator right) {
  iterator pivot, leftwall, iter;
  dl_any pivot_ref, iter_ref;
  
  if (dl_safety(col == NULL || col->settings.comparer.func == NULL))
    return false;

  if (iterator_equal(col, left, right)
      || (!iterator_is_valid(col, left)
	  && !iterator_is_valid(col, right)))
    return true;

  /* Partition */

  pivot = left;
  leftwall = left;
  iter = left;

  pivot_ref = collection_ref(col, pivot);

  for (iter_ref = collection_next(col, &iter);
       iter_ref != NULL && !iterator_equal(col, iter, right);
       iter_ref = collection_next(col, &iter)) {
    if (compare->func(compare->data, iter_ref, pivot_ref) < 0) {
      if (!collection_swap(col, &iter, &leftwall))
        return false;
      if (iterator_equal(col, pivot, leftwall)) {
        pivot = iter;
        pivot_ref = collection_ref(col, pivot);
      }
      if (!collection_next(col, &leftwall))
        return false;
    }
  }

  if (!collection_swap(col, &pivot, &leftwall))
    return false;

  /* Recurse */

  if (!collection_quick_sort_region(col, compare, left, leftwall))
    return false;

  if (!collection_next(col, &leftwall))
    return true;

  return collection_quick_sort_region(col, compare, leftwall, right);
}

dl_api dl_bool collection_quick_sort(collection *dl_restrict col, dl_comparator *compare) {
  return collection_quick_sort_region(col, compare, collection_begin(col), collection_end(col));
}

dl_api dl_integer collection_destroy_all(collection *dl_restrict col, dl_filter *f) {
  dl_natural count;
  dl_any item;
  iterator iter;
  
  if (dl_safety(col == NULL || f == NULL))
    return 0;

  if (collection_is_empty(col))
    return 0;

  count = 0;
  iter = collection_begin(col);

  while ((item = collection_ref(col, iter))) {
    if (f->func(f->data, item)) {
      collection_destroy_at(col, &iter);
      count++;
    }
    else
      collection_next(col, &iter);
  }

  return count;
}

dl_api dl_bool collection_contains(const collection *dl_restrict col, dl_any item) {
  iterator iter;
  iter= collection_index_of(col, item);
  return iterator_is_valid(col, iter);
}

dl_api dl_any collection_push(collection *dl_restrict col, dl_any value) {
  iterator iter;
  return collection_push_index(col, value, &iter);
}

dl_api dl_any collection_push_index(collection *dl_restrict col, dl_any value, iterator *out_index) {
  dl_any ref;
  
  ref = collection_push_start(col, out_index);
  if (!iterator_is_valid(col, *out_index))
    return NULL;

  dl_memory_copy(ref, value, collection_element_size(col));
  return collection_push_finish(col, out_index);
}

dl_api dl_integer collection_remove_range(collection *dl_restrict col, iterator *index, dl_natural count, collection *out) {
  dl_natural total;
  iterator new_iter;
  dl_any ref;
  
  if (dl_safety(col == NULL || out == NULL || index == NULL || !iterator_is_valid(col, *index)))
    return 0;

  total = 0;
  while (total < count && iterator_is_valid(col, *index)) {
    ref = collection_push_start(out, &new_iter);
    if (!ref)
      break;

    if (!collection_remove_at(col, index, ref)) {
      collection_forget_at(col, &new_iter);
      break;
    }
    else
      collection_push_finish(out, &new_iter);
    ++total;
  }

  return total;
}


dl_api dl_integer collection_remove_all(collection *dl_restrict col, dl_filter *f, collection *out) {
  iterator index, new_iter;
  dl_natural total;
  dl_any ref, new_ref;
  
  if (dl_safety(col == NULL || out == NULL || f == NULL))
    return 0;

  total = 0;
  for (ref = collection_begin_ref(col, &index); iterator_is_valid(col, index);) {
    if (f->func(f->data, ref)) {
      new_ref = collection_push_start(out, &new_iter);
      if (!ref)
        break;

      if (!collection_remove_at(col, &index, new_ref)) {
        collection_forget_at(col, &new_iter);
        break;
      }
      else
        collection_push_finish(out, &new_iter);

      ref = collection_ref(col, index);
      ++total;
    }
    else
      ref = collection_next(col, &index);
  }

  return total;
}

dl_api dl_integer collection_copy(const collection *dl_restrict original, collection *dl_restrict target) {
  iterator iter, new_iter;
  dl_integer count;
  dl_any ref, new_ref;
  dl_bool is_set;
  
  if (dl_safety(original == NULL || target == NULL))
    return 0;

  is_set = collection_is_set(target);

  count = 0;
  for (ref = collection_begin_ref(original, &iter); ref != NULL; ref = collection_next(original, &iter)) {
    if (is_set && collection_contains(target, ref))
      continue;

    new_ref = collection_push_start(target, &new_iter);
    if (!new_ref) {
      count = -count;
      break;
    }

    ref = collection_ref(original, iter);

    dl_memory_copy(new_ref, ref, collection_element_size(original));
    collection_push_finish(target, &new_iter);

    ++count;
  }

  return count;
}

dl_api dl_integer collection_copy_array(const dl_any data, dl_natural count, collection *dl_restrict target) {
  collection source;

  if (dl_safety(target == NULL || data == NULL))
    return 0;

  if (dl_unlikely(!init_collection_array(&source, COLLECTION_TYPE_LIST, NULL, NULL, data, collection_element_size(target), count)))
    return 0;

  return collection_copy(&source, target);
}



/*****************************************************************************
 **  Container Dispatchers
 ****************************************************************************/

dl_api dl_integer _vector_iterator_compare(const collection *dl_restrict col, iterator left, iterator right) {
  return left.vector.index - right.vector.index;
}

dl_api dl_integer _linked_list_iterator_compare(const collection *dl_restrict col, iterator left, iterator right) {
  return (dl_integer)(left.linked_list.node - right.linked_list.node);
}

dl_api dl_bool _vector_iterator_is_valid(const collection *dl_restrict col, iterator index) {
  return index.vector.index < col->data.vector.index[1]
    && index.vector.index >= col->data.vector.index[0];
}

dl_api dl_bool _linked_list_iterator_is_valid(const collection *dl_restrict col, iterator index) {
  return index.linked_list.node != NULL;
}

dl_api iterator _vector_make_invalid_iterator(const collection *dl_restrict col) {
  iterator bad;
  bad.vector.index = DL_NATURAL_MAX;
  return bad;
}

dl_api iterator _linked_list_make_invalid_iterator(const collection *dl_restrict col) {
  iterator bad;
  bad.linked_list.node = NULL;

  return bad;
}

dl_api dl_any _vector_collection_push_start(collection *dl_restrict col, iterator *iter) {
  if (col->data.vector.index[1] >= vector_capacity(&col->data.vector.container)
      && !vector_grow(&col->data.vector.container))
    return NULL;

  iter->vector.index = col->data.vector.index[1];
  col->data.vector.index[1]++;

  return vector_ref(&col->data.vector.container, iter->vector.index);
}

dl_api dl_any _linked_list_collection_push_start(collection *dl_restrict col, iterator *iter) {
  linked_list *list;
  
  list = &col->data.linked_list.container;
  if ((iter->linked_list.node = linked_list_add(list, col->data.linked_list.container.last, NULL)))
    return linked_list_ref(iter->linked_list.node);
  else {
    if (linked_list_grow(list) && (iter->linked_list.node = linked_list_add(list, col->data.linked_list.container.last, NULL)))
      return linked_list_ref(iter->linked_list.node);
    return NULL;
  }
}

dl_api dl_bool _vector_collection_is_empty(const collection *dl_restrict col) {
  return col->data.vector.index[0] == col->data.vector.index[1];
}

dl_api dl_bool _linked_list_collection_is_empty(const collection *dl_restrict col) {
  return col->data.linked_list.container.first == NULL;
}

dl_api dl_any _vector_collection_pop(collection *dl_restrict col, dl_any out) {
  vector *v;
  dl_natural idx;

  if (collection_is_queue(col)) {
    v = &col->data.vector.container;
    idx = col->data.vector.index[0];

    if (dl_unlikely(!vector_get(v, idx, out)))
      return NULL;

    col->data.vector.index[0]++;

    _vector_queue_roll_slices(col);

    return out;
  }
  else {
    v = &col->data.vector.container;
    idx = collection_end(col).vector.index - 1;

    col->data.vector.index[1] = idx;
    return vector_get(v, idx, out);
  }
}

dl_api dl_any _linked_list_collection_pop(collection * dl_restrict col, dl_any out) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_remove(list, position, out);
}

dl_api dl_bool _vector_collection_pop_destroy(collection *dl_restrict col) {
  dl_natural index;
  dl_any ref;
  
  if (collection_is_queue(col)) {
    index = col->data.vector.index[0];

    if (col->settings.deconstruct_entry.func != NULL) {
      if (!(ref = vector_ref(&col->data.vector.container, index)))
        return false;

      col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
    }

    col->data.vector.index[0]++;

    _vector_queue_roll_slices(col);

    return true;
  }
  else {
    index = collection_end(col).vector.index - 1;

    if (col->settings.deconstruct_entry.func != NULL) {
      if (!(ref = vector_ref(&col->data.vector.container, index)))
        return false;

      col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, ref);
    }

    col->data.vector.index[1] = index;

    return true;
  }
}

dl_api dl_bool _linked_list_collection_pop_destroy(collection *dl_restrict col) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_destroy(list, position, &col->settings.deconstruct_entry);
}

dl_api dl_bool _vector_collection_pop_forget(collection *dl_restrict col) {
  dl_natural index;
  
  if (collection_is_queue(col)) {
    col->data.vector.index[0]++;

    _vector_queue_roll_slices(col);

    return true;
  }
  else {
    index = collection_end(col).vector.index - 1;

    col->data.vector.index[1] = index;

    return true;
  }
}

dl_api dl_bool _linked_list_collection_pop_forget(collection *dl_restrict col) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_destroy(list, position, NULL);
}

dl_api dl_integer _vector_collection_ref_array(collection *dl_restrict col, iterator iter, dl_any *out_array) {
  dl_natural count, limit;
    
  count = vector_ref_array(&col->data.vector.container, iter.vector.index, out_array);
  limit = col->data.vector.index[1] - iter.vector.index;

  return count < limit ? count : limit;
}

dl_api dl_integer _linked_list_collection_ref_array(collection *dl_restrict col, iterator iter, dl_any *out_array) {
  out_array[0] = linked_list_ref(iter.linked_list.node);
  return 1;
}

dl_api const dl_any _vector_collection_ref(const collection *dl_restrict col, iterator iter) {
  return vector_ref(&col->data.vector.container, iter.vector.index);
}

dl_api const dl_any _linked_list_collection_ref(const collection *dl_restrict col, iterator iter) {
  return linked_list_ref(iter.linked_list.node);
}

dl_api dl_bool _vector_collection_swap(collection *dl_restrict col, iterator *iter_a, iterator *iter_b) {
  iterator t;

  if (!vector_swap(&col->data.vector.container, iter_a->vector.index, iter_b->vector.index))
    return false;
  t = *iter_a;
  *iter_a = *iter_b;
  *iter_b = t;
  return true;
}

dl_api dl_bool _linked_list_collection_swap(collection *dl_restrict col, iterator *iter_a, iterator *iter_b) {
  if (!linked_list_swap(&col->data.linked_list.container, iter_a->linked_list.node, iter_b->linked_list.node, false))
    return false;
  return true;
}

dl_any _vector_collection_get(const collection *dl_restrict col, iterator iter, dl_any out) {
  return vector_get(&col->data.vector.container, iter.vector.index, out);
}

dl_any _linked_list_collection_get(const collection *dl_restrict col, iterator iter, dl_any out) {
  return linked_list_get(&col->data.linked_list.container, iter.linked_list.node, out);
}

dl_any _vector_collection_set(collection *dl_restrict col, iterator *iter, dl_any value) {
  dl_any ref;

  if ((ref = vector_set(&col->data.vector.container, iter->vector.index, value)))
    return collection_ref(col, *iter);
  return NULL;
}

dl_any _linked_list_collection_set(collection *dl_restrict col, iterator *iter, dl_any value) {
  dl_any ref;

  if ((ref = linked_list_set(&col->data.linked_list.container, iter->linked_list.node, value)))
    return collection_ref(col, *iter);
  return NULL;
}

dl_api iterator _vector_collection_index(collection *dl_restrict col, dl_natural index) {
  iterator iter;
  iter.vector.index = index;
  return iter;
}

dl_api iterator _linked_list_collection_index(collection *dl_restrict col, dl_natural index) {
  iterator iter;
  iter.linked_list.node = linked_list_index(&col->data.linked_list.container, index);
  return iter;
}

dl_api void _vector_collection_next(const collection *dl_restrict col, iterator *iter) {
  iter->vector.index++;
}

dl_api void _linked_list_collection_next(const collection *dl_restrict col, iterator *iter) {
  iter->linked_list.node = iter->linked_list.node->next;
}

dl_api void _vector_collection_prev(const collection *dl_restrict col, iterator *iter) {
  if (!iterator_is_valid(col, *iter))
    iter->vector.index = col->data.vector.index[1] - 1;
  else if (iter->vector.index > col->data.vector.index[0])
    iter->vector.index--;
  else
    *iter = make_invalid_iterator(col);
}

dl_api void _linked_list_collection_prev(const collection *dl_restrict col, iterator *iter) {
  if (!iterator_is_valid(col, *iter))
    iter->linked_list.node = col->data.linked_list.container.last;
  else if (iter->linked_list.node != col->data.linked_list.container.first)
    iter->linked_list.node = iter->linked_list.node->previous;
  else
    *iter = make_invalid_iterator(col);
}

dl_api dl_integer _vector_collection_count(const collection *dl_restrict col) {
  return col->data.vector.index[1] - col->data.vector.index[0];
}

dl_api dl_integer _linked_list_collection_count(const collection *dl_restrict col) {
  return linked_list_length(&col->data.linked_list.container);
}

dl_api iterator _vector_collection_begin(const collection *dl_restrict col) {
  iterator iter;
  iter.vector.index = col->data.vector.index[0];
  return iter;
}

dl_api iterator _linked_list_collection_begin(const collection *dl_restrict col) {
  iterator iter;
  iter.linked_list.node = col->data.linked_list.container.first;
  return iter;
}

dl_api iterator _vector_collection_end(const collection *dl_restrict col) {
  iterator iter;
  iter.vector.index = col->data.vector.index[1];
  return iter;
}

dl_api iterator _linked_list_collection_end(const collection *dl_restrict col) {
  iterator iter;
  iter.linked_list.node = NULL;
  return iter;
}

dl_api const dl_any _vector_collection_search_region(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter) {
  if (collection_is_sorted(col))
    return _collection_search_region_vector(col, predicate, left, right, iter);
  return _collection_linear_search(col, predicate, left, right, iter);
}

dl_api const dl_any _linked_list_collection_search_region(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter) {
  return _collection_linear_search(col, predicate, left, right, iter);
}

dl_api dl_bool _vector_collection_destroy_at(collection *dl_restrict col, iterator *iter, dl_handler *destructor) {
  vector *v;
  dl_natural index, swap_index;
  dl_any item;
  
  v = &col->data.vector.container;
  index = iter->vector.index;

  if (destructor != NULL) {
    if (dl_unlikely(!(item = vector_ref(v, index))))
      return false;
    destructor->func(destructor->data, item);
  }

  if (collection_count(col) > 1)
    for (swap_index = col->data.vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
      vector_swap(v, swap_index, index);
  col->data.vector.index[1]--;

  return true;
}

dl_api dl_bool _linked_list_collection_destroy_at(collection *dl_restrict col, iterator *iter, dl_handler *destructor) {
  struct linked_list_node *next;
  next = iter->linked_list.node->next;
  if (linked_list_destroy(&col->data.linked_list.container, iter->linked_list.node, destructor)) {
    iter->linked_list.node = next;
    return true;
  }
  return false;
}
  
dl_api dl_any _vector_collection_remove_at(collection *dl_restrict col, iterator *iter, dl_any out) {
  vector *v;
  dl_natural index, swap_index;
  
  v = &col->data.vector.container;
  index = iter->vector.index;

  if (!(out = vector_get(v, index, out)))
    return NULL;

  if (collection_count(col) > 1)
    for (swap_index = col->data.vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
      vector_swap(v, swap_index, index);
  col->data.vector.index[1]--;

  return out;
}

dl_api dl_any _linked_list_collection_remove_at(collection *dl_restrict col, iterator *iter, dl_any out) {
  struct linked_list_node *next;
  
  next = iter->linked_list.node->next;
  if ((out = linked_list_remove(&col->data.linked_list.container, iter->linked_list.node, out))) {
    iter->linked_list.node = next;
    return out;
  }

  return NULL;
}

  
dl_api dl_integer _vector_collection_destroy_range(collection *dl_restrict col, iterator *iter, dl_natural count) {
  dl_natural index, start, total;
  dl_handler destruct;
  vector *v;
  dl_any item;
  
  index = iter->vector.index;
  start = index;
  v = &col->data.vector.container;

  total = 0;
  destruct = col->settings.deconstruct_entry;

  for (item = vector_ref(v, index);
       item != NULL && total < count;
       ++index, item = vector_ref(v, index), ++total)
    if (destruct.func != NULL)
      destruct.func(destruct.data, item);

  if (total != 0) {
    do {
      vector_swap(v, start, index);
      ++start;
      ++index;
    } while (NULL != vector_ref(v, index));
  }

  col->data.vector.index[1] -= total;

  return total;
}
  
dl_api dl_integer _linked_list_collection_destroy_range(collection *dl_restrict col, iterator *index, dl_natural count) {
  struct linked_list_node *next;
  dl_natural idx;

  next = index->linked_list.node;
  for (idx = 0; idx < count && next != NULL; ++idx)
    next = next->next;
  count = linked_list_destroy_range(&col->data.linked_list.container, index->linked_list.node, count, &col->settings.deconstruct_entry);
  index->linked_list.node = next;
  return count;
}

dl_bool _vector_collection_insert(collection *dl_restrict col, iterator *dl_restrict position, dl_any item) {
  iterator index, next;
  
  index = collection_end(col);

  if (!collection_push(col, item))
    return false;

  while (!iterator_equal(col, index, *position)) {
    next = index;
    collection_prev(col, &next);
    collection_swap(col, &next, &index);
  }

  return true;
}

dl_bool _linked_list_collection_insert(collection *dl_restrict col, iterator *dl_restrict position, dl_any item) {
  struct linked_list_node *node;
  linked_list *list;

  node = position->linked_list.node;
  list = &col->data.linked_list.container;

  position->linked_list.node = linked_list_add(list, node, item);

  return true;
}



/*****************************************************************************
 **  Init/Destroy Collections
 ****************************************************************************/

struct collection_dispatch_functions default_vector_collection_dispatch_functions = {
  (dl_integer (*)(const collection *dl_restrict col, iterator left, iterator right))_vector_iterator_compare,
  (dl_bool (*)(const collection *dl_restrict col, iterator index))_vector_iterator_is_valid,
  (iterator (*)(const collection *dl_restrict col))_vector_make_invalid_iterator,
  (dl_any (*)(collection *dl_restrict col, iterator *iter))_vector_collection_push_start,
  (dl_bool (*)(const collection *dl_restrict col))_vector_collection_is_empty,
  (dl_any (*)(const collection *dl_restrict col, dl_any out))_vector_collection_pop,
  (dl_bool (*)(collection *dl_restrict col))_vector_collection_pop_destroy,
  (dl_bool (*)(collection *dl_restrict col))_vector_collection_pop_forget,
  (dl_integer (*)(collection *dl_restrict col, iterator iter, dl_any *out_array))_vector_collection_ref_array,
  (dl_any (*)(collection *dl_restrict col, iterator iter))_vector_collection_ref,
  (dl_bool (*)(collection *dl_restrict col, iterator *iter_a, iterator *iter_b))_vector_collection_swap,
  (dl_any (*)(const collection *dl_restrict col, iterator iter, dl_any out))_vector_collection_get,
  (dl_any (*)(collection *dl_restrict col, iterator *iter, dl_any value))_vector_collection_set,
  (iterator (*)(collection *dl_restrict col, dl_natural index))_vector_collection_index,
  (void (*)(const collection *dl_restrict col, iterator *iter))_vector_collection_next,
  (void (*)(const collection *dl_restrict col, iterator *iter))_vector_collection_prev,
  (dl_integer (*)(const collection *dl_restrict col))_vector_collection_count,
  (iterator (*)(const collection *dl_restrict col))_vector_collection_begin,
  (iterator (*)(const collection *dl_restrict col))_vector_collection_end,
  (const dl_any (*)(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter))_vector_collection_search_region,
  (dl_bool (*)(collection *dl_restrict col, iterator *iter, dl_handler *destructor))_vector_collection_destroy_at,
  (dl_any (*)(collection *dl_restrict col, iterator *iter, dl_any out))_vector_collection_remove_at,
  (dl_integer (*)(collection *dl_restrict col, iterator *iter, dl_natural count))_vector_collection_destroy_range,
  (dl_bool (*)(collection *dl_restrict col, iterator *dl_restrict position, dl_any item))_vector_collection_insert
};

struct collection_dispatch_functions default_linked_list_collection_dispatch_functions = {
  (dl_integer (*)(const collection *dl_restrict col, iterator left, iterator right))_linked_list_iterator_compare,
  (dl_bool (*)(const collection *dl_restrict col, iterator index))_linked_list_iterator_is_valid,
  (iterator (*)(const collection *dl_restrict col))_linked_list_make_invalid_iterator,
  (dl_any (*)(collection *dl_restrict col, iterator *iter))_linked_list_collection_push_start,
  (dl_bool (*)(const collection *dl_restrict col))_linked_list_collection_is_empty,
  (dl_any (*)(const collection *dl_restrict col, dl_any out))_linked_list_collection_pop,
  (dl_bool (*)(collection *dl_restrict col))_linked_list_collection_pop_destroy,
  (dl_bool (*)(collection *dl_restrict col))_linked_list_collection_pop_forget,
  (dl_integer (*)(collection *dl_restrict col, iterator iter, dl_any *out_array))_linked_list_collection_ref_array,
  (dl_any (*)(collection *dl_restrict col, iterator iter))_linked_list_collection_ref,
  (dl_bool (*)(collection *dl_restrict col, iterator *iter_a, iterator *iter_b))_linked_list_collection_swap,
  (dl_any (*)(const collection *dl_restrict col, iterator iter, dl_any out))_linked_list_collection_get,
  (dl_any (*)(collection *dl_restrict col, iterator *iter, dl_any value))_linked_list_collection_set,
  (iterator (*)(collection *dl_restrict col, dl_natural index))_linked_list_collection_index,
  (void (*)(const collection *dl_restrict col, iterator *iter))_linked_list_collection_next,
  (void (*)(const collection *dl_restrict col, iterator *iter))_linked_list_collection_prev,
  (dl_integer (*)(const collection *dl_restrict col))_linked_list_collection_count,
  (iterator (*)(const collection *dl_restrict col))_linked_list_collection_begin,
  (iterator (*)(const collection *dl_restrict col))_linked_list_collection_end,
  (const dl_any (*)(const collection *dl_restrict col, dl_filter *predicate, iterator left, iterator right, iterator *iter))_linked_list_collection_search_region,
  (dl_bool (*)(collection *dl_restrict col, iterator *iter, dl_handler *destructor))_linked_list_collection_destroy_at,
  (dl_any (*)(collection *dl_restrict col, iterator *iter, dl_any out))_linked_list_collection_remove_at,
  (dl_integer (*)(collection *dl_restrict col, iterator *iter, dl_natural count))_linked_list_collection_destroy_range,
  (dl_bool (*)(collection *dl_restrict col, iterator *dl_restrict position, dl_any item))_linked_list_collection_insert
};

collection_settings default_vector_collection_settings = {
  COLLECTION_TYPE_LIST,
  STORAGE_TYPE_VECTOR,
  {0},
  {0},
  32,
  sizeof(dl_byte),
  &default_vector_collection_dispatch_functions
};

collection_settings default_linked_list_collection_settings = {
  COLLECTION_TYPE_LIST,
  STORAGE_TYPE_LINKED_LIST,
  {0},
  {0},
  32,
  sizeof(dl_byte),
  &default_linked_list_collection_dispatch_functions
};

void _check_init_collection(collection *dl_restrict col, collection_settings settings, dl_natural count) {
  col->settings = settings;
  
  if (col->settings.comparer.func == NULL) {
    switch (settings.element_size) {
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

  if (col->settings.storage == STORAGE_TYPE_VECTOR) {
    col->data.vector.index[0] = 0;
    col->data.vector.index[1] = count;
  }
}

dl_api collection *init_collection(collection *dl_restrict col, collection_type type, storage_type storage, dl_comparator *compare, dl_handler *destructor, dl_natural element_size) {
  collection_settings settings;
  
  if (dl_safety(col == NULL || element_size < 1))
    return NULL;

  switch (storage) {
  case STORAGE_TYPE_VECTOR: {
    settings = default_vector_collection_settings;
    break;
  }
  case STORAGE_TYPE_LINKED_LIST: {
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

  _check_init_collection(col, settings, 0);
  
  switch (settings.storage) {
  case STORAGE_TYPE_VECTOR: {
    if (!init_vector(&col->data.vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  case STORAGE_TYPE_LINKED_LIST: {
    if (!init_linked_list(&col->data.linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  }
  default:
    return NULL;
  }
  
  _force_collection_properties(col);

  return col;
}

dl_api collection *init_collection_custom(collection *dl_restrict col, collection_settings settings, dl_natural element_size) {
  if (dl_safety(col == NULL))
    return NULL;

  _check_init_collection(col, settings, 0);

  switch (col->settings.storage) {
  case STORAGE_TYPE_VECTOR:
    if (!init_vector(&col->data.vector.container, element_size, col->settings.capacity))
      return NULL;
    break;
  case STORAGE_TYPE_LINKED_LIST:
    if (!init_linked_list(&col->data.linked_list.container, element_size, col->settings.capacity))
      return NULL;
    break;
  default:
    return NULL;
  }

  _force_collection_properties(col);

  return col;
}

dl_api collection *init_collection_array(collection *dl_restrict col, collection_type type, dl_comparator *comp, dl_handler *deconstruct_entry, dl_byte *data, dl_natural element_size, dl_natural count) {
  collection_settings settings;
  
  if (dl_safety(col == NULL || data == NULL))
    return NULL;

  if (!init_vector_array(&col->data.vector.container, data, element_size, count))
    return NULL;

  settings = default_vector_collection_settings;
  settings.storage = STORAGE_TYPE_VECTOR;
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

  _check_init_collection(col, settings, count);

  _force_collection_properties(col);

  return col;
}

dl_api void destroy_collection(collection *dl_restrict col) {
  iterator index;
  dl_any item;
  
  if (dl_safety(col == NULL))
    return;

  switch (col->settings.storage) {
  case STORAGE_TYPE_VECTOR:
    if (col->settings.deconstruct_entry.func != NULL) {
      index = collection_begin(col);
      for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index))
	col->settings.deconstruct_entry.func(col->settings.deconstruct_entry.data, item);
    }

    destroy_vector(&col->data.vector.container, NULL);
    break;
  case STORAGE_TYPE_LINKED_LIST:
    destroy_linked_list(&col->data.linked_list.container, &col->settings.deconstruct_entry);
    break;
  }
}


#endif /* DL_USE_CONTAINERS */
  
#endif /* DL_IMPLEMENTATION */

#endif

