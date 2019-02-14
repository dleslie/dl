#ifndef DL_TWEEN_H
#define DL_TWEEN_H 1

#include "dl.h"

#ifdef __cplusplus
extern "C" {
#endif



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

  typedef dl_real *(*dl_selector_function)(const dl_real *values, dl_natural length, dl_real percent, dl_real *out);

  dl_api dl_real *dl_interpolate(const dl_selector_function select, const dl_real *values, dl_natural length, dl_real percent, dl_real *out);

  dl_api dl_real *dl_select_linear(const dl_real *v, dl_natural l, dl_real p, dl_real *out);
  dl_api dl_real *dl_select_catmullrom(const dl_real *v, dl_natural l, dl_real p, dl_real *out);

  typedef dl_point2 *(*dl_selector_function_point2)(const dl_point2 *values, dl_natural length, dl_real percent, dl_point2 *out);

  dl_api dl_point2 *dl_interpolate_point2(const dl_selector_function_point2 select, const dl_point2 *values, dl_natural length, dl_real percent, dl_point2 *out);

  dl_api dl_point2 *dl_select_linear_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out);
  dl_api dl_point2 *dl_select_bezier_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out);
  dl_api dl_point2 *dl_select_catmullrom_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out);

  typedef dl_point3 *(*dl_selector_function_point3)(const dl_point3 *values, dl_natural length, dl_real percent, dl_point3 *out);

  dl_api dl_point3 *dl_interpolate_point3(const dl_selector_function_point3 select, const dl_point3 *values, dl_natural length, dl_real percent, dl_point3 *out);

  dl_api dl_point3 *dl_select_linear_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out);
  dl_api dl_point3 *dl_select_bezier_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out);
  dl_api dl_point3 *dl_select_catmullrom_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out);



  dl_api dl_integer *dl_lerp_integer(dl_integer a, dl_integer b, dl_real p, dl_integer *out);
  dl_api dl_real *dl_lerp_real(dl_real a, dl_real b, dl_real p, dl_real *out);
  dl_api dl_point2 *dl_lerp_point2(const dl_point2 *a, const dl_point2 *b, dl_real p, dl_point2 *out);
  dl_api dl_point3 *dl_lerp_point3(const dl_point3 *a, const dl_point3 *b, dl_real p, dl_point3 *out);
  dl_api dl_vec2 *dl_lerp_vec2(const dl_vec2 *a, const dl_vec2 *b, dl_real p, dl_vec2 *out);
  dl_api dl_vec3 *dl_lerp_vec3(const dl_vec3 *a, const dl_vec3 *b, dl_real p, dl_vec3 *out);

#ifdef __cplusplus
}
#endif



/***************************************
 * Implementation
 **************************************/

#if DL_IMPLEMENTATION
  
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



dl_real *dl_interpolate(const dl_selector_function select, const dl_real *values, dl_natural length, dl_real percent, dl_real *out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_real *dl_select_linear(const dl_real *v, dl_natural l, dl_real p, dl_real *out) {
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

dl_real *dl_select_catmullrom(const dl_real *v, dl_natural l, dl_real p, dl_real *out) {
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



dl_point2 *dl_interpolate_point2(const dl_selector_function_point2 select, const dl_point2 *values, dl_natural length, dl_real percent, dl_point2 *out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_point2 *dl_select_linear_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out) {
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

dl_point2 *dl_select_bezier_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out) {
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

dl_point2 *dl_select_catmullrom_point2(const dl_point2 *v, dl_natural l, dl_real p, dl_point2 *out) {
  dl_natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  dl_real target, t, t2, t3;
  dl_point2 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;

  #if DL_USE_SAFETY_CHECKS
  /* These may throw uninitialized warnings. */
  v0 = v1 = threev1 = threev2 = ntwov2 = twov1 = dl_point2_zero;
  #endif
  
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



dl_point3 *dl_interpolate_point3(const dl_selector_function_point3 select, const dl_point3 *values, dl_natural length, dl_real percent, dl_point3 *out) {
  if (dl_safety(select == NULL || values == NULL || length == 0))
    return NULL;
  if (dl_unlikely(length == 1)) {
    *out = values[0];
    return out;
  }
  
  percent = dl_clamp01(percent);
  return select(values, length, percent, out);
}

dl_point3 *dl_select_linear_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out) {
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

dl_point3 *dl_select_bezier_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out) {
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

dl_point3 *dl_select_catmullrom_point3(const dl_point3 *v, dl_natural l, dl_real p, dl_point3 *out) {
  dl_natural max_idx, idx, a_idx, b_idx, c_idx, d_idx;
  dl_real target, t, t2, t3;
  dl_point3 v0, v1, threev1, threev2, twov0, ntwov2, twov1, b, c, d;

  #if DL_USE_SAFETY_CHECKS
  /* These may throw uninitialized warnings. */
  v0 = v1 = threev1 = threev2 = ntwov2 = twov1 = dl_point3_zero;
  #endif
  
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

dl_api dl_point2 *dl_lerp_point2(const dl_point2 *a, const dl_point2 *b, dl_real p, dl_point2 *out) {
  return dl_point2_add(dl_point2_mul_scalar(dl_point2_sub(b, a, out), p, out), a, out);
}

dl_api dl_point3 *dl_lerp_point3(const dl_point3 *a, const dl_point3 *b, dl_real p, dl_point3 *out) {
  return dl_point3_add(dl_point3_mul_scalar(dl_point3_sub(b, a, out), p, out), a, out);
}

dl_api dl_vec2 *dl_lerp_vec2(const dl_vec2 *a, const dl_vec2 *b, dl_real p, dl_vec2 *out) {
  return dl_vec2_add(dl_vec2_mul_scalar(dl_vec2_sub(b, a, out), p, out), a, out);
}

dl_api dl_vec3 *dl_lerp_vec3(const dl_vec3 *a, const dl_vec3 *b, dl_real p, dl_vec3 *out) {
  return dl_vec3_add(dl_vec3_mul_scalar(dl_vec3_sub(b, a, out), p, out), a, out);
}

#endif /* DL_IMPLEMENTATION */

#endif
