#include "dl.h"

#if DL_IS_ATLEAST_C99

/* For testing purposes; MAX * MAX should be a sane value */
dl_real MIN_REAL = -1024;
dl_real MAX_REAL = 1024;

dl_bool test_approximately_equal() {
  if (!dl_check(dl_approximately_equal(0.5f, 0.51f, 0.01f),
    "Expected 0.5 ~= 0.51"))
    return false;
  if (!dl_check(!dl_approximately_equal(0.5f, 0.51f, 0.005f),
    "Expected 0.5 != 0.51"))
    return false;
  if (!dl_check(!dl_approximately_equal(10.0f, 1000.0f, 0.1f),
    "Expected 10.0 != 1000.0"))
    return false;
  return true;
}

dl_bool test_floor_to_integer() {
  if (!dl_check(dl_floor_to_integer(0.1f) == 0,
    "Expected 0.1 to become 0"))
    return false;

  if (!dl_check(dl_floor_to_integer(0.6f) == 0,
    "Expected 0.6 to become 0"))
    return false;

  return true;
}

dl_bool test_ceil_to_integer() {
  if (!dl_check(dl_ceil_to_integer(0.1f) == 1,
    "Expected 0.1 to become 1"))
    return false;

  if (!dl_check(dl_ceil_to_integer(0.6f) == 1,
    "Expected 0.6 to become 1"))
    return false;

  return true;
}

dl_bool test_round_to_integer() {
  if (!dl_check(dl_round_to_integer(0.1f) == 0,
    "Expected 0.1 to become 0"))
    return false;

  if (!dl_check(dl_round_to_integer(0.6f) == 1,
    "Expected 0.6 to become 1"))
    return false;

  return true;
}

dl_bool test_min() {
  if (!dl_check(dl_min(0, 1) == 0,
    "Expected 0 to be less than 1"))
    return false;
  return true;
}

dl_bool test_max() {
  if (!dl_check(dl_max(0, 1) == 1,
    "Expected 0 to be less than 1"))
    return false;
  return true;
}

dl_bool test_clamp() {
  if (!dl_check(dl_clamp(5, 1, 2) == 2,
    "Expected 5 to clamp to 2"))
    return false;

  if (!dl_check(dl_clamp(0, 1, 2) == 1,
    "Expected 0 to clamp to 1"))
    return false;

  if (!dl_check(dl_clamp(1.5, 1, 2) == 1.5,
    "Expected 0.5 to clamp to 0.5, not %f", dl_clamp(1.5, 1, 2)))
    return false;

  return true;
}

dl_bool test_factorial() {
  if (!dl_check(dl_factorial(3) == 6,
    "Expected %i to be 6", dl_factorial(3)))
    return false;

  if (!dl_check(dl_factorial(-1) == 1,
    "Expected %i to be 1", dl_factorial(-1)))
    return false;

  if (!dl_check(dl_factorial(8) == 40320,
    "Expected %i to be 40320", dl_factorial(8)))
    return false;

  return true;
}

dl_bool test_rationalize() {
  if (!dl_check(dl_approximately_equal(dl_rationalize(0.05f, 1), 0.1f, DL_EPSILON),
    "Expected %f to be 0.1", dl_rationalize(0.05f, 1)))
    return false;

  if (!dl_check(dl_approximately_equal(dl_rationalize(10.5f, 1), 10.5f, DL_EPSILON),
    "Expected %f to be 10.5", dl_rationalize(10.5f, 1)))
    return false;

  return true;
}

dl_bool test_degree_to_radian() {
  dl_real result;
  result = dl_degree_to_radian(286.5);
  return dl_check(dl_approximately_equal(result, 5.0f, DL_EPSILON),
    "Expected %f to equal %f", result, 5.0);
}

dl_bool test_radian_to_degree() {
  dl_real result;
  result = dl_radian_to_degree(5.0);
  return dl_check(dl_approximately_equal(result, 286.5f, 0.1f),
    "Expected %f to equal %f", result, 286.5f);
}

dl_bool test_random_degree() {
  dl_random_state r;
  dl_real value;
  
  dl_init_random_time(&r);
  
  value = dl_random_degree(&r);
  if (!dl_check(value >= 0 && value <= 360,
    "Expected %f to be between 0 and 360", value))
    return false;
  return true;
}

dl_bool test_random_radian() {
  dl_random_state r;
  dl_real value;
  
  dl_init_random_time(&r);
  
  value = dl_random_radian(&r);
  if (!dl_check(value >= 0 && value <= 2 * DL_PI,
    "Expected %f to be between 0 and %f", value, 2 * DL_PI))
    return false;
  return true;
}

dl_bool test_random_real() {
  dl_random_state r;
  dl_real value;
  dl_init_random_time(&r);
  
  value = dl_random_real(&r, DL_PI);
  if (!dl_check(value >= 0 && value <= DL_PI,
    "Expected %f to be between 0 and %f", value, DL_PI))
    return false;
  return true;
}

dl_bool test_random_real_range() {
  dl_random_state r;
  dl_real value;
  dl_init_random_time(&r);
  
  value = dl_random_real_range(&r, -DL_PI, 0);
  if (!dl_check(value >= -DL_PI && value <= DL_PI,
    "Expected %f to be between %f and 0", value, -DL_PI))
    return false;
  return true;
}

dl_bool test_random() {
  dl_random_state r;
  dl_integer value;
  dl_init_random_time(&r);
  
  value = dl_random_integer(&r, 1337);
  if (!dl_check(value >= 0 && value <= 1337,
    "Expected %i to be between 0 and 1337", value))
    return false;
  return true;
}

dl_bool test_random_range() {
  dl_random_state r;
  dl_integer value;
  dl_init_random_time(&r);
  
  value = dl_random_integer_range(&r, -1337, 0);
  if (!dl_check(value >= -1337 && value <= 0,
    "Expected %i to be between -1337 and 1337", value))
    return false;
  return true;
}



dl_bool test_init_vec2() {
  dl_vec2 vec;
  dl_random_state r;
  dl_real a,b;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  return dl_check(dl_approximately_equal(vec.x, a, DL_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b, DL_EPSILON),
      "Expected y to be %f, was %f", b, vec.y);
}

dl_bool test_vec2_add() {
  dl_vec2 vec_a;
  dl_vec2 vec_b;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec_a, a, b);
  dl_init_vec2(&vec_b, c, d);

  dl_vec2_add(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(vec_b.x, a + c, DL_EPSILON),
    "Expected x to be %f, was %f", a + c, vec_b.x) &&
    dl_check(dl_approximately_equal(vec_b.y, b + d, DL_EPSILON),
      "Expected y to be %f, was %f", b + d, vec_b.y);
}

dl_bool test_vec2_sub() {
  dl_vec2 vec_a;
  dl_vec2 vec_b;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec_a, a, b);
  dl_init_vec2(&vec_b, c, d);

  dl_vec2_sub(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(vec_b.x, a - c, DL_EPSILON),
    "Expected x to be %f, was %f", a - c, vec_b.x) &&
    dl_check(dl_approximately_equal(vec_b.y, b - d, DL_EPSILON),
      "Expected y to be %f, was %f", b - d, vec_b.y);
}

dl_bool test_vec2_mul_scalar() {
  dl_vec2 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_mul_scalar(&vec, c, &vec);

  return dl_check(dl_approximately_equal(vec.x, a * c, DL_EPSILON),
    "Expected x to be %f, was %f", a * c, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b * c, DL_EPSILON),
      "Expected y to be %f, was %f", b * c, vec.y);
}

dl_bool test_vec2_div_scalar() {
  dl_vec2 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_div_scalar(&vec, c, &vec);

  return dl_check(dl_approximately_equal(vec.x, a / c, DL_EPSILON),
    "Expected x to be %f, was %f", a / c, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b / c, DL_EPSILON),
      "Expected y to be %f, was %f", b / c, vec.y);
}

dl_bool test_vec2_add_scalar() {
  dl_vec2 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_add_scalar(&vec, c, &vec);

  return dl_check(dl_approximately_equal(vec.x, a + c, DL_EPSILON),
    "Expected x to be %f, was %f", a + c, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b + c, DL_EPSILON),
      "Expected y to be %f, was %f", b + c, vec.y);
}

dl_bool test_vec2_sub_scalar() {
  dl_vec2 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_sub_scalar(&vec, c, &vec);

  return dl_check(dl_approximately_equal(vec.x, a - c, DL_EPSILON),
    "Expected x to be %f, was %f", a - c, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b - c, DL_EPSILON),
      "Expected y to be %f, was %f", b - c, vec.y);
}

dl_bool test_vec2_normalize() {
  dl_vec2 vec;
  dl_real a,b,sqr_m,inv_m;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_normalize(&vec, &vec);

  sqr_m = a * a + b * b;
#if DL_IS_C89 || DL_IS_C90
  inv_m = 1.0f / sqrt(sqr_m);
#else  
  inv_m = 1.0f / sqrt(sqr_m);
#endif

  return dl_check(dl_approximately_equal(vec.x, a * inv_m, DL_EPSILON),
    "Expected x to be %f, was %f", a * inv_m, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b * inv_m, DL_EPSILON),
      "Expected y to be %f, was %f", b * inv_m, vec.y) &&
    dl_check(dl_approximately_equal(dl_vec2_magnitude(&vec), 1.0, DL_EPSILON),
      "Expected magnitude to be 1");
}

dl_bool test_vec2_negate() {
  dl_vec2 vec;
  dl_real a,b;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  dl_vec2_negate(&vec, &vec);

  return dl_check(dl_approximately_equal(vec.x, -a, DL_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, -b, DL_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y);
}

dl_bool test_vec2_dot() {
  dl_vec2 vec_a;
  dl_vec2 vec_b;
  dl_real a,b,c,d,dot,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec_a, a, b);
  dl_init_vec2(&vec_b, c, d);

  dot = dl_vec2_dot(&vec_a, &vec_b);
  expected = a * c + b * d;

  return dl_check(dl_approximately_equal(dot, expected, DL_EPSILON),
    "Expected dot product to be %f, was %f", expected, dot);
}

dl_bool test_vec2_approximately_equal() {
  dl_vec2 vec_a;
  dl_vec2 vec_b;
  dl_real a,b;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec_a, a, b);
  dl_init_vec2(&vec_b, a + 0.5 * DL_EPSILON, b + 0.5 * DL_EPSILON);

  if (!dl_check(dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) ~= (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, -vec_a.x, -vec_a.y);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, -vec_a.x, vec_a.y);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, vec_a.x, -vec_a.y);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, vec_a.x + 2 * DL_EPSILON, vec_a.y);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, vec_a.x, vec_a.y + 2 * DL_EPSILON);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  dl_init_vec2(&vec_b, vec_a.x + 2 * DL_EPSILON, vec_a.y + 2 * DL_EPSILON);

  if (!dl_check(!dl_vec2_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  return true;
}

dl_bool test_vec2_square_magnitude() {
  dl_vec2 vec;
  dl_real a,b,m,expected_m;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  m = dl_vec2_square_magnitude(&vec);
  expected_m = a * a + b * b;

  return dl_check(dl_approximately_equal(m, expected_m, DL_EPSILON),
    "Expected %f ~= %f", m, expected_m);
}

dl_bool test_vec2_magnitude() {
  dl_vec2 vec;
  dl_real a,b,m,expected_m;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec2(&vec, a, b);

  m = dl_vec2_magnitude(&vec);
  
#if DL_IS_C89 || DL_IS_C90
  expected_m = sqrt(a * a + b * b);
#else
  expected_m = hypotf(a, b);
#endif

  return dl_check(dl_approximately_equal(m, expected_m, DL_EPSILON),
    "Expected %f ~= %f", m, expected_m);
}

dl_bool test_vec2_reflect() {
  dl_vec2 in, out, result;
  in.x = 0.5;
  in.y = 0.5;
  out.x = 0.5;
  out.y = -0.5;

  dl_vec2_reflect(&in, &dl_vec2_up, &result);

  if (!dl_check(dl_vec2_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  dl_init_vec2(&out, -0.5, 0.5);
  dl_vec2_reflect(&in, &dl_vec2_left, &result);

  if (!dl_check(dl_vec2_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  return true;
}

dl_bool test_vec2_refract() {
  dl_vec2 in, out, result;
  in.x = 1.0;
  in.y = 1.0;
  out.x = 0.3;
  out.y = 0.0;
  dl_vec2_refract(&in, &dl_vec2_up, 0.3, &result);

  if (!dl_check(dl_vec2_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  dl_init_vec2(&out, 0.0, 0.3);
  dl_vec2_refract(&in, &dl_vec2_left, 0.3, &result);

  if (!dl_check(dl_vec2_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  return true;
}



dl_bool test_init_point2() {
  dl_point2 point;
  dl_real a,b;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  return dl_check(dl_approximately_equal(point.x, a, DL_EPSILON),
    "Expected x to be %f, was %f", a, point.x) &&
    dl_check(dl_approximately_equal(point.y, b, DL_EPSILON),
      "Expected y to be %f, was %f", b, point.y);
}

dl_bool test_point2_add() {
  dl_point2 point_a;
  dl_point2 point_b;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point_a, a, b);
  dl_init_point2(&point_b, c, d);

  dl_point2_add(&point_a, &point_b, &point_b);

  return dl_check(dl_approximately_equal(point_b.x, a + c, DL_EPSILON),
    "Expected x to be %f, was %f", a + c, point_b.x) &&
    dl_check(dl_approximately_equal(point_b.y, b + d, DL_EPSILON),
      "Expected y to be %f, was %f", b + d, point_b.y);
}

dl_bool test_point2_sub() {
  dl_point2 point_a;
  dl_point2 point_b;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point_a, a, b);
  dl_init_point2(&point_b, c, d);

  dl_point2_sub(&point_a, &point_b, &point_b);

  return dl_check(dl_approximately_equal(point_b.x, a - c, DL_EPSILON),
    "Expected x to be %f, was %f", a - c, point_b.x) &&
    dl_check(dl_approximately_equal(point_b.y, b - d, DL_EPSILON),
      "Expected y to be %f, was %f", b - d, point_b.y);
}

dl_bool test_point2_mul_scalar() {
  dl_point2 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  dl_point2_mul_scalar(&point, c, &point);

  return dl_check(dl_approximately_equal(point.x, a * c, DL_EPSILON),
    "Expected x to be %f, was %f", a * c, point.x) &&
    dl_check(dl_approximately_equal(point.y, b * c, DL_EPSILON),
      "Expected y to be %f, was %f", b * c, point.y);
}

dl_bool test_point2_div_scalar() {
  dl_point2 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  dl_point2_div_scalar(&point, c, &point);

  return dl_check(dl_approximately_equal(point.x, a / c, DL_EPSILON),
    "Expected x to be %f, was %f", a / c, point.x) &&
    dl_check(dl_approximately_equal(point.y, b / c, DL_EPSILON),
      "Expected y to be %f, was %f", b / c, point.y);
}

dl_bool test_point2_add_scalar() {
  dl_point2 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  dl_point2_add_scalar(&point, c, &point);

  return dl_check(dl_approximately_equal(point.x, a + c, DL_EPSILON),
    "Expected x to be %f, was %f", a + c, point.x) &&
    dl_check(dl_approximately_equal(point.y, b + c, DL_EPSILON),
      "Expected y to be %f, was %f", b + c, point.y);
}

dl_bool test_point2_sub_scalar() {
  dl_point2 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  dl_point2_sub_scalar(&point, c, &point);

  return dl_check(dl_approximately_equal(point.x, a - c, DL_EPSILON),
    "Expected x to be %f, was %f", a - c, point.x) &&
    dl_check(dl_approximately_equal(point.y, b - c, DL_EPSILON),
      "Expected y to be %f, was %f", b - c, point.y);
}

dl_bool test_point2_negate() {
  dl_point2 point;
  dl_real a,b;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point, a, b);

  dl_point2_negate(&point, &point);

  return dl_check(dl_approximately_equal(point.x, -a, DL_EPSILON),
    "Expected x to be %f, was %f", -a, point.x) &&
    dl_check(dl_approximately_equal(point.y, -b, DL_EPSILON),
      "Expected y to be %f, was %f", -b, point.y);
}

dl_bool test_point2_approximately_equal() {
  dl_point2 point_a;
  dl_point2 point_b;
  dl_real a,b;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point2(&point_a, a, b);
  dl_init_point2(&point_b, a, b);

  if (!dl_check(dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) ~= (%f, %f)", point_b.x, point_b.y, point_a.x, point_a.y))
    return false;

  dl_init_point2(&point_b, -a, -b);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  dl_init_point2(&point_b, a, -b);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  dl_init_point2(&point_b, -a, b);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  dl_init_point2(&point_b, a + DL_EPSILON * 2, b);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  dl_init_point2(&point_b, a, b + DL_EPSILON * 2);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  dl_init_point2(&point_b, a + DL_EPSILON * 2, b + DL_EPSILON * 2);

  if (!dl_check(!dl_point2_approximately_equal(&point_a, &point_b, DL_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  return true;
}

dl_bool test_point2_line_orientation() {
  dl_point2 above, below, on, a, b;
  dl_real f;
  
  dl_init_point2(&above, 0.0, 1.0);
  dl_init_point2(&below, 1.0, 0.0);
  dl_init_point2(&on, 0.5, 0.5);

  dl_init_point2(&a, 0.0, 0.0);
  dl_init_point2(&b, 1.0, 1.0);

  /* Above is negative */
  f = dl_point2_line_orientation(&above, &a, &b);
  if (!dl_check(0 > f, "Expected value to be negative, was %f", f))
    return false;

  /* Below is positive */
  f = dl_point2_line_orientation(&below, &a, &b);
  if (!dl_check(0 < f, "Expected value to be positive, was %f", f))
    return false;

  /* On is approximately zero */
  f = dl_point2_line_orientation(&on, &a, &b);
  if (!dl_check(dl_approximately_equal(f, 0.0, DL_EPSILON),
    "Expected value to be zero, was %f", f))
    return false;

  return true;
}



dl_bool test_init_vec4() {
  dl_vec4 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  return dl_check(dl_approximately_equal(vec.x, a, DL_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b, DL_EPSILON),
      "Expected y to be %f, was %f", b, vec.y) &&
    dl_check(dl_approximately_equal(vec.z, c, DL_EPSILON),
      "Expected z to be %f, was %f", c, vec.z) &&
    dl_check(dl_approximately_equal(vec.w, d, DL_EPSILON),
      "Expected w to be %f, was %f", d, vec.w);
}

dl_bool test_vec4_add() {
  dl_vec4 vec_a, vec_b;
  dl_real a,b,c,d,e,f,g,h;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  g = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  h = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec_a, a, b, c, d);
  dl_init_vec4(&vec_b, e, f, g, h);

  dl_vec4_add(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(a + e, vec_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + e, vec_b.x) &&
    dl_check(dl_approximately_equal(b + f, vec_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + f, vec_b.y) &&
    dl_check(dl_approximately_equal(c + g, vec_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + g, vec_b.z) &&
    dl_check(dl_approximately_equal(d + h, vec_b.w, DL_EPSILON),
      "Expected w to be %f, was %f", d + h, vec_b.w);
}

dl_bool test_vec4_sub() {
  dl_vec4 vec_a, vec_b;
  dl_real a,b,c,d,e,f,g,h;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  g = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  h = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec_a, a, b, c, d);
  dl_init_vec4(&vec_b, e, f, g, h);

  dl_vec4_sub(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(a - e, vec_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - e, vec_b.x) &&
    dl_check(dl_approximately_equal(b - f, vec_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - f, vec_b.y) &&
    dl_check(dl_approximately_equal(c - g, vec_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - g, vec_b.z) &&
    dl_check(dl_approximately_equal(d - h, vec_b.w, DL_EPSILON),
      "Expected w to be %f, was %f", d - h, vec_b.w);
}

dl_bool test_vec4_mul_scalar() {
  dl_vec4 vec;
  dl_real a,b,c,d,e;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  dl_vec4_mul_scalar(&vec, e, &vec);

  return dl_check(dl_approximately_equal(a * e, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a * e, vec.x) &&
    dl_check(dl_approximately_equal(b * e, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b * e, vec.y) &&
    dl_check(dl_approximately_equal(c * e, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c * e, vec.z) &&
    dl_check(dl_approximately_equal(d * e, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", d * e, vec.w);
}

dl_bool test_vec4_div_scalar() {
  dl_vec4 vec;
  dl_real a,b,c,d,e;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  dl_vec4_div_scalar(&vec, e, &vec);

  return dl_check(dl_approximately_equal(a / e, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a / e, vec.x) &&
    dl_check(dl_approximately_equal(b / e, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b / e, vec.y) &&
    dl_check(dl_approximately_equal(c / e, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c / e, vec.z) &&
    dl_check(dl_approximately_equal(d / e, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", d / e, vec.w);
}

dl_bool test_vec4_add_scalar() {
  dl_vec4 vec;
  dl_real a,b,c,d,e;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  dl_vec4_add_scalar(&vec, e, &vec);

  return dl_check(dl_approximately_equal(a + e, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + e, vec.x) &&
    dl_check(dl_approximately_equal(b + e, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + e, vec.y) &&
    dl_check(dl_approximately_equal(c + e, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + e, vec.z) &&
    dl_check(dl_approximately_equal(d + e, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", d + e, vec.w);
}

dl_bool test_vec4_sub_scalar() {
  dl_vec4 vec;
  dl_real a,b,c,d,e;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  dl_vec4_sub_scalar(&vec, e, &vec);

  return dl_check(dl_approximately_equal(a - e, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - e, vec.x) &&
    dl_check(dl_approximately_equal(b - e, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - e, vec.y) &&
    dl_check(dl_approximately_equal(c - e, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - e, vec.z) &&
    dl_check(dl_approximately_equal(d - e, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", d - e, vec.w);
}

dl_bool test_vec4_dot() {
  dl_vec4 vec_a, vec_b;
  dl_real a,b,c,d,e,f,dot,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec_a, a, b, c, d);
  dl_init_vec4(&vec_b, c, d, e, f);

  dot = dl_vec4_dot(&vec_a, &vec_b);
  expected = a * c + b * d + c * e + d * f;

  return dl_check(dl_approximately_equal(dot, expected, DL_EPSILON),
    "Expected dot product to be %f, was %f", dot, expected);
}

dl_bool test_vec4_normalize() {
  dl_vec4 vec;
  dl_real a,b,c,d,m;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);
  dl_vec4_normalize(&vec, &vec);

#if DL_IS_C89 || DL_IS_C90
  m = sqrt(a * a + b * b + c * c + d * d);
#else
  m = sqrtf(a * a + b * b + c * c + d * d);
#endif

  return dl_check(dl_approximately_equal(a / m, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a / m, vec.x) &&
    dl_check(dl_approximately_equal(b / m, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b / m, vec.y) &&
    dl_check(dl_approximately_equal(c / m, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c / m, vec.z) &&
    dl_check(dl_approximately_equal(d / m, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", d / m, vec.w) &&
    dl_check(dl_approximately_equal(dl_vec4_magnitude(&vec), 1.0, DL_EPSILON),
      "Expected magnitude to be 1");
}

dl_bool test_vec4_negate() {
  dl_vec4 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);
  dl_vec4_negate(&vec, &vec);

  return dl_check(dl_approximately_equal(-a, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    dl_check(dl_approximately_equal(-b, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y) &&
    dl_check(dl_approximately_equal(-c, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", -c, vec.z) &&
    dl_check(dl_approximately_equal(-d, vec.w, DL_EPSILON),
      "Expected w to be %f, was %f", -d, vec.w);
}

dl_bool test_vec4_approximately_equal() {
  dl_vec4 vec_a, vec_b;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec_a, a, b, c, d);
  dl_init_vec4(&vec_b, a, b, c, d);

  if (!dl_check(dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) ~= (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, -a, -b, -c, -d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, -a, -b, c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, -a, b, -c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, -a, b, c, -d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, -b, c, -d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, b, -c, -d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, -b, -c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, -b, c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, b, -c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a, b, c, -d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;
  
  dl_init_vec4(&vec_b, a + 2 * DL_EPSILON, b, c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a + 2 * DL_EPSILON, b + 2 * DL_EPSILON, c, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a + 2 * DL_EPSILON, b + 2 * DL_EPSILON, c + 2 * DL_EPSILON, d);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  dl_init_vec4(&vec_b, a + 2 * DL_EPSILON, b + 2 * DL_EPSILON, c + 2 * DL_EPSILON, d + 2 * DL_EPSILON);

  if (!dl_check(!dl_vec4_approximately_equal(&vec_a, &vec_b, DL_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  return true;
}

dl_bool test_vec4_square_magnitude() {
  dl_vec4 vec;
  dl_real a,b,c,d,square_m,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  square_m = dl_vec4_square_magnitude(&vec);
  expected = a * a + b * b + c * c + d * d;

  return dl_check(dl_approximately_equal(square_m, expected, DL_EPSILON),
    "Expected %f to be approximately %f", square_m, expected);
}

dl_bool test_vec4_magnitude() {
  dl_vec4 vec;
  dl_real a,b,c,d,m,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec4(&vec, a, b, c, d);

  m = dl_vec4_magnitude(&vec);

#if DL_IS_C89 || DL_IS_C90
  expected = sqrt(a * a + b * b + c * c + d * d);
#else
  expected = sqrtf(a * a + b * b + c * c + d * d);
#endif

  return dl_check(dl_approximately_equal(m, expected, DL_EPSILON),
    "Expected %f to be approximately %f", m, expected);
}



dl_bool test_init_point3() {
  dl_point3 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  return dl_check(dl_approximately_equal(point.x, a, DL_EPSILON),
    "Expected x to be %f, was %f", a, point.x) &&
    dl_check(dl_approximately_equal(point.y, b, DL_EPSILON),
      "Expected y to be %f, was %f", b, point.y) &&
    dl_check(dl_approximately_equal(point.z, c, DL_EPSILON),
      "Expected z to be %f, was %f", c, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_add() {
  dl_point3 point_a, point_b;
  dl_real a,b,c,d,e,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point_a, a, b, c);
  dl_init_point3(&point_b, d, e, f);

  dl_point3_add(&point_a, &point_b, &point_b);

  return dl_check(dl_approximately_equal(a + d, point_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + d, point_b.x) &&
    dl_check(dl_approximately_equal(b + e, point_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + e, point_b.y) &&
    dl_check(dl_approximately_equal(c + f, point_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + f, point_b.z) &&
    dl_check(point_b.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_sub() {
  dl_point3 point_a, point_b;
  dl_real a,b,c,d,e,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point_a, a, b, c);
  dl_init_point3(&point_b, d, e, f);

  dl_point3_sub(&point_a, &point_b, &point_b);

  return dl_check(dl_approximately_equal(a - d, point_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - d, point_b.x) &&
    dl_check(dl_approximately_equal(b - e, point_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - e, point_b.y) &&
    dl_check(dl_approximately_equal(c - f, point_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - f, point_b.z) &&
    dl_check(point_b.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_mul_scalar() {
  dl_point3 point;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  dl_point3_mul_scalar(&point, d, &point);

  return dl_check(dl_approximately_equal(a * d, point.x, DL_EPSILON),
    "Expected x to be %f, was %f", a * d, point.x) &&
    dl_check(dl_approximately_equal(b * d, point.y, DL_EPSILON),
      "Expected y to be %f, was %f", b * d, point.y) &&
    dl_check(dl_approximately_equal(c * d, point.z, DL_EPSILON),
      "Expected z to be %f, was %f", c * d, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_div_scalar() {
  dl_point3 point;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  dl_point3_div_scalar(&point, d, &point);

  return dl_check(dl_approximately_equal(a / d, point.x, DL_EPSILON),
    "Expected x to be %f, was %f", a / d, point.x) &&
    dl_check(dl_approximately_equal(b / d, point.y, DL_EPSILON),
      "Expected y to be %f, was %f", b / d, point.y) &&
    dl_check(dl_approximately_equal(c / d, point.z, DL_EPSILON),
      "Expected z to be %f, was %f", c / d, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_add_scalar() {
  dl_point3 point;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  dl_point3_add_scalar(&point, d, &point);

  return dl_check(dl_approximately_equal(a + d, point.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + d, point.x) &&
    dl_check(dl_approximately_equal(b + d, point.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + d, point.y) &&
    dl_check(dl_approximately_equal(c + d, point.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + d, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_sub_scalar() {
  dl_point3 point;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  dl_point3_sub_scalar(&point, d, &point);

  return dl_check(dl_approximately_equal(a - d, point.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - d, point.x) &&
    dl_check(dl_approximately_equal(b - d, point.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - d, point.y) &&
    dl_check(dl_approximately_equal(c - d, point.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - d, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_rotate() {
  dl_point3 point, expected_x, expected_y, expected_z, out;
  dl_real angle;

  dl_random_state r;
  dl_init_random_time(&r);
  
  /* Identity: no angle */
  point = dl_point3_one;
  dl_point3_rotate(&point, &dl_vec3_up, 0.0, &out);

  if (!dl_check(dl_point3_approximately_equal(&dl_point3_one, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, dl_point3_one.x, dl_point3_one.y, dl_point3_one.z))
    return false;

  /* Identity: zero vector */
  point = dl_point3_one;
  dl_point3_rotate(&point, &dl_vec3_zero, 0.0, &out);

  if (!dl_check(dl_point3_approximately_equal(&dl_point3_one, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, dl_point3_one.x, dl_point3_one.y, dl_point3_one.z))
    return false;

  angle = DL_PI * 0.5f;

#if USE_LEFT_HANDED
  /* Positive is clockwise in left-handed, we're testing for counter-clockwise */
  dl_init_point3(&expected_x, dl_vec3_forward.x, dl_vec3_forward.y, dl_vec3_forward.z);
  dl_init_point3(&expected_y, dl_vec3_backward.x, dl_vec3_backward.y, dl_vec3_backward.z);
  dl_init_point3(&expected_z, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);
#else
  dl_init_point3(&expected_x, dl_vec3_backward.x, dl_vec3_backward.y, dl_vec3_backward.z);
  dl_init_point3(&expected_y, dl_vec3_forward.x, dl_vec3_forward.y, dl_vec3_forward.z);
  dl_init_point3(&expected_z, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);
#endif
  
  /* Rotate around x, 90 degrees */
  dl_init_point3(&point, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);

  dl_point3_rotate(&point, &dl_vec3_right, angle, &out);

  if (!dl_check(dl_point3_approximately_equal(&expected_x, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_x.x, expected_x.y, expected_x.z))
    return false;

  /* Rotate around y, 90 degrees */
  dl_init_point3(&point, dl_vec3_right.x, dl_vec3_right.y, dl_vec3_right.z);

  dl_point3_rotate(&point, &dl_vec3_up, angle, &out);

  if (!dl_check(dl_point3_approximately_equal(&expected_y, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_y.x, expected_y.y, expected_y.z))
    return false;
  
  /* Rotate around z, 90 degrees */
  dl_init_point3(&point, dl_vec3_right.x, dl_vec3_right.y, dl_vec3_right.z);

  dl_point3_rotate(&point, &dl_vec3_forward, angle, &out);

  if (!dl_check(dl_point3_approximately_equal(&expected_z, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_z.x, expected_z.y, expected_z.z))
    return false;

  return true;
}

dl_bool test_point3_negate() {
  dl_point3 point;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point, a, b, c);

  dl_point3_negate(&point, &point);

  return dl_check(dl_approximately_equal(-a, point.x, DL_EPSILON),
    "Expected x to be %f, was %f", -a, point.x) &&
    dl_check(dl_approximately_equal(-b, point.y, DL_EPSILON),
      "Expected y to be %f, was %f", -b, point.y) &&
    dl_check(dl_approximately_equal(-c, point.z, DL_EPSILON),
      "Expected z to be %f, was %f", -c, point.z) &&
    dl_check(point.w == 1.0, "Expected w to be 1.0");
}

dl_bool test_point3_approximately_equal() {
  dl_point3 point_a, point_b;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_point3(&point_a, a, b, c);
  dl_init_point3(&point_b, a, b, c);

  if (!dl_check(dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be equal"))
    return false;

  dl_init_point3(&point_b, -a, b, c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, -a, -b, c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, -a, b, -c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, -a, -b, -c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, a + DL_EPSILON * 2, b, c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, a, b + DL_EPSILON * 2, c);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  dl_init_point3(&point_b, a, b, c + DL_EPSILON * 2);
  if (!dl_check(!dl_point3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected points to be unequal"))
    return false;

  return true;
}



dl_bool test_init_vec3() {
  dl_vec3 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  return dl_check(dl_approximately_equal(vec.x, a, DL_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    dl_check(dl_approximately_equal(vec.y, b, DL_EPSILON),
      "Expected y to be %f, was %f", b, vec.y) &&
    dl_check(dl_approximately_equal(vec.z, c, DL_EPSILON),
      "Expected z to be %f, was %f", c, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_add() {
  dl_vec3 vec_a, vec_b;
  dl_real a,b,c,d,e,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec_a, a, b, c);
  dl_init_vec3(&vec_b, d, e, f);

  dl_vec3_add(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(a + d, vec_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + d, vec_b.x) &&
    dl_check(dl_approximately_equal(b + e, vec_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + e, vec_b.y) &&
    dl_check(dl_approximately_equal(c + f, vec_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + f, vec_b.z) &&
    dl_check(vec_b.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_sub() {
  dl_vec3 vec_a, vec_b;
  dl_real a,b,c,d,e,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec_a, a, b, c);
  dl_init_vec3(&vec_b, d, e, f);

  dl_vec3_sub(&vec_a, &vec_b, &vec_b);

  return dl_check(dl_approximately_equal(a - d, vec_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - d, vec_b.x) &&
    dl_check(dl_approximately_equal(b - e, vec_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - e, vec_b.y) &&
    dl_check(dl_approximately_equal(c - f, vec_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - f, vec_b.z) &&
    dl_check(vec_b.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_cross() {
  dl_vec3 vec_a, vec_b;
  dl_real a,b,c,d,e,f,cross_x,cross_y,cross_z;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec_a, a, b, c);
  dl_init_vec3(&vec_b, d, e, f);

  dl_vec3_cross(&vec_a, &vec_b, &vec_b);

  cross_x = b * f - c * e;
  cross_y = c * d - a * f;
  cross_z = a * e - b * d;

  return dl_check(dl_approximately_equal(cross_x, vec_b.x, DL_EPSILON),
    "Expected x to be %f, was %f", cross_x, vec_b.x) &&
    dl_check(dl_approximately_equal(cross_y, vec_b.y, DL_EPSILON),
      "Expected y to be %f, was %f", cross_y, vec_b.y) &&
    dl_check(dl_approximately_equal(cross_z, vec_b.z, DL_EPSILON),
      "Expected z to be %f, was %f", cross_z, vec_b.z) &&
    dl_check(vec_b.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_dot() {
  dl_vec3 vec_a, vec_b;
  dl_real a,b,c,d,e,f,dot,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  e = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  f = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec_a, a, b, c);
  dl_init_vec3(&vec_b, d, e, f);

  dot = dl_vec3_dot(&vec_a, &vec_b);
  expected = a * d + b * e + c * f;

  return dl_check(dl_approximately_equal(dot, expected, DL_EPSILON),
    "Expected dot product to be %f, was %f", dot, expected);
}

dl_bool test_vec3_mul_scalar() {
  dl_vec3 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_mul_scalar(&vec, d, &vec);

  return dl_check(dl_approximately_equal(a * d, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a * d, vec.x) &&
    dl_check(dl_approximately_equal(b * d, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b * d, vec.y) &&
    dl_check(dl_approximately_equal(c * d, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c * d, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_div_scalar() {
  dl_vec3 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_div_scalar(&vec, d, &vec);

  return dl_check(dl_approximately_equal(a / d, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a / d, vec.x) &&
    dl_check(dl_approximately_equal(b / d, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b / d, vec.y) &&
    dl_check(dl_approximately_equal(c / d, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c / d, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_add_scalar() {
  dl_vec3 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_add_scalar(&vec, d, &vec);

  return dl_check(dl_approximately_equal(a + d, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a + d, vec.x) &&
    dl_check(dl_approximately_equal(b + d, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b + d, vec.y) &&
    dl_check(dl_approximately_equal(c + d, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c + d, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w);
}

dl_bool test_vec3_sub_scalar() {
  dl_vec3 vec;
  dl_real a,b,c,d;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  d = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_sub_scalar(&vec, d, &vec);

  return dl_check(dl_approximately_equal(a - d, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a - d, vec.x) &&
    dl_check(dl_approximately_equal(b - d, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b - d, vec.y) &&
    dl_check(dl_approximately_equal(c - d, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c - d, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w);
}

dl_bool test_vec3_rotate() {
  dl_vec3 vec, expected_x, expected_y, expected_z, out;
  dl_real angle;

  /* Identity: no angle */
  vec = dl_vec3_one;
  dl_vec3_rotate(&vec, &dl_vec3_up, 0.0, &out);

  if (!dl_check(dl_vec3_approximately_equal(&dl_vec3_one, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, dl_vec3_one.x, dl_vec3_one.y, dl_vec3_one.z))
    return false;

  /* Identity: zero vector */
  vec = dl_vec3_one;
  dl_vec3_rotate(&vec, &dl_vec3_zero, 0.0, &out);

  if (!dl_check(dl_vec3_approximately_equal(&dl_vec3_one, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, dl_vec3_one.x, dl_vec3_one.y, dl_vec3_one.z))
    return false;

  angle = DL_PI * 0.5f;

#if USE_LEFT_HANDED
  dl_init_vec3(&expected_x, dl_vec3_down.x, dl_vec3_down.y, dl_vec3_down.z);
  dl_init_vec3(&expected_y, dl_vec3_backward.x, dl_vec3_backward.y, dl_vec3_backward.z);
  dl_init_vec3(&expected_z, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);
#else
  dl_init_vec3(&expected_x, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);
  dl_init_vec3(&expected_y, dl_vec3_forward.x, dl_vec3_forward.y, dl_vec3_forward.z);
  dl_init_vec3(&expected_z, dl_vec3_up.x, dl_vec3_up.y, dl_vec3_up.z);
#endif

  /* Rotate around x, 90 degrees */
  dl_init_vec3(&vec, dl_vec3_forward.x, dl_vec3_forward.y, dl_vec3_forward.z);

  dl_vec3_rotate(&vec, &dl_vec3_right, angle, &out);

  if (!dl_check(dl_vec3_approximately_equal(&expected_x, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_x.x, expected_x.y, expected_x.z))
    return false;

  /* Rotate around y, 90 degrees */
  dl_init_vec3(&vec, dl_vec3_right.x, dl_vec3_right.y, dl_vec3_right.z);

  dl_vec3_rotate(&vec, &dl_vec3_up, angle, &out);

  if (!dl_check(dl_vec3_approximately_equal(&expected_y, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_y.x, expected_y.y, expected_y.z))
    return false;

  /* Rotate around z, 90 degrees */
  dl_init_vec3(&vec, dl_vec3_right.x, dl_vec3_right.y, dl_vec3_right.z);

  dl_vec3_rotate(&vec, &dl_vec3_forward, angle, &out);

  if (!dl_check(dl_vec3_approximately_equal(&expected_z, &out, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_z.x, expected_z.y, expected_z.z))
    return false;

  return true;
}

dl_bool test_vec3_normalize() {
  dl_vec3 vec;
  dl_real a,b,c,m;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_normalize(&vec, &vec);

#if DL_IS_C89 || DL_IS_C90
  m = sqrt(a * a + b * b + c * c);
#else
  m = sqrtf(a * a + b * b + c * c);
#endif

  return dl_check(dl_approximately_equal(a / m, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", a / m, vec.x) &&
    dl_check(dl_approximately_equal(b / m, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", b / m, vec.y) &&
    dl_check(dl_approximately_equal(c / m, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", c / m, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w) &&
    dl_check(dl_approximately_equal(dl_vec3_magnitude(&vec), 1.0, DL_EPSILON),
      "Expected magnitude to be 1");
}

dl_bool test_vec3_negate() {
  dl_vec3 vec;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  dl_vec3_negate(&vec, &vec);

  return dl_check(dl_approximately_equal(-a, vec.x, DL_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    dl_check(dl_approximately_equal(-b, vec.y, DL_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y) &&
    dl_check(dl_approximately_equal(-c, vec.z, DL_EPSILON),
      "Expected z to be %f, was %f", -c, vec.z) &&
    dl_check(vec.w == 0.0, "Expected w to be 0.0");
}

dl_bool test_vec3_approximately_equal() {
  dl_vec3 point_a, point_b;
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&point_a, a, b, c);
  dl_init_vec3(&point_b, a, b, c);

  if (!dl_check(dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be equal"))
    return false;

  dl_init_vec3(&point_b, -a, b, c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, -a, -b, c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, -a, b, -c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, -a, -b, -c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, a + DL_EPSILON * 2, b, c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, a, b + DL_EPSILON * 2, c);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  dl_init_vec3(&point_b, a, b, c + DL_EPSILON * 2);
  if (!dl_check(!dl_vec3_approximately_equal(&point_a, &point_b, DL_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  return true;
}

dl_bool test_vec3_square_magnitude() {
  dl_vec3 vec;
  dl_real a,b,c,square_m,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  square_m = dl_vec3_square_magnitude(&vec);
  expected = a * a + b * b + c * c;

  return dl_check(dl_approximately_equal(square_m, expected, DL_EPSILON),
    "Expected square magnitude to be %f, was %f", expected, square_m);
}

dl_bool test_vec3_magnitude() {
  dl_vec3 vec;
  dl_real a,b,c,m,expected;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec, a, b, c);

  m = dl_vec3_magnitude(&vec);

#if DL_IS_C89 || DL_IS_C90
  expected = sqrt(a * a + b * b + c * c);
#else
  expected = sqrtf(a * a + b * b + c * c);
#endif

  return dl_check(dl_approximately_equal(m, expected, DL_EPSILON),
    "Expected square magnitude to be %f, was %f", expected, m);
}

dl_bool test_vec3_reflect() {
  dl_vec3 in, out, result;
  in = out = result = dl_vec3_zero;
  in.x = 0.5;
  in.y = 0.5;
  in.z = 0.5;
  out.x = 0.5;
  out.y = -0.5;
  out.z = 0.5;
  
  dl_vec3_reflect(&in, &dl_vec3_up, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  dl_init_vec3(&out, -0.5, 0.5, 0.5);
  dl_vec3_reflect(&in, &dl_vec3_left, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  dl_init_vec3(&out, 0.5, 0.5, -0.5);
  dl_vec3_reflect(&in, &dl_vec3_forward, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  return true;
}

dl_bool test_vec3_refract() {
  dl_vec3 in, out, result;
  in = out = result = dl_vec3_zero;
  in.x = 1.0;
  in.y = 1.0;
  in.z = 1.0;
  out.x = 0.3;
  out.y = 0.0;
  out.z = 0.3;

  dl_vec3_refract(&in, &dl_vec3_up, 0.3, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  dl_init_vec3(&out, 0.0, 0.3, 0.3);
  dl_vec3_refract(&in, &dl_vec3_left, 0.3, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  dl_init_vec3(&out, 0.3, 0.3, 0.0);
  dl_vec3_refract(&in, &dl_vec3_forward, 0.3, &result);

  if (!dl_check(dl_vec3_approximately_equal(&result, &out, DL_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  return true;
}



dl_bool test_init_mat4() {
  dl_mat4 mat;
  dl_vec4 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3;
  dl_natural v_idx, col_idx, row_idx;
  dl_real mat_val, col_val;
  dl_random_state r;
  dl_init_random_time(&r);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      mat_val = mat.ary[col_idx][row_idx];
      col_val = ((dl_real *)&col[col_idx])[row_idx];
      if (!dl_check(dl_approximately_equal(mat_val, col_val, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, col_val, mat_val))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_add() {
  dl_mat4 mat[2];
  dl_vec4 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3;
  dl_natural v_idx, col_idx, row_idx;
  dl_real v,f;
  dl_random_state r;
  dl_init_random_time(&r);  

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  dl_init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  dl_mat4_add(&mat[a], &mat[b], &mat[b]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] + ((dl_real *)&col[3 - col_idx])[row_idx];
      f = mat[b].ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_sub() {
  dl_mat4 mat[2];
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3;
  dl_natural v_idx, col_idx, row_idx;
  dl_real v,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  dl_init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  dl_mat4_sub(&mat[a], &mat[b], &mat[b]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] - ((dl_real *)&col[3 - col_idx])[row_idx];
      f = mat[b].ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_mul() {
  dl_mat4 mat[2];
  dl_vec4 col[4];
  dl_natural v_idx, a = 0, b = 1, c = 2, d = 3;
  dl_real expected, found;
  dl_random_state r;
  
  dl_init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  dl_init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  dl_mat4_mul(&mat[a], &mat[b], &mat[b]);

  expected = col[a].x * col[d].x + col[b].x * col[d].y + col[c].x * col[d].z + col[d].x * col[d].w;
  found = mat[b].ary[0][0];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[d].x + col[b].y * col[d].y + col[c].y * col[d].z + col[d].y * col[d].w;
  found = mat[b].ary[0][1];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[d].x + col[b].z * col[d].y + col[c].z * col[d].z + col[d].z * col[d].w;
  found = mat[b].ary[0][2];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[d].x + col[b].w * col[d].y + col[c].w * col[d].z + col[d].w * col[d].w;
  found = mat[b].ary[0][3];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[c].x + col[b].x * col[c].y + col[c].x * col[c].z + col[d].x * col[c].w;
  found = mat[b].ary[1][0];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[c].x + col[b].y * col[c].y + col[c].y * col[c].z + col[d].y * col[c].w;
  found = mat[b].ary[1][1];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[c].x + col[b].z * col[c].y + col[c].z * col[c].z + col[d].z * col[c].w;
  found = mat[b].ary[1][2];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[c].x + col[b].w * col[c].y + col[c].w * col[c].z + col[d].w * col[c].w;
  found = mat[b].ary[1][3];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[b].x + col[b].x * col[b].y + col[c].x * col[b].z + col[d].x * col[b].w;
  found = mat[b].ary[2][0];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[b].x + col[b].y * col[b].y + col[c].y * col[b].z + col[d].y * col[b].w;
  found = mat[b].ary[2][1];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[b].x + col[b].z * col[b].y + col[c].z * col[b].z + col[d].z * col[b].w;
  found = mat[b].ary[2][2];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[b].x + col[b].w * col[b].y + col[c].w * col[b].z + col[d].w * col[b].w;
  found = mat[b].ary[2][3];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[a].x + col[b].x * col[a].y + col[c].x * col[a].z + col[d].x * col[a].w;
  found = mat[b].ary[3][0];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[a].x + col[b].y * col[a].y + col[c].y * col[a].z + col[d].y * col[a].w;
  found = mat[b].ary[3][1];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[a].x + col[b].z * col[a].y + col[c].z * col[a].z + col[d].z * col[a].w;
  found = mat[b].ary[3][2];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[a].x + col[b].w * col[a].y + col[c].w * col[a].z + col[d].w * col[a].w;
  found = mat[b].ary[3][3];

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  dl_mat4_mul(&mat[a], &dl_mat4_identity, &mat[b]);

  if (!dl_check(dl_mat4_approximately_equal(&mat[a], &mat[b], DL_EPSILON),
    "Expected identity to change nothing"))
    return false;

  return true;
}

dl_bool test_mat4_mul_vec4() {
  dl_natural a = 2, b = 3, c = 4, d = 5, i;
  dl_vec4 vec[6];
  dl_mat4 mat;
  dl_real expected, found;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (i = 0; i < 6; ++i) {
    dl_init_vec4(&vec[i],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_mat4_mul_vec4(&dl_mat4_identity, &vec[0], &vec[1]);

  if (!dl_check(dl_vec4_approximately_equal(&vec[0], &vec[1], DL_EPSILON),
    "Expected vector/identity multiplication to work"))
    return false;

  dl_init_mat4_cols(&mat, &vec[a], &vec[b], &vec[c], &vec[d]);
  dl_mat4_mul_vec4(&mat, &vec[0], &vec[1]);

  expected = vec[a].x * vec[0].x + vec[b].x * vec[0].y + vec[c].x * vec[0].z + vec[d].x * vec[0].w;
  found = vec[1].x;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].y * vec[0].x + vec[b].y * vec[0].y + vec[c].y * vec[0].z + vec[d].y * vec[0].w;
  found = vec[1].y;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].z * vec[0].x + vec[b].z * vec[0].y + vec[c].z * vec[0].z + vec[d].z * vec[0].w;
  found = vec[1].z;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].w * vec[0].x + vec[b].w * vec[0].y + vec[c].w * vec[0].z + vec[d].w * vec[0].w;
  found = vec[1].w;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  return true;
}

dl_bool test_mat4_mul_vec3() {
  dl_vec4 mat_vec[4];
  dl_vec3 vec, vec_out;
  dl_mat4 mat;
  dl_natural i;
  dl_real expected, found;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (i = 0; i < 4; ++i) {
    dl_init_vec4(&mat_vec[i],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_vec3(&vec,
    dl_random_real_range(&r, MIN_REAL, MAX_REAL),
    dl_random_real_range(&r, MIN_REAL, MAX_REAL),
    dl_random_real_range(&r, MIN_REAL, MAX_REAL));

  dl_mat4_mul_vec3(&dl_mat4_identity, &vec, &vec_out);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &vec_out, DL_EPSILON),
    "Expected vector/identity multiplication to work"))
    return false;

  dl_init_mat4_cols(&mat, &mat_vec[0], &mat_vec[1], &mat_vec[2], &mat_vec[3]);
  dl_mat4_mul_vec3(&mat, &vec, &vec_out);

  expected = mat.ary[0][0] * vec.x + mat.ary[1][0] * vec.y + mat.ary[2][0] * vec.z + mat.ary[3][0] * vec.w;
  found = vec_out.x;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][1] * vec.x + mat.ary[1][1] * vec.y + mat.ary[2][1] * vec.z + mat.ary[3][1] * vec.w;
  found = vec_out.y;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][2] * vec.x + mat.ary[1][2] * vec.y + mat.ary[2][2] * vec.z + mat.ary[3][2] * vec.w;
  found = vec_out.z;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  if (!dl_check(vec_out.w == 0, "Expected %f to be %f", vec_out.w, 0.0))
    return false;

  return true;
}

dl_bool test_mat4_mul_point3() {
  dl_vec4 mat_vec[4];
  dl_point3 point, point_out;
  dl_mat4 mat;
  dl_natural i;
  dl_real expected, found;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (i = 0; i < 4; ++i) {
    dl_init_vec4(&mat_vec[i],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_point3(&point,
    dl_random_real_range(&r, MIN_REAL, MAX_REAL),
    dl_random_real_range(&r, MIN_REAL, MAX_REAL),
    dl_random_real_range(&r, MIN_REAL, MAX_REAL));

  dl_mat4_mul_point3(&dl_mat4_identity, &point, &point_out);

  if (!dl_check(dl_point3_approximately_equal(&point, &point_out, DL_EPSILON),
    "Expected point/identity multiplication to work"))
    return false;

  dl_init_mat4_cols(&mat, &mat_vec[0], &mat_vec[1], &mat_vec[2], &mat_vec[3]);
  dl_mat4_mul_point3(&mat, &point, &point_out);

  expected = mat.ary[0][0] * point.x + mat.ary[1][0] * point.y + mat.ary[2][0] * point.z + mat.ary[3][0] * point.w;
  found = point_out.x;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][1] * point.x + mat.ary[1][1] * point.y + mat.ary[2][1] * point.z + mat.ary[3][1] * point.w;
  found = point_out.y;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][2] * point.x + mat.ary[1][2] * point.y + mat.ary[2][2] * point.z + mat.ary[3][2] * point.w;
  found = point_out.z;

  if (!dl_check(dl_approximately_equal(expected, found, DL_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  if (!dl_check(point_out.w == 1, "Expected %f to be %f", point_out.w, 1.0))
    return false;

  return true;
}

dl_bool test_mat4_transpose() {
  dl_mat4 mat;
  dl_vec3 col[4];
  dl_natural v_idx, a = 0, b = 1, c = 2, d = 3, col_idx, row_idx;
  dl_real v,f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  dl_mat4_transpose(&mat, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[row_idx])[col_idx];
      f = mat.ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_mul_scalar() {
  dl_mat4 mat;
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  dl_real scalar, v, f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  scalar = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  dl_mat4_mul_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] * scalar;
      f = mat.ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_div_scalar() {
  dl_mat4 mat;
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  dl_real scalar, v, f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  scalar = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  dl_mat4_div_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] / scalar;
      f = mat.ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_add_scalar() {
  dl_mat4 mat;
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  dl_real v, f, scalar;
  dl_random_state r;
  dl_init_random_time(&r);
  
  scalar = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  dl_mat4_add_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] + scalar;
      f = mat.ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_sub_scalar() {
  dl_mat4 mat;
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  dl_real scalar, v, f;
  dl_random_state r;
  dl_init_random_time(&r);
  
  scalar = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  dl_mat4_sub_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((dl_real *)&col[col_idx])[row_idx] - scalar;
      f = mat.ary[col_idx][row_idx];
      if (!dl_check(dl_approximately_equal(v, f, DL_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

dl_bool test_mat4_translate() {
  dl_mat4 mat;
  dl_point3 point[2];
  dl_vec3 vec[2];
  dl_real a,b,c;
  dl_random_state r;
  dl_init_random_time(&r);
  
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec[0], a, b, c);
  dl_init_point3(&point[0], a, b, c);

  dl_init_mat4_translate(&mat, a, b, c);
  dl_mat4_mul_point3(&mat, &point[0], &point[1]);

  if (!dl_check(dl_approximately_equal(point[1].x, a + a, DL_EPSILON) &&
    dl_approximately_equal(point[1].y, b + b, DL_EPSILON) &&
    dl_approximately_equal(point[1].z, c + c, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    point[1].x, point[1].y, point[1].z, a + a, b + b, c + c))
    return false;

  dl_mat4_mul_vec3(&mat, &vec[0], &vec[1]);

  if (!dl_check(dl_vec3_approximately_equal(&vec[0], &vec[1], DL_EPSILON),
    "Expected no change to vector"))
    return false;

  return true;
}

dl_bool test_mat4_rotate() {
  dl_random_state r;
  dl_real angle, a, b, c;
  dl_vec3 vec[3];
  dl_mat4 mat[2];
  
  dl_init_random_time(&r);
  
  angle = dl_random_radian(&r);
  a = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  b = dl_random_real_range(&r, MIN_REAL, MAX_REAL);
  c = dl_random_real_range(&r, MIN_REAL, MAX_REAL);

  dl_init_vec3(&vec[0], a, b, c);

  dl_init_mat4_rotate(&mat[0], &dl_vec3_right, angle);
  dl_init_mat4_rotate_x(&mat[1], angle);

  dl_mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  dl_mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!dl_check(dl_vec3_approximately_equal(&vec[1], &vec[2], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  dl_init_mat4_rotate(&mat[0], &dl_vec3_up, angle);
  dl_init_mat4_rotate_y(&mat[1], angle);

  dl_mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  dl_mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!dl_check(dl_vec3_approximately_equal(&vec[1], &vec[2], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  dl_init_mat4_rotate(&mat[0], &dl_vec3_forward, angle);
  dl_init_mat4_rotate_z(&mat[1], angle);

  dl_mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  dl_mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!dl_check(dl_vec3_approximately_equal(&vec[1], &vec[2], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  return true;
}

dl_bool test_mat4_rotate_x() {
  dl_mat4 mat;
  dl_vec3 vec;
  dl_real angle;
#if USE_LEFT_HANDED
  dl_vec3 expected[4];
  expected[0] = dl_vec3_forward;
  expected[1] = dl_vec3_down;
  expected[2] = dl_vec3_backward;
  expected[3] = dl_vec3_up;
#else
  dl_vec3 expected[4];
  expected[0] = dl_vec3_backward;
  expected[1] = dl_vec3_down;
  expected[2] = dl_vec3_forward;
  expected[3] = dl_vec3_up;
#endif

  vec = expected[3];
  
  angle = DL_PI * 0.5f;

  dl_init_mat4_rotate_x(&mat, angle);
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[0], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[0].x, expected[0].y, expected[0].z))
    return false;

  vec = expected[0];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[1], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[1].x, expected[1].y, expected[1].z))
    return false;

  vec = expected[1];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[2], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[2].x, expected[2].y, expected[2].z))
    return false;

  vec = expected[2];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[3], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[3].x, expected[3].y, expected[3].z))
    return false;

  return true;
}

dl_bool test_mat4_rotate_y() {
  dl_mat4 mat;
  dl_vec3 vec;
  dl_real angle;
#if USE_LEFT_HANDED
  dl_vec3 expected[4];
  expected[0] = dl_vec3_backward;
  expected[1] = dl_vec3_left;
  expected[2] = dl_vec3_forward;
  expected[3] = dl_vec3_right;
#else
  dl_vec3 expected[4];
  expected[0] = dl_vec3_forward;
  expected[1] = dl_vec3_left;
  expected[2] = dl_vec3_backward;
  expected[3] = dl_vec3_right;
#endif
  
  vec = expected[3];

  angle = DL_PI * 0.5f;

  dl_init_mat4_rotate_y(&mat, angle);
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[0], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[0].x, expected[0].y, expected[0].z))
    return false;

  vec = expected[0];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[1], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[1].x, expected[1].y, expected[1].z))
    return false;

  vec = expected[1];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[2], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[2].x, expected[2].y, expected[2].z))
    return false;

  vec = expected[2];
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected[3], DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[3].x, expected[3].y, expected[3].z))
    return false;

  return true;
}

dl_bool test_mat4_rotate_z() {
  dl_mat4 mat;
  dl_vec3 vec = dl_vec3_up;
  dl_vec3 expected = dl_vec3_left;

  dl_real angle = DL_PI * 0.5f;

  dl_init_mat4_rotate_z(&mat, angle);
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = dl_vec3_left;
  expected = dl_vec3_down;
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = dl_vec3_down;
  expected = dl_vec3_right;
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = dl_vec3_right;
  expected = dl_vec3_up;
  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_vec3_approximately_equal(&vec, &expected, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  return true;
}

dl_bool test_mat4_scale() {
  dl_point3 point = dl_point3_one;
  dl_point3 expected_point;
  dl_vec3 vec = dl_vec3_one;
  dl_vec3 expected_vec;
  dl_mat4 mat;

  dl_point3_mul_scalar(&point, 2, &expected_point);

  dl_init_mat4_scale(&mat, 2, 2, 2);

  dl_mat4_mul_point3(&mat, &point, &point);

  if (!dl_check(dl_point3_approximately_equal(&point, &expected_point, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    point.x, point.y, point.z, expected_point.x, expected_point.y, expected_point.z))
    return false;

  dl_vec3_mul_scalar(&vec, 2, &expected_vec);

  dl_mat4_mul_vec3(&mat, &vec, &vec);

  if (!dl_check(dl_point3_approximately_equal(&vec, &expected_vec, DL_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected_vec.x, expected_vec.y, expected_vec.z))
    return false;

  return true;
}

dl_bool test_mat4_approximately_equal() {
  dl_mat4 mat[2];
  dl_vec3 col[4];
  dl_natural a = 0, b = 1, c = 2, d = 3, v_idx;
  dl_random_state r;
  dl_init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    dl_init_vec4(&col[v_idx],
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL),
      dl_random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  dl_init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  dl_init_mat4_cols(&mat[b], &col[a], &col[b], &col[c], &col[d]);

  return dl_check(dl_mat4_approximately_equal(&mat[a], &mat[b], DL_EPSILON),
    "Expected matrices to be equal");
}

#endif
