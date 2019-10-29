#ifndef DL_MATH_H
#define DL_MATH_H 1

/***************************************
 * Configuration
 **************************************/

#ifndef DL_USE_LEFT_HANDED
#define DL_USE_LEFT_HANDED 0
#endif

#ifndef DL_BEZIER_DEGREE
#define DL_BEZIER_DEGREE 4
#endif

#include <math.h>
#include "dl_core.h"

#if defined(__cplusplus)
extern "C" {
#endif

extern const dl_real DL_PI;
extern const dl_real DL_INV_PI;
extern const dl_real DL_E;
extern const dl_real DL_INV_E;
extern const dl_real DL_EPSILON;

#if DL_USE_EXTENIONS && DL_IS_ATLEAST_C90 && (DL_IS_GNUC || DL_IS_CLANG)
#define dl_max(a, b) ({ \
  __auto_type _a = (a); \
  __auto_type _b = (b); \
  _a > _b ? _a : _b;    \
})
#define dl_min(a, b) ({ \
  __auto_type _a = (a); \
  __auto_type _b = (b); \
  _a < _b ? _a : _b;    \
})
#define dl_clamp(x, i, j) ({  \
  __auto_type _i = (i);       \
  __auto_type _j = (j);       \
  __auto_type _x = (x);       \
  dl_max(dl_min(_j, _x), _i); \
})
#define dl_clamp01(x) dl_clamp(x, 0, 1)
#else
#define dl_min(x, y) ((x) <= (y) ? (x) : (y))
#define dl_max(x, y) ((x) >= (y) ? (x) : (y))
#define dl_clamp(x, a, b) dl_max(dl_min(b, x), a)
#define dl_clamp01(x) dl_clamp(x, 0, 1)
#endif

dl_api dl_bool dl_approximately_equal(dl_real a, dl_real b, dl_real epsilon);
dl_api dl_integer dl_floor_to_integer(dl_real n);
dl_api dl_integer dl_ceil_to_integer(dl_real n);
dl_api dl_integer dl_round_to_integer(dl_real n);
dl_api dl_real dl_rationalize(dl_real value, dl_natural decimal_points);

dl_api dl_real dl_degree_to_radian(dl_real degree);
dl_api dl_real dl_radian_to_degree(dl_real radian);
dl_api dl_integer dl_factorial(dl_integer n);

#define dl_sqrt(v) sqrt(v)
#define dl_cos(v) cos(v)
#define dl_sin(v) sin(v)
#define dl_tan(v) tan(v)
#define dl_acos(v) acos(v)
#define dl_asin(v) asin(v)
#define dl_atan(v) atan(v)
#define dl_pow(a, b) pow((a), (b))
#define dl_exp(v) exp(v)
#define dl_floor(v) floor(v)
#define dl_ceil(v) ceil(v)
#define dl_abs(v) ((v) > 0 ? (v) : -(v))

#if !DL_IS_ATLEAST_C99
#define dl_hypot(a, b) dl_sqrt((a) * (a) + (b) * (b))
#define dl_round(a) dl_floor(a)
#else
#define dl_hypot(a, b) hypot((a), (b))
#define dl_round(a) roundf(a)
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

dl_api dl_vec2 *dl_init_vec2(dl_vec2 *v, dl_real x, dl_real y);

dl_api dl_vec2 *dl_vec2_add(const dl_vec2 *left, const dl_vec2 *right, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_sub(const dl_vec2 *left, const dl_vec2 *right, dl_vec2 *out);

dl_api dl_vec2 *dl_vec2_mul_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_div_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_add_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_sub_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out);

dl_api dl_vec2 *dl_vec2_normalize(const dl_vec2 *left, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_negate(const dl_vec2 *left, dl_vec2 *out);

dl_api dl_real dl_vec2_dot(const dl_vec2 *left, const dl_vec2 *right);
dl_api dl_bool dl_vec2_approximately_equal(const dl_vec2 *left, const dl_vec2 *right, dl_real epsilon);
dl_api dl_real dl_vec2_square_magnitude(const dl_vec2 *left);
dl_api dl_real dl_vec2_magnitude(const dl_vec2 *left);

dl_api dl_vec2 *dl_vec2_reflect(const dl_vec2 *left, const dl_vec2 *normal, dl_vec2 *out);
dl_api dl_vec2 *dl_vec2_refract(const dl_vec2 *left, const dl_vec2 *normal, float eta, dl_vec2 *out);

typedef dl_vec2 dl_point2;
extern const dl_point2 dl_point2_zero;
extern const dl_point2 dl_point2_one;

dl_api dl_point2 *dl_init_point2(dl_point2 *p, dl_real x, dl_real y);

dl_api dl_point2 *dl_point2_add(const dl_point2 *left, const dl_point2 *right, dl_point2 *out);
dl_api dl_point2 *dl_point2_sub(const dl_point2 *left, const dl_point2 *right, dl_point2 *out);

dl_api dl_point2 *dl_point2_negate(const dl_point2 *left, dl_point2 *out);

dl_api dl_point2 *dl_point2_mul_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out);
dl_api dl_point2 *dl_point2_div_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out);
dl_api dl_point2 *dl_point2_add_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out);
dl_api dl_point2 *dl_point2_sub_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out);

dl_api dl_bool dl_point2_approximately_equal(const dl_point2 *left, const dl_point2 *right, dl_real epsilon);

dl_api dl_real dl_point2_line_orientation(const dl_point2 *point, const dl_point2 *line_a, const dl_point2 *line_b);

typedef struct {
  dl_real x, y, z, w;
} dl_vec4;

typedef dl_vec4 dl_point3;
typedef dl_vec4 dl_vec3;

dl_api dl_vec4 *dl_init_vec4(dl_vec4 *v, dl_real x, dl_real y, dl_real z, dl_real w);

dl_api dl_vec4 *dl_vec4_add(const dl_vec4 *left, const dl_vec4 *right, dl_vec4 *out);
dl_api dl_vec4 *dl_vec4_sub(const dl_vec4 *left, const dl_vec4 *right, dl_vec4 *out);

dl_api dl_real dl_vec4_dot(const dl_vec4 *left, const dl_vec4 *right);
dl_api dl_vec4 *dl_vec4_normalize(const dl_vec4 *left, dl_vec4 *out);
dl_api dl_vec4 *dl_vec4_negate(const dl_vec4 *left, dl_vec4 *out);
dl_api dl_real dl_vec4_square_magnitude(const dl_vec4 *left);
dl_api dl_real dl_vec4_magnitude(const dl_vec4 *left);
dl_api dl_bool dl_vec4_approximately_equal(const dl_vec4 *left, const dl_vec4 *right, dl_real epsilon);

dl_api dl_vec4 *dl_vec4_mul_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out);
dl_api dl_vec4 *dl_vec4_div_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out);
dl_api dl_vec4 *dl_vec4_add_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out);
dl_api dl_vec4 *dl_vec4_sub_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out);

extern const dl_point3 dl_point3_zero;
extern const dl_point3 dl_point3_one;

dl_api dl_point3 *dl_init_point3(dl_point3 *p, dl_real x, dl_real y, dl_real z);

dl_api dl_point3 *dl_point3_add(const dl_point3 *left, const dl_point3 *right, dl_point3 *out);
dl_api dl_point3 *dl_point3_sub(const dl_point3 *left, const dl_point3 *right, dl_point3 *out);

dl_api dl_point3 *dl_point3_mul_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out);
dl_api dl_point3 *dl_point3_div_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out);
dl_api dl_point3 *dl_point3_add_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out);
dl_api dl_point3 *dl_point3_sub_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out);

dl_api dl_point3 *dl_point3_rotate(const dl_point3 *left, const dl_vec3 *axis, const dl_real angle, dl_point3 *out);

dl_api dl_point3 *dl_point3_negate(const dl_point3 *left, dl_point3 *out);

dl_api dl_bool dl_point3_approximately_equal(const dl_point3 *left, const dl_point3 *out, dl_real epsilon);

/* dl_real point3_triangle_orientation(const dl_point3 *point, const dl_point3 *triangle_a, const dl_point3 *triangle_b, const dl_point3 *triangle_c); */

extern const dl_vec3 dl_vec3_up;
extern const dl_vec3 dl_vec3_right;
extern const dl_vec3 dl_vec3_forward;
extern const dl_vec3 dl_vec3_down;
extern const dl_vec3 dl_vec3_left;
extern const dl_vec3 dl_vec3_backward;
extern const dl_vec3 dl_vec3_zero;
extern const dl_vec3 dl_vec3_one;

dl_api dl_vec3 *dl_init_vec3(dl_vec3 *v, dl_real x, dl_real y, dl_real z);

dl_api dl_vec3 *dl_vec3_reflect(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out);
dl_api dl_vec3 *dl_vec3_refract(const dl_vec3 *left, const dl_vec3 *right, float eta, dl_vec3 *out);

dl_api dl_vec3 *dl_vec3_add(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out);
dl_api dl_vec3 *dl_vec3_sub(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out);

dl_api dl_vec3 *dl_vec3_cross(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out);
dl_api dl_real dl_vec3_dot(const dl_vec3 *left, const dl_vec3 *out);

dl_api dl_vec3 *dl_vec3_mul_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out);
dl_api dl_vec3 *dl_vec3_div_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out);
dl_api dl_vec3 *dl_vec3_add_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out);
dl_api dl_vec3 *dl_vec3_sub_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out);

dl_api dl_vec3 *dl_vec3_rotate(const dl_vec3 *left, const dl_vec3 *axis, const dl_real angle, dl_vec3 *out);

dl_api dl_vec3 *dl_vec3_normalize(const dl_vec3 *left, dl_vec3 *out);
dl_api dl_bool dl_vec3_normalized(const dl_vec3 *left);
dl_api dl_vec3 *dl_vec3_negate(const dl_vec3 *left, dl_vec3 *out);

dl_api dl_bool dl_vec3_approximately_equal(const dl_vec3 *left, const dl_vec3 *out, dl_real epsilon);
dl_api dl_real dl_vec3_square_magnitude(const dl_vec3 *left);
dl_api dl_real dl_vec3_magnitude(const dl_vec3 *left);

typedef union {
  dl_real vals[16];
  dl_real ary[4][4];
  dl_vec4 cols[4];
} dl_mat4;

extern const dl_mat4 dl_mat4_identity;

dl_api dl_mat4 *dl_init_mat4(dl_mat4 *m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3);

dl_api dl_mat4 *dl_init_mat4_cols(dl_mat4 *m, const dl_vec4 *column_a, const dl_vec4 *column_b, const dl_vec4 *column_c, const dl_vec4 *column_d);

dl_api dl_mat4 *dl_init_mat4_translate(dl_mat4 *m, dl_real x, dl_real y, dl_real z);
dl_api dl_mat4 *dl_init_mat4_rotate_x(dl_mat4 *m, dl_real radians);
dl_api dl_mat4 *dl_init_mat4_rotate_y(dl_mat4 *m, dl_real radians);
dl_api dl_mat4 *dl_init_mat4_rotate_z(dl_mat4 *m, dl_real radians);
dl_api dl_mat4 *dl_init_mat4_rotate(dl_mat4 *m, const dl_vec3 *axis, dl_real radians);
dl_api dl_mat4 *dl_init_mat4_scale(dl_mat4 *m, dl_real x, dl_real y, dl_real z);

dl_api dl_mat4 *dl_init_mat4_look_at(dl_mat4 *m, const dl_vec3 *origin, const dl_vec3 *target, const dl_vec3 *up);
dl_api dl_mat4 *dl_init_mat4_orthographic(dl_mat4 *m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far);
dl_api dl_mat4 *dl_init_mat4_perspective(dl_mat4 *m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far);

dl_api dl_mat4 *dl_mat4_add(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out);
dl_api dl_mat4 *dl_mat4_sub(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out);
dl_api dl_mat4 *dl_mat4_mul(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out);

dl_api dl_vec4 *dl_mat4_mul_vec4(const dl_mat4 *left, const dl_vec4 *right, dl_vec4 *out);
dl_api dl_vec3 *dl_mat4_mul_vec3(const dl_mat4 *left, const dl_vec3 *right, dl_vec3 *out);
dl_api dl_point3 *dl_mat4_mul_point3(const dl_mat4 *left, const dl_point3 *right, dl_point3 *out);

dl_api dl_mat4 *dl_mat4_transpose(const dl_mat4 *left, dl_mat4 *out);

dl_api dl_mat4 *dl_mat4_mul_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out);
dl_api dl_mat4 *dl_mat4_div_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out);
dl_api dl_mat4 *dl_mat4_add_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out);
dl_api dl_mat4 *dl_mat4_sub_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out);

dl_api dl_bool dl_mat4_approximately_equal(const dl_mat4 *left, const dl_mat4 *right, dl_real epsilon);

/* typedef dl_vec4 dl_quat; */

/* dl_quat *dl_init_quat(dl_quat *q, dl_real x, dl_real y, dl_real z, dl_real w); */
/* dl_quat *dl_init_quat_axis_angle(dl_quat *q, const dl_vec3 *axis, dl_real angle); */
/* dl_quat *dl_init_quat_euler_angles(dl_quat *q, const dl_vec3 *angles); */
/* dl_quat *dl_init_quat_identity(dl_quat *q); */

/* dl_quat *dl_quat_add(const dl_quat *left, const quat *right, quat *out); */
/* dl_quat *dl_quat_sub(const dl_quat *left, const quat *right, quat *out); */
/* dl_quat *dl_quat_mul(const dl_quat *left, const quat *right, quat *out); */
/* dl_quat *dl_quat_div(const dl_quat *left, const quat *right, quat *out); */

/* dl_quat *dl_quat_mul_scalar(const dl_quat *left, dl_real scalar, quat *out); */
/* dl_quat *dl_quat_div_scalar(const dl_quat *left, dl_real scalar, quat *out); */

/* dl_real dl_quat_dot(const dl_quat *left, const quat *right); */
/* dl_real dl_quat_magnitude(const dl_quat *q); */

/* dl_quat *dl_quat_normalize(const quat *q, dl_quat *out); */
/* dl_quat *dl_quat_conjugate(const quat *q, dl_quat *out); */
/* dl_quat *dl_quat_inverse(const quat *q, dl_quat *out); */

/* dl_bool dl_quat_axis_angle(const dl_quat *q, dl_vec3 *out_axis, dl_real *out_angle); */
/* dl_quat *dl_quat_euler_angles(const dl_quat *q, dl_vec3 *out); */

/* dl_vec3 *dl_vec3_rotate_quat(const dl_vec3 *left, const quat *q, dl_vec3 *out); */

/* dl_mat4 *dl_quat_to_mat4(const dl_quat *q, dl_mat4 *out); */
/* dl_quat *dl_mat4_to_quat(const dl_mat4 *m, dl_quat *out); */

#if defined(__cplusplus)
}
#endif

/***************************************
 * Implementation
 **************************************/

#if defined(DL_IMPLEMENTATION)

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

const dl_mat4 dl_mat4_identity = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
const dl_point2 dl_point2_zero = {0, 0};
const dl_point2 dl_point2_one = {1, 1};
const dl_vec2 dl_vec2_up = {0, 1};
const dl_vec2 dl_vec2_right = {1, 0};
const dl_vec3 dl_vec3_up = {0, 1, 0, 0};
const dl_vec3 dl_vec3_right = {1, 0, 0, 0};

const dl_vec2 dl_vec2_down = {0, -1};
const dl_vec2 dl_vec2_left = {-1, 0};
const dl_vec3 dl_vec3_down = {0, -1, 0, 0};
const dl_vec3 dl_vec3_left = {-1, 0, 0, 0};

#if DL_USE_LEFT_HANDED
const dl_vec3 dl_vec3_forward = {0, 0, 1, 0};
const dl_vec3 dl_vec3_backward = {0, 0, -1, 0};
#else
const dl_vec3 dl_vec3_forward = {0, 0, -1, 0};
const dl_vec3 dl_vec3_backward = {0, 0, 1, 0};
#endif

const dl_vec2 dl_vec2_zero = {0, 0};
const dl_vec3 dl_vec3_zero = {0, 0, 0, 0};
const dl_point3 dl_point3_zero = {0, 0, 0, 1};

const dl_vec2 dl_vec2_one = {1, 1};
const dl_vec3 dl_vec3_one = {1, 1, 1, 0};
const dl_point3 dl_point3_one = {1, 1, 1, 1};

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

dl_integer _factorial_cache[13] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

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

dl_api dl_random_state *dl_init_random_time(dl_random_state *state) {
#ifdef DL_TIME_H
  dl_natural sec, usec;
  dl_time(&usec, &sec);
  return dl_init_random(state, usec + sec);
#else
  return dl_init_random(state, 0);
#endif
}

dl_api dl_vec2 *dl_init_vec2(dl_vec2 *v, dl_real x, dl_real y) {
  if (dl_safety(v == NULL))
    return NULL;

  v->x = x;
  v->y = y;

  return v;
}

dl_api dl_vec2 *dl_vec2_add(const dl_vec2 *left, const dl_vec2 *right, dl_vec2 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x + right->x;
  out->y = left->y + right->y;

  return out;
}

dl_api dl_vec2 *dl_vec2_sub(const dl_vec2 *left, const dl_vec2 *right, dl_vec2 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x - right->x;
  out->y = left->y - right->y;

  return out;
}

dl_api dl_vec2 *dl_vec2_mul_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x * scalar;
  out->y = left->y * scalar;

  return out;
}

dl_api dl_vec2 *dl_vec2_div_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out) {
  return dl_vec2_mul_scalar(left, 1.0f / scalar, out);
}

dl_api dl_vec2 *dl_vec2_add_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x + scalar;
  out->y = left->y + scalar;

  return out;
}

dl_api dl_vec2 *dl_vec2_sub_scalar(const dl_vec2 *left, dl_real scalar, dl_vec2 *out) {
  return dl_vec2_add_scalar(left, -scalar, out);
}

dl_api dl_vec2 *dl_vec2_normalize(const dl_vec2 *left, dl_vec2 *out) {
  dl_real inv_magnitude;
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  inv_magnitude = 1.0f / dl_hypot(left->x, left->y);
  dl_vec2_mul_scalar(left, inv_magnitude, out);

  return out;
}

dl_api dl_vec2 *dl_vec2_negate(const dl_vec2 *left, dl_vec2 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = -left->x;
  out->y = -left->y;

  return out;
}

dl_api dl_real dl_vec2_dot(const dl_vec2 *left, const dl_vec2 *right) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  return left->x * right->x + left->y * right->y;
}

dl_api dl_bool dl_vec2_approximately_equal(const dl_vec2 *left, const dl_vec2 *right, dl_real epsilon) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;
  return fabs(left->x - right->x) < epsilon && fabs(left->y - right->y) < epsilon;
}

dl_api dl_real dl_vec2_square_magnitude(const dl_vec2 *left) {
  if (dl_safety(left == NULL))
    return 0;
  return left->x * left->x + left->y * left->y;
}

dl_api dl_real dl_vec2_magnitude(const dl_vec2 *left) {
  if (dl_safety(left == NULL))
    return 0;

  return dl_hypot(left->x, left->y);
}

dl_api dl_vec2 *dl_vec2_reflect(const dl_vec2 *left, const dl_vec2 *normal, dl_vec2 *out) {
  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  return dl_vec2_sub(left, dl_vec2_mul_scalar(normal, dl_vec2_dot(normal, left) * 2.0f, out), out);
}

dl_api dl_vec2 *dl_vec2_refract(const dl_vec2 *left, const dl_vec2 *normal, float eta, dl_vec2 *out) {
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

dl_api dl_point2 *dl_init_point2(dl_point2 *p, dl_real x, dl_real y) {
  return dl_init_vec2((dl_vec2 *)p, x, y);
}

dl_api dl_point2 *dl_point2_add(const dl_point2 *left, const dl_point2 *right, dl_point2 *out) {
  return dl_vec2_add(left, right, out);
}

dl_api dl_point2 *dl_point2_sub(const dl_point2 *left, const dl_point2 *right, dl_point2 *out) {
  return dl_vec2_sub(left, right, out);
}

dl_api dl_point2 *dl_point2_mul_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out) {
  return dl_vec2_mul_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_div_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out) {
  return dl_vec2_div_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_add_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out) {
  return dl_vec2_add_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_sub_scalar(const dl_point2 *left, dl_real scalar, dl_point2 *out) {
  return dl_vec2_sub_scalar(left, scalar, out);
}

dl_api dl_point2 *dl_point2_negate(const dl_point2 *left, dl_point2 *out) {
  return dl_vec2_negate(left, out);
}

dl_api dl_bool dl_point2_approximately_equal(const dl_point2 *left, const dl_point2 *right, dl_real epsilon) {
  return dl_vec2_approximately_equal(left, right, epsilon);
}

dl_api dl_real dl_point2_line_orientation(const dl_point2 *point, const dl_point2 *line_a, const dl_point2 *line_b) {
  dl_real rise, run, m, b;

  if (dl_safety(point == NULL || line_a == NULL || line_b == NULL))
    return 0;

  rise = line_b->y - line_a->y;
  run = line_b->x - line_a->x;
  m = rise / run;
  b = -(m * line_a->x - line_a->y);

  return m * point->x + b - point->y;
}

dl_api dl_vec4 *dl_init_vec4(dl_vec4 *v, dl_real x, dl_real y, dl_real z, dl_real w) {
  if (dl_safety(v == NULL))
    return NULL;

  v->x = x;
  v->y = y;
  v->z = z;
  v->w = w;

  return v;
}

dl_api dl_vec4 *dl_vec4_add(const dl_vec4 *left, const dl_vec4 *right, dl_vec4 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = left->w + right->w;

  return out;
}

dl_api dl_vec4 *dl_vec4_sub(const dl_vec4 *left, const dl_vec4 *right, dl_vec4 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = left->w - right->w;

  return out;
}

dl_api dl_real dl_vec4_dot(const dl_vec4 *left, const dl_vec4 *right) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;

  return left->x * right->x + left->y * right->y + left->z * right->z + left->w * right->w;
}

dl_api dl_vec4 *dl_vec4_normalize(const dl_vec4 *left, dl_vec4 *out) {
  return dl_vec4_div_scalar(left, dl_vec4_magnitude(left), out);
}

dl_api dl_vec4 *dl_vec4_negate(const dl_vec4 *left, dl_vec4 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = -left->w;

  return out;
}

dl_api dl_real dl_vec4_square_magnitude(const dl_vec4 *left) {
  if (dl_safety(left == NULL))
    return 0;

  return (left->x * left->x) + (left->y * left->y) + (left->z * left->z) + (left->w * left->w);
}

dl_api dl_real dl_vec4_magnitude(const dl_vec4 *left) {
  return dl_sqrt(dl_vec4_square_magnitude(left));
}

dl_api dl_bool dl_vec4_approximately_equal(const dl_vec4 *left, const dl_vec4 *right, dl_real epsilon) {
  if (dl_safety(left == NULL || right == NULL))
    return 0;

  return fabs(left->x - right->x) < epsilon && fabs(left->y - right->y) < epsilon && fabs(left->z - right->z) < epsilon && fabs(left->w - right->w) < epsilon;
}

dl_api dl_vec4 *dl_vec4_mul_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = left->w * scalar;

  return out;
}

dl_api dl_vec4 *dl_vec4_div_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out) {
  return dl_vec4_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_vec4 *dl_vec4_add_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = left->w + scalar;

  return out;
}

dl_api dl_vec4 *dl_vec4_sub_scalar(const dl_vec4 *left, dl_real scalar, dl_vec4 *out) {
  return dl_vec4_add_scalar(left, -scalar, out);
}

dl_api dl_point3 *dl_init_point3(dl_point3 *p, dl_real x, dl_real y, dl_real z) {
  return dl_init_vec4(p, x, y, z, 1);
}

dl_api dl_point3 *dl_point3_add(const dl_point3 *left, const dl_point3 *right, dl_point3 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_sub(const dl_point3 *left, const dl_point3 *right, dl_point3 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_mul_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_div_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out) {
  return dl_point3_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_point3 *dl_point3_add_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 1;

  return out;
}

dl_api dl_point3 *dl_point3_sub_scalar(const dl_point3 *left, dl_real scalar, dl_point3 *out) {
  return dl_point3_add_scalar(left, -scalar, out);
}

dl_api dl_point3 *dl_point3_negate(const dl_point3 *left, dl_point3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 1;

  return out;
}

dl_api dl_bool dl_point3_approximately_equal(const dl_point3 *left, const dl_point3 *right, dl_real epsilon) {
  return dl_vec4_approximately_equal(left, right, epsilon);
}

dl_api dl_point3 *dl_point3_rotate(const dl_point3 *left, const dl_vec3 *axis, const dl_real angle, dl_point3 *out) {
  dl_mat4 mat;

  return dl_mat4_mul_point3(dl_init_mat4_rotate(&mat, axis, angle), left, out);
}

dl_api dl_vec3 *dl_init_vec3(dl_vec3 *v, dl_real x, dl_real y, dl_real z) {
  return dl_init_vec4(v, x, y, z, 0);
}

dl_api dl_vec3 *dl_vec3_add(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x + right->x;
  out->y = left->y + right->y;
  out->z = left->z + right->z;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_sub(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out) {
  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  out->x = left->x - right->x;
  out->y = left->y - right->y;
  out->z = left->z - right->z;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_cross(const dl_vec3 *left, const dl_vec3 *right, dl_vec3 *out) {
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

dl_api dl_real dl_vec3_dot(const dl_vec3 *left, const dl_vec3 *right) {
  return left->x * right->x + left->y * right->y + left->z * right->z;
}

dl_api dl_vec3 *dl_vec3_mul_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x * scalar;
  out->y = left->y * scalar;
  out->z = left->z * scalar;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_div_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out) {
  return dl_vec3_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_vec3 *dl_vec3_add_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = left->x + scalar;
  out->y = left->y + scalar;
  out->z = left->z + scalar;
  out->w = 0;

  return out;
}

dl_api dl_vec3 *dl_vec3_sub_scalar(const dl_vec3 *left, dl_real scalar, dl_vec3 *out) {
  return dl_vec3_add_scalar(left, -scalar, out);
}

dl_api dl_vec3 *dl_vec3_normalize(const dl_vec3 *left, dl_vec3 *out) {
  return dl_vec3_div_scalar(left, dl_vec3_magnitude(left), out);
}

dl_api dl_bool dl_vec3_normalized(const dl_vec3 *left) {
  return dl_approximately_equal(fabs(left->x) + fabs(left->y) + fabs(left->z), 1.0, DL_EPSILON);
}

dl_api dl_vec3 *dl_vec3_negate(const dl_vec3 *left, dl_vec3 *out) {
  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  out->x = -left->x;
  out->y = -left->y;
  out->z = -left->z;
  out->w = 0;

  return out;
}

dl_api dl_bool dl_vec3_approximately_equal(const dl_vec3 *left, const dl_vec3 *right, dl_real epsilon) {
  return dl_vec4_approximately_equal(left, right, epsilon);
}

dl_api dl_real dl_vec3_square_magnitude(const dl_vec3 *left) {
  if (dl_safety(left == NULL))
    return 0;

  return left->x * left->x + left->y * left->y + left->z * left->z;
}

dl_api dl_real dl_vec3_magnitude(const dl_vec3 *left) {
  return dl_sqrt(dl_vec3_square_magnitude(left));
}

dl_api dl_vec3 *dl_vec3_rotate(const dl_vec3 *left, const dl_vec3 *axis, const dl_real angle, dl_vec3 *out) {
  dl_mat4 mat;

  return dl_mat4_mul_vec3(dl_init_mat4_rotate(&mat, axis, angle), left, out);
}

dl_api dl_vec3 *dl_vec3_reflect(const dl_vec3 *left, const dl_vec3 *normal, dl_vec3 *out) {
  if (dl_safety(left == NULL || normal == NULL || out == NULL))
    return NULL;

  return dl_vec3_sub(left, dl_vec3_mul_scalar(normal, dl_vec3_dot(normal, left) * 2.0, out), out);
}

dl_api dl_vec3 *dl_vec3_refract(const dl_vec3 *left, const dl_vec3 *normal, float eta, dl_vec3 *out) {
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

dl_api dl_mat4 *dl_init_mat4(dl_mat4 *m, dl_real c0r0, dl_real c0r1, dl_real c0r2, dl_real c0r3, dl_real c1r0, dl_real c1r1, dl_real c1r2, dl_real c1r3, dl_real c2r0, dl_real c2r1, dl_real c2r2, dl_real c2r3, dl_real c3r0, dl_real c3r1, dl_real c3r2, dl_real c3r3) {
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

dl_api dl_mat4 *dl_init_mat4_cols(dl_mat4 *m, const dl_vec4 *column_a, const dl_vec4 *column_b, const dl_vec4 *column_c, const dl_vec4 *column_d) {
  if (dl_safety(m == NULL || column_a == NULL || column_b == NULL || column_c == NULL || column_d == NULL))
    return NULL;

  *(dl_vec4 *)(&m->ary[0]) = *column_a;
  *(dl_vec4 *)(&m->ary[1]) = *column_b;
  *(dl_vec4 *)(&m->ary[2]) = *column_c;
  *(dl_vec4 *)(&m->ary[3]) = *column_d;

  return m;
}

dl_api dl_mat4 *dl_mat4_add(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out) {
  dl_integer i, j;

  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + right->ary[i][j];

  return out;
}

dl_api dl_mat4 *dl_mat4_sub(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out) {
  dl_integer i, j;

  if (dl_safety(left == NULL || right == NULL || out == NULL))
    return NULL;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] - right->ary[i][j];

  return out;
}

dl_api dl_mat4 *dl_mat4_mul(const dl_mat4 *left, const dl_mat4 *right, dl_mat4 *out) {
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

dl_bool dl_mat4_approximately_equal(const dl_mat4 *left, const dl_mat4 *right, dl_real epsilon) {
  dl_integer i, j;

  if (dl_safety(left == NULL || right == NULL))
    return false;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (dl_abs(left->ary[i][j] - right->ary[i][j]) > epsilon)
        return false;

  return true;
}

dl_api dl_vec4 *dl_mat4_mul_vec4(const dl_mat4 *left, const dl_vec4 *right, dl_vec4 *out) {
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

dl_api dl_vec3 *dl_mat4_mul_vec3(const dl_mat4 *left, const dl_vec3 *right, dl_vec3 *out) {
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

dl_api dl_point3 *dl_mat4_mul_point3(const dl_mat4 *left, const dl_point3 *right, dl_point3 *out) {
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

dl_api dl_mat4 *dl_mat4_transpose(const dl_mat4 *left, dl_mat4 *out) {
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

dl_api dl_mat4 *dl_mat4_mul_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out) {
  dl_integer i, j;

  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] * scalar;

  return out;
}

dl_api dl_mat4 *dl_mat4_div_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out) {
  return dl_mat4_mul_scalar(left, 1.0 / scalar, out);
}

dl_api dl_mat4 *dl_mat4_add_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out) {
  dl_integer i, j;

  if (dl_safety(left == NULL || out == NULL))
    return NULL;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      out->ary[i][j] = left->ary[i][j] + scalar;

  return out;
}

dl_api dl_mat4 *dl_mat4_sub_scalar(const dl_mat4 *left, dl_real scalar, dl_mat4 *out) {
  return dl_mat4_add_scalar(left, -scalar, out);
}

dl_api dl_mat4 *dl_init_mat4_translate(dl_mat4 *m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;

  *m = dl_mat4_identity;
  m->ary[3][0] = x;
  m->ary[3][1] = y;
  m->ary[3][2] = z;

  return m;
}

dl_api dl_mat4 *dl_init_mat4_rotate_x(dl_mat4 *m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);

  return dl_init_mat4(m,
                      1, 0, 0, 0,
                      0, c, -s, 0,
                      0, s, c, 0,
                      0, 0, 0, 1);
#else
#if !DL_IS_ATLEAST_C99
  c = (dl_real)cos(radians);
  s = (dl_real)sin(radians);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
#endif

  return dl_init_mat4(m,
                      1, 0, 0, 0,
                      0, c, s, 0,
                      0, -s, c, 0,
                      0, 0, 0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_rotate_y(dl_mat4 *m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

#if DL_USE_LEFT_HANDED
  c = dl_cos(-radians);
  s = dl_sin(-radians);

  return dl_init_mat4(m,
                      c, 0, s, 0,
                      0, 1, 0, 0,
                      -s, 0, c, 0,
                      0, 0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);

  return dl_init_mat4(m,
                      c, 0, -s, 0,
                      0, 1, 0, 0,
                      s, 0, c, 0,
                      0, 0, 0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_rotate_z(dl_mat4 *m, dl_real radians) {
  dl_real c, s;

  if (dl_safety(m == NULL))
    return NULL;

  c = dl_cos(radians);
  s = dl_sin(radians);

  return dl_init_mat4(m,
                      c, s, 0, 0,
                      -s, c, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
}

dl_api dl_mat4 *dl_init_mat4_rotate(dl_mat4 *m, const dl_vec3 *a, dl_real radians) {
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
                      c + i.x * t.x, i.x * t.y - s * t.z, -(-i.x * t.z - s * t.y), 0,
                      i.y * t.x + s * t.z, c + i.y * t.y, -(-i.y * t.z + s * t.x), 0,
                      -(i.z * t.x + s * t.y), -(i.z * t.y - s * t.x), c + i.z * t.z, 0,
                      0, 0, 0, 1);
#else
  c = dl_cos(radians);
  s = dl_sin(radians);
  dl_vec3_mul_scalar(&t, 1.0 - c, &i);

  return dl_init_mat4(m,
                      c + i.x * t.x, i.x * t.y - s * t.z, -i.x * t.z - s * t.y, 0,
                      i.y * t.x + s * t.z, c + i.y * t.y, -i.y * t.z + s * t.x, 0,
                      i.z * t.x + s * t.y, i.z * t.y - s * t.x, c + i.z * t.z, 0,
                      0, 0, 0, 1);
#endif
}

dl_api dl_mat4 *dl_init_mat4_scale(dl_mat4 *m, dl_real x, dl_real y, dl_real z) {
  if (dl_safety(m == NULL))
    return NULL;

  *m = dl_mat4_identity;
  m->ary[0][0] = x;
  m->ary[1][1] = y;
  m->ary[2][2] = z;

  return m;
}

dl_api dl_mat4 *dl_init_mat4_look_at(dl_mat4 *m, const dl_vec3 *origin, const dl_vec3 *target, const dl_vec3 *up) {
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

dl_api dl_mat4 *dl_init_mat4_orthographic(dl_mat4 *m, dl_real yop, dl_real y_bottom, dl_real x_left, dl_real x_right, dl_real z_near, dl_real z_far) {
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

dl_api dl_mat4 *dl_init_mat4_perspective(dl_mat4 *m, dl_real vertical_fov, dl_real aspect_ratio, dl_real z_near, dl_real z_far) {
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

#endif /* DL_IMPLEMENTATION */

#endif
