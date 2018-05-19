#ifndef DL_H
#define DL_H 1



#if defined(__STDC__)
# if __STDC_VERSION__ >= 201112L
#   define IS_C11 1
#   define IS_C99 0
#   define IS_C90 0
#   define IS_C89 0
# elif __STDC_VERSION__ >= 199901L
#   define IS_C11 0
#   define IS_C99 1
#   define IS_C90 0
#   define IS_C89 0
# elif __STDC_VERSION__ >= 199409L
#   define IS_C11 0
#   define IS_C99 0
#   define IS_C90 1
#   define IS_C89 0
# else
#   define IS_C11 0
#   define IS_C99 0
#   define IS_C90 0
#   define IS_C89 1
# endif
#else
# error "C version not determined."
# define IS_C11 0
# define IS_C99 0
# define IS_C90 0
# define IS_C89 0
#endif

#define IS_ATLEAST_C89 (IS_C11 || IS_C99 || IS_C90 || IS_C89)
#define IS_ATLEAST_C90 (IS_C11 || IS_C99 || IS_C90)
#define IS_ATLEAST_C99 (IS_C11 || IS_C99)
#define IS_ATLEAST_C11 (IS_C11)



#if _WIN32 || _WIN64
# define IS_WINDOWS 1
#else
# define IS_WINDOWS 0
#endif

#if __linux__ || __linux || linux
# define IS_LINUX 1
#else
# define IS_LINUX 0
#endif

#if __APPLE__ || __MACH__
# define IS_APPLE 1
#else
# define IS_APPLE 0
#endif

#if _MSC_VER
# define IS_MSC 1
#else
# define IS_MSC 0
#endif

#if __GNUC__ && !__clang__ && !__MINGW32__
# define IS_GNUC 1
#else
# define IS_GNUC 0
#endif

#if __clang__
# define IS_CLANG 1
#else
# define IS_CLANG 0
#endif

#if __MINGW32__
# define IS_MINGW 1
#else
# define IS_MINGW 0
#endif

#if defined(NDEBUG) || defined(_DEBUG)
# define IS_DEBUG 1
#else
# define IS_DEBUG 0
#endif



#ifndef DL_USE_MALLOC
# define DL_USE_MALLOC 1
#endif

#ifndef DL_USE_SAFETY_CHECKS
# define DL_USE_SAFETY_CHECKS IS_DEBUG
#endif

#ifndef DL_USE_LEFT_HANDED
# define DL_USE_LEFT_HANDED 0
#endif

#ifndef DL_USE_LOGGING
# if IS_ATLEAST_C99
#   define DL_USE_LOGGING 1
# else
#   define DL_USE_LOGGING 0
# endif
#endif

#ifndef DL_USE_TEST
# if IS_ATLEAST_C99
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

#ifndef DL_IMPLEMENTATION
# define DL_IMPLEMENTATION 0
#endif

#ifndef DL_BEZIER_DEGREE
# define DL_BEZIER_DEGREE 4
#endif



#if DL_USE_LOGGING
# include <stdarg.h>
# if IS_LINUX || IS_APPLE
#   define HAS_TIME 1
#   include <time.h>
#   include <sys/time.h>
# elif IS_WINDOWS
#   define HAS_TIME 1
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <stdint.h>
# else
#   define HAS_TIME 0
# endif
#endif

#if DL_USE_MATH
# include <math.h>
#endif

#if DL_USE_MALLOC
# include <malloc.h>
#endif



#ifndef inline
# define inline  
# if IS_GNUC || IS_CLANG || IS_MINGW
#   undef inline
#   define inline inline __attribute__((__always_inline__))
# endif
#endif

#ifndef api
# define api  
# if IS_GNUC || IS_CLANG || IS_MINGW
#   undef api
#   define api
# endif
#endif

#ifndef restrict
# define restrict
# if IS_GNUC || IS_CLANG || IS_MINGW
#   undef restrict
#   define restrict __restrict__
# endif
# if IS_MSC
#   undef restrict
#   define restrict __restrict
# endif
#endif

#ifndef likely
# define likely(x) !!(x)
# if IS_GNUC || IS_CLANG || IS_MINGW
#   undef likely
#   define likely(x) __builtin_expect(!!(x), 1)
# endif
#endif

#ifndef unlikely
# define unlikely(x) !!(x)
# if IS_GNUC || IS_CLANG || IS_MINGW
#   undef unlikely
#   define unlikely(x) __builtin_expect(!!(x), 0)
# endif
#endif

#if DL_USE_SAFETY_CHECKS
# if DL_USE_LOGGING
#   define safety(x) (unlikely(x) ? ERROR("Safety triggered") || 1 : 0)
# else
#   define safety(x) (unlikely(x) ? 1 : 0)
# endif
#else
# define safety(x) (1 == 0)
#endif



typedef void* any;
typedef signed long int integer;
typedef float real;
typedef unsigned char byte;
typedef unsigned long int natural;
typedef byte bool;

#define true 1
#define false 0

#define INTEGER_MAX 0x7fffffffL
#define INTEGER_MIN (-INTEGER_MAX - 1L)

#define NATURAL_MAX 0xffffffffUL
#define MATURAL_MIN 0

#define REAL_MAX 3.402823e+38f
#define REAL_MIN 1.175494e-38f

#ifndef NULL
# define NULL 0
#endif



#if defined(__cplusplus)
extern "C" {
#endif

  /*****************************************************************************
   **  Tools
   ****************************************************************************/

  api any memory_swap(any target, any source, natural bytes);
  api any memory_copy(any target, any source, natural bytes);
  api any memory_set(any target, byte val, natural bytes);

  typedef struct {
    integer (*func)(any data, const any value);
    any data;
  } filter;

  typedef struct {
    any (*func)(any data, any value);
    any data;
  } handler;

  typedef struct {
    any (*func)(any data, any item, const any left);
    any data;
  } folder;

  typedef struct {
    integer (*func)(any data, const any left, const any right);
    any data;
  } comparator;

  typedef struct {
    any (*func)(any data, const any left, const any right, any out);
    any data;
  } zipper;

  typedef struct {
    any (*func)(any data, const any left, any out);
    any data;
  } converter;



  /*****************************************************************************
   **  Logging
   ****************************************************************************/

#if DL_USE_LOGGING
# if !IS_ATLEAST_C99
#   error "DL_USE_LOGGING not available for C90 or lower"
#   undef DL_USE_LOGGING
#   define DL_USE_LOGGING 0
# endif
  
  typedef enum {
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_TEST = 4,
    LOG_MESSAGE = 5
  } log_channel;

  extern natural (*active_log_handler)(log_channel, const char *restrict, natural, const char *restrict, const char *restrict);

  natural log_message(log_channel ch, const char *restrict file, natural line, const char *restrict function, const char *restrict fmt, ...);

# if IS_GNUC || IS_CLANG || IS_MSC || IS_MINGW
#   define INFO(...) log_message(LOG_INFO, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define WARN(...) log_message(LOG_WARNING, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define ERROR(...) log_message(LOG_ERROR, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define TEST(...) log_message(LOG_TEST, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define MSG(...) log_message(LOG_MESSAGE, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
# else
#   define INFO(...) log_message(LOG_INFO, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define WARN(...) log_message(LOG_WARNING, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define ERROR(...) log_message(LOG_ERROR, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define TEST(...) log_message(LOG_TEST, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define MSG(...) log_message(LOG_MESSAGE, __FILE__, __LINE__, "", ## __VA_ARGS__)
# endif
#endif /* DL_USE_LOGGING */



  /*****************************************************************************
   **  Testing
   ****************************************************************************/

#if DL_USE_TEST
# if !IS_ATLEAST_C99
#   error "DL_USE_TEST is not available for C90 or lower"
#   undef DL_USE_TEST
#   define DL_USE_TEST 0
# endif
    
  integer test_run(bool (**tests)(), const char **names, integer count);
  integer test_count(bool (**tests)(), integer max);

# define check(predicate, ...) ((predicate) ? true : ERROR(__VA_ARGS__) && false)
# define not_implemented() check(false, "Test is not implemented.")
# define BEGIN_TEST_SUITE(name) \
  void _test_suite_##name(integer *out_count, integer *out_passed) {\
    bool (*tests[256])();\
    const char *test_names[256];\
    integer current = 0;\
    if (out_count == NULL || out_passed == NULL)\
      return;\
    memory_set(tests, 0, sizeof(bool (*)()) * 256);
# define END_TEST_SUITE \
    *out_count = test_count(tests, 256);\
    *out_passed = test_run(tests, test_names, *out_count);\
  }
# define DECLARE_TEST(test_name) \
  bool test_name();\
  tests[current] = test_name;\
  test_names[current] = #test_name;\
  current++;
# define RUN_TEST_SUITE(name)\
  do {\
    integer count, passed;\
    TEST("<<"#name ">>");\
    _test_suite_##name(&count, &passed);\
    TEST("<<" #name ">> %i/%i", passed, count);\
  } while (0)
#endif /* DL_USE_TEST */



  /*****************************************************************************
   **  Math
   ****************************************************************************/

#if DL_USE_MATH
# ifdef M_PI
#   undef M_PI
# endif
# ifdef M_INV_PI
#   undef M_INV_PI
# endif
# ifdef M_E
#   undef M_E
# endif
# ifdef M_INV_E
#   undef M_INV_E
# endif
# ifdef M_EPSILON
#   undef M_EPSILON
# endif
# ifdef min
#   undef min
# endif
# ifdef max
#   undef max
# endif
# ifdef clamp
#   undef clamp
# endif
# ifdef clamp01
#   undef clamp01
# endif
  
  extern const real M_PI;
  extern const real M_INV_PI;
  extern const real M_E;
  extern const real M_INV_E;
  extern const real M_EPSILON;
  
# define min(x, y) ((x) <= (y) ? (x) : (y))
# define max(x, y) ((x) >= (y) ? (x) : (y))
# define clamp(x, a, b) max(min(b, x), a)
# define clamp01(x) clamp(x, 0, 1)

  api bool approximately_equal(real a, real b, real epsilon);
  api integer floor_to_integer(real n);
  api integer ceil_to_integer(real n);
  api integer round_to_integer(real n);
  api real rationalize(real value, natural decimal_points);

  api real degree_to_radian(real degree);
  api real radian_to_degree(real radian);
  api integer factorial(integer n);
  
# if !IS_ATLEAST_C99
#   define _sqrt(v) sqrt(v)
#   define _cos(v) cos(v)
#   define _sin(v) sin(v)
#   define _tan(v) tan(v)
#   define _acos(v) acos(v)
#   define _asin(v) asin(v)
#   define _atan(v) atan(v)
#   define _hypot(a, b) _sqrt((a) * (a) + (b) * (b))
#   define _pow(a, b) pow(a, b)
#   define _exp(v) exp(v)
#   define _floor(v) floor(v)
#   define _ceil(v) ceil(v)
#   define _abs(v) ((v) > 0 ? (v) : -(v))
#else
#   define _sqrt(v) sqrtf(v)
#   define _cos(v) cosf(v)
#   define _sin(v) sinf(v)
#   define _tan(v) tanf(v)
#   define _acos(v) acosf(v)
#   define _asin(v) asinf(v)
#   define _atan(v) atanf(v)
#   define _hypot(a, b) hypotf(a, b)
#   define _pow(a, b) powf(a, b)
#   define _exp(v) expf(v)
#   define _floor(v) floorf(v)
#   define _ceil(v) ceilf(v)
#   define _abs(v) ((v) > 0 ? (v) : -(v))
#endif

  typedef struct {
    integer m, a, c;
    integer seed;
  } random_state;

  api random_state *init_random(random_state *state, integer seed);
  api random_state *init_random_custom(random_state *state, integer m, integer a, integer c, integer seed);
  api random_state *init_random_time(random_state *state);

  api integer random_integer(random_state *state, integer max);
  api integer random_integer_range(random_state *state, integer min, integer max);
  api real random_degree(random_state *state);
  api real random_radian(random_state *state);
  api real random_real(random_state *state, real max);
  api real random_real_range(random_state *state, real min, real max);

  
  
  typedef struct {
    real x, y;
  } vec2;

  extern const vec2 vec2_up;
  extern const vec2 vec2_right;
  extern const vec2 vec2_down;
  extern const vec2 vec2_left;
  extern const vec2 vec2_zero;
  extern const vec2 vec2_one;

  api vec2 *init_vec2(vec2 * restrict v, real x, real y);

  api vec2 *vec2_add(const vec2 *restrict left, const vec2 *restrict right, vec2 *restrict out);
  api vec2 *vec2_sub(const vec2 *restrict left, const vec2 *restrict right, vec2 *restrict out);

  api vec2 *vec2_mul_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out);
  api vec2 *vec2_div_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out);
  api vec2 *vec2_add_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out);
  api vec2 *vec2_sub_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out);

  api vec2 *vec2_normalize(const vec2 *restrict left, vec2 *restrict out);
  api vec2 *vec2_negate(const vec2 *restrict left, vec2 *restrict out);

  api real vec2_dot(const vec2 *restrict left, const vec2 *restrict right);
  api bool vec2_approximately_equal(const vec2 *restrict left, const vec2 *restrict right, real epsilon);
  api real vec2_square_magnitude(const vec2 *restrict left);
  api real vec2_magnitude(const vec2 *restrict left);

  api vec2 *vec2_reflect(const vec2 *restrict left, const vec2 *restrict normal, vec2 *restrict out);
  api vec2 *vec2_refract(const vec2 *restrict left, const vec2 *restrict normal, float eta, vec2 *restrict out);


  
  typedef vec2 point2;
  extern const point2 point2_zero;
  extern const point2 point2_one;

  api point2 *init_point2(point2 * restrict p, real x, real y);

  api point2 *point2_add(const point2 *restrict left, const point2 *restrict right, point2 *restrict out);
  api point2 *point2_sub(const point2 *restrict left, const point2 *restrict right, point2 *restrict out);

  api point2 *point2_negate(const point2 *restrict left, point2 *restrict out);

  api point2 *point2_mul_scalar(const point2 *restrict left, real scalar, point2 *restrict out);
  api point2 *point2_div_scalar(const point2 *restrict left, real scalar, point2 *restrict out);
  api point2 *point2_add_scalar(const point2 *restrict left, real scalar, point2 *restrict out);
  api point2 *point2_sub_scalar(const point2 *restrict left, real scalar, point2 *restrict out);

  api bool point2_approximately_equal(const point2 *restrict left, const point2 *restrict right, real epsilon);

  api real point2_line_orientation(const point2 *restrict point, const point2 *restrict line_a, const point2 *restrict line_b);
  

  
  typedef struct {
    real x, y, z, w;
  } vec4;

  typedef vec4 point3;
  typedef vec4 vec3;

  api vec4 *init_vec4(vec4 * restrict v, real x, real y, real z, real w);

  api vec4 *vec4_add(const vec4 *restrict left, const vec4 *restrict right, vec4 *restrict out);
  api vec4 *vec4_sub(const vec4 *restrict left, const vec4 *restrict right, vec4 *restrict out);

  api real vec4_dot(const vec4 *restrict left, const vec4 *restrict right);
  api vec4 *vec4_normalize(const vec4 *restrict left, vec4 *restrict out);
  api vec4 *vec4_negate(const vec4 *restrict left, vec4 *restrict out);
  api real vec4_square_magnitude(const vec4 *restrict left);
  api real vec4_magnitude(const vec4 *restrict left);
  api bool vec4_approximately_equal(const vec4 *restrict left, const vec4 *restrict right, real epsilon);

  api vec4 *vec4_mul_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out);
  api vec4 *vec4_div_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out);
  api vec4 *vec4_add_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out);
  api vec4 *vec4_sub_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out);


  
  extern const point3 point3_zero;
  extern const point3 point3_one;

  api point3 *init_point3(point3 * restrict p, real x, real y, real z);

  api point3 *point3_add(const point3 *restrict left, const point3 *restrict right, point3 *restrict out);
  api point3 *point3_sub(const point3 *restrict left, const point3 *restrict right, point3 *restrict out);

  api point3 *point3_mul_scalar(const point3 *restrict left, real scalar, point3 *restrict out);
  api point3 *point3_div_scalar(const point3 *restrict left, real scalar, point3 *restrict out);
  api point3 *point3_add_scalar(const point3 *restrict left, real scalar, point3 *restrict out);
  api point3 *point3_sub_scalar(const point3 *restrict left, real scalar, point3 *restrict out);

  api point3 *point3_rotate(const point3 *restrict left, const vec3 *axis, const real angle, point3 *restrict out);

  api point3 *point3_negate(const point3 *restrict left, point3 *restrict out);

  api bool point3_approximately_equal(const point3 *restrict left, const point3 *restrict out, real epsilon);

  /* real point3_triangle_orientation(const point3 *restrict point, const point3 *restrict triangle_a, const point3 *restrict triangle_b, const point3 *restrict triangle_c); */



  extern const vec3 vec3_up;
  extern const vec3 vec3_right;
  extern const vec3 vec3_forward;
  extern const vec3 vec3_down;
  extern const vec3 vec3_left;
  extern const vec3 vec3_backward;
  extern const vec3 vec3_zero;
  extern const vec3 vec3_one;

  api vec3 *init_vec3(vec3 * restrict v, real x, real y, real z);

  api vec3 *vec3_reflect(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out);
  api vec3 *vec3_refract(const vec3 *restrict left, const vec3 *restrict right, float eta, vec3 *restrict out);

  api vec3 *vec3_add(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out);
  api vec3 *vec3_sub(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out);

  api vec3 *vec3_cross(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out);
  api real vec3_dot(const vec3 *restrict left, const vec3 *restrict out);

  api vec3 *vec3_mul_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out);
  api vec3 *vec3_div_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out);
  api vec3 *vec3_add_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out);
  api vec3 *vec3_sub_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out);

  api vec3 *vec3_rotate(const vec3 *restrict left, const vec3 *restrict axis, const real angle, vec3 *restrict out);

  api vec3 *vec3_normalize(const vec3 *restrict left, vec3 *restrict out);
  api bool vec3_normalized(const vec3 *restrict left);
  api vec3 *vec3_negate(const vec3 *restrict left, vec3 *restrict out);

  api bool vec3_approximately_equal(const vec3 *restrict left, const vec3 *restrict out, real epsilon);
  api real vec3_square_magnitude(const vec3 *restrict left);
  api real vec3_magnitude(const vec3 *restrict left);


  
  typedef union {
    real vals[16];
    real ary[4][4];
    vec4 cols[4];
  } mat4;
  
  extern const mat4 mat4_identity;

  api mat4 *init_mat4(mat4 * restrict m, real c0r0, real c0r1, real c0r2, real c0r3, real c1r0, real c1r1, real c1r2, real c1r3, real c2r0, real c2r1, real c2r2, real c2r3, real c3r0, real c3r1, real c3r2, real c3r3);

  api mat4 *init_mat4_cols(mat4 * restrict m, const vec4 *restrict column_a, const vec4 *restrict column_b, const vec4 *restrict column_c, const vec4 *restrict column_d);

  api mat4 *init_mat4_translate(mat4 * restrict m, real x, real y, real z);
  api mat4 *init_mat4_rotate_x(mat4 * restrict m, real radians);
  api mat4 *init_mat4_rotate_y(mat4 * restrict m, real radians);
  api mat4 *init_mat4_rotate_z(mat4 * restrict m, real radians);
  api mat4 *init_mat4_rotate(mat4 * restrict m, const vec3 *restrict axis, real radians);
  api mat4 *init_mat4_scale(mat4 * restrict m, real x, real y, real z);

  api mat4 *init_mat4_look_at(mat4 * restrict m, const vec3 *restrict origin, const vec3 *restrict target, const vec3 *restrict up);
  api mat4 *init_mat4_orthographic(mat4 * restrict m, real yop, real y_bottom, real x_left, real x_right, real z_near, real z_far);
  api mat4 *init_mat4_perspective(mat4 * restrict m, real vertical_fov, real aspect_ratio, real z_near, real z_far);

  api mat4 *mat4_add(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out);
  api mat4 *mat4_sub(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out);
  api mat4 *mat4_mul(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out);

  api vec4 *mat4_mul_vec4(const mat4 *restrict left, const vec4 *restrict right, vec4 *restrict out);
  api vec3 *mat4_mul_vec3(const mat4 *restrict left, const vec3 *restrict right, vec3 *restrict out);
  api point3 *mat4_mul_point3(const mat4 *restrict left, const point3 *restrict right, point3 *restrict out);

  api mat4 *mat4_transpose(const mat4 *restrict left, mat4 *restrict out);

  api mat4 *mat4_mul_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out);
  api mat4 *mat4_div_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out);
  api mat4 *mat4_add_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out);
  api mat4 *mat4_sub_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out);

  api bool mat4_approximately_equal(const mat4 *restrict left, const mat4 *restrict right, real epsilon);

  
  
  /* typedef vec4 quat; */

  /* quat *init_quat(quat *restrict q, real x, real y, real z, real w); */
  /* quat *init_quat_axis_angle(quat *restrict q, const vec3 *restrict axis, real angle); */
  /* quat *init_quat_euler_angles(quat *restrict q, const vec3 *restrict angles); */
  /* quat *init_quat_identity(quat *restrict q); */

  /* quat *quat_add(const quat *restrict left, const quat *restrict right, quat *restrict out); */
  /* quat *quat_sub(const quat *restrict left, const quat *restrict right, quat *restrict out); */
  /* quat *quat_mul(const quat *restrict left, const quat *restrict right, quat *restrict out); */
  /* quat *quat_div(const quat *restrict left, const quat *restrict right, quat *restrict out); */

  /* quat *quat_mul_scalar(const quat *restrict left, real scalar, quat *restrict out); */
  /* quat *quat_div_scalar(const quat *restrict left, real scalar, quat *restrict out); */

  /* real quat_dot(const quat *restrict left, const quat *restrict right); */
  /* real quat_magnitude(const quat *restrict q); */

  /* quat *quat_normalize(const quat *restrict q, quat *restrict out); */
  /* quat *quat_conjugate(const quat *restrict q, quat *restrict out); */
  /* quat *quat_inverse(const quat *restrict q, quat *restrict out); */

  /* bool quat_axis_angle(const quat *restrict q, vec3 *restrict out_axis, real *out_angle); */
  /* quat *quat_euler_angles(const quat *restrict q, vec3 *restrict out); */

  /* vec3 *vec3_rotate_quat(const vec3 *restrict left, const quat *restrict q, vec3 *restrict out); */

  /* mat4 *quat_to_mat4(const quat *restrict q, mat4 *restrict out); */
  /* quat *mat4_to_quat(const mat4 *restrict m, quat *restrict out); */

#endif /* DL_USE_MATH */
  


  /*****************************************************************************
   **  Tweening
   ****************************************************************************/

#if DL_USE_TWEEN

  typedef enum {
    EASE_IN,
    EASE_OUT,
    EASE_INOUT
  } ease_direction;

  typedef real(*easing_function)(ease_direction d, real p);

  api real tween(easing_function ease, ease_direction direction, real percent);

  api real ease_linear(ease_direction d, real p);
  api real ease_quadratic(ease_direction d, real p);
  api real ease_cubic(ease_direction d, real p);
  api real ease_quartic(ease_direction d, real p);
  api real ease_quintic(ease_direction d, real p);
  api real ease_sinusoidal(ease_direction d, real p);
  api real ease_exponential(ease_direction d, real p);
  api real ease_circular(ease_direction d, real p);
  api real ease_elastic(ease_direction d, real p);
  api real ease_elastic_tunable(ease_direction d, real p, real a, real k);
  api real ease_back(ease_direction d, real p);
  api real ease_back_tunable(ease_direction d, real p, real s, real t);
  api real ease_bounce(ease_direction d, real p);

  typedef real *(*selector_function)(const real *restrict values, natural length, real percent, real *out);

  api real *interpolate(selector_function select, const real *restrict values, natural length, real percent, real *restrict out);

  api real *select_linear(const real *restrict v, natural l, real p, real *restrict out);
  api real *select_bezier(const real *restrict v, natural l, real p, real *restrict out);
  api real *select_catmullrom(const real *restrict v, natural l, real p, real *restrict out);

  typedef point2 *(*selector_function_point2)(const point2 *restrict values, natural length, real percent, point2 *restrict out);

  api point2 *interpolate_point2(const selector_function_point2 select, const point2 *restrict values, natural length, real percent, point2 *restrict out);

  api point2 *select_linear_point2(const point2 *restrict v, natural l, real p, point2 *restrict out);
  api point2 *select_bezier_point2(const point2 *restrict v, natural l, real p, point2 *restrict out);
  api point2 *select_catmullrom_point2(const point2 *restrict v, natural l, real p, point2 *restrict out);

  typedef point3 *(*selector_function_point3)(const point3 *restrict values, natural length, real percent, point3 *restrict out);

  api point3 *interpolate_point3(const selector_function_point3 select, const point3 *restrict values, natural length, real percent, point3 *restrict out);

  api point3 *select_linear_point3(const point3 *restrict v, natural l, real p, point3 *restrict out);
  api point3 *select_bezier_point3(const point3 *restrict v, natural l, real p, point3 *restrict out);
  api point3 *select_catmullrom_point3(const point3 *restrict v, natural l, real p, point3 *restrict out);



  api integer lerp_integer(integer a, integer b, real p);
  api real lerp_real(real a, real b, real p);
  api point2 *lerp_point2(const point2 *restrict a, const point2 *restrict b, real p, point2 *restrict out);
  api point3 *lerp_point3(const point3 *restrict a, const point3 *restrict b, real p, point3 *restrict out);
  api vec2 *lerp_vec2(const vec2 *restrict a, const vec2 *restrict b, real p, vec2 *restrict out);
  api vec3 *lerp_vec3(const vec3 *restrict a, const vec3 *restrict b, real p, vec3 *restrict out);



#endif /* DL_USE_TWEEN */
  
  /*****************************************************************************
   **  Vectors
   ****************************************************************************/

#if DL_USE_CONTAINERS
  
  typedef struct {
    natural element_size;
    natural slice_length;
    any (*alloc)(natural count, natural element_size);
    void (*free)(any data);
  } vector_settings;

  typedef struct {
    vector_settings settings;
    natural slice_count;

    union {
      byte **slices;
      byte *array;
    } data;
  } vector;

  extern vector_settings default_vector_settings;

  api vector *init_vector(vector * restrict target, natural element_size, natural capacity);
  api vector *init_vector_custom(vector * restrict target, vector_settings settings, natural capacity);

  api vector *init_vector_array(vector * restrict target, byte *data, natural element_size, natural count);

  api natural vector_copy(vector * restrict target, natural target_offset_index, const vector * restrict original);
  api natural vector_copy_array(vector * restrict target, natural target_offset_index, const byte *restrict data, natural count);

  api void destroy_vector(vector * restrict target, const handler *restrict deconstruct_entry);

  api natural vector_capacity(const vector * restrict v);

  api bool vector_grow(vector * restrict v);
  api bool vector_shrink(vector * restrict v, handler *deconstruct_entry);
  api bool vector_resize(vector * restrict v, natural minimum_capacity, handler *deconstruct_entry);

  api any vector_get(const vector * restrict v, natural index, any out);
  api const any vector_ref(const vector * restrict v, natural index);
  api any vector_set(vector * restrict v, natural index, any value);

  api bool vector_swap(vector * restrict v, natural index1, natural index2);
  api natural vector_ref_array(vector * restrict v, natural index, any *restrict out);



  /*****************************************************************************
   **  Linked Lists
   ****************************************************************************/

  typedef struct {
    natural element_size;
    natural cache_length;
    any (*alloc)(natural count, natural element_size);
    void (*free)(any data);
  } linked_list_settings;

  struct linked_list_node {
    struct linked_list_node *next;
    struct linked_list_node *previous;
  };

#define LINKED_LIST_HEADER_SIZE (sizeof(struct linked_list_node))
#define LINKED_LIST_DATA(element) ((any)&((struct linked_list_node *)element)[1])

  typedef struct {
    struct linked_list_node *first;
    struct linked_list_node *last;
    struct linked_list_node *free;

    linked_list_settings settings;
    vector node_cache;
  } linked_list;

  extern linked_list_settings default_linked_list_settings;

  api linked_list *init_linked_list(linked_list * restrict target, natural element_size, natural cache_length);
  api linked_list *init_linked_list_custom(linked_list * restrict target, linked_list_settings settings);

  api natural linked_list_copy(linked_list * restrict target, struct linked_list_node *target_position, const linked_list *restrict original);
  api natural linked_list_copy_array(linked_list * restrict target, struct linked_list_node *target_position, const byte *restrict data, natural count);

  api void destroy_linked_list(linked_list * restrict target, handler *restrict deconstruct_entry);

  api natural linked_list_capacity(const linked_list * restrict list);
  api natural linked_list_length(const linked_list * restrict list);

  api bool linked_list_grow(linked_list * restrict list);
  api bool linked_list_shrink(linked_list * restrict list, handler *restrict deconstruct_entry);
  api bool linked_list_resize(linked_list * restrict list, natural minimum_capacity, handler *restrict deconstruct_entry);

  api any linked_list_get(const linked_list * restrict list, struct linked_list_node *restrict position, any out);
  api const any linked_list_ref(const struct linked_list_node *restrict position);
  api any linked_list_set(linked_list * restrict list, struct linked_list_node *restrict position, any value);
  api struct linked_list_node *linked_list_index(linked_list * restrict list, natural position);

  api struct linked_list_node *linked_list_add(linked_list * restrict list, struct linked_list_node *restrict position, any value);
  api any linked_list_remove(linked_list * restrict list, struct linked_list_node * restrict position, any out);
  api natural linked_list_destroy_range(linked_list * restrict list, struct linked_list_node *restrict position, natural count, handler *restrict destruct_entry);
  api bool linked_list_destroy(linked_list * restrict list, struct linked_list_node *restrict position, handler *deconstruct_entry);

  api bool linked_list_swap(linked_list * restrict list, struct linked_list_node *restrict position1, struct linked_list_node *restrict position2, bool data);



  /*****************************************************************************
   **  Collections
   ****************************************************************************/


  typedef union {
    struct {
      natural index;
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
  typedef byte collection_type;

  enum {
    STORAGE_TYPE_VECTOR,
    STORAGE_TYPE_LINKED_LIST
  };
  typedef byte storage_type;  

  struct collection_dispatch_functions;
  typedef struct {
    collection_type type;
    storage_type storage;
    
    comparator comparer;
    handler deconstruct_entry;
    natural capacity;
    natural element_size;

    struct collection_dispatch_functions *functions;
  } collection_settings;

  extern collection_settings default_vector_collection_settings;
  extern collection_settings default_linked_list_collection_settings;

  typedef struct {
    collection_settings settings;

    union {
      struct {
	vector container;
	natural index[2];
      } vector;
      struct {
	linked_list container;
      } linked_list;
    } data;
  } collection;

  struct collection_dispatch_functions {
    integer (*_iterator_compare)(const collection *restrict col, iterator left, iterator right);
    bool (*_iterator_is_valid)(const collection *restrict col, iterator index);
    iterator (*_make_invalid_iterator)(const collection *restrict col);

    any (*_collection_push_start)(collection *restrict col, iterator *iter);
    bool (*_collection_is_empty)(const collection *restrict col);
    any (*_collection_pop)(const collection *restrict col, any out);
    bool (*_collection_pop_destroy)(collection *restrict col);
    bool (*_collection_pop_forget)(collection *restrict col);
    integer (*_collection_ref_array)(collection *restrict col, iterator iter, any *out_array);
    any (*_collection_ref)(collection *restrict col, iterator iter);
    bool (*_collection_swap)(collection *restrict col, iterator *iter_a, iterator *iter_b);
    any (*_collection_get)(const collection *restrict col, iterator iter, any out);
    any (*_collection_set)(collection *restrict col, iterator *iter, any value);
    iterator (*_collection_index)(collection *restrict col, natural index);
    void (*_collection_next)(const collection *restrict col, iterator *iter);
    void (*_collection_prev)(const collection *restrict col, iterator *iter);
    integer (*_collection_count)(const collection *restrict col);
    iterator (*_collection_begin)(const collection *restrict col);
    iterator (*_collection_end)(const collection *restrict col);
    const any (*_collection_search_region)(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter);
    bool (*_collection_destroy_at)(collection *restrict col, iterator *iter, handler *destructor);
    any (*_collection_remove_at)(collection *restrict col, iterator *iter, any out);
    integer (*_collection_destroy_range)(collection *restrict col, iterator *iter, natural count);
    bool (*_collection_insert)(collection *restrict col, iterator *restrict position, any item);
  };
  
  api integer iterator_compare(const collection *restrict col, iterator left, iterator right);
  api bool iterator_equal(const collection *restrict col, iterator left, iterator right);
  api bool iterator_is_valid(const collection *restrict col, iterator index);
  api iterator make_invalid_iterator(const collection *restrict col);

  api collection *init_collection(collection *restrict col, collection_type type, storage_type storage, comparator *restrict compare, handler *restrict destructor, natural element_size);
  api collection *init_collection_custom(collection *restrict col, collection_settings settings, natural element_size);

  api collection *init_collection_array(collection *restrict col, collection_type type, comparator *restrict comp, handler *restrict destruct_entry, byte * data, natural element_size, natural count);

  api natural collection_element_size(const collection *restrict col);

  api void destroy_collection(collection *restrict col);

  api bool collection_is_set(const collection *restrict col);
  api bool collection_is_sorted(const collection * restrict col);
  api bool collection_is_queue(const collection * restrict col);
  api bool collection_is_vector(const collection * restrict col);
  api bool collection_is_linked_list(const collection * restrict col);

  api integer collection_copy(const collection *restrict original, collection *restrict target);
  api integer collection_copy_array(const any data, natural count, collection *restrict target);

  api iterator collection_begin(const collection *restrict col);
  api iterator collection_end(const collection *restrict col);

  api any collection_pop(collection *restrict col, any out);
  api bool collection_pop_forget(collection *restrict col);
  api bool collection_pop_destroy(collection *restrict col);

  api any collection_push(collection *restrict col, const any value);
  api any collection_push_index(collection *restrict col, const any value, iterator *out_index);
  api any collection_push_start(collection *restrict col, iterator *iter);
  api any collection_push_finish(collection *restrict col, iterator *iter);

  api const any collection_begin_ref(const collection *restrict col, iterator *iter);
  api const any collection_end_ref(const collection *restrict col, iterator *iter);

  api iterator collection_index(collection *restrict col, natural index);
  api iterator collection_index_of(const collection *restrict col, const any item);

  api const any collection_next(const collection *restrict col, iterator *iter);
  api const any collection_prev(const collection *restrict col, iterator *iter);

  api any collection_find(const collection *restrict col, filter *restrict predicate, iterator *iter);
  api any collection_find_last(const collection *restrict col, filter *restrict predicate, iterator *iter);

  api any collection_foldl(const collection *restrict col, any initial, folder *func);
  api any collection_foldr(const collection *restrict col, any initial, folder *func);
  api const any collection_search(const collection *restrict col, filter *predicate, iterator *iter);
  api const any collection_search_region(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter);

  api any collection_get(const collection *restrict col, iterator index, any out);
  api any collection_insert(collection *restrict col, iterator *index, any item);
  api const any collection_peek(const collection *restrict col);
  api const any collection_ref(const collection *restrict col, iterator iter);
  api any collection_remove_at(collection *restrict col, iterator *index, any out);
  api any collection_remove_first(collection *restrict col, filter *predicate, iterator *index, any out);
  api any collection_remove_last(collection *restrict col, filter *predicate, iterator *index, any out);
  api any collection_set(collection *restrict col, iterator *index, any value);

  api bool collection_all(const collection *restrict col, filter *func);
  api bool collection_any(const collection *restrict col, filter *func);
  api bool collection_contains(const collection *restrict col, const any item);
  api bool collection_is_empty(const collection *restrict col);

  api integer collection_map(const collection *restrict col, converter *func, collection *out);
  api integer collection_zip(const collection *restrict col1, const collection *restrict col2, zipper *zip, collection *out);

  api bool collection_forget_at(collection *restrict col, iterator *iter);
  api bool collection_destroy_at(collection *restrict col, iterator *iter);
  api bool collection_destroy_first(collection *restrict col, filter *predicate, iterator *iter);
  api bool collection_destroy_last(collection *restrict col, filter *predicate, iterator *iter);
  api bool collection_swap(collection *restrict col, iterator *iter_a, iterator *iter_b);
  api bool collection_quick_sort(collection *restrict col, comparator *compare);
  api bool collection_quick_sort_region(collection *restrict col, comparator *compare, iterator left, iterator right);
  api bool collection_sort(collection *restrict col);

  api void collection_clear(collection *restrict col);

  api integer collection_count(const collection *restrict col);
  api integer collection_find_all(const collection *restrict col, filter *predicate, collection *out);

  api integer collection_take(collection *restrict col, natural count, collection *out);
  api integer collection_destroy_all(collection *restrict col, filter *predicate);
  api integer collection_destroy_range(collection *restrict col, iterator *index, natural count);
  api integer collection_drop(collection *restrict col, natural count);
  api integer collection_ref_array(collection *restrict col, iterator index, any *out_array);
  api integer collection_remove_all(collection *restrict col, filter *predicate, collection *out);
  api integer collection_remove_range(collection *restrict col, iterator *index, natural count, collection *out);

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

natural _default_log_handler(log_channel ch, const char *restrict file, natural line, const char *restrict function, const char *restrict msg) {
  char time_buf[20];

#if HAS_TIME
# if IS_MSC
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
  case LOG_INFO:
    fprintf(stdout, "%s:%lu:%s\n[LOG %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case LOG_WARNING:
    fprintf(stdout, "%s:%lu:%s\n[WRN %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case LOG_ERROR:
    fprintf(stderr, "%s:%lu:%s\n[ERR %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case LOG_TEST:
    fprintf(stdout, "[TST %s] %s\n", time_buf, msg);
    return ch;
  case LOG_MESSAGE:
    fprintf(stdout, "[MSG %s] %s\n", time_buf, msg);
    return ch;
  default:
    return 0;
  }
}

natural (*active_log_handler)(log_channel ch, const char *restrict, natural, const char *restrict, const char *restrict) = _default_log_handler;

natural log_message(log_channel ch, const char *restrict file, natural line, const char *restrict function, const char *restrict fmt, ...) {
  char buf[256];
  va_list args1, args2;
  va_start(args1, fmt);
  va_copy(args2, args1);
  natural count = 1 + vsnprintf(NULL, 0, fmt, args1);
  count = count > 254 ? 254 : count;
  va_end(args1);
  vsnprintf(buf, count, fmt, args2);
  buf[count + 1] = 0;
  va_end(args2);

  return active_log_handler(ch, file, line, function, buf);
}

#endif /* DL_USE_LOGGING */



/*****************************************************************************
 **  Tests
 ****************************************************************************/

#if DL_USE_TEST

integer test_run(bool (**tests)(), const char **names, integer count) {
  integer i, passed = 0;

  for (i = 0; i < count; ++i) {
    TEST("[%3i/%3i] %s", i + 1, count, names[i]);
    if (tests[i]())
      passed++;
  }

  return passed;
}

integer test_count(bool (**tests)(), integer max) {
  integer idx;
  
  for (idx = 0; idx < max && tests[idx] != NULL; ++idx);
  return idx;
}

#endif /* DL_USE_TEST */



/*****************************************************************************
 **  Math
 ****************************************************************************/

#if DL_USE_MATH

#ifndef M_PI
const real M_PI = 3.14159265359f;
#endif
#ifndef M_E
const real M_E = 2.71828182846f;
#endif

#ifndef M_INV_PI
const real M_INV_PI = 0.318309886185f;
#endif
#ifndef M_INV_E
const real M_INV_E = 0.367879441171f;
#endif
#ifndef M_EPSILON
const real M_EPSILON = 0.001f;
#endif

const mat4 mat4_identity = { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
const point2 point2_zero = { 0, 0 };
const point2 point2_one = { 1, 1 };
const vec2 vec2_up = { 0, 1 };
const vec2 vec2_right = { 1, 0 };
const vec3 vec3_up = { 0, 1, 0, 0 };
const vec3 vec3_right = { 1, 0, 0, 0 };

const vec2 vec2_down = { 0, -1 };
const vec2 vec2_left = { -1, 0 };
const vec3 vec3_down = { 0, -1, 0, 0 };
const vec3 vec3_left = { -1, 0, 0, 0 };

#if DL_USE_LEFT_HANDED
const vec3 vec3_forward = { 0, 0, 1, 0 };
const vec3 vec3_backward = { 0, 0, -1, 0 };
#else
const vec3 vec3_forward = { 0, 0, -1, 0 };
const vec3 vec3_backward = { 0, 0, 1, 0 };
#endif

const vec2 vec2_zero = { 0, 0 };
const vec3 vec3_zero = { 0, 0, 0, 0 };
const point3 point3_zero = { 0, 0, 0, 1 };

const vec2 vec2_one = { 1, 1 };
const vec3 vec3_one = { 1, 1, 1, 0 };
const point3 point3_one = { 1, 1, 1, 1 };

api bool approximately_equal(real a, real b, real epsilon) {
  return _abs(a - b) < epsilon;
}

api integer floor_to_integer(real n) {
  return (integer)_floor(n);
}

api integer ceil_to_integer(real n) {
  return (integer)_ceil(n);
}

api integer round_to_integer(real n) {
#if !IS_ATLEAST_C99
  real floored = _floor(n);
  real frac = n - floored;
  return frac > 0.5 ? (integer)(floored + 1) : (integer)floored;
#else
  return (integer)lround(n);
#endif
}

api real rationalize(real value, natural decimal_points) {
#if !IS_ATLEAST_C99
  real d = (real)pow(10, (real)decimal_points);
  real floored = floor(value);
  real frac = value - floored;
  real n = frac > 0.5 ? (floored + 1) : floored;
  value = (value - n) * d;
  floored = floor(value);
  frac = value - floored;
  value = frac > 0.5 ? (floored + 1) : floored;
  return n + (value / d);
#else  
  real d = (real)_pow(10, (real)decimal_points);
  real n = (real)roundf(value);
  return n + (roundf((value - n) * d) / d);
#endif
}

api real degree_to_radian(real degree) {
  static const real factor = 0.01745329252f;
  return degree * factor;
}

api real radian_to_degree(real radian) {
  static const real factor = 57.2957795131f;
  return radian * factor;
}

integer _factorial_cache[13] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600 };

api integer factorial(integer n) {
  integer i;
  if (likely((natural)n < sizeof(_factorial_cache)))
    return _factorial_cache[n];
  if (unlikely(n < 0))
    return 1;
  for (i = n - 1; i > 0; --i) {
    n = n * i;
  }
  return n;
}



api random_state *init_random(random_state *state, integer seed) {
  return init_random_custom(state, INTEGER_MAX, 1103515245, 12345, seed);
}

api random_state *init_random_custom(random_state *state, integer m, integer a, integer c, integer seed) {
  if (safety(state == NULL))
    return NULL;

  state->m = _abs(m);
  state->a = _abs(a);
  state->c = _abs(c);
  state->seed = (a * seed + c) % m;

  return state;
}

api real random_degree(random_state *state) {
  return random_real_range(state, 0, 360);
}

api real random_radian(random_state *state) {
  return random_real_range(state, 0, 2 * M_PI);
}

api real random_real(random_state *state, real max) {
  if (safety(state == NULL))
    return 0;

  state->seed = (state->a * state->seed + state->c) % state->m;

  return fabs((real)state->seed / (real)state->m) * max;
}

api integer random_integer(random_state *state, integer max) {
  if (safety(state == NULL))
    return 0;

  state->seed = (state->a * state->seed + state->c) % state->m;

  return _abs(state->seed) % max;
}

api integer random_integer_range(random_state *state, integer min, integer max) {
  return random_integer(state, max - min) + min;
}

api real random_real_range(random_state *state, real min, real max) {
  return random_real(state, max - min) + min;
}

#if IS_WINDOWS

api random_state *init_random_time(random_state *state) {
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

  return init_random(state, tv_usec + tv_sec);
}

#elsif (IS_LINUX || IS_APPLE) && IS_ATLEAST_C99

api random_state *init_random_time(random_state *state) {
  struct timeval t1;
  gettimeofday(&t1, NULL);
  return init_random(state, t1.tv_usec + t1.tv_sec);
}

#else

api random_state *init_random_time(random_state *state) {
  return init_random(state, (integer)__LINE__);
}

#endif



api vec2 *init_vec2(vec2 * restrict v, real x, real y) {
  if (safety(v == NULL))
    return NULL;

  v->x = x;
  v->y = y;
  
  return v;
}

api vec2 *vec2_add(const vec2 *restrict left, const vec2 *restrict right, vec2 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;

  return out;
}

api vec2 *vec2_sub(const vec2 *restrict left, const vec2 *restrict right, vec2 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;

  return out;
}

api vec2 *vec2_mul_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;

  return out;
}

api vec2 *vec2_div_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out) {
  return vec2_mul_scalar(left, 1.0f / scalar, out);
}

api vec2 *vec2_add_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;

  return out;
}

api vec2 *vec2_sub_scalar(const vec2 *restrict left, real scalar, vec2 *restrict out) {
  return vec2_add_scalar(left, -scalar, out);
}

api vec2 *vec2_normalize(const vec2 *restrict left, vec2 *restrict out) {
  real inv_magnitude;
  if (safety(left == NULL || out == NULL))
    return NULL;

  inv_magnitude = 1.0f / _hypot(left->x, left->y);
  vec2_mul_scalar(left, inv_magnitude, out);

  return out;
}

api vec2 *vec2_negate(const vec2 *restrict left, vec2 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;

  return out;
}

api real vec2_dot(const vec2 *restrict left, const vec2 *restrict right) {
  if (safety(left == NULL || right == NULL))
    return 0;
  return left->x * right->x + left->y * right->y;
}

api bool vec2_approximately_equal(const vec2 *restrict left, const vec2 *restrict right, real epsilon) {
  if (safety(left == NULL || right == NULL))
    return 0;
  return fabs(left->x - right->x) < epsilon && fabs(left->y - right->y) < epsilon;
}

api real vec2_square_magnitude(const vec2 *restrict left) {
  if (safety(left == NULL))
    return 0;
  return left->x * left->x + left->y * left->y;
}

api real vec2_magnitude(const vec2 *restrict left) {
  if (safety(left == NULL))
    return 0;
  
  return _hypot(left->x, left->y);
}

api vec2 *vec2_reflect(const vec2 *restrict left, const vec2 *restrict normal, vec2 *restrict out) {
  if (safety(left == NULL || normal == NULL || out == NULL))
    return NULL;
  
  return vec2_sub(left, vec2_mul_scalar(normal, vec2_dot(normal, left) * 2.0f, out), out);
}

api vec2 *vec2_refract(const vec2 *restrict left, const vec2 *restrict normal, float eta, vec2 *restrict out) {
  float dot, k;
  vec2 t;

  if (safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  dot = vec2_dot(normal, left);
  k = 1.0f - (eta * eta) * (1.0f - (dot * dot));
  if (unlikely(k < 0.0f)) {
    *out = vec2_zero;
    return out;
  }

  return vec2_sub(vec2_mul_scalar(left, eta, &t), vec2_mul_scalar(normal, eta * dot * _sqrt(k), out), out);
}



api point2 *init_point2(point2 * restrict p, real x, real y) {
  return init_vec2((vec2 *)p, x, y);
}

api point2 *point2_add(const point2 *restrict left, const point2 *restrict right, point2 *restrict out) {
  return vec2_add(left, right, out);
}

api point2 *point2_sub(const point2 *restrict left, const point2 *restrict right, point2 *restrict out) {
  return vec2_sub(left, right, out);
}

api point2 *point2_mul_scalar(const point2 *restrict left, real scalar, point2 *restrict out) {
  return vec2_mul_scalar(left, scalar, out);
}

api point2 *point2_div_scalar(const point2 *restrict left, real scalar, point2 *restrict out) {
  return vec2_div_scalar(left, scalar, out);
}

api point2 *point2_add_scalar(const point2 *restrict left, real scalar, point2 *restrict out) {
  return vec2_add_scalar(left, scalar, out);
}

api point2 *point2_sub_scalar(const point2 *restrict left, real scalar, point2 *restrict out) {
  return vec2_sub_scalar(left, scalar, out);
}

api point2 *point2_negate(const point2 *restrict left, point2 *restrict out) {
  return vec2_negate(left, out);
}

api bool point2_approximately_equal(const point2 *restrict left, const point2 *restrict right, real epsilon) {
  return vec2_approximately_equal(left, right, epsilon);
}

api real point2_line_orientation(const point2 *restrict point, const point2 *restrict line_a, const point2 *restrict line_b) {
  real rise, run, m, b;

  if (safety(point == NULL || line_a == NULL || line_b == NULL))
    return 0;
  
  rise = line_b->y - line_a->y;
  run = line_b->x - line_a->x;
  m = rise / run;
  b = -(m * line_a->x - line_a->y);

  return m * point->x + b - point->y;
}



api vec4 *init_vec4(vec4 * restrict v, real x, real y, real z, real w) {
  if (safety(v == NULL))
    return NULL;
  
  v->x = x;
  v->y = y;
  v->z = z;
  v->w = w;

  return v;
}

api vec4 *vec4_add(const vec4 *restrict left, const vec4 *restrict right, vec4 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = left->w + right->w;

  return out;
}

api vec4 *vec4_sub(const vec4 *restrict left, const vec4 *restrict right, vec4 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = left->w - right->w;

  return out;
}

api real vec4_dot(const vec4 *restrict left, const vec4 *restrict right) {
  if (safety(left == NULL || right == NULL))
    return 0;
  
  return left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
}

api vec4 *vec4_normalize(const vec4 *restrict left, vec4 *restrict out) {
  return vec4_div_scalar(left, vec4_magnitude(left), out);
}

api vec4 *vec4_negate(const vec4 *restrict left, vec4 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = -left->w;

  return out;
}

api real vec4_square_magnitude(const vec4 *restrict left) {
  if (safety(left == NULL))
    return 0;
  
  return (left->x * left->x)
    + (left->y * left->y)
    + (left->z * left->z)
    + (left->w * left->w);
}

api real vec4_magnitude(const vec4 *restrict left) {
  return _sqrt(vec4_square_magnitude(left));
}

api bool vec4_approximately_equal(const vec4 *restrict left, const vec4 *restrict right, real epsilon) {
  if (safety(left == NULL || right == NULL))
    return 0;
  
  return fabs(left->x - right->x) < epsilon
    && fabs(left->y - right->y) < epsilon
    && fabs(left->z - right->z) < epsilon
    && fabs(left->w - right->w) < epsilon;
}

api vec4 *vec4_mul_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = left->w * scalar;

  return out;
}

api vec4 *vec4_div_scalar(const vec4 *left, real scalar, vec4 *restrict out) {
  return vec4_mul_scalar(left, 1.0 / scalar, out);
}

api vec4 *vec4_add_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = left->w + scalar;

  return out;
}

api vec4 *vec4_sub_scalar(const vec4 *restrict left, real scalar, vec4 *restrict out) {
  return vec4_add_scalar(left, -scalar, out);
}



api point3 *init_point3(point3 * restrict p, real x, real y, real z) {
  return init_vec4(p, x, y, z, 1);
}

api point3 *point3_add(const point3 *restrict left, const point3 *restrict right, point3 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 1;

  return out;
}

api point3 *point3_sub(const point3 *restrict left, const point3 *restrict right, point3 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 1;

  return out;
}

api point3 *point3_mul_scalar(const point3 *restrict left, real scalar, point3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 1;

  return out;
}

api point3 *point3_div_scalar(const point3 *restrict left, real scalar, point3 *restrict out) {
  return point3_mul_scalar(left, 1.0 / scalar, out);
}

api point3 *point3_add_scalar(const point3 *restrict left, real scalar, point3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 1;

  return out;
}

api point3 *point3_sub_scalar(const point3 *restrict left, real scalar, point3 *restrict out) {
  return point3_add_scalar(left, -scalar, out);
}

api point3 *point3_negate(const point3 *restrict left, point3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 1;

  return out;
}

api bool point3_approximately_equal(const point3 *restrict left, const point3 *restrict right, real epsilon) {
  return vec4_approximately_equal(left, right, epsilon);
}

api point3 *point3_rotate(const point3 *restrict left, const vec3 *restrict axis, const real angle, point3 *restrict out) {
  mat4 mat;

  return mat4_mul_point3(init_mat4_rotate(&mat, axis, angle), left, out);
}



api vec3 *init_vec3(vec3 * restrict v, real x, real y, real z) {
  return init_vec4(v, x, y, z, 0);
}

api vec3 *vec3_add(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 0;

  return out;
}

api vec3 *vec3_sub(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out) {
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 0;

  return out;
}

api vec3 *vec3_cross(const vec3 *restrict left, const vec3 *restrict right, vec3 *restrict out) {
  real orig_x, orig_y;
  
  if (safety(left == NULL || right == NULL || out == NULL))
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

api real vec3_dot(const vec3 *restrict left, const vec3 *restrict right) {
  return left->x * right->x + left->y * right->y + left->z * right->z;
}

api vec3 *vec3_mul_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 0;

  return out;
}

api vec3 *vec3_div_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out) {
  return vec3_mul_scalar(left, 1.0 / scalar, out);
}

api vec3 *vec3_add_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 0;

  return out;
}

api vec3 *vec3_sub_scalar(const vec3 *restrict left, real scalar, vec3 *restrict out) {
  return vec3_add_scalar(left, -scalar, out);
}

api vec3 *vec3_normalize(const vec3 *restrict left, vec3 *restrict out) {
  return vec3_div_scalar(left, vec3_magnitude(left), out);
}

api bool vec3_normalized(const vec3 *restrict left) {
  return approximately_equal(fabs(left->x) + fabs(left->y) + fabs(left->z), 1.0, M_EPSILON);
}

api vec3 *vec3_negate(const vec3 *restrict left, vec3 *restrict out) {
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 0;

  return out;
}

api bool vec3_approximately_equal(const vec3 *restrict left, const vec3 *restrict right, real epsilon) {
  return vec4_approximately_equal(left, right, epsilon);
}

api real vec3_square_magnitude(const vec3 *restrict left) {
  if (safety(left == NULL))
    return 0;

  return left->x * left->x
    + left->y * left->y
    + left->z * left->z;
}

api real vec3_magnitude(const vec3 *restrict left) {
  return _sqrt(vec3_square_magnitude(left));
}

api vec3 *vec3_rotate(const vec3 *restrict left, const vec3 *restrict axis, const real angle, vec3 *restrict out) {
  mat4 mat;

  return mat4_mul_vec3(init_mat4_rotate(&mat, axis, angle), left, out);
}

api vec3 *vec3_reflect(const vec3 *restrict left, const vec3 *restrict normal, vec3 *restrict out) {
  if (safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  return vec3_sub(left, vec3_mul_scalar(normal, vec3_dot(normal, left) * 2.0, out), out);
}

api vec3 *vec3_refract(const vec3 *restrict left, const vec3 *restrict normal, float eta, vec3 *restrict out) {
  float dot, k;
  vec3 t;

  if (safety(left == NULL || normal == NULL || out == NULL))
    return NULL;
  
  dot = vec3_dot(normal, left);
  k = 1.0 - (eta * eta) * (1.0 - (dot * dot));
  if (unlikely(k < 0.0)) {
    *out = vec3_zero;
    return out;
  }

  return vec3_sub(vec3_mul_scalar(left, eta, &t), vec3_mul_scalar(normal, eta * dot * _sqrt(k), out), out);
}



api mat4 *init_mat4(mat4 * restrict m, real c0r0, real c0r1, real c0r2, real c0r3, real c1r0, real c1r1, real c1r2, real c1r3, real c2r0, real c2r1, real c2r2, real c2r3, real c3r0, real c3r1, real c3r2, real c3r3) {
  if (safety(m == NULL))
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

api mat4 *init_mat4_cols(mat4 * restrict m, const vec4 *restrict column_a, const vec4 *restrict column_b, const vec4 *restrict column_c, const vec4 *restrict column_d) {
  if (safety(m == NULL || column_a == NULL || column_b == NULL || column_c == NULL || column_d == NULL))
    return NULL;
  
  *(vec4 *)(&m->ary[0]) = *column_a;
  *(vec4 *)(&m->ary[1]) = *column_b;
  *(vec4 *)(&m->ary[2]) = *column_c;
  *(vec4 *)(&m->ary[3]) = *column_d;

  return m;
}

api mat4 *mat4_add(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out) {
  integer i, j;
  
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + right->ary[i][j];

  return out;
}

api mat4 *mat4_sub(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out) {
  integer i, j;
  
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] - right->ary[i][j];

  return out;
}

api mat4 *mat4_mul(const mat4 *restrict left, const mat4 *restrict right, mat4 *restrict out) {
  vec4 *A, *B;
  mat4 temp;

  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  /* The copy prevents errors arising from right/left and out pointing at the same data. */
  temp = *right;
  A = (vec4 *)&left->ary;
  B = (vec4 *)&temp.ary;

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

bool mat4_approximately_equal(const mat4 *restrict left, const mat4 *restrict right, real epsilon) {
  integer i, j;
  
  if (safety(left == NULL || right == NULL))
    return false;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (_abs(left->ary[i][j] - right->ary[i][j]) > epsilon)
	return false;
  
  return true;
}

api vec4 *mat4_mul_vec4(const mat4 *restrict left, const vec4 *restrict right, vec4 *restrict out) {
  vec4 v;

  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  v = *right;
  out->x = left->ary[0][0] * v.x + left->ary[1][0] * v.y + left->ary[2][0] * v.z + left->ary[3][0] * v.w;
  out->y = left->ary[0][1] * v.x + left->ary[1][1] * v.y + left->ary[2][1] * v.z + left->ary[3][1] * v.w;
  out->z = left->ary[0][2] * v.x + left->ary[1][2] * v.y + left->ary[2][2] * v.z + left->ary[3][2] * v.w;
  out->w = left->ary[0][3] * v.x + left->ary[1][3] * v.y + left->ary[2][3] * v.z + left->ary[3][3] * v.w;

  return out;
}

api vec3 *mat4_mul_vec3(const mat4 *restrict left, const vec3 *restrict right, vec3 *restrict out) {
  vec3 v;
  
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;
  
  v = *right;
  out->x = left->ary[0][0] * v.x + left->ary[1][0] * v.y + left->ary[2][0] * v.z + left->ary[3][0] * v.w;
  out->y = left->ary[0][1] * v.x + left->ary[1][1] * v.y + left->ary[2][1] * v.z + left->ary[3][1] * v.w;
  out->z = left->ary[0][2] * v.x + left->ary[1][2] * v.y + left->ary[2][2] * v.z + left->ary[3][2] * v.w;
  out->w = 0.0;

  return out;
}

api point3 *mat4_mul_point3(const mat4 *restrict left, const point3 *restrict right, point3 *restrict out) {
  point3 p;
  
  if (safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  p = *right;
  out->x = left->ary[0][0] * p.x + left->ary[1][0] * p.y + left->ary[2][0] * p.z + left->ary[3][0] * p.w;
  out->y = left->ary[0][1] * p.x + left->ary[1][1] * p.y + left->ary[2][1] * p.z + left->ary[3][1] * p.w;
  out->z = left->ary[0][2] * p.x + left->ary[1][2] * p.y + left->ary[2][2] * p.z + left->ary[3][2] * p.w;
  out->w = 1.0;

  return out;
}

api mat4 *mat4_transpose(const mat4 *restrict left, mat4 *restrict out) {
  real v[4][4];
  natural a_idx, b_idx;

  if (safety(left == NULL || out == NULL))
    return NULL;

  for (a_idx = 0; a_idx < 4; ++a_idx)
    for (b_idx = 0; b_idx < 4; ++b_idx)
      v[a_idx][b_idx] = left->ary[a_idx][b_idx];
  for (a_idx = 0; a_idx < 4; ++a_idx)
    for (b_idx = 0; b_idx < 4; ++b_idx)
      out->ary[b_idx][a_idx] = v[a_idx][b_idx];

  return out;
}

api mat4 *mat4_mul_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out) {
  integer i,j;
  
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] * scalar;

  return out;
}

api mat4 *mat4_div_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out) {
  return mat4_mul_scalar(left, 1.0 / scalar, out);
}

api mat4 *mat4_add_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out) {
  integer i, j;
  
  if (safety(left == NULL || out == NULL))
    return NULL;
  
  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + scalar;

  return out;
}

api mat4 *mat4_sub_scalar(const mat4 *restrict left, real scalar, mat4 *restrict out) {
  return mat4_add_scalar(left, -scalar, out);
}

api mat4 *init_mat4_translate(mat4 * restrict m, real x, real y, real z) {
  if (safety(m == NULL))
    return NULL;
  
  *m = mat4_identity;
  m->ary[3][0] = x;
  m->ary[3][1] = y;
  m->ary[3][2] = z;

  return m;
}

api mat4 *init_mat4_rotate_x(mat4 * restrict m, real radians) {
  real c, s;

  if (safety(m == NULL))
    return NULL;

#if DL_USE_LEFT_HANDED
  c = _cos(-radians);
  s = _sin(-radians);

  return init_mat4(m,
		   1,  0,  0, 0,
		   0,  c, -s, 0,
		   0,  s,  c, 0,
		   0,  0,  0, 1);
#else
# if !IS_ATLEAST_C99
  c = (real)cos(radians);
  s = (real)sin(radians);
# else
  c = _cos(radians);
  s = _sin(radians);
# endif

  return init_mat4(m,
		   1, 0,  0, 0,
		   0, c,  s, 0,
		   0, -s, c, 0,
		   0, 0,  0, 1);
#endif
}

api mat4 *init_mat4_rotate_y(mat4 * restrict m, real radians) {
  real c, s;

  if (safety(m == NULL))
    return NULL;
  
#if DL_USE_LEFT_HANDED
  c = _cos(-radians);
  s = _sin(-radians);
  
  return init_mat4(m,
		   c,  0, s, 0,
		   0,  1, 0, 0,
		   -s, 0, c, 0,
		   0,  0, 0, 1);
#else
  c = _cos(radians);
  s = _sin(radians);
  
  return init_mat4(m,
		   c, 0, -s, 0,
		   0, 1,  0, 0,
		   s, 0,  c, 0,
		   0, 0,  0, 1);
#endif
}

api mat4 *init_mat4_rotate_z(mat4 * restrict m, real radians) {
  real c, s;

  if (safety(m == NULL))
    return NULL;

  c = _cos(radians);
  s = _sin(radians);
  
  return init_mat4(m,
		   c,  s, 0, 0,
		   -s, c, 0, 0,
		   0,  0, 1, 0,
		   0,  0, 0, 1);
}

api mat4 *init_mat4_rotate(mat4 * restrict m, const vec3 *restrict a, real radians) {
  vec3 i, t;
  real c, s;

  if (safety(m == NULL))
    return NULL;
  
  if (unlikely(vec3_approximately_equal(a, &vec3_zero, M_EPSILON))) {
    *m = mat4_identity;
    return m;
  }
  
  if (unlikely(!vec3_normalized(a)))
    vec3_normalize(a, &t);
  else
    t = *a;

#if DL_USE_LEFT_HANDED
  c = _cos(-radians);
  s = _sin(-radians);
  vec3_mul_scalar(&t, 1.0 - c, &i);  

  return init_mat4(m,
		   c + i.x * t.x,          i.x * t.y - s * t.z,    -(-i.x * t.z - s * t.y), 0,
		   i.y * t.x + s * t.z,    c + i.y * t.y,          -(-i.y * t.z + s * t.x), 0,
		   -(i.z * t.x + s * t.y), -(i.z * t.y - s * t.x), c + i.z * t.z,          0,
		   0, 0, 0, 1);
#else
  c = _cos(radians);
  s = _sin(radians);
  vec3_mul_scalar(&t, 1.0 - c, &i);  

  return init_mat4(m,
		   c + i.x * t.x,       i.x * t.y - s * t.z, -i.x * t.z - s * t.y, 0,
		   i.y * t.x + s * t.z, c + i.y * t.y,       -i.y * t.z + s * t.x, 0,
		   i.z * t.x + s * t.y, i.z * t.y - s * t.x, c + i.z * t.z,       0,
		   0, 0, 0, 1);
#endif
}

api mat4 *init_mat4_scale(mat4 * restrict m, real x, real y, real z) {
  if (safety(m == NULL))
    return NULL;
  
  *m = mat4_identity;
  m->ary[0][0] = x;
  m->ary[1][1] = y;
  m->ary[2][2] = z;

  return m;
}

api mat4 *init_mat4_look_at(mat4 * restrict m, const vec3 *restrict origin, const vec3 *restrict target, const vec3 *restrict up) {
  vec3 zaxis, yaxis, xaxis, temp;

  if (safety(m == NULL || origin == NULL || target == NULL || up == NULL))
    return NULL;

  vec3_sub(origin, target, &temp);
  vec3_normalize(&temp, &zaxis);
  vec3_cross(up, &zaxis, &temp);
  vec3_normalize(&temp, &xaxis);
  vec3_cross(&zaxis, &xaxis, &yaxis);

  return init_mat4(m,
		   xaxis.x, yaxis.x, zaxis.x, 0,
		   xaxis.y, yaxis.y, zaxis.y, 0,
		   xaxis.z, yaxis.z, zaxis.z, 0,
		   -vec3_dot(&xaxis, origin), -vec3_dot(&yaxis, origin), -vec3_dot(&zaxis, origin), 1);
}

api mat4 *init_mat4_orthographic(mat4 * restrict m, real yop, real y_bottom, real x_left, real x_right, real z_near, real z_far) {
  real inv_width, inv_height, inv_depth;

  if (safety(m == NULL))
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

api mat4 *init_mat4_perspective(mat4 * restrict m, real vertical_fov, real aspect_ratio, real z_near, real z_far) {
  real half_fov, invan_fov, neg_depth, inv_neg_depth;

  if (safety(m == NULL))
    return NULL;

  half_fov = vertical_fov * 0.5f;
  invan_fov = 1.0 / _tan(half_fov);
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
  
api real tween(easing_function ease, ease_direction direction, real percent) {
  percent = clamp01(percent);
  return ease(direction, percent);
}

api real ease_linear(ease_direction d, real p) {
  return p;
}

api real ease_quadratic(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return p * p;
  case EASE_OUT:
    return p * (2.0 - p);
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * p * p;

    p = p - 1.0;
    return -0.5f * ((p * (p - 2.0)) - 1.0);
  }
  return 0;
}

api real ease_cubic(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return (p * p * p);
  case EASE_OUT:
    p = p - 1.0;
    return 1.0 + (p * p * p);
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p);

    p = p - 2.0;
    return 0.5f * (2.0 + (p * p * p));
  }
  return 0;
}

api real ease_quartic(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return (p * p * p * p);
  case EASE_OUT:
    p = p - 1.0;
    return 1.0 - (p * p * p * p);
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p * p);

    p = p - 2.0;
    return -0.5f * ((p * p * p * p) - 2.0);
  }
  return 0;
}

api real ease_quintic(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return (p * p * p * p * p);
  case EASE_OUT:
    p = p - 1.0;
    return 1.0 + (p * p * p * p * p);
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * (p * p * p * p * p);

    p = p - 2.0;
    return 0.5f * (2.0 + (p * p * p * p * p));
  }
  return 0;
}

api real ease_sinusoidal(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return 1.0 - _cos(p * M_PI * 0.5f);
  case EASE_OUT:
    return _sin(p * M_PI * 0.5f);
  case EASE_INOUT:
    return 0.5f * (1 - _cos(M_PI * p));
  }
  return 0;
}

api real ease_exponential(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return _pow(1024.0, (p - 1.0));
  case EASE_OUT:
    return 1.0 - _pow(2.0, -10.0 * p);
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return 0.5f * _pow(1024.0, (p - 1.0));
    return 0.5f * (2.0 - _pow(2.0, -10.0 * (p - 1.0)));
  }
  return 0;
}

api real ease_circular(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return 1.0 - _sqrt(1.0 - (p * p));
  case EASE_OUT:
    p = p - 1.0;
    return _sqrt(1.0 - (p * p));
  case EASE_INOUT:
    p = 2.0 * p;
    if (p < 1.0)
      return -0.5f * (_sqrt(1.0 - (p * p)) - 1.0);

    p = p - 2.0;
    return 0.5f * (_sqrt(1.0 - (p * p)) + 1.0);
  }
  return 0;
}

api real ease_elastic(ease_direction d, real p) {
  static const real a = 1.0;
  static const real k = 0.4f;

  return ease_elastic_tunable(d, p, a, k);
}

api real ease_elastic_tunable(ease_direction d, real p, real a, real k) {
  real invk = 1.0 / k;
  real s = a < 1.0 ? (k * 0.25f) : (k * _asin(1.0 / a) * 0.5f * M_INV_PI);

  switch (d) {
  case EASE_IN:
    p = p - 1.0;
    return -(a * _pow(2.0, 10.0 * p) * _sin((p - s) * 2.0 * M_PI * invk));
  case EASE_OUT:
    return 1.0 + (a * _pow(2.0, -10.0 * p) * _sin((p - s) * 2.0 * M_PI * invk));
  case EASE_INOUT:
    p = p * 2.0;
    if (p < 1)
      return -0.5f * a * _pow(2.0, 10.0 * (p - 1.0)) * _sin((p - 1 - s) * 2.0 * M_PI * invk);
    return 1.0 + (0.5f * a * _pow(2.0, -10.0 * (p - 1.0)) * _sin((p - 1 - s) * 2.0 * M_PI * invk));
  }
  return 0;
}

api real ease_back(ease_direction d, real p) {
  static const real s = 1.70158f;
  static const real t = 1.525f;

  return ease_back_tunable(d, p, s, t);
}

api real ease_back_tunable(ease_direction d, real p, real s, real t) {
  switch (d) {
  case EASE_IN:
    return (p * p) * ((p * (s + 1)) - s);
  case EASE_OUT:
    p = p - 1;
    return 1 + ((p * p) * (s + (p * (s + 1))));
  case EASE_INOUT:
    s = s * t;
    p = 2.0 * p;

    if (p < 1)
      return 0.5f * ((p * p) * ((s + 1) * p - s));

    p = p - 2.0;
    return 0.5f * (2.0 + ((p * p) * (s + (p * (s + 1)))));
  }
  return 0;
}

api real _ease_bounce_out(real p) {
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

api real _ease_bounce_in(real p) {
  return 1 - _ease_bounce_out(1 - p);
}

real ease_bounce(ease_direction d, real p) {
  switch (d) {
  case EASE_IN:
    return _ease_bounce_in(p);
  case EASE_OUT:
    return _ease_bounce_out(p);
  case EASE_INOUT:
    if (p < 0.5)
      return 0.5f * _ease_bounce_in(p * 2.0);
    return 0.5f + (0.5f * _ease_bounce_out((p * 2.0) - 1.0));
  }
  return 0;
}



real *interpolate(selector_function select, const real *restrict values, natural length, real percent, real *out) {
  if (safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = clamp01(percent);
  return select(values, length, percent, out);
}

real *select_linear(const real *restrict v, natural l, real p, real *restrict out) {
  natural max_idx, idx, next_idx;
  real scaled_p;
  
  if (safety(v ==  NULL || out == NULL))
    return NULL;
  
  max_idx = l - 1;
  scaled_p = (real)max_idx * p;
  idx = (natural)_floor(scaled_p);
  next_idx = idx + 1;
  
  if (unlikely(next_idx > max_idx))
    *out = v[max_idx];
  else
    *out = lerp_real(v[idx], v[next_idx], (scaled_p - (real)idx));
  return out;
}

real *select_bezier(const real *restrict v, natural l, real p, real *restrict out) {
  natural max_idx, idx, i, j, desired_idx, degree;
  real target;
  real compute_v[DL_BEZIER_DEGREE + 1];

  if (safety(v ==  NULL || out == NULL) || l < 1)
    return NULL;

  max_idx = l - 1;
  degree = clamp(DL_BEZIER_DEGREE, 1, max_idx);
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  for (i = 0; i < degree + 1; ++i) {
    desired_idx = idx + i;
    desired_idx = clamp(desired_idx, 0, max_idx);
    compute_v[i] = v[desired_idx];
  }
    
  for (i = 1; i <= degree; ++i)
    for (j = 0; j <= degree - i; ++j)
      compute_v[j] = ((1.0 - p) * compute_v[j]) + ((p) * compute_v[j+1]);

  *out = compute_v[0];
  return out;
}

real *select_catmullrom(const real *restrict v, natural l, real p, real *restrict out) {
  natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  real target, t, v0, v1, t2, t3;
  
  if (safety(v == NULL || out == NULL))
    return NULL;
  
  max_idx = l - 1;
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  a_idx = unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (real)idx;

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



point2 *interpolate_point2(const selector_function_point2 select, const point2 *restrict values, natural length, real percent, point2 *restrict out) {
  if (safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = clamp01(percent);
  return select(values, length, percent, out);
}

point2 *select_linear_point2(const point2 *restrict v, natural l, real p, point2 *restrict out) {
  natural max_idx, idx, next_idx;
  real scaled_p;
  
  max_idx = l - 1;
  scaled_p = (real)max_idx * p;
  idx = (natural)_floor(scaled_p);
  next_idx = idx + 1;

  if (unlikely(next_idx > max_idx)) {
    *out = v[max_idx];
    return out;
  }

  return lerp_point2(&v[idx], &v[next_idx], (scaled_p - (real)idx), out);
}

point2 *select_bezier_point2(const point2 *restrict v, natural l, real p, point2 *restrict out) {
  natural max_idx, idx, degree;
  real target;
  point2 temp[2], compute_v[DL_BEZIER_DEGREE + 1];
  integer i, j, desired_idx;
  
  max_idx = l - 1;
  degree = clamp(DL_BEZIER_DEGREE, 1, max_idx);
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  for (i = 0; i < degree + 1; ++i) {
    desired_idx = idx + i;
    desired_idx = clamp(desired_idx, 0, max_idx);
    compute_v[i] = v[desired_idx];
  }
    
  for (i = 1; i <= degree; ++i)
    for (j = 0; j <= degree - i; ++j)
      point2_add(point2_mul_scalar(&compute_v[j], 1.0 - p, &temp[0]), point2_mul_scalar(&compute_v[j + 1], p, &temp[1]), &compute_v[j]);

  *out = compute_v[0];
  return out;
}

point2 *select_catmullrom_point2(const point2 *restrict v, natural l, real p, point2 *restrict out) {
  natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  real target, t, t2, t3;
  point2 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;
  
  max_idx = l - 1;
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  a_idx = unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (real)idx;
  t2 = t * t;
  t3 = t2 * t;

  point2_mul_scalar(point2_sub(&v[c_idx], &v[a_idx], &v0), 0.5f, &v0);
  point2_mul_scalar(point2_sub(&v[d_idx], &v[b_idx], &v1), 0.5f, &v1);

  point2_mul_scalar(&v[b_idx], 3, &threev1);
  point2_mul_scalar(&v[c_idx], 3, &threev2);
  point2_mul_scalar(&v0, 2, &twov0);
  point2_mul_scalar(&v[c_idx], -2, &ntwov2);
  point2_mul_scalar(&v[b_idx], 2, &twov1);

  /* a = v[b_idx]; */
  point2_mul_scalar(&v0, t, &b);
  point2_mul_scalar(point2_sub(point2_add(point2_sub(point2_negate(&v1, &c), &twov0, &c), &threev2, &c), &threev1, &c), t2, &c);
  point2_mul_scalar(point2_add(&v1, point2_add(&v0, point2_add(&twov1, &ntwov2, &d), &d), &d), t3, &d);

  return point2_add(&v[b_idx], point2_add(&b, point2_add(&c, &d, &v0), &v0), out);
}



point3 *interpolate_point3(const selector_function_point3 select, const point3 *restrict values, natural length, real percent, point3 *restrict out) {
  if (safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = clamp01(percent);
  return select(values, length, percent, out);
}

point3 *select_linear_point3(const point3 *restrict v, natural l, real p, point3 *restrict out) {
  natural max_idx, idx, next_idx;
  real scaled_p;
  
  max_idx = l - 1;
  scaled_p = (real)max_idx * p;
  idx = (natural)_floor(scaled_p);
  next_idx = idx + 1;

  if (unlikely(next_idx > max_idx)) {
    *out = v[max_idx];
    return out;
  }

  return lerp_point3(&v[idx], &v[next_idx], (scaled_p - (real)idx), out);
}

point3 *select_bezier_point3(const point3 *restrict v, natural l, real p, point3 *restrict out) {
  natural max_idx, idx;
  real target;
  point3 temp[2], compute_v[DL_BEZIER_DEGREE + 1];
  integer i, j, desired_idx;
  
  max_idx = l - 1;
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  for (i = 0; i < DL_BEZIER_DEGREE + 1; ++i) {
    desired_idx = idx + i;
    desired_idx = clamp(desired_idx, 0, max_idx);
    compute_v[i] = v[desired_idx];
  }
    
  for (i = 1; i <= DL_BEZIER_DEGREE; ++i)
    for (j = 0; j <= DL_BEZIER_DEGREE - i; ++j)
      point3_add(point3_mul_scalar(&compute_v[j], 1.0 - p, &temp[0]), point3_mul_scalar(&compute_v[j + 1], p, &temp[1]), &compute_v[j]);

  *out = compute_v[0];
  return out;
}

point3 *select_catmullrom_point3(const point3 *restrict v, natural l, real p, point3 *restrict out) {
  natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  real target, t, t2, t3;
  point3 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;
  
  max_idx = l - 1;
  target = (real)max_idx * p;
  idx = (natural)_floor(target);

  a_idx = unlikely(0 < idx) ? idx - 1 : 0;
  b_idx = idx;
  c_idx = unlikely(max_idx < idx + 1) ? max_idx : idx + 1;
  d_idx = unlikely(max_idx < idx + 2) ? max_idx : idx + 2;

  t = target - (real)idx;
  t2 = t * t;
  t3 = t2 * t;

  point3_mul_scalar(point3_sub(&v[c_idx], &v[a_idx], &v0), 0.5f, &v0);
  point3_mul_scalar(point3_sub(&v[d_idx], &v[b_idx], &v1), 0.5f, &v1);

  point3_mul_scalar(&v[b_idx], 3, &threev1);
  point3_mul_scalar(&v[c_idx], 3, &threev2);
  point3_mul_scalar(&v0, 2, &twov0);
  point3_mul_scalar(&v[c_idx], -2, &ntwov2);
  point3_mul_scalar(&v[b_idx], 2, &twov1);

  /* a = v[b_idx]; */
  point3_mul_scalar(&v0, t, &b);
  point3_mul_scalar(point3_sub(point3_add(point3_sub(point3_negate(&v1, &c), &twov0, &c), &threev2, &c), &threev1, &c), t2, &c);
  point3_mul_scalar(point3_add(&v1, point3_add(&v0, point3_add(&twov1, &ntwov2, &d), &d), &d), t3, &d);

  return point3_add(&v[b_idx], point3_add(&b, point3_add(&c, &d, &v0), &v0), out);
}



api integer lerp_integer(integer a, integer b, real p) {
  return (integer)((real)(b - a) * p) + a;
}

api real lerp_real(real a, real b, real p) {
  return ((b - a) * p) + a;
}

api point2 *lerp_point2(const point2 *restrict a, const point2 *restrict b, real p, point2 *restrict out) {
  return point2_add(point2_mul_scalar(point2_sub(b, a, out), p, out), a, out);
}

api point3 *lerp_point3(const point3 *restrict a, const point3 *restrict b, real p, point3 *restrict out) {
  return point3_add(point3_mul_scalar(point3_sub(b, a, out), p, out), a, out);
}

api vec2 *lerp_vec2(const vec2 *restrict a, const vec2 *restrict b, real p, vec2 *restrict out) {
  return vec2_add(vec2_mul_scalar(vec2_sub(b, a, out), p, out), a, out);
}

api vec3 *lerp_vec3(const vec3 *restrict a, const vec3 *restrict b, real p, vec3 *restrict out) {
  return vec3_add(vec3_mul_scalar(vec3_sub(b, a, out), p, out), a, out);
}

#endif /* DL_USE_TWEEN */
  


/*****************************************************************************
 **  Tools
 ****************************************************************************/

#if DL_USE_MALLOC
#if IS_GNUC
any _default_alloc(natural count, natural element_size) {
  return (any)memalign(sizeof(any), count * element_size);
}
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  _default_alloc,				\
  (void(*)(any))free
#else
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  (any (*)(natural, natural))calloc,		\
  (void (*)(any))free
#endif
#else
#define DECLARE_ALLOC_MEMBERS(alloc, free)	\
  (any (*)(natural, natural))NULL,		\
  (void (*)(any))NULL
#endif

api any memory_swap(any left, any right, natural bytes) {
  natural nat_count, byte_count, *nat_left, *nat_right, nat_temp;
  byte *byte_left, *byte_right, byte_temp;
  
  nat_count = bytes / sizeof(natural);
  byte_count = bytes - (nat_count * sizeof(natural));

  nat_left = (natural *)left;
  nat_right = (natural *)right;

  for (; nat_count > 0; --nat_count) {
    nat_temp = *nat_left;
    *nat_left = *nat_right;
    *nat_right = nat_temp;
    ++nat_left;
    ++nat_right;
  }

  byte_left = (byte *)nat_left;
  byte_right = (byte *)nat_right;

  for (; byte_count > 0; --byte_count) {
    byte_temp = *byte_left;
    *byte_left = *byte_right;
    *byte_right = byte_temp;
    ++byte_left;
    ++byte_right;
  }

  return left;
}

api any memory_copy(any left, any right, natural bytes) {
  natural nat_count, byte_count, *nat_left, *nat_right;
  byte *byte_left, *byte_right;
  
  nat_count = bytes / sizeof(natural);
  byte_count = bytes - (nat_count * sizeof(natural));

  nat_left = (natural *)left;
  nat_right = (natural *)right;

  for (; nat_count > 0; --nat_count) {
    *nat_left = *nat_right;
    ++nat_left;
    ++nat_right;
  }

  byte_left = (byte *)nat_left;
  byte_right = (byte *)nat_right;

  for (; byte_count > 0; --byte_count) {
    *byte_left = *byte_right;
    ++byte_left;
    ++byte_right;
  }

  return left;
}

any memory_set(any left, byte val, natural bytes) {
  natural *nat_left, nat_count, byte_count, nat_val, shift;
  byte *byte_left;
  
  nat_left = (natural *)left;
  nat_count = bytes / sizeof(natural);
  byte_count = bytes - (nat_count * sizeof(natural));

  nat_val = val;
  
  for (shift = 1; shift < sizeof(natural); ++shift)
    nat_val |= (val << shift);

  for (; nat_count > 0; --nat_count) {
    *(natural *)nat_left = nat_val;
    ++nat_left;
  }

  byte_left = (byte *)nat_left;

  for (; byte_count > 0; --byte_count) {
    *(byte *)byte_left = val;
    ++byte_left;
  }

  return left;
}



/*****************************************************************************
 **  Collections
 ****************************************************************************/

#if DL_USE_CONTAINERS

integer _default_compare_8(any data, any left, any right) {
  return (integer)(*(unsigned char *)left - *(unsigned char *)right);
}

integer _default_compare_16(any data, any left, any right) {
  return (integer)(*(unsigned int *)left - *(unsigned int *)right);
}

integer _default_compare_32(any data, any left, any right) {
  return (integer)(*(unsigned long int *)left - *(unsigned long int *)right);
}

integer _default_compare_64(any data, any left, any right) {
#if IS_ATLEAST_C99
  return (integer)(*(unsigned long long int *)left - *(unsigned long long int *)right);
#else
  return 0;
#endif
}

integer _default_compare_any(any data, any left, any right) {
  return 0;
}

typedef struct {
  comparator c;
  any item;
} _collection_sorted_list_predicate_data;

integer _collection_sorted_list_predicate_func(any data, any value) {
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

api vector *init_vector(vector * restrict target, natural element_size, natural capacity) {
  vector_settings settings = default_vector_settings;
  settings.element_size = element_size;
  return init_vector_custom(target, settings, capacity);
}

api vector *init_vector_custom(vector * restrict target, vector_settings settings, natural capacity) {
  real real_count;
  natural slice_count, idx;
  
  if (safety(target == NULL || settings.alloc == NULL || settings.free == NULL))
    return NULL;

  if (settings.element_size < 1)
    return NULL;

  settings.slice_length = settings.slice_length < 1 ? default_vector_settings.slice_length : settings.slice_length;

  real_count = ((real)capacity / (real)settings.slice_length);
  slice_count = (natural)real_count;
  if (real_count > (real)(natural)real_count)
    slice_count++;

  target->slice_count = slice_count < 1 ? 1 : slice_count;

  target->settings = settings;

  target->data.slices = (byte **)target->settings.alloc(target->slice_count, sizeof(byte *));
  if (unlikely(target->data.slices == NULL))
    return NULL;

  for (idx = 0; idx < target->slice_count; ++idx) {
    target->data.slices[idx] = (byte *)target->settings.alloc(target->settings.slice_length, target->settings.element_size);

    if (unlikely(target->data.slices[idx] == NULL))
      break;
  }

  if (unlikely(idx < slice_count)) {
    for (--idx; idx != 0; --idx)
      target->settings.free((any)target->data.slices[idx]);
    target->settings.free((any)target->data.slices);

    return NULL;
  }

  return target;
}

api vector *init_vector_array(vector * restrict target, byte *data, natural element_size, natural count) {
  if (safety(target == NULL || data == NULL))
    return NULL;

  target->settings.alloc = NULL;
  target->settings.free = NULL;
  target->settings.element_size = element_size;
  target->settings.slice_length = count;
  target->slice_count = 0;

  target->data.slices = (byte **)data;

  return target;
}

api void destroy_vector(vector * restrict target, const handler *deconstruct_entry) {
  any entry;
  natural slice_idx, idx;
  
  if (safety(target == NULL) || target->settings.free == NULL)
    return;

  if (target->data.slices != NULL) {
    for (slice_idx = 0; slice_idx < target->slice_count; ++slice_idx) {
      if (deconstruct_entry != NULL) {
        for (idx = 0; idx < target->settings.slice_length; ++idx) {
          entry = &target->data.slices[slice_idx][idx * target->settings.element_size];
	  deconstruct_entry->func(deconstruct_entry->data, entry);
        }
      }

      target->settings.free((any)target->data.slices[slice_idx]);
    }

    target->settings.free((any)target->data.slices);
  }
}

api natural vector_capacity(const vector * restrict v) {
  if (safety(v == NULL))
    return 0;

  return v->slice_count > 0 ? v->slice_count * v->settings.slice_length : v->settings.slice_length;
}

api any vector_get(const vector * restrict v, natural index, any out) {
  natural slice, slice_index;
  byte *target_slice;
  
  if (safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (unlikely(index >= v->settings.slice_length))
      return NULL;

    memory_copy(out, (void *)&v->data.array[index * v->settings.element_size], v->settings.element_size);
  }
  else {
    slice = index / v->settings.slice_length;
    if (unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    memory_copy(out, (void *)&target_slice[slice_index * v->settings.element_size], v->settings.element_size);
  }

  return out;
}

api const any vector_ref(const vector * restrict v, natural index) {
  natural slice, slice_index;
  byte *target_slice;
  
  if (safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (unlikely(index >= v->settings.slice_length))
      return NULL;

    return &v->data.array[index * v->settings.element_size];
  }
  else {
    slice = index / v->settings.slice_length;
    if (unlikely(slice >= v->slice_count))
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    target_slice = v->data.slices[slice];

    return &target_slice[slice_index * v->settings.element_size];
  }
}

api any vector_set(vector * restrict v, natural index, any value) {
  natural base_index, slice, slice_index;
  byte *source, *target;
  
  if (safety(v == NULL))
    return NULL;

  if (v->slice_count == 0) {
    if (unlikely(index >= v->settings.slice_length))
      return NULL;

    base_index = index * v->settings.element_size;
    source = (byte *)value;
    target = v->data.array;

    return memory_copy((void *)&target[base_index], (void *)source, v->settings.element_size);
  }
  else {
    slice = index / v->settings.slice_length;
    if (slice >= v->slice_count)
      return NULL;

    slice_index = index - (slice * v->settings.slice_length);
    base_index = slice_index * v->settings.element_size;
    source = (byte *)value;

    return memory_copy((void *)&v->data.slices[slice][base_index], (void *)source, v->settings.element_size);
  }
}

api bool vector_grow(vector * restrict v) {
  byte **new_slices, *new_slice, **existing_slices;
  natural idx;
  
  if (safety(v == NULL))
    return false;

  if (v->settings.free == NULL || v->settings.alloc == NULL)
    return false;

  new_slices = (byte **)v->settings.alloc(v->slice_count + 1, sizeof(byte *));
  if (unlikely(new_slices == NULL))
    return false;

  new_slice = v->settings.alloc(v->settings.slice_length, v->settings.element_size);
  if (unlikely(new_slice == NULL)) {
    v->settings.free((any)new_slices);
    return false;
  }

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->data.slices[v->slice_count] = new_slice;

  v->settings.free((any)existing_slices);

  v->slice_count++;

  return true;
}

api bool vector_swap(vector * restrict v, natural index1, natural index2) {
  any left, right;
  
  if (safety(v == NULL))
    return false;

  if (index1 == index2)
    return true;

  left = vector_ref(v, index1);
  if (unlikely(left == NULL))
    return false;

  right = vector_ref(v, index2);
  if (unlikely(right == NULL))
    return false;

  memory_swap(left, right, v->settings.element_size);
  return true;
}

api bool vector_shrink(vector * restrict v, handler *restrict deconstruct_entry) {
  byte **new_slices, **existing_slices;
  natural idx;
  any entry;
  
  if (safety(v == NULL))
    return false;

  if (v->slice_count <= 1 || v->settings.free == NULL || v->settings.alloc == NULL)
    return false;

  new_slices = (byte **)v->settings.alloc(v->slice_count - 1, sizeof(byte *));
  if (unlikely(new_slices == NULL))
    return false;

  if (deconstruct_entry != NULL) {
    for (idx = 0; idx < v->settings.slice_length; ++idx) {
      entry = &v->data.slices[v->slice_count - 1][idx * v->settings.element_size];
      deconstruct_entry->func(deconstruct_entry->data, entry);
    }
  }

  v->settings.free((any)v->data.slices[v->slice_count - 1]);

  existing_slices = v->data.slices;
  v->data.slices = new_slices;

  for (idx = 0; idx < v->slice_count - 1; ++idx)
    v->data.slices[idx] = existing_slices[idx];
  v->slice_count--;

  return true;
}

api bool vector_resize(vector * restrict v, natural minimum_capacity, handler *deconstruct_entry) {
  natural current_capacity, new_slice_count, slice_idx, slice_count, item_idx;
  real needed;
  integer needed_count;
  byte **new_slices, *slice;
  any item;
  
  if (safety(v == NULL))
    return false;

  if (v->settings.free == NULL || v->settings.alloc == NULL || v->slice_count < 1 || minimum_capacity < 1)
    return false;

  current_capacity = vector_capacity(v);
  needed = (real)((integer)minimum_capacity - (integer)current_capacity) / (real)v->settings.slice_length;

  needed_count = needed < 0 ? (integer)needed : (integer)needed + 1;

  if (likely(needed_count != 0)) {
    new_slice_count = v->slice_count + needed_count;
    new_slices = (byte **)v->settings.alloc(new_slice_count, sizeof(byte **));
    if (unlikely(new_slices == NULL))
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

        v->settings.free((any)slice);
      }
    }
    /* Growing */
    else {
      for (; slice_idx < new_slice_count; ++slice_idx) {
        new_slices[slice_idx] = v->settings.alloc(v->settings.slice_length, v->settings.element_size);
        if (unlikely(new_slices[slice_idx] == NULL))
          break;
      }
      if (unlikely(slice_idx != new_slice_count)) {
        for (slice_idx--; slice_idx >= v->slice_count; --slice_idx)
          v->settings.free((any)new_slices[slice_idx]);
        v->settings.free((any)new_slices);
        return false;
      }
    }

    v->settings.free((any)v->data.slices);
    v->data.slices = new_slices;
    v->slice_count = new_slice_count;
  }

  return true;
}

api natural vector_copy_array(vector * restrict target, natural target_offset_index, const byte *data, natural count) {
  vector source;
  if (unlikely(!init_vector_array(&source, (byte *)data, target->settings.element_size, count)))
    return 0;

  return vector_copy(target, target_offset_index, &source);
}

api natural vector_copy(vector * restrict target, natural target_offset_index, const vector * restrict original) {
  natural original_capacity, element_size, target_length, original_length, target_remainder, original_remainder, total_remainder, target_slice_idx, original_slice_idx, min_remainder, count_to_copy;
  byte *target_slice, *original_slice;
  
  if (safety(original == NULL || target == NULL))
    return 0;

  if (unlikely(original->settings.element_size != target->settings.element_size))
    return 0;

  /* target is too small */
  original_capacity = vector_capacity(original);
  if (unlikely(vector_capacity(target) - target_offset_index < original_capacity))
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

    memory_copy(&target_slice[target_length - target_remainder], &original_slice[original_length - original_remainder], count_to_copy);

    total_remainder -= count_to_copy;
    target_remainder -= count_to_copy;
    original_remainder -= count_to_copy;
  }

  return original_capacity;
}

api natural vector_ref_array(vector * restrict v, natural index, any *restrict out) {
  natural last_idx, slice, slice_index;
  
  if (safety(v == NULL))
    return 0;

  last_idx = v->settings.slice_length - 1;

  if (v->slice_count == 0 && index < last_idx) {
    *((byte **)out) = &v->data.array[index];
    return last_idx - index;
  }
  else {
    slice = index / v->settings.slice_length;
    if (unlikely(slice >= v->slice_count))
      return 0;

    slice_index = index - (slice * v->settings.slice_length);
    *((byte **)out) = &v->data.slices[slice][slice_index];
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

api struct linked_list_node *_linked_list_node_alloc(linked_list * restrict list, struct linked_list_node * restrict after) {
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

api void _linked_list_node_free(linked_list * restrict list, struct linked_list_node * restrict node) {
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

api void _linked_list_node_detach_free(linked_list *restrict list, struct linked_list_node *e) {
  if (list->free == e)
    list->free = e->next;
  if (e->next != NULL)
    e->next->previous = e->previous;
  if (e->previous != NULL)
    e->previous->next = e->next;
  e->next = e->previous = NULL;
}

typedef struct {
  handler *original_destructor;
  linked_list *list;
} _linked_list_node_deconstructor_data;

api any _linked_list_node_deconstructor(any data, any element) {
  _linked_list_node_deconstructor_data *d;
  struct linked_list_node *f, *e, *new_node;
  handler *destruct;

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

api linked_list *init_linked_list(linked_list * restrict target, natural element_size, natural cache_length) {
  linked_list_settings settings;
  
  if (safety(target == NULL || element_size == 0))
    return false;

  settings = default_linked_list_settings;
  settings.element_size = element_size;
  settings.cache_length = cache_length;

  return init_linked_list_custom(target, settings);
}

api linked_list *_linked_list_cache_grow(linked_list * restrict target) {
  linked_list_settings settings;
  vector *v;
  natural zero, length, idx;
  struct linked_list_node *node;
  
  if (safety(target == NULL))
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
    memory_set(node, 0, LINKED_LIST_HEADER_SIZE);
    _linked_list_node_free(target, node);
  }

  return target;
}

api linked_list *init_linked_list_custom(linked_list * restrict target, linked_list_settings settings) {
  if (safety(target == NULL || settings.element_size < 1))
    return NULL;

  target->first = target->last = target->free = NULL;
  target->settings = settings;

  return _linked_list_cache_grow(target);
}

api natural linked_list_copy(linked_list * restrict target, struct linked_list_node *restrict target_position, const linked_list * restrict original) {
  struct linked_list_node *source_node, *next;
  natural count;
  
  if (safety(original == NULL || target == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  source_node = original->first;
  count = 0;

  while (source_node != NULL) {
    next = linked_list_add(target, target_position, LINKED_LIST_DATA(source_node));
    if (unlikely(next == NULL))
      break;

    target_position = next;
    source_node = source_node->next;
    ++count;
  }

  return count;
}

api natural linked_list_copy_array(linked_list * restrict target, struct linked_list_node *target_position, const byte *data, natural length) {
  natural count, byte_size, idx;
  struct linked_list_node *next;
  
  if (safety(target == NULL || data == NULL))
    return 0;

  if (target_position == NULL)
    target_position = target->last;

  count = 0;
  byte_size = target->settings.element_size;

  for (idx = 0; idx < length; ++idx) {
    next = linked_list_add(target, target_position, (byte *)&data[idx * byte_size]);
    if (unlikely(next == NULL))
      break;

    target_position = next;
    ++count;
  }

  return count;
}

api void destroy_linked_list(linked_list * restrict target, handler *restrict deconstruct_entry) {
  struct linked_list_node *node;
  
  if (safety(target == NULL))
    return;

  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL) {
    node = target->first;
    while (node != NULL) {
      deconstruct_entry->func(deconstruct_entry->data, LINKED_LIST_DATA(node));
      node = node->previous;
    }
  }

  destroy_vector(&target->node_cache, NULL);

  memory_set(target, 0, sizeof(linked_list));
}

api natural linked_list_capacity(const linked_list * restrict list) {
  if (safety(list == NULL))
    return 0;

  return vector_capacity(&list->node_cache);
}

api natural linked_list_length(const linked_list * restrict list) {
  natural length;
  struct linked_list_node *node;
  
  if (safety(list == NULL))
    return 0;

  length = 0;
  node = list->first;
  while (node != NULL) {
    node = node->next;
    ++length;
  }

  return length;
}

api bool linked_list_grow(linked_list * restrict list) {
  return _linked_list_cache_grow(list) != NULL;
}

api bool linked_list_shrink(linked_list * restrict list, handler *restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  handler destructor;
  
  if (safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;

  return vector_shrink(&list->node_cache, &destructor);
}

api bool linked_list_resize(linked_list * restrict list, natural minimum_capacity, handler *restrict deconstruct_entry) {
  _linked_list_node_deconstructor_data data;
  handler destructor;
  
  if (safety(list == NULL))
    return false;

  data.original_destructor = deconstruct_entry;
  data.list = list;
  destructor.func = _linked_list_node_deconstructor;
  destructor.data = &data;
  
  return vector_resize(&list->node_cache, minimum_capacity, &destructor);
}

api any linked_list_get(const linked_list * restrict list, struct linked_list_node *position, any out) {
  if (safety(list == NULL || position == NULL || out == NULL))
    return NULL;

  return memory_copy(out, LINKED_LIST_DATA(position), list->settings.element_size);
}

api const any linked_list_ref(const struct linked_list_node *position) {
  if (safety(position == NULL))
    return NULL;

  return (const any)LINKED_LIST_DATA(position);
}

api any linked_list_set(linked_list * restrict list, struct linked_list_node *position, any value) {
  if (safety(list == NULL || position == NULL || value == NULL))
    return NULL;

  return memory_copy(LINKED_LIST_DATA(position), value, list->settings.element_size);
}

api struct linked_list_node *linked_list_add(linked_list * restrict list, struct linked_list_node *position, any value) {
  struct linked_list_node *node;
  
  if (safety(list == NULL))
    return NULL;

  if (list->free == NULL)
    return NULL;

  node = _linked_list_node_alloc(list, position);
  if (unlikely(value != NULL && !memory_copy(LINKED_LIST_DATA(node), value, list->settings.element_size))) {
    _linked_list_node_free(list, node);
    return NULL;
  }

  return node;
}

api any linked_list_remove(linked_list * restrict list, struct linked_list_node *position, any out) {
  if (safety(list == NULL || position == NULL))
    return NULL;

  if (unlikely(!memory_copy(out, LINKED_LIST_DATA(position), list->settings.element_size)))
    return NULL;

  _linked_list_node_free(list, position);

  return out;
}

api bool linked_list_destroy(linked_list * restrict list, struct linked_list_node *position, handler *deconstruct_entry) {
  if (safety(list == NULL || position == NULL))
    return false;
  
  if (deconstruct_entry != NULL && deconstruct_entry->func != NULL)
    deconstruct_entry->func(deconstruct_entry->data, LINKED_LIST_DATA(position));

  _linked_list_node_free(list, position);

  return true;
}

api natural linked_list_destroy_range(linked_list * restrict list, struct linked_list_node *position, natural count, handler *deconstruct_entry) {
  natural removed;
  struct linked_list_node *next;
  
  if (safety(list == NULL || position == NULL || count == 0))
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

api bool linked_list_swap(linked_list * restrict list, struct linked_list_node *position1, struct linked_list_node *position2, bool data) {
  struct linked_list_node *t;
  
  if (safety(list == NULL || position1 == NULL || position2 == NULL))
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
    memory_swap(LINKED_LIST_DATA(position1), LINKED_LIST_DATA(position2), list->settings.element_size);

  return true;
}

api struct linked_list_node *linked_list_index(linked_list * restrict list, natural position) {
  struct linked_list_node *node;
  
  if (safety(list == NULL))
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

api integer iterator_compare(const collection *restrict col, iterator left, iterator right) {
  if (!iterator_is_valid(col, left) && !iterator_is_valid(col, right))
    return 0;

  if (iterator_is_valid(col, left) && !iterator_is_valid(col, right))
    return 1;

  if (!iterator_is_valid(col, left) && iterator_is_valid(col, right))
    return -1;

  return col->settings.functions->_iterator_compare(col, left, right);
}

api bool iterator_equal(const collection *restrict col, const iterator left, const iterator right) {
  return 0 == iterator_compare(col, left, right);
}

api bool iterator_is_valid(const collection *restrict col, const iterator index) {
  if (safety(col == NULL))
    return false;

  return col->settings.functions->_iterator_is_valid(col, index);
}

api iterator make_invalid_iterator(const collection *restrict col) {
  return col->settings.functions->_make_invalid_iterator(col);
}



/*****************************************************************************
 **  Container Dependent
 ****************************************************************************/

api void _force_collection_properties(collection *restrict col) {
  iterator current, next;
  any ref;
  
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

api any collection_push_start(collection *restrict col, iterator *iter) {
  if (safety(col == NULL))
    return NULL;

  return col->settings.functions->_collection_push_start(col, iter);
}

api any collection_push_finish(collection *restrict col, iterator *iter) {
  comparator *comp;
  bool is_set;
  any ref, prev_ref;
  iterator prev;
  integer c;

  if (safety(col == NULL || iter == NULL || !iterator_is_valid(col, *iter)))
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

api void _vector_queue_roll_slices(collection *restrict col) {
  byte *slice_left, *slice_first;
  natural last_idx, slice_idx;
  
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

api bool collection_is_empty(const collection *restrict col) {
  if (safety(col == NULL))
    return true;

  return col->settings.functions->_collection_is_empty(col);
}

api any collection_pop(collection *restrict col, any out) {
  if (safety(col == NULL || out == NULL))
    return NULL;

  if (collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_pop(col, out);
}

api bool collection_pop_destroy(collection *restrict col) {
  if (safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_destroy(col);
}

api bool collection_pop_forget(collection *restrict col) {
  if (safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_pop_forget(col);
}

api integer collection_ref_array(collection *restrict col, iterator iter, any *out_array) {  
  if (safety(col == NULL || out_array == NULL))
    return 0;

  if (!iterator_is_valid(col, iter))
    return 0;

  return col->settings.functions->_collection_ref_array(col, iter, out_array);
}

api const any collection_ref(const collection *restrict col, iterator iter) {
  if (safety(col == NULL))
    return NULL;

  if (!iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_ref((collection *restrict )col, iter);
}

api bool collection_swap(collection *restrict col, iterator *iter_a, iterator *iter_b) {
  if (safety(col == NULL || iter_a == NULL || iter_b == NULL))
    return false;

  if (!iterator_is_valid(col, *iter_a) || !iterator_is_valid(col, *iter_b))
    return false;

  return col->settings.functions->_collection_swap(col, iter_a, iter_b);
}

any collection_get(const collection *restrict col, iterator iter, any out) {
  if (safety(col == NULL))
    return NULL;

  if (!iterator_is_valid(col, iter))
    return NULL;

  return col->settings.functions->_collection_get(col, iter, out);
}

any collection_set(collection *restrict col, iterator *iter, any value) {
  if (safety(col == NULL))
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

api iterator collection_index(collection *restrict col, natural index) {
  if (safety(col == NULL || (integer)index >= collection_count(col)))
    return collection_end(col);

  return col->settings.functions->_collection_index(col, index);
}

api const any collection_next(const collection *restrict col, iterator *iter) {
  if (safety(col == NULL || iter == NULL)) {
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

api const any collection_prev(const collection *restrict col, iterator *iter) {
  if (safety(col == NULL || iter == NULL))
    return NULL;

  col->settings.functions->_collection_prev(col, iter);
  
  return collection_ref(col, *iter);
}

api integer collection_count(const collection *restrict col) {
  if (safety(col == NULL))
    return 0;

  return col->settings.functions->_collection_count(col);
}

api iterator collection_begin(const collection *restrict col) {
  if (safety(col == NULL))
    return make_invalid_iterator(col);

  return col->settings.functions->_collection_begin(col);
}

api iterator collection_end(const collection *restrict col) {
  if (safety(col == NULL))
    return make_invalid_iterator(col);

  return col->settings.functions->_collection_end(col);
}

const any _collection_search_region_vector(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter) {
  any ref;
  integer outcome;
  iterator iter_next;
  
  if (left.vector.index >= right.vector.index)
    return NULL;

  iter->vector.index = ((right.vector.index - left.vector.index) >> 1) + left.vector.index;

  if (unlikely(!(ref = vector_ref(&col->data.vector.container, iter->vector.index))))
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

const any _collection_linear_search(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter) {
  any ref;
  integer outcome;
  
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

const any collection_search_region(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter) {
  if (safety(col == NULL || predicate == NULL || iter == NULL))
    return NULL;

  return col->settings.functions->_collection_search_region(col, predicate, left, right, iter);
}

const any collection_search(const collection *restrict col, filter *predicate, iterator *iter) {
  return collection_search_region(col, predicate, collection_begin(col), collection_end(col), iter);
}

api bool collection_destroy_at(collection *restrict col, iterator *iter) {
  handler *destructor;
  
  if (safety(col == NULL || !iterator_is_valid(col, *iter)))
    return false;

  if (collection_is_empty(col))
    return false;

  destructor = col->settings.deconstruct_entry.func == NULL ? NULL : &col->settings.deconstruct_entry;

  return col->settings.functions->_collection_destroy_at(col, iter, destructor);
}

api bool collection_forget_at(collection *restrict col, iterator *iter) {
  if (safety(col == NULL))
    return false;

  if (collection_is_empty(col))
    return false;

  return col->settings.functions->_collection_destroy_at(col, iter, NULL);
}

api any collection_remove_at(collection *restrict col, iterator *iter, any out) {
  if (safety(col == NULL || out == NULL))
    return NULL;

  if (collection_is_empty(col))
    return NULL;

  return col->settings.functions->_collection_remove_at(col, iter, out);
}

api integer collection_destroy_range(collection *restrict col, iterator *index, natural count) {
  if (safety(col == NULL || !iterator_is_valid(col, *index)))
    return 0;

  return col->settings.functions->_collection_destroy_range(col, index, count);
}

any collection_insert(collection *restrict col, iterator *restrict position, any item) {
  if (safety(col == NULL || !iterator_is_valid(col, *position)))
    return NULL;

  if (collection_is_set(col) && collection_contains(col, item))
    return NULL;

  if (collection_is_sorted(col))
    return collection_push_index(col, item, position);

  if (col->settings.functions->_collection_insert(col, position, item))
    return collection_ref(col, *position);
  return NULL;
}

api natural collection_element_size(const collection *restrict col) {
  if (safety(col == NULL))
    return 0;

  return col->settings.element_size;
}



/*****************************************************************************
 **  Container Independent
 ****************************************************************************/

api bool collection_is_vector(const collection * restrict col) {
  return col != NULL && col->settings.storage == STORAGE_TYPE_VECTOR;
}

api bool collection_is_linked_list(const collection * restrict col) {
  return col != NULL && col->settings.storage == STORAGE_TYPE_LINKED_LIST;
}

api bool collection_is_queue(const collection *restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_QUEUE);
}

api bool collection_is_set(const collection *restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_SET);
}

api bool collection_is_sorted(const collection *restrict col) {
  return col != NULL &&
    (col->settings.type == COLLECTION_TYPE_SORTED_LIST
     || col->settings.type == COLLECTION_TYPE_SET);
}

api const any collection_begin_ref(const collection *restrict col, iterator *iter) {
  if (safety(iter == NULL || col == NULL))
    return NULL;

  *iter = collection_begin(col);
  return collection_ref(col, *iter);
}

api const any collection_end_ref(const collection *restrict col, iterator *iter) {
  if (safety(iter == NULL || col == NULL))
    return NULL;

  *iter = collection_end(col);
  return collection_prev(col, iter);
}

api bool collection_sort(collection *restrict col) {
  if (safety(col == NULL))
    return false;

  return collection_quick_sort(col, &col->settings.comparer);
}

api void collection_clear(collection *restrict col) {
  while (collection_pop_destroy(col));
}

api const any collection_peek(const collection *restrict col) {
  iterator iter;
  
  if (safety(col == NULL))
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

api iterator collection_index_of(const collection *restrict col, const any item) {
  iterator bad, iter, index;
  any candidate;
  _collection_sorted_list_predicate_data data;
  filter predicate;
  
  bad = make_invalid_iterator(col);

  if (safety(col == NULL))
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

api bool collection_all(const collection *restrict col, filter *restrict f) {
  iterator index;
  any item;
  
  if (safety(col == NULL || f == NULL))
    return false;

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index))
    if (!f->func(f->data, item))
      return false;

  return true;
}

api bool collection_any(const collection *restrict col, filter *restrict f) {
  iterator index;
  any item;
  
  if (safety(col == NULL || f == NULL))
    return false;

  index = collection_begin(col);
  for (item = collection_ref(col, index);
       item != NULL;
       item = collection_next(col, &index))
    if (f->func(f->data, item))
      return true;

  return false;
}

api integer collection_drop(collection *restrict col, natural count) {
  natural removed = 0;

  if (safety(col == NULL))
    return 0;

  for (; count > 0; --count, ++removed)
    if (!collection_pop_destroy(col))
      return removed;

  return removed;
}

api integer collection_map(const collection *restrict col, converter *func, collection *out) {
  iterator iter, new_iter;
  integer count;
  any new_ref;
  any ref;
  
  if (safety(col == NULL || func == NULL || out == NULL))
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

api any collection_foldl(const collection *restrict col, any initial, folder *func) {
  iterator index;
  any item;
  
  if (safety(col == NULL || func == NULL))
    return NULL;

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index))
    func->func(func->data, initial, item);

  return initial;
}

api any collection_foldr(const collection *restrict col, any initial, folder *func) {
  iterator index;
  any item;
  
  if (safety(col == NULL || func == NULL))
    return NULL;

  index = collection_end(col);

  while ((item = collection_prev(col, &index)))
    func->func(func->data, initial, item);

  return initial;
}

api integer collection_zip(const collection *restrict col1, const collection *restrict col2, zipper *zip, collection *out) {
  iterator iter1, iter2, new_iter;
  integer added;
  any ref1, ref2, new_ref;
  
  if (safety(col1 == NULL || col2 == NULL || zip == NULL || out == NULL))
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

api integer collection_take(collection *restrict col, natural count, collection *out) {
  any ref;
  natural added;
  if (safety(col == NULL || out == NULL))
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

api any collection_find(const collection *restrict col, filter *f, iterator *index) {
  any item;
  
  if (safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  for (item = collection_ref(col, *index); item != NULL; item = collection_next(col, index))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

api any collection_find_last(const collection *restrict col, filter *f, iterator *index) {
  any item;
  
  if (safety(col == NULL || f == NULL || index == NULL))
    return NULL;

  while ((item = collection_prev(col, index)))
    if (f->func(f->data, item))
      return item;

  return NULL;
}

api integer collection_find_all(const collection *restrict col, filter *f, collection *out) {
  natural start_count;
  iterator index;
  any item;
  
  if (safety(col == NULL || f == NULL || out == NULL))
    return 0;

  start_count = collection_count(out);

  index = collection_begin(col);
  for (item = collection_ref(col, index); item != NULL; item = collection_next(col, &index)) {
    if (f->func(f->data, item) && !collection_push(out, item))
      break;
  }

  return collection_count(out) - start_count;
}

api any collection_remove_first(collection *restrict col, filter *f, iterator *iter, any out) {
  if (safety(col == NULL || iter == NULL))
    return NULL;

  if (!collection_find(col, f, iter))
    return NULL;

  return collection_remove_at(col, iter, out);
}

api any collection_remove_last(collection *restrict col, filter *f, iterator *iter, any out) {
  if (safety(col == NULL || iter == NULL))
    return NULL;

  if (!collection_find_last(col, f, iter))
    return NULL;

  return collection_remove_at(col, iter, out);
}

api bool collection_destroy_first(collection *restrict col, filter *f, iterator *index) {
  if (safety(col == NULL || index == NULL))
    return false;

  if (!collection_find(col, f, index))
    return false;

  return collection_destroy_at(col, index);
}

api bool collection_destroy_last(collection *restrict col, filter *f, iterator *index) {
  if (safety(col == NULL || index == NULL))
    return false;

  if (!collection_find_last(col, f, index))
    return false;

  return collection_destroy_at(col, index);
}

bool collection_quick_sort_region(collection *restrict col, comparator *compare, iterator left, iterator right) {
  iterator pivot, leftwall, iter;
  any pivot_ref, iter_ref;
  
  if (safety(col == NULL || col->settings.comparer.func == NULL))
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

api bool collection_quick_sort(collection *restrict col, comparator *compare) {
  return collection_quick_sort_region(col, compare, collection_begin(col), collection_end(col));
}

api integer collection_destroy_all(collection *restrict col, filter *f) {
  natural count;
  any item;
  iterator iter;
  
  if (safety(col == NULL || f == NULL))
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

api bool collection_contains(const collection *restrict col, any item) {
  iterator iter;
  iter= collection_index_of(col, item);
  return iterator_is_valid(col, iter);
}

api any collection_push(collection *restrict col, any value) {
  iterator iter;
  return collection_push_index(col, value, &iter);
}

api any collection_push_index(collection *restrict col, any value, iterator *out_index) {
  any ref;
  
  ref = collection_push_start(col, out_index);
  if (!iterator_is_valid(col, *out_index))
    return NULL;

  memory_copy(ref, value, collection_element_size(col));
  return collection_push_finish(col, out_index);
}

api integer collection_remove_range(collection *restrict col, iterator *index, natural count, collection *out) {
  natural total;
  iterator new_iter;
  any ref;
  
  if (safety(col == NULL || out == NULL || index == NULL || !iterator_is_valid(col, *index)))
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


api integer collection_remove_all(collection *restrict col, filter *f, collection *out) {
  iterator index, new_iter;
  natural total;
  any ref, new_ref;
  
  if (safety(col == NULL || out == NULL || f == NULL))
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

api integer collection_copy(const collection *restrict original, collection *restrict target) {
  iterator iter, new_iter;
  integer count;
  any ref, new_ref;
  bool is_set;
  
  if (safety(original == NULL || target == NULL))
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

    memory_copy(new_ref, ref, collection_element_size(original));
    collection_push_finish(target, &new_iter);

    ++count;
  }

  return count;
}

api integer collection_copy_array(const any data, natural count, collection *restrict target) {
  collection source;

  if (safety(target == NULL || data == NULL))
    return 0;

  if (unlikely(!init_collection_array(&source, COLLECTION_TYPE_LIST, NULL, NULL, data, collection_element_size(target), count)))
    return 0;

  return collection_copy(&source, target);
}



/*****************************************************************************
 **  Container Dispatchers
 ****************************************************************************/

api integer _vector_iterator_compare(const collection *restrict col, iterator left, iterator right) {
  return left.vector.index - right.vector.index;
}

api integer _linked_list_iterator_compare(const collection *restrict col, iterator left, iterator right) {
  return (integer)(left.linked_list.node - right.linked_list.node);
}

api bool _vector_iterator_is_valid(const collection *restrict col, iterator index) {
  return index.vector.index < col->data.vector.index[1]
    && index.vector.index >= col->data.vector.index[0];
}

api bool _linked_list_iterator_is_valid(const collection *restrict col, iterator index) {
  return index.linked_list.node != NULL;
}

api iterator _vector_make_invalid_iterator(const collection *restrict col) {
  iterator bad;
  bad.vector.index = NATURAL_MAX;
  return bad;
}

api iterator _linked_list_make_invalid_iterator(const collection *restrict col) {
  iterator bad;
  bad.linked_list.node = NULL;

  return bad;
}

api any _vector_collection_push_start(collection *restrict col, iterator *iter) {
  if (col->data.vector.index[1] >= vector_capacity(&col->data.vector.container)
      && !vector_grow(&col->data.vector.container))
    return NULL;

  iter->vector.index = col->data.vector.index[1];
  col->data.vector.index[1]++;

  return vector_ref(&col->data.vector.container, iter->vector.index);
}

api any _linked_list_collection_push_start(collection *restrict col, iterator *iter) {
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

api bool _vector_collection_is_empty(const collection *restrict col) {
  return col->data.vector.index[0] == col->data.vector.index[1];
}

api bool _linked_list_collection_is_empty(const collection *restrict col) {
  return col->data.linked_list.container.first == NULL;
}

api any _vector_collection_pop(collection *restrict col, any out) {
  vector *v;
  natural idx;

  if (collection_is_queue(col)) {
    v = &col->data.vector.container;
    idx = col->data.vector.index[0];

    if (unlikely(!vector_get(v, idx, out)))
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

api any _linked_list_collection_pop(collection * restrict col, any out) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_remove(list, position, out);
}

api bool _vector_collection_pop_destroy(collection *restrict col) {
  natural index;
  any ref;
  
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

api bool _linked_list_collection_pop_destroy(collection *restrict col) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_destroy(list, position, &col->settings.deconstruct_entry);
}

api bool _vector_collection_pop_forget(collection *restrict col) {
  natural index;
  
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

api bool _linked_list_collection_pop_forget(collection *restrict col) {
  linked_list *list;
  struct linked_list_node *position;

  list = &col->data.linked_list.container;
  
  if (collection_is_queue(col))
    position = list->first;
  else
    position = list->last;

  return linked_list_destroy(list, position, NULL);
}

api integer _vector_collection_ref_array(collection *restrict col, iterator iter, any *out_array) {
  natural count, limit;
    
  count = vector_ref_array(&col->data.vector.container, iter.vector.index, out_array);
  limit = col->data.vector.index[1] - iter.vector.index;

  return count < limit ? count : limit;
}

api integer _linked_list_collection_ref_array(collection *restrict col, iterator iter, any *out_array) {
  out_array[0] = linked_list_ref(iter.linked_list.node);
  return 1;
}

api const any _vector_collection_ref(const collection *restrict col, iterator iter) {
  return vector_ref(&col->data.vector.container, iter.vector.index);
}

api const any _linked_list_collection_ref(const collection *restrict col, iterator iter) {
  return linked_list_ref(iter.linked_list.node);
}

api bool _vector_collection_swap(collection *restrict col, iterator *iter_a, iterator *iter_b) {
  iterator t;

  if (!vector_swap(&col->data.vector.container, iter_a->vector.index, iter_b->vector.index))
    return false;
  t = *iter_a;
  *iter_a = *iter_b;
  *iter_b = t;
  return true;
}

api bool _linked_list_collection_swap(collection *restrict col, iterator *iter_a, iterator *iter_b) {
  if (!linked_list_swap(&col->data.linked_list.container, iter_a->linked_list.node, iter_b->linked_list.node, false))
    return false;
  return true;
}

any _vector_collection_get(const collection *restrict col, iterator iter, any out) {
  return vector_get(&col->data.vector.container, iter.vector.index, out);
}

any _linked_list_collection_get(const collection *restrict col, iterator iter, any out) {
  return linked_list_get(&col->data.linked_list.container, iter.linked_list.node, out);
}

any _vector_collection_set(collection *restrict col, iterator *iter, any value) {
  any ref;

  if ((ref = vector_set(&col->data.vector.container, iter->vector.index, value)))
    return collection_ref(col, *iter);
  return NULL;
}

any _linked_list_collection_set(collection *restrict col, iterator *iter, any value) {
  any ref;

  if ((ref = linked_list_set(&col->data.linked_list.container, iter->linked_list.node, value)))
    return collection_ref(col, *iter);
  return NULL;
}

api iterator _vector_collection_index(collection *restrict col, natural index) {
  iterator iter;
  iter.vector.index = index;
  return iter;
}

api iterator _linked_list_collection_index(collection *restrict col, natural index) {
  iterator iter;
  iter.linked_list.node = linked_list_index(&col->data.linked_list.container, index);
  return iter;
}

api void _vector_collection_next(const collection *restrict col, iterator *iter) {
  iter->vector.index++;
}

api void _linked_list_collection_next(const collection *restrict col, iterator *iter) {
  iter->linked_list.node = iter->linked_list.node->next;
}

api void _vector_collection_prev(const collection *restrict col, iterator *iter) {
  if (!iterator_is_valid(col, *iter))
    iter->vector.index = col->data.vector.index[1] - 1;
  else if (iter->vector.index > col->data.vector.index[0])
    iter->vector.index--;
  else
    *iter = make_invalid_iterator(col);
}

api void _linked_list_collection_prev(const collection *restrict col, iterator *iter) {
  if (!iterator_is_valid(col, *iter))
    iter->linked_list.node = col->data.linked_list.container.last;
  else if (iter->linked_list.node != col->data.linked_list.container.first)
    iter->linked_list.node = iter->linked_list.node->previous;
  else
    *iter = make_invalid_iterator(col);
}

api integer _vector_collection_count(const collection *restrict col) {
  return col->data.vector.index[1] - col->data.vector.index[0];
}

api integer _linked_list_collection_count(const collection *restrict col) {
  return linked_list_length(&col->data.linked_list.container);
}

api iterator _vector_collection_begin(const collection *restrict col) {
  iterator iter;
  iter.vector.index = col->data.vector.index[0];
  return iter;
}

api iterator _linked_list_collection_begin(const collection *restrict col) {
  iterator iter;
  iter.linked_list.node = col->data.linked_list.container.first;
  return iter;
}

api iterator _vector_collection_end(const collection *restrict col) {
  iterator iter;
  iter.vector.index = col->data.vector.index[1];
  return iter;
}

api iterator _linked_list_collection_end(const collection *restrict col) {
  iterator iter;
  iter.linked_list.node = NULL;
  return iter;
}

api const any _vector_collection_search_region(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter) {
  if (collection_is_sorted(col))
    return _collection_search_region_vector(col, predicate, left, right, iter);
  return _collection_linear_search(col, predicate, left, right, iter);
}

api const any _linked_list_collection_search_region(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter) {
  return _collection_linear_search(col, predicate, left, right, iter);
}

api bool _vector_collection_destroy_at(collection *restrict col, iterator *iter, handler *destructor) {
  vector *v;
  natural index, swap_index;
  any item;
  
  v = &col->data.vector.container;
  index = iter->vector.index;

  if (destructor != NULL) {
    if (unlikely(!(item = vector_ref(v, index))))
      return false;
    destructor->func(destructor->data, item);
  }

  if (collection_count(col) > 1)
    for (swap_index = col->data.vector.index[1] - 1; swap_index > 0 && swap_index > index; --swap_index)
      vector_swap(v, swap_index, index);
  col->data.vector.index[1]--;

  return true;
}

api bool _linked_list_collection_destroy_at(collection *restrict col, iterator *iter, handler *destructor) {
  struct linked_list_node *next;
  next = iter->linked_list.node->next;
  if (linked_list_destroy(&col->data.linked_list.container, iter->linked_list.node, destructor)) {
    iter->linked_list.node = next;
    return true;
  }
  return false;
}
  
api any _vector_collection_remove_at(collection *restrict col, iterator *iter, any out) {
  vector *v;
  natural index, swap_index;
  
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

api any _linked_list_collection_remove_at(collection *restrict col, iterator *iter, any out) {
  struct linked_list_node *next;
  
  next = iter->linked_list.node->next;
  if ((out = linked_list_remove(&col->data.linked_list.container, iter->linked_list.node, out))) {
    iter->linked_list.node = next;
    return out;
  }

  return NULL;
}

  
api integer _vector_collection_destroy_range(collection *restrict col, iterator *iter, natural count) {
  natural index, start, total;
  handler destruct;
  vector *v;
  any item;
  
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
  
api integer _linked_list_collection_destroy_range(collection *restrict col, iterator *index, natural count) {
  struct linked_list_node *next;
  natural idx;

  next = index->linked_list.node;
  for (idx = 0; idx < count && next != NULL; ++idx)
    next = next->next;
  count = linked_list_destroy_range(&col->data.linked_list.container, index->linked_list.node, count, &col->settings.deconstruct_entry);
  index->linked_list.node = next;
  return count;
}

bool _vector_collection_insert(collection *restrict col, iterator *restrict position, any item) {
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

bool _linked_list_collection_insert(collection *restrict col, iterator *restrict position, any item) {
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
  (integer (*)(const collection *restrict col, iterator left, iterator right))_vector_iterator_compare,
  (bool (*)(const collection *restrict col, iterator index))_vector_iterator_is_valid,
  (iterator (*)(const collection *restrict col))_vector_make_invalid_iterator,
  (any (*)(collection *restrict col, iterator *iter))_vector_collection_push_start,
  (bool (*)(const collection *restrict col))_vector_collection_is_empty,
  (any (*)(const collection *restrict col, any out))_vector_collection_pop,
  (bool (*)(collection *restrict col))_vector_collection_pop_destroy,
  (bool (*)(collection *restrict col))_vector_collection_pop_forget,
  (integer (*)(collection *restrict col, iterator iter, any *out_array))_vector_collection_ref_array,
  (any (*)(collection *restrict col, iterator iter))_vector_collection_ref,
  (bool (*)(collection *restrict col, iterator *iter_a, iterator *iter_b))_vector_collection_swap,
  (any (*)(const collection *restrict col, iterator iter, any out))_vector_collection_get,
  (any (*)(collection *restrict col, iterator *iter, any value))_vector_collection_set,
  (iterator (*)(collection *restrict col, natural index))_vector_collection_index,
  (void (*)(const collection *restrict col, iterator *iter))_vector_collection_next,
  (void (*)(const collection *restrict col, iterator *iter))_vector_collection_prev,
  (integer (*)(const collection *restrict col))_vector_collection_count,
  (iterator (*)(const collection *restrict col))_vector_collection_begin,
  (iterator (*)(const collection *restrict col))_vector_collection_end,
  (const any (*)(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter))_vector_collection_search_region,
  (bool (*)(collection *restrict col, iterator *iter, handler *destructor))_vector_collection_destroy_at,
  (any (*)(collection *restrict col, iterator *iter, any out))_vector_collection_remove_at,
  (integer (*)(collection *restrict col, iterator *iter, natural count))_vector_collection_destroy_range,
  (bool (*)(collection *restrict col, iterator *restrict position, any item))_vector_collection_insert
};

struct collection_dispatch_functions default_linked_list_collection_dispatch_functions = {
  (integer (*)(const collection *restrict col, iterator left, iterator right))_linked_list_iterator_compare,
  (bool (*)(const collection *restrict col, iterator index))_linked_list_iterator_is_valid,
  (iterator (*)(const collection *restrict col))_linked_list_make_invalid_iterator,
  (any (*)(collection *restrict col, iterator *iter))_linked_list_collection_push_start,
  (bool (*)(const collection *restrict col))_linked_list_collection_is_empty,
  (any (*)(const collection *restrict col, any out))_linked_list_collection_pop,
  (bool (*)(collection *restrict col))_linked_list_collection_pop_destroy,
  (bool (*)(collection *restrict col))_linked_list_collection_pop_forget,
  (integer (*)(collection *restrict col, iterator iter, any *out_array))_linked_list_collection_ref_array,
  (any (*)(collection *restrict col, iterator iter))_linked_list_collection_ref,
  (bool (*)(collection *restrict col, iterator *iter_a, iterator *iter_b))_linked_list_collection_swap,
  (any (*)(const collection *restrict col, iterator iter, any out))_linked_list_collection_get,
  (any (*)(collection *restrict col, iterator *iter, any value))_linked_list_collection_set,
  (iterator (*)(collection *restrict col, natural index))_linked_list_collection_index,
  (void (*)(const collection *restrict col, iterator *iter))_linked_list_collection_next,
  (void (*)(const collection *restrict col, iterator *iter))_linked_list_collection_prev,
  (integer (*)(const collection *restrict col))_linked_list_collection_count,
  (iterator (*)(const collection *restrict col))_linked_list_collection_begin,
  (iterator (*)(const collection *restrict col))_linked_list_collection_end,
  (const any (*)(const collection *restrict col, filter *predicate, iterator left, iterator right, iterator *iter))_linked_list_collection_search_region,
  (bool (*)(collection *restrict col, iterator *iter, handler *destructor))_linked_list_collection_destroy_at,
  (any (*)(collection *restrict col, iterator *iter, any out))_linked_list_collection_remove_at,
  (integer (*)(collection *restrict col, iterator *iter, natural count))_linked_list_collection_destroy_range,
  (bool (*)(collection *restrict col, iterator *restrict position, any item))_linked_list_collection_insert
};

collection_settings default_vector_collection_settings = {
  COLLECTION_TYPE_LIST,
  STORAGE_TYPE_VECTOR,
  {0},
  {0},
  32,
  sizeof(byte),
  &default_vector_collection_dispatch_functions
};

collection_settings default_linked_list_collection_settings = {
  COLLECTION_TYPE_LIST,
  STORAGE_TYPE_LINKED_LIST,
  {0},
  {0},
  32,
  sizeof(byte),
  &default_linked_list_collection_dispatch_functions
};

void _check_init_collection(collection *restrict col, collection_settings settings, natural count) {
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

api collection *init_collection(collection *restrict col, collection_type type, storage_type storage, comparator *compare, handler *destructor, natural element_size) {
  collection_settings settings;
  
  if (safety(col == NULL || element_size < 1))
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

api collection *init_collection_custom(collection *restrict col, collection_settings settings, natural element_size) {
  if (safety(col == NULL))
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

api collection *init_collection_array(collection *restrict col, collection_type type, comparator *comp, handler *deconstruct_entry, byte *data, natural element_size, natural count) {
  collection_settings settings;
  
  if (safety(col == NULL || data == NULL))
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

api void destroy_collection(collection *restrict col) {
  iterator index;
  any item;
  
  if (safety(col == NULL))
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

