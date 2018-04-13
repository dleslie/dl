#include "dl.h"

#if DL_USE_MATH

/* For testing purposes; MAX * MAX should be a sane value */
real MIN_REAL = -1024;
real MAX_REAL = 1024;

bool test_approximately_equal() {
  if (!check(approximately_equal(0.5f, 0.51f, 0.01f),
    "Expected 0.5 ~= 0.51"))
    return false;
  if (!check(!approximately_equal(0.5f, 0.51f, 0.005f),
    "Expected 0.5 != 0.51"))
    return false;
  if (!check(!approximately_equal(10.0f, 1000.0f, 0.1f),
    "Expected 10.0 != 1000.0"))
    return false;
  return true;
}

bool test_floor_to_integer() {
  if (!check(floor_to_integer(0.1f) == 0,
    "Expected 0.1 to become 0"))
    return false;

  if (!check(floor_to_integer(0.6f) == 0,
    "Expected 0.6 to become 0"))
    return false;

  return true;
}

bool test_ceil_to_integer() {
  if (!check(ceil_to_integer(0.1f) == 1,
    "Expected 0.1 to become 1"))
    return false;

  if (!check(ceil_to_integer(0.6f) == 1,
    "Expected 0.6 to become 1"))
    return false;

  return true;
}

bool test_round_to_integer() {
  if (!check(round_to_integer(0.1f) == 0,
    "Expected 0.1 to become 0"))
    return false;

  if (!check(round_to_integer(0.6f) == 1,
    "Expected 0.6 to become 1"))
    return false;

  return true;
}

bool test_min() {
  if (!check(min(0, 1) == 0,
    "Expected 0 to be less than 1"))
    return false;
  return true;
}

bool test_max() {
  if (!check(max(0, 1) == 1,
    "Expected 0 to be less than 1"))
    return false;
  return true;
}

bool test_clamp() {
  if (!check(clamp(5, 1, 2) == 2,
    "Expected 5 to clamp to 2"))
    return false;

  if (!check(clamp(0, 1, 2) == 1,
    "Expected 0 to clamp to 1"))
    return false;

  if (!check(clamp(1.5, 1, 2) == 1.5,
    "Expected 0.5 to clamp to 0.5, not %f", clamp(1.5, 1, 2)))
    return false;

  return true;
}

bool test_factorial() {
  if (!check(factorial(3) == 6,
    "Expected %i to be 6", factorial(3)))
    return false;

  if (!check(factorial(-1) == 1,
    "Expected %i to be 1", factorial(-1)))
    return false;

  if (!check(factorial(8) == 40320,
    "Expected %i to be 40320", factorial(8)))
    return false;

  return true;
}

bool test_rationalize() {
  if (!check(approximately_equal(rationalize(0.05f, 1), 0.1f, M_EPSILON),
    "Expected %f to be 0.1", rationalize(0.05f, 1)))
    return false;

  if (!check(approximately_equal(rationalize(10.5f, 1), 10.5f, M_EPSILON),
    "Expected %f to be 10.5", rationalize(10.5f, 1)))
    return false;

  return true;
}

bool test_degree_to_radian() {
  real result;
  result = degree_to_radian(286.5);
  return check(approximately_equal(result, 5.0f, M_EPSILON),
    "Expected %f to equal %f", result, 5.0);
}

bool test_radian_to_degree() {
  real result;
  result = radian_to_degree(5.0);
  return check(approximately_equal(result, 286.5f, 0.1f),
    "Expected %f to equal %f", result, 286.5f);
}

bool test_random_degree() {
  random_state r;
  real value;
  
  init_random_time(&r);
  
  value = random_degree(&r);
  if (!check(value >= 0 && value <= 360,
    "Expected %f to be between 0 and 360", value))
    return false;
  return true;
}

bool test_random_radian() {
  random_state r;
  real value;
  
  init_random_time(&r);
  
  value = random_radian(&r);
  if (!check(value >= 0 && value <= 2 * M_PI,
    "Expected %f to be between 0 and %f", value, 2 * M_PI))
    return false;
  return true;
}

bool test_random_real() {
  random_state r;
  real value;
  init_random_time(&r);
  
  value = random_real(&r, M_PI);
  if (!check(value >= 0 && value <= M_PI,
    "Expected %f to be between 0 and %f", value, M_PI))
    return false;
  return true;
}

bool test_random_real_range() {
  random_state r;
  real value;
  init_random_time(&r);
  
  value = random_real_range(&r, -M_PI, 0);
  if (!check(value >= -M_PI && value <= M_PI,
    "Expected %f to be between %f and 0", value, -M_PI))
    return false;
  return true;
}

bool test_random() {
  random_state r;
  integer value;
  init_random_time(&r);
  
  value = random_integer(&r, 1337);
  if (!check(value >= 0 && value <= 1337,
    "Expected %i to be between 0 and 1337", value))
    return false;
  return true;
}

bool test_random_range() {
  random_state r;
  integer value;
  init_random_time(&r);
  
  value = random_integer_range(&r, -1337, 0);
  if (!check(value >= -1337 && value <= 0,
    "Expected %i to be between -1337 and 1337", value))
    return false;
  return true;
}



bool test_init_vec2() {
  vec2 vec;
  random_state r;
  real a,b;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  return check(approximately_equal(vec.x, a, M_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    check(approximately_equal(vec.y, b, M_EPSILON),
      "Expected y to be %f, was %f", b, vec.y);
}

bool test_vec2_add() {
  vec2 vec_a;
  vec2 vec_b;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec_a, a, b);
  init_vec2(&vec_b, c, d);

  vec2_add(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(vec_b.x, a + c, M_EPSILON),
    "Expected x to be %f, was %f", a + c, vec_b.x) &&
    check(approximately_equal(vec_b.y, b + d, M_EPSILON),
      "Expected y to be %f, was %f", b + d, vec_b.y);
}

bool test_vec2_sub() {
  vec2 vec_a;
  vec2 vec_b;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec_a, a, b);
  init_vec2(&vec_b, c, d);

  vec2_sub(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(vec_b.x, a - c, M_EPSILON),
    "Expected x to be %f, was %f", a - c, vec_b.x) &&
    check(approximately_equal(vec_b.y, b - d, M_EPSILON),
      "Expected y to be %f, was %f", b - d, vec_b.y);
}

bool test_vec2_mul_scalar() {
  vec2 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_mul_scalar(&vec, c, &vec);

  return check(approximately_equal(vec.x, a * c, M_EPSILON),
    "Expected x to be %f, was %f", a * c, vec.x) &&
    check(approximately_equal(vec.y, b * c, M_EPSILON),
      "Expected y to be %f, was %f", b * c, vec.y);
}

bool test_vec2_div_scalar() {
  vec2 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_div_scalar(&vec, c, &vec);

  return check(approximately_equal(vec.x, a / c, M_EPSILON),
    "Expected x to be %f, was %f", a / c, vec.x) &&
    check(approximately_equal(vec.y, b / c, M_EPSILON),
      "Expected y to be %f, was %f", b / c, vec.y);
}

bool test_vec2_add_scalar() {
  vec2 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_add_scalar(&vec, c, &vec);

  return check(approximately_equal(vec.x, a + c, M_EPSILON),
    "Expected x to be %f, was %f", a + c, vec.x) &&
    check(approximately_equal(vec.y, b + c, M_EPSILON),
      "Expected y to be %f, was %f", b + c, vec.y);
}

bool test_vec2_sub_scalar() {
  vec2 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_sub_scalar(&vec, c, &vec);

  return check(approximately_equal(vec.x, a - c, M_EPSILON),
    "Expected x to be %f, was %f", a - c, vec.x) &&
    check(approximately_equal(vec.y, b - c, M_EPSILON),
      "Expected y to be %f, was %f", b - c, vec.y);
}

bool test_vec2_normalize() {
  vec2 vec;
  real a,b,sqr_m,inv_m;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_normalize(&vec, &vec);

  sqr_m = a * a + b * b;
#if IS_C89 || IS_C90
  inv_m = 1.0f / sqrt(sqr_m);
#else  
  inv_m = 1.0f / sqrt(sqr_m);
#endif

  return check(approximately_equal(vec.x, a * inv_m, M_EPSILON),
    "Expected x to be %f, was %f", a * inv_m, vec.x) &&
    check(approximately_equal(vec.y, b * inv_m, M_EPSILON),
      "Expected y to be %f, was %f", b * inv_m, vec.y) &&
    check(approximately_equal(vec2_magnitude(&vec), 1.0, M_EPSILON),
      "Expected magnitude to be 1");
}

bool test_vec2_negate() {
  vec2 vec;
  real a,b;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  vec2_negate(&vec, &vec);

  return check(approximately_equal(vec.x, -a, M_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    check(approximately_equal(vec.y, -b, M_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y);
}

bool test_vec2_dot() {
  vec2 vec_a;
  vec2 vec_b;
  real a,b,c,d,dot,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec_a, a, b);
  init_vec2(&vec_b, c, d);

  dot = vec2_dot(&vec_a, &vec_b);
  expected = a * c + b * d;

  return check(approximately_equal(dot, expected, M_EPSILON),
    "Expected dot product to be %f, was %f", expected, dot);
}

bool test_vec2_approximately_equal() {
  vec2 vec_a;
  vec2 vec_b;
  real a,b;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec_a, a, b);
  init_vec2(&vec_b, a + 0.5 * M_EPSILON, b + 0.5 * M_EPSILON);

  if (!check(vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) ~= (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, -vec_a.x, -vec_a.y);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, -vec_a.x, vec_a.y);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, vec_a.x, -vec_a.y);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, vec_a.x + 2 * M_EPSILON, vec_a.y);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, vec_a.x, vec_a.y + 2 * M_EPSILON);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  init_vec2(&vec_b, vec_a.x + 2 * M_EPSILON, vec_a.y + 2 * M_EPSILON);

  if (!check(!vec2_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", vec_a.x, vec_a.y, vec_b.x, vec_b.y))
    return false;

  return true;
}

bool test_vec2_square_magnitude() {
  vec2 vec;
  real a,b,m,expected_m;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  m = vec2_square_magnitude(&vec);
  expected_m = a * a + b * b;

  return check(approximately_equal(m, expected_m, M_EPSILON),
    "Expected %f ~= %f", m, expected_m);
}

bool test_vec2_magnitude() {
  vec2 vec;
  real a,b,m,expected_m;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec2(&vec, a, b);

  m = vec2_magnitude(&vec);
  
#if IS_C89 || IS_C90
  expected_m = sqrt(a * a + b * b);
#else
  expected_m = hypotf(a, b);
#endif

  return check(approximately_equal(m, expected_m, M_EPSILON),
    "Expected %f ~= %f", m, expected_m);
}

bool test_vec2_reflect() {
  vec2 in, out, result;
  in.x = 0.5;
  in.y = 0.5;
  out.x = 0.5;
  out.y = -0.5;

  vec2_reflect(&in, &vec2_up, &result);

  if (!check(vec2_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  init_vec2(&out, -0.5, 0.5);
  vec2_reflect(&in, &vec2_left, &result);

  if (!check(vec2_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  return true;
}

bool test_vec2_refract() {
  vec2 in, out, result;
  in.x = 1.0;
  in.y = 1.0;
  out.x = 0.3;
  out.y = 0.0;
  vec2_refract(&in, &vec2_up, 0.3, &result);

  if (!check(vec2_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  init_vec2(&out, 0.0, 0.3);
  vec2_refract(&in, &vec2_left, 0.3, &result);

  if (!check(vec2_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f} to be {%f, %f}",
    result.x, result.y,
    out.x, out.y))
    return false;

  return true;
}



bool test_init_point2() {
  point2 point;
  real a,b;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  return check(approximately_equal(point.x, a, M_EPSILON),
    "Expected x to be %f, was %f", a, point.x) &&
    check(approximately_equal(point.y, b, M_EPSILON),
      "Expected y to be %f, was %f", b, point.y);
}

bool test_point2_add() {
  point2 point_a;
  point2 point_b;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point_a, a, b);
  init_point2(&point_b, c, d);

  point2_add(&point_a, &point_b, &point_b);

  return check(approximately_equal(point_b.x, a + c, M_EPSILON),
    "Expected x to be %f, was %f", a + c, point_b.x) &&
    check(approximately_equal(point_b.y, b + d, M_EPSILON),
      "Expected y to be %f, was %f", b + d, point_b.y);
}

bool test_point2_sub() {
  point2 point_a;
  point2 point_b;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point_a, a, b);
  init_point2(&point_b, c, d);

  point2_sub(&point_a, &point_b, &point_b);

  return check(approximately_equal(point_b.x, a - c, M_EPSILON),
    "Expected x to be %f, was %f", a - c, point_b.x) &&
    check(approximately_equal(point_b.y, b - d, M_EPSILON),
      "Expected y to be %f, was %f", b - d, point_b.y);
}

bool test_point2_mul_scalar() {
  point2 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  point2_mul_scalar(&point, c, &point);

  return check(approximately_equal(point.x, a * c, M_EPSILON),
    "Expected x to be %f, was %f", a * c, point.x) &&
    check(approximately_equal(point.y, b * c, M_EPSILON),
      "Expected y to be %f, was %f", b * c, point.y);
}

bool test_point2_div_scalar() {
  point2 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  point2_div_scalar(&point, c, &point);

  return check(approximately_equal(point.x, a / c, M_EPSILON),
    "Expected x to be %f, was %f", a / c, point.x) &&
    check(approximately_equal(point.y, b / c, M_EPSILON),
      "Expected y to be %f, was %f", b / c, point.y);
}

bool test_point2_add_scalar() {
  point2 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  point2_add_scalar(&point, c, &point);

  return check(approximately_equal(point.x, a + c, M_EPSILON),
    "Expected x to be %f, was %f", a + c, point.x) &&
    check(approximately_equal(point.y, b + c, M_EPSILON),
      "Expected y to be %f, was %f", b + c, point.y);
}

bool test_point2_sub_scalar() {
  point2 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  point2_sub_scalar(&point, c, &point);

  return check(approximately_equal(point.x, a - c, M_EPSILON),
    "Expected x to be %f, was %f", a - c, point.x) &&
    check(approximately_equal(point.y, b - c, M_EPSILON),
      "Expected y to be %f, was %f", b - c, point.y);
}

bool test_point2_negate() {
  point2 point;
  real a,b;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point, a, b);

  point2_negate(&point, &point);

  return check(approximately_equal(point.x, -a, M_EPSILON),
    "Expected x to be %f, was %f", -a, point.x) &&
    check(approximately_equal(point.y, -b, M_EPSILON),
      "Expected y to be %f, was %f", -b, point.y);
}

bool test_point2_approximately_equal() {
  point2 point_a;
  point2 point_b;
  real a,b;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point2(&point_a, a, b);
  init_point2(&point_b, a, b);

  if (!check(point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) ~= (%f, %f)", point_b.x, point_b.y, point_a.x, point_a.y))
    return false;

  init_point2(&point_b, -a, -b);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  init_point2(&point_b, a, -b);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  init_point2(&point_b, -a, b);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  init_point2(&point_b, a + M_EPSILON * 2, b);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  init_point2(&point_b, a, b + M_EPSILON * 2);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  init_point2(&point_b, a + M_EPSILON * 2, b + M_EPSILON * 2);

  if (!check(!point2_approximately_equal(&point_a, &point_b, M_EPSILON),
    "Expected (%f, %f) != (%f, %f)", point_b.x, point_b.y, a, b))
    return false;

  return true;
}

bool test_point2_line_orientation() {
  point2 above, below, on, a, b;
  real f;
  
  init_point2(&above, 0.0, 1.0);
  init_point2(&below, 1.0, 0.0);
  init_point2(&on, 0.5, 0.5);

  init_point2(&a, 0.0, 0.0);
  init_point2(&b, 1.0, 1.0);

  /* Above is negative */
  f = point2_line_orientation(&above, &a, &b);
  if (!check(0 > f, "Expected value to be negative, was %f", f))
    return false;

  /* Below is positive */
  f = point2_line_orientation(&below, &a, &b);
  if (!check(0 < f, "Expected value to be positive, was %f", f))
    return false;

  /* On is approximately zero */
  f = point2_line_orientation(&on, &a, &b);
  if (!check(approximately_equal(f, 0.0, M_EPSILON),
    "Expected value to be zero, was %f", f))
    return false;

  return true;
}



bool test_init_vec4() {
  vec4 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  return check(approximately_equal(vec.x, a, M_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    check(approximately_equal(vec.y, b, M_EPSILON),
      "Expected y to be %f, was %f", b, vec.y) &&
    check(approximately_equal(vec.z, c, M_EPSILON),
      "Expected z to be %f, was %f", c, vec.z) &&
    check(approximately_equal(vec.w, d, M_EPSILON),
      "Expected w to be %f, was %f", d, vec.w);
}

bool test_vec4_add() {
  vec4 vec_a, vec_b;
  real a,b,c,d,e,f,g,h;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);
  g = random_real_range(&r, MIN_REAL, MAX_REAL);
  h = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec_a, a, b, c, d);
  init_vec4(&vec_b, e, f, g, h);

  vec4_add(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(a + e, vec_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a + e, vec_b.x) &&
    check(approximately_equal(b + f, vec_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b + f, vec_b.y) &&
    check(approximately_equal(c + g, vec_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c + g, vec_b.z) &&
    check(approximately_equal(d + h, vec_b.w, M_EPSILON),
      "Expected w to be %f, was %f", d + h, vec_b.w);
}

bool test_vec4_sub() {
  vec4 vec_a, vec_b;
  real a,b,c,d,e,f,g,h;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);
  g = random_real_range(&r, MIN_REAL, MAX_REAL);
  h = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec_a, a, b, c, d);
  init_vec4(&vec_b, e, f, g, h);

  vec4_sub(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(a - e, vec_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a - e, vec_b.x) &&
    check(approximately_equal(b - f, vec_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b - f, vec_b.y) &&
    check(approximately_equal(c - g, vec_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c - g, vec_b.z) &&
    check(approximately_equal(d - h, vec_b.w, M_EPSILON),
      "Expected w to be %f, was %f", d - h, vec_b.w);
}

bool test_vec4_mul_scalar() {
  vec4 vec;
  real a,b,c,d,e;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  vec4_mul_scalar(&vec, e, &vec);

  return check(approximately_equal(a * e, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a * e, vec.x) &&
    check(approximately_equal(b * e, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b * e, vec.y) &&
    check(approximately_equal(c * e, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c * e, vec.z) &&
    check(approximately_equal(d * e, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", d * e, vec.w);
}

bool test_vec4_div_scalar() {
  vec4 vec;
  real a,b,c,d,e;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  vec4_div_scalar(&vec, e, &vec);

  return check(approximately_equal(a / e, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a / e, vec.x) &&
    check(approximately_equal(b / e, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b / e, vec.y) &&
    check(approximately_equal(c / e, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c / e, vec.z) &&
    check(approximately_equal(d / e, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", d / e, vec.w);
}

bool test_vec4_add_scalar() {
  vec4 vec;
  real a,b,c,d,e;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  vec4_add_scalar(&vec, e, &vec);

  return check(approximately_equal(a + e, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a + e, vec.x) &&
    check(approximately_equal(b + e, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b + e, vec.y) &&
    check(approximately_equal(c + e, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c + e, vec.z) &&
    check(approximately_equal(d + e, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", d + e, vec.w);
}

bool test_vec4_sub_scalar() {
  vec4 vec;
  real a,b,c,d,e;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  vec4_sub_scalar(&vec, e, &vec);

  return check(approximately_equal(a - e, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a - e, vec.x) &&
    check(approximately_equal(b - e, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b - e, vec.y) &&
    check(approximately_equal(c - e, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c - e, vec.z) &&
    check(approximately_equal(d - e, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", d - e, vec.w);
}

bool test_vec4_dot() {
  vec4 vec_a, vec_b;
  real a,b,c,d,e,f,dot,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec_a, a, b, c, d);
  init_vec4(&vec_b, c, d, e, f);

  dot = vec4_dot(&vec_a, &vec_b);
  expected = a * c + b * d + c * e + d * f;

  return check(approximately_equal(dot, expected, M_EPSILON),
    "Expected dot product to be %f, was %f", dot, expected);
}

bool test_vec4_normalize() {
  vec4 vec;
  real a,b,c,d,m;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);
  vec4_normalize(&vec, &vec);

#if IS_C89 || IS_C90
  m = sqrt(a * a + b * b + c * c + d * d);
#else
  m = sqrtf(a * a + b * b + c * c + d * d);
#endif

  return check(approximately_equal(a / m, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a / m, vec.x) &&
    check(approximately_equal(b / m, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b / m, vec.y) &&
    check(approximately_equal(c / m, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c / m, vec.z) &&
    check(approximately_equal(d / m, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", d / m, vec.w) &&
    check(approximately_equal(vec4_magnitude(&vec), 1.0, M_EPSILON),
      "Expected magnitude to be 1");
}

bool test_vec4_negate() {
  vec4 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);
  vec4_negate(&vec, &vec);

  return check(approximately_equal(-a, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    check(approximately_equal(-b, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y) &&
    check(approximately_equal(-c, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", -c, vec.z) &&
    check(approximately_equal(-d, vec.w, M_EPSILON),
      "Expected w to be %f, was %f", -d, vec.w);
}

bool test_vec4_approximately_equal() {
  vec4 vec_a, vec_b;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec_a, a, b, c, d);
  init_vec4(&vec_b, a, b, c, d);

  if (!check(vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) ~= (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, -a, -b, -c, -d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, -a, -b, c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, -a, b, -c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, -a, b, c, -d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, -b, c, -d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, b, -c, -d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, -b, -c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, -b, c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, b, -c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a, b, c, -d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;
  
  init_vec4(&vec_b, a + 2 * M_EPSILON, b, c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a + 2 * M_EPSILON, b + 2 * M_EPSILON, c, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a + 2 * M_EPSILON, b + 2 * M_EPSILON, c + 2 * M_EPSILON, d);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  init_vec4(&vec_b, a + 2 * M_EPSILON, b + 2 * M_EPSILON, c + 2 * M_EPSILON, d + 2 * M_EPSILON);

  if (!check(!vec4_approximately_equal(&vec_a, &vec_b, M_EPSILON),
    "Expected (%f, %f, %f, %f) != (%f, %f, %f, %f)", vec_a.x, vec_a.y, vec_a.z, vec_a.w, vec_b.x, vec_b.y, vec_b.z, vec_b.w))
    return false;

  return true;
}

bool test_vec4_square_magnitude() {
  vec4 vec;
  real a,b,c,d,square_m,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  square_m = vec4_square_magnitude(&vec);
  expected = a * a + b * b + c * c + d * d;

  return check(approximately_equal(square_m, expected, M_EPSILON),
    "Expected %f to be approximately %f", square_m, expected);
}

bool test_vec4_magnitude() {
  vec4 vec;
  real a,b,c,d,m,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec4(&vec, a, b, c, d);

  m = vec4_magnitude(&vec);

#if IS_C89 || IS_C90
  expected = sqrt(a * a + b * b + c * c + d * d);
#else
  expected = sqrtf(a * a + b * b + c * c + d * d);
#endif

  return check(approximately_equal(m, expected, M_EPSILON),
    "Expected %f to be approximately %f", m, expected);
}



bool test_init_point3() {
  point3 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  return check(approximately_equal(point.x, a, M_EPSILON),
    "Expected x to be %f, was %f", a, point.x) &&
    check(approximately_equal(point.y, b, M_EPSILON),
      "Expected y to be %f, was %f", b, point.y) &&
    check(approximately_equal(point.z, c, M_EPSILON),
      "Expected z to be %f, was %f", c, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_add() {
  point3 point_a, point_b;
  real a,b,c,d,e,f;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point_a, a, b, c);
  init_point3(&point_b, d, e, f);

  point3_add(&point_a, &point_b, &point_b);

  return check(approximately_equal(a + d, point_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a + d, point_b.x) &&
    check(approximately_equal(b + e, point_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b + e, point_b.y) &&
    check(approximately_equal(c + f, point_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c + f, point_b.z) &&
    check(point_b.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_sub() {
  point3 point_a, point_b;
  real a,b,c,d,e,f;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point_a, a, b, c);
  init_point3(&point_b, d, e, f);

  point3_sub(&point_a, &point_b, &point_b);

  return check(approximately_equal(a - d, point_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a - d, point_b.x) &&
    check(approximately_equal(b - e, point_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b - e, point_b.y) &&
    check(approximately_equal(c - f, point_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c - f, point_b.z) &&
    check(point_b.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_mul_scalar() {
  point3 point;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  point3_mul_scalar(&point, d, &point);

  return check(approximately_equal(a * d, point.x, M_EPSILON),
    "Expected x to be %f, was %f", a * d, point.x) &&
    check(approximately_equal(b * d, point.y, M_EPSILON),
      "Expected y to be %f, was %f", b * d, point.y) &&
    check(approximately_equal(c * d, point.z, M_EPSILON),
      "Expected z to be %f, was %f", c * d, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_div_scalar() {
  point3 point;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  point3_div_scalar(&point, d, &point);

  return check(approximately_equal(a / d, point.x, M_EPSILON),
    "Expected x to be %f, was %f", a / d, point.x) &&
    check(approximately_equal(b / d, point.y, M_EPSILON),
      "Expected y to be %f, was %f", b / d, point.y) &&
    check(approximately_equal(c / d, point.z, M_EPSILON),
      "Expected z to be %f, was %f", c / d, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_add_scalar() {
  point3 point;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  point3_add_scalar(&point, d, &point);

  return check(approximately_equal(a + d, point.x, M_EPSILON),
    "Expected x to be %f, was %f", a + d, point.x) &&
    check(approximately_equal(b + d, point.y, M_EPSILON),
      "Expected y to be %f, was %f", b + d, point.y) &&
    check(approximately_equal(c + d, point.z, M_EPSILON),
      "Expected z to be %f, was %f", c + d, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_sub_scalar() {
  point3 point;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  point3_sub_scalar(&point, d, &point);

  return check(approximately_equal(a - d, point.x, M_EPSILON),
    "Expected x to be %f, was %f", a - d, point.x) &&
    check(approximately_equal(b - d, point.y, M_EPSILON),
      "Expected y to be %f, was %f", b - d, point.y) &&
    check(approximately_equal(c - d, point.z, M_EPSILON),
      "Expected z to be %f, was %f", c - d, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_rotate() {
  point3 point, expected_x, expected_y, expected_z, out;
  real angle;

  random_state r;
  init_random_time(&r);
  
  /* Identity: no angle */
  point = point3_one;
  point3_rotate(&point, &vec3_up, 0.0, &out);

  if (!check(point3_approximately_equal(&point3_one, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, point3_one.x, point3_one.y, point3_one.z))
    return false;

  /* Identity: zero vector */
  point = point3_one;
  point3_rotate(&point, &vec3_zero, 0.0, &out);

  if (!check(point3_approximately_equal(&point3_one, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, point3_one.x, point3_one.y, point3_one.z))
    return false;

  angle = M_PI * 0.5f;

#if USE_LEFT_HANDED
  /* Positive is clockwise in left-handed, we're testing for counter-clockwise */
  init_point3(&expected_x, vec3_forward.x, vec3_forward.y, vec3_forward.z);
  init_point3(&expected_y, vec3_backward.x, vec3_backward.y, vec3_backward.z);
  init_point3(&expected_z, vec3_up.x, vec3_up.y, vec3_up.z);
#else
  init_point3(&expected_x, vec3_backward.x, vec3_backward.y, vec3_backward.z);
  init_point3(&expected_y, vec3_forward.x, vec3_forward.y, vec3_forward.z);
  init_point3(&expected_z, vec3_up.x, vec3_up.y, vec3_up.z);
#endif
  
  /* Rotate around x, 90 degrees */
  init_point3(&point, vec3_up.x, vec3_up.y, vec3_up.z);

  point3_rotate(&point, &vec3_right, angle, &out);

  if (!check(point3_approximately_equal(&expected_x, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_x.x, expected_x.y, expected_x.z))
    return false;

  /* Rotate around y, 90 degrees */
  init_point3(&point, vec3_right.x, vec3_right.y, vec3_right.z);

  point3_rotate(&point, &vec3_up, angle, &out);

  if (!check(point3_approximately_equal(&expected_y, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_y.x, expected_y.y, expected_y.z))
    return false;
  
  /* Rotate around z, 90 degrees */
  init_point3(&point, vec3_right.x, vec3_right.y, vec3_right.z);

  point3_rotate(&point, &vec3_forward, angle, &out);

  if (!check(point3_approximately_equal(&expected_z, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_z.x, expected_z.y, expected_z.z))
    return false;

  return true;
}

bool test_point3_negate() {
  point3 point;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point, a, b, c);

  point3_negate(&point, &point);

  return check(approximately_equal(-a, point.x, M_EPSILON),
    "Expected x to be %f, was %f", -a, point.x) &&
    check(approximately_equal(-b, point.y, M_EPSILON),
      "Expected y to be %f, was %f", -b, point.y) &&
    check(approximately_equal(-c, point.z, M_EPSILON),
      "Expected z to be %f, was %f", -c, point.z) &&
    check(point.w == 1.0, "Expected w to be 1.0");
}

bool test_point3_approximately_equal() {
  point3 point_a, point_b;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_point3(&point_a, a, b, c);
  init_point3(&point_b, a, b, c);

  if (!check(point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be equal"))
    return false;

  init_point3(&point_b, -a, b, c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, -a, -b, c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, -a, b, -c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, -a, -b, -c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, a + M_EPSILON * 2, b, c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, a, b + M_EPSILON * 2, c);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  init_point3(&point_b, a, b, c + M_EPSILON * 2);
  if (!check(!point3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected points to be unequal"))
    return false;

  return true;
}



bool test_init_vec3() {
  vec3 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  return check(approximately_equal(vec.x, a, M_EPSILON),
    "Expected x to be %f, was %f", a, vec.x) &&
    check(approximately_equal(vec.y, b, M_EPSILON),
      "Expected y to be %f, was %f", b, vec.y) &&
    check(approximately_equal(vec.z, c, M_EPSILON),
      "Expected z to be %f, was %f", c, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_add() {
  vec3 vec_a, vec_b;
  real a,b,c,d,e,f;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec_a, a, b, c);
  init_vec3(&vec_b, d, e, f);

  vec3_add(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(a + d, vec_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a + d, vec_b.x) &&
    check(approximately_equal(b + e, vec_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b + e, vec_b.y) &&
    check(approximately_equal(c + f, vec_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c + f, vec_b.z) &&
    check(vec_b.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_sub() {
  vec3 vec_a, vec_b;
  real a,b,c,d,e,f;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec_a, a, b, c);
  init_vec3(&vec_b, d, e, f);

  vec3_sub(&vec_a, &vec_b, &vec_b);

  return check(approximately_equal(a - d, vec_b.x, M_EPSILON),
    "Expected x to be %f, was %f", a - d, vec_b.x) &&
    check(approximately_equal(b - e, vec_b.y, M_EPSILON),
      "Expected y to be %f, was %f", b - e, vec_b.y) &&
    check(approximately_equal(c - f, vec_b.z, M_EPSILON),
      "Expected z to be %f, was %f", c - f, vec_b.z) &&
    check(vec_b.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_cross() {
  vec3 vec_a, vec_b;
  real a,b,c,d,e,f,cross_x,cross_y,cross_z;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec_a, a, b, c);
  init_vec3(&vec_b, d, e, f);

  vec3_cross(&vec_a, &vec_b, &vec_b);

  cross_x = b * f - c * e;
  cross_y = c * d - a * f;
  cross_z = a * e - b * d;

  return check(approximately_equal(cross_x, vec_b.x, M_EPSILON),
    "Expected x to be %f, was %f", cross_x, vec_b.x) &&
    check(approximately_equal(cross_y, vec_b.y, M_EPSILON),
      "Expected y to be %f, was %f", cross_y, vec_b.y) &&
    check(approximately_equal(cross_z, vec_b.z, M_EPSILON),
      "Expected z to be %f, was %f", cross_z, vec_b.z) &&
    check(vec_b.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_dot() {
  vec3 vec_a, vec_b;
  real a,b,c,d,e,f,dot,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);
  e = random_real_range(&r, MIN_REAL, MAX_REAL);
  f = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec_a, a, b, c);
  init_vec3(&vec_b, d, e, f);

  dot = vec3_dot(&vec_a, &vec_b);
  expected = a * d + b * e + c * f;

  return check(approximately_equal(dot, expected, M_EPSILON),
    "Expected dot product to be %f, was %f", dot, expected);
}

bool test_vec3_mul_scalar() {
  vec3 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_mul_scalar(&vec, d, &vec);

  return check(approximately_equal(a * d, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a * d, vec.x) &&
    check(approximately_equal(b * d, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b * d, vec.y) &&
    check(approximately_equal(c * d, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c * d, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_div_scalar() {
  vec3 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_div_scalar(&vec, d, &vec);

  return check(approximately_equal(a / d, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a / d, vec.x) &&
    check(approximately_equal(b / d, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b / d, vec.y) &&
    check(approximately_equal(c / d, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c / d, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_add_scalar() {
  vec3 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_add_scalar(&vec, d, &vec);

  return check(approximately_equal(a + d, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a + d, vec.x) &&
    check(approximately_equal(b + d, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b + d, vec.y) &&
    check(approximately_equal(c + d, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c + d, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w);
}

bool test_vec3_sub_scalar() {
  vec3 vec;
  real a,b,c,d;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);
  d = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_sub_scalar(&vec, d, &vec);

  return check(approximately_equal(a - d, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a - d, vec.x) &&
    check(approximately_equal(b - d, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b - d, vec.y) &&
    check(approximately_equal(c - d, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c - d, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w);
}

bool test_vec3_rotate() {
  vec3 vec, expected_x, expected_y, expected_z, out;
  real angle;

  /* Identity: no angle */
  vec = vec3_one;
  vec3_rotate(&vec, &vec3_up, 0.0, &out);

  if (!check(vec3_approximately_equal(&vec3_one, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, vec3_one.x, vec3_one.y, vec3_one.z))
    return false;

  /* Identity: zero vector */
  vec = vec3_one;
  vec3_rotate(&vec, &vec3_zero, 0.0, &out);

  if (!check(vec3_approximately_equal(&vec3_one, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, vec3_one.x, vec3_one.y, vec3_one.z))
    return false;

  angle = M_PI * 0.5f;

#if USE_LEFT_HANDED
  init_vec3(&expected_x, vec3_down.x, vec3_down.y, vec3_down.z);
  init_vec3(&expected_y, vec3_backward.x, vec3_backward.y, vec3_backward.z);
  init_vec3(&expected_z, vec3_up.x, vec3_up.y, vec3_up.z);
#else
  init_vec3(&expected_x, vec3_up.x, vec3_up.y, vec3_up.z);
  init_vec3(&expected_y, vec3_forward.x, vec3_forward.y, vec3_forward.z);
  init_vec3(&expected_z, vec3_up.x, vec3_up.y, vec3_up.z);
#endif

  /* Rotate around x, 90 degrees */
  init_vec3(&vec, vec3_forward.x, vec3_forward.y, vec3_forward.z);

  vec3_rotate(&vec, &vec3_right, angle, &out);

  if (!check(vec3_approximately_equal(&expected_x, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_x.x, expected_x.y, expected_x.z))
    return false;

  /* Rotate around y, 90 degrees */
  init_vec3(&vec, vec3_right.x, vec3_right.y, vec3_right.z);

  vec3_rotate(&vec, &vec3_up, angle, &out);

  if (!check(vec3_approximately_equal(&expected_y, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_y.x, expected_y.y, expected_y.z))
    return false;

  /* Rotate around z, 90 degrees */
  init_vec3(&vec, vec3_right.x, vec3_right.y, vec3_right.z);

  vec3_rotate(&vec, &vec3_forward, angle, &out);

  if (!check(vec3_approximately_equal(&expected_z, &out, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)", out.x, out.y, out.z, expected_z.x, expected_z.y, expected_z.z))
    return false;

  return true;
}

bool test_vec3_normalize() {
  vec3 vec;
  real a,b,c,m;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_normalize(&vec, &vec);

#if IS_C89 || IS_C90
  m = sqrt(a * a + b * b + c * c);
#else
  m = sqrtf(a * a + b * b + c * c);
#endif

  return check(approximately_equal(a / m, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", a / m, vec.x) &&
    check(approximately_equal(b / m, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", b / m, vec.y) &&
    check(approximately_equal(c / m, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", c / m, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0, was %f", vec.w) &&
    check(approximately_equal(vec3_magnitude(&vec), 1.0, M_EPSILON),
      "Expected magnitude to be 1");
}

bool test_vec3_negate() {
  vec3 vec;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  vec3_negate(&vec, &vec);

  return check(approximately_equal(-a, vec.x, M_EPSILON),
    "Expected x to be %f, was %f", -a, vec.x) &&
    check(approximately_equal(-b, vec.y, M_EPSILON),
      "Expected y to be %f, was %f", -b, vec.y) &&
    check(approximately_equal(-c, vec.z, M_EPSILON),
      "Expected z to be %f, was %f", -c, vec.z) &&
    check(vec.w == 0.0, "Expected w to be 0.0");
}

bool test_vec3_approximately_equal() {
  vec3 point_a, point_b;
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&point_a, a, b, c);
  init_vec3(&point_b, a, b, c);

  if (!check(vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be equal"))
    return false;

  init_vec3(&point_b, -a, b, c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, -a, -b, c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, -a, b, -c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, -a, -b, -c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, a + M_EPSILON * 2, b, c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, a, b + M_EPSILON * 2, c);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  init_vec3(&point_b, a, b, c + M_EPSILON * 2);
  if (!check(!vec3_approximately_equal(&point_a, &point_b, M_EPSILON),
	     "Expected vectors to be unequal"))
    return false;

  return true;
}

bool test_vec3_square_magnitude() {
  vec3 vec;
  real a,b,c,square_m,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  square_m = vec3_square_magnitude(&vec);
  expected = a * a + b * b + c * c;

  return check(approximately_equal(square_m, expected, M_EPSILON),
    "Expected square magnitude to be %f, was %f", expected, square_m);
}

bool test_vec3_magnitude() {
  vec3 vec;
  real a,b,c,m,expected;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec, a, b, c);

  m = vec3_magnitude(&vec);

#if IS_C89 || IS_C90
  expected = sqrt(a * a + b * b + c * c);
#else
  expected = sqrtf(a * a + b * b + c * c);
#endif

  return check(approximately_equal(m, expected, M_EPSILON),
    "Expected square magnitude to be %f, was %f", expected, m);
}

bool test_vec3_reflect() {
  vec3 in, out, result;
  in.x = 0.5;
  in.y = 0.5;
  in.z = 0.5;
  out.x = 0.5;
  out.y = -0.5;
  out.z = 0.5;
  vec3_reflect(&in, &vec3_up, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  init_vec3(&out, -0.5, 0.5, 0.5);
  vec3_reflect(&in, &vec3_left, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  init_vec3(&out, 0.5, 0.5, -0.5);
  vec3_reflect(&in, &vec3_forward, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  return true;
}

bool test_vec3_refract() {
  vec3 in, out, result;
  in.x = 1.0;
  in.y = 1.0;
  in.z = 1.0;
  out.x = 0.3;
  out.y = 0.0;
  out.z = 0.3;
  vec3_refract(&in, &vec3_up, 0.3, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  init_vec3(&out, 0.0, 0.3, 0.3);
  vec3_refract(&in, &vec3_left, 0.3, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  init_vec3(&out, 0.3, 0.3, 0.0);
  vec3_refract(&in, &vec3_forward, 0.3, &result);

  if (!check(vec3_approximately_equal(&result, &out, M_EPSILON),
    "Expected {%f, %f, %f} to be {%f, %f, %f}",
    result.x, result.y, result.z,
    out.x, out.y, out.z))
    return false;

  return true;
}



bool test_init_mat4() {
  mat4 mat;
  vec4 col[4];
  natural a = 0, b = 1, c = 2, d = 3;
  natural v_idx, col_idx, row_idx;
  real mat_val, col_val;
  random_state r;
  init_random_time(&r);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      mat_val = mat.ary[col_idx][row_idx];
      col_val = ((real *)&col[col_idx])[row_idx];
      if (!check(approximately_equal(mat_val, col_val, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, col_val, mat_val))
        return false;
    }
  }

  return true;
}

bool test_mat4_add() {
  mat4 mat[2];
  vec4 col[4];
  natural a = 0, b = 1, c = 2, d = 3;
  natural v_idx, col_idx, row_idx;
  real v,f;
  random_state r;
  init_random_time(&r);  

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  mat4_add(&mat[a], &mat[b], &mat[b]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] + ((real *)&col[3 - col_idx])[row_idx];
      f = mat[b].ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_sub() {
  mat4 mat[2];
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3;
  natural v_idx, col_idx, row_idx;
  real v,f;
  random_state r;
  init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  mat4_sub(&mat[a], &mat[b], &mat[b]);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] - ((real *)&col[3 - col_idx])[row_idx];
      f = mat[b].ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_mul() {
  mat4 mat[2];
  vec4 col[4];
  natural v_idx, a = 0, b = 1, c = 2, d = 3;
  real expected, found;
  random_state r;
  
  init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  init_mat4_cols(&mat[b], &col[d], &col[c], &col[b], &col[a]);

  mat4_mul(&mat[a], &mat[b], &mat[b]);

  expected = col[a].x * col[d].x + col[b].x * col[d].y + col[c].x * col[d].z + col[d].x * col[d].w;
  found = mat[b].ary[0][0];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[d].x + col[b].y * col[d].y + col[c].y * col[d].z + col[d].y * col[d].w;
  found = mat[b].ary[0][1];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[d].x + col[b].z * col[d].y + col[c].z * col[d].z + col[d].z * col[d].w;
  found = mat[b].ary[0][2];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[d].x + col[b].w * col[d].y + col[c].w * col[d].z + col[d].w * col[d].w;
  found = mat[b].ary[0][3];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[c].x + col[b].x * col[c].y + col[c].x * col[c].z + col[d].x * col[c].w;
  found = mat[b].ary[1][0];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[c].x + col[b].y * col[c].y + col[c].y * col[c].z + col[d].y * col[c].w;
  found = mat[b].ary[1][1];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[c].x + col[b].z * col[c].y + col[c].z * col[c].z + col[d].z * col[c].w;
  found = mat[b].ary[1][2];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[c].x + col[b].w * col[c].y + col[c].w * col[c].z + col[d].w * col[c].w;
  found = mat[b].ary[1][3];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[b].x + col[b].x * col[b].y + col[c].x * col[b].z + col[d].x * col[b].w;
  found = mat[b].ary[2][0];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[b].x + col[b].y * col[b].y + col[c].y * col[b].z + col[d].y * col[b].w;
  found = mat[b].ary[2][1];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[b].x + col[b].z * col[b].y + col[c].z * col[b].z + col[d].z * col[b].w;
  found = mat[b].ary[2][2];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[b].x + col[b].w * col[b].y + col[c].w * col[b].z + col[d].w * col[b].w;
  found = mat[b].ary[2][3];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].x * col[a].x + col[b].x * col[a].y + col[c].x * col[a].z + col[d].x * col[a].w;
  found = mat[b].ary[3][0];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].y * col[a].x + col[b].y * col[a].y + col[c].y * col[a].z + col[d].y * col[a].w;
  found = mat[b].ary[3][1];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].z * col[a].x + col[b].z * col[a].y + col[c].z * col[a].z + col[d].z * col[a].w;
  found = mat[b].ary[3][2];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  expected = col[a].w * col[a].x + col[b].w * col[a].y + col[c].w * col[a].z + col[d].w * col[a].w;
  found = mat[b].ary[3][3];

  if (!check(approximately_equal(expected, found, M_EPSILON),
    "Expected %f to be %f", expected, found))
    return false;

  mat4_mul(&mat[a], &mat4_identity, &mat[b]);

  if (!check(mat4_approximately_equal(&mat[a], &mat[b], M_EPSILON),
    "Expected identity to change nothing"))
    return false;

  return true;
}

bool test_mat4_mul_vec4() {
  natural a = 2, b = 3, c = 4, d = 5, i;
  vec4 vec[6];
  mat4 mat;
  real expected, found;
  random_state r;
  init_random_time(&r);
  
  for (i = 0; i < 6; ++i) {
    init_vec4(&vec[i],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  mat4_mul_vec4(&mat4_identity, &vec[0], &vec[1]);

  if (!check(vec4_approximately_equal(&vec[0], &vec[1], M_EPSILON),
    "Expected vector/identity multiplication to work"))
    return false;

  init_mat4_cols(&mat, &vec[a], &vec[b], &vec[c], &vec[d]);
  mat4_mul_vec4(&mat, &vec[0], &vec[1]);

  expected = vec[a].x * vec[0].x + vec[b].x * vec[0].y + vec[c].x * vec[0].z + vec[d].x * vec[0].w;
  found = vec[1].x;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].y * vec[0].x + vec[b].y * vec[0].y + vec[c].y * vec[0].z + vec[d].y * vec[0].w;
  found = vec[1].y;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].z * vec[0].x + vec[b].z * vec[0].y + vec[c].z * vec[0].z + vec[d].z * vec[0].w;
  found = vec[1].z;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = vec[a].w * vec[0].x + vec[b].w * vec[0].y + vec[c].w * vec[0].z + vec[d].w * vec[0].w;
  found = vec[1].w;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  return true;
}

bool test_mat4_mul_vec3() {
  vec4 mat_vec[4];
  vec3 vec, vec_out;
  mat4 mat;
  natural i;
  real expected, found;
  random_state r;
  init_random_time(&r);
  
  for (i = 0; i < 4; ++i) {
    init_vec4(&mat_vec[i],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_vec3(&vec,
    random_real_range(&r, MIN_REAL, MAX_REAL),
    random_real_range(&r, MIN_REAL, MAX_REAL),
    random_real_range(&r, MIN_REAL, MAX_REAL));

  mat4_mul_vec3(&mat4_identity, &vec, &vec_out);

  if (!check(vec3_approximately_equal(&vec, &vec_out, M_EPSILON),
    "Expected vector/identity multiplication to work"))
    return false;

  init_mat4_cols(&mat, &mat_vec[0], &mat_vec[1], &mat_vec[2], &mat_vec[3]);
  mat4_mul_vec3(&mat, &vec, &vec_out);

  expected = mat.ary[0][0] * vec.x + mat.ary[1][0] * vec.y + mat.ary[2][0] * vec.z + mat.ary[3][0] * vec.w;
  found = vec_out.x;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][1] * vec.x + mat.ary[1][1] * vec.y + mat.ary[2][1] * vec.z + mat.ary[3][1] * vec.w;
  found = vec_out.y;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][2] * vec.x + mat.ary[1][2] * vec.y + mat.ary[2][2] * vec.z + mat.ary[3][2] * vec.w;
  found = vec_out.z;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  if (!check(vec_out.w == 0, "Expected %f to be %f", vec_out.w, 0.0))
    return false;

  return true;
}

bool test_mat4_mul_point3() {
  vec4 mat_vec[4];
  point3 point, point_out;
  mat4 mat;
  natural i;
  real expected, found;
  random_state r;
  init_random_time(&r);
  
  for (i = 0; i < 4; ++i) {
    init_vec4(&mat_vec[i],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_point3(&point,
    random_real_range(&r, MIN_REAL, MAX_REAL),
    random_real_range(&r, MIN_REAL, MAX_REAL),
    random_real_range(&r, MIN_REAL, MAX_REAL));

  mat4_mul_point3(&mat4_identity, &point, &point_out);

  if (!check(point3_approximately_equal(&point, &point_out, M_EPSILON),
    "Expected point/identity multiplication to work"))
    return false;

  init_mat4_cols(&mat, &mat_vec[0], &mat_vec[1], &mat_vec[2], &mat_vec[3]);
  mat4_mul_point3(&mat, &point, &point_out);

  expected = mat.ary[0][0] * point.x + mat.ary[1][0] * point.y + mat.ary[2][0] * point.z + mat.ary[3][0] * point.w;
  found = point_out.x;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][1] * point.x + mat.ary[1][1] * point.y + mat.ary[2][1] * point.z + mat.ary[3][1] * point.w;
  found = point_out.y;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  expected = mat.ary[0][2] * point.x + mat.ary[1][2] * point.y + mat.ary[2][2] * point.z + mat.ary[3][2] * point.w;
  found = point_out.z;

  if (!check(approximately_equal(expected, found, M_EPSILON), "Expected %f to be %f", expected, found))
    return false;

  if (!check(point_out.w == 1, "Expected %f to be %f", point_out.w, 1.0))
    return false;

  return true;
}

bool test_mat4_transpose() {
  mat4 mat;
  vec3 col[4];
  natural v_idx, a = 0, b = 1, c = 2, d = 3, col_idx, row_idx;
  real v,f;
  random_state r;
  init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  mat4_transpose(&mat, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[row_idx])[col_idx];
      f = mat.ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_mul_scalar() {
  mat4 mat;
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  real scalar, v, f;
  random_state r;
  init_random_time(&r);
  
  scalar = random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  mat4_mul_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] * scalar;
      f = mat.ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_div_scalar() {
  mat4 mat;
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  real scalar, v, f;
  random_state r;
  init_random_time(&r);
  
  scalar = random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  mat4_div_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] / scalar;
      f = mat.ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_add_scalar() {
  mat4 mat;
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  real v, f, scalar;
  random_state r;
  init_random_time(&r);
  
  scalar = random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  mat4_add_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] + scalar;
      f = mat.ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_sub_scalar() {
  mat4 mat;
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3, v_idx, col_idx, row_idx;
  real scalar, v, f;
  random_state r;
  init_random_time(&r);
  
  scalar = random_real_range(&r, MIN_REAL, MAX_REAL);

  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat, &col[a], &col[b], &col[c], &col[d]);

  mat4_sub_scalar(&mat, scalar, &mat);

  for (col_idx = 0; col_idx < 4; ++col_idx) {
    for (row_idx = 0; row_idx < 4; ++row_idx) {
      v = ((real *)&col[col_idx])[row_idx] - scalar;
      f = mat.ary[col_idx][row_idx];
      if (!check(approximately_equal(v, f, M_EPSILON),
        "Expected (%lu, %lu) to be %f, was %f",
        col_idx, row_idx, v, f))
        return false;
    }
  }

  return true;
}

bool test_mat4_translate() {
  mat4 mat;
  point3 point[2];
  vec3 vec[2];
  real a,b,c;
  random_state r;
  init_random_time(&r);
  
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec[0], a, b, c);
  init_point3(&point[0], a, b, c);

  init_mat4_translate(&mat, a, b, c);
  mat4_mul_point3(&mat, &point[0], &point[1]);

  if (!check(approximately_equal(point[1].x, a + a, M_EPSILON) &&
    approximately_equal(point[1].y, b + b, M_EPSILON) &&
    approximately_equal(point[1].z, c + c, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    point[1].x, point[1].y, point[1].z, a + a, b + b, c + c))
    return false;

  mat4_mul_vec3(&mat, &vec[0], &vec[1]);

  if (!check(vec3_approximately_equal(&vec[0], &vec[1], M_EPSILON),
    "Expected no change to vector"))
    return false;

  return true;
}

bool test_mat4_rotate() {
  random_state r;
  real angle, a, b, c;
  vec3 vec[3];
  mat4 mat[2];
  
  init_random_time(&r);
  
  angle = random_radian(&r);
  a = random_real_range(&r, MIN_REAL, MAX_REAL);
  b = random_real_range(&r, MIN_REAL, MAX_REAL);
  c = random_real_range(&r, MIN_REAL, MAX_REAL);

  init_vec3(&vec[0], a, b, c);

  init_mat4_rotate(&mat[0], &vec3_right, angle);
  init_mat4_rotate_x(&mat[1], angle);

  mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!check(vec3_approximately_equal(&vec[1], &vec[2], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  init_mat4_rotate(&mat[0], &vec3_up, angle);
  init_mat4_rotate_y(&mat[1], angle);

  mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!check(vec3_approximately_equal(&vec[1], &vec[2], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  init_mat4_rotate(&mat[0], &vec3_forward, angle);
  init_mat4_rotate_z(&mat[1], angle);

  mat4_mul_vec3(&mat[0], &vec[0], &vec[1]);
  mat4_mul_vec3(&mat[1], &vec[0], &vec[2]);

  if (!check(vec3_approximately_equal(&vec[1], &vec[2], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec[1].x, vec[1].y, vec[1].z, vec[2].x, vec[2].y, vec[2].z))
    return false;

  return true;
}

bool test_mat4_rotate_x() {
  mat4 mat;
  vec3 vec;
  real angle;
#if USE_LEFT_HANDED
  vec3 expected[4];
  expected[0] = vec3_forward;
  expected[1] = vec3_down;
  expected[2] = vec3_backward;
  expected[3] = vec3_up;
#else
  vec3 expected[4];
  expected[0] = vec3_backward;
  expected[1] = vec3_down;
  expected[2] = vec3_forward;
  expected[3] = vec3_up;
#endif

  vec = expected[3];
  
  angle = M_PI * 0.5f;

  init_mat4_rotate_x(&mat, angle);
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[0], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[0].x, expected[0].y, expected[0].z))
    return false;

  vec = expected[0];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[1], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[1].x, expected[1].y, expected[1].z))
    return false;

  vec = expected[1];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[2], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[2].x, expected[2].y, expected[2].z))
    return false;

  vec = expected[2];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[3], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[3].x, expected[3].y, expected[3].z))
    return false;

  return true;
}

bool test_mat4_rotate_y() {
  mat4 mat;
  vec3 vec;
  real angle;
#if USE_LEFT_HANDED
  vec3 expected[4];
  expected[0] = vec3_backward;
  expected[1] = vec3_left;
  expected[2] = vec3_forward;
  expected[3] = vec3_right;
#else
  vec3 expected[4];
  expected[0] = vec3_forward;
  expected[1] = vec3_left;
  expected[2] = vec3_backward;
  expected[3] = vec3_right;
#endif
  
  vec = expected[3];

  angle = M_PI * 0.5f;

  init_mat4_rotate_y(&mat, angle);
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[0], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[0].x, expected[0].y, expected[0].z))
    return false;

  vec = expected[0];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[1], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[1].x, expected[1].y, expected[1].z))
    return false;

  vec = expected[1];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[2], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[2].x, expected[2].y, expected[2].z))
    return false;

  vec = expected[2];
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected[3], M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected[3].x, expected[3].y, expected[3].z))
    return false;

  return true;
}

bool test_mat4_rotate_z() {
  mat4 mat;
  vec3 vec = vec3_up;
  vec3 expected = vec3_left;

  real angle = M_PI * 0.5f;

  init_mat4_rotate_z(&mat, angle);
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = vec3_left;
  expected = vec3_down;
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = vec3_down;
  expected = vec3_right;
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  vec = vec3_right;
  expected = vec3_up;
  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(vec3_approximately_equal(&vec, &expected, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected.x, expected.y, expected.z))
    return false;

  return true;
}

bool test_mat4_scale() {
  point3 point = point3_one;
  point3 expected_point;
  vec3 vec = vec3_one;
  vec3 expected_vec;
  mat4 mat;

  point3_mul_scalar(&point, 2, &expected_point);

  init_mat4_scale(&mat, 2, 2, 2);

  mat4_mul_point3(&mat, &point, &point);

  if (!check(point3_approximately_equal(&point, &expected_point, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    point.x, point.y, point.z, expected_point.x, expected_point.y, expected_point.z))
    return false;

  vec3_mul_scalar(&vec, 2, &expected_vec);

  mat4_mul_vec3(&mat, &vec, &vec);

  if (!check(point3_approximately_equal(&vec, &expected_vec, M_EPSILON),
    "Expected (%f, %f, %f) to be (%f, %f, %f)",
    vec.x, vec.y, vec.z, expected_vec.x, expected_vec.y, expected_vec.z))
    return false;

  return true;
}

bool test_mat4_approximately_equal() {
  mat4 mat[2];
  vec3 col[4];
  natural a = 0, b = 1, c = 2, d = 3, v_idx;
  random_state r;
  init_random_time(&r);
  
  for (v_idx = 0; v_idx < 4; ++v_idx) {
    init_vec4(&col[v_idx],
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL),
      random_real_range(&r, MIN_REAL, MAX_REAL));
  }

  init_mat4_cols(&mat[a], &col[a], &col[b], &col[c], &col[d]);
  init_mat4_cols(&mat[b], &col[a], &col[b], &col[c], &col[d]);

  return check(mat4_approximately_equal(&mat[a], &mat[b], M_EPSILON),
    "Expected matrices to be equal");
}

#endif
