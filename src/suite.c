#include <stdio.h>

#define DL_IMPLEMENTATION 1
#include "dl.h"

#if DL_USE_TEST

/*******************************************************************************
 math
*******************************************************************************/

#if DL_USE_MATH
BEGIN_TEST_SUITE(math)
DECLARE_TEST(test_approximately_equal)
DECLARE_TEST(test_ceil_to_integer)
DECLARE_TEST(test_clamp)
DECLARE_TEST(test_factorial)
DECLARE_TEST(test_floor_to_integer)
DECLARE_TEST(test_max)
DECLARE_TEST(test_min)
DECLARE_TEST(test_rationalize)
DECLARE_TEST(test_round_to_integer)
DECLARE_TEST(test_radian_to_degree)
DECLARE_TEST(test_degree_to_radian)
DECLARE_TEST(test_random)
DECLARE_TEST(test_random_degree)
DECLARE_TEST(test_random_radian)
DECLARE_TEST(test_random_range)
DECLARE_TEST(test_random_real)
DECLARE_TEST(test_random_real_range)
DECLARE_TEST(test_init_mat4)
DECLARE_TEST(test_init_point2)
DECLARE_TEST(test_init_point3)
DECLARE_TEST(test_init_vec2)
DECLARE_TEST(test_init_vec3)
DECLARE_TEST(test_init_vec4)
DECLARE_TEST(test_vec2_add)
DECLARE_TEST(test_vec2_add_scalar)
DECLARE_TEST(test_vec2_approximately_equal)
DECLARE_TEST(test_vec2_div_scalar)
DECLARE_TEST(test_vec2_dot)
DECLARE_TEST(test_vec2_magnitude)
DECLARE_TEST(test_vec2_mul_scalar)
DECLARE_TEST(test_vec2_negate)
DECLARE_TEST(test_vec2_normalize)
DECLARE_TEST(test_vec2_reflect)
DECLARE_TEST(test_vec2_refract)
DECLARE_TEST(test_vec2_square_magnitude)
DECLARE_TEST(test_vec2_sub)
DECLARE_TEST(test_vec2_sub_scalar)
DECLARE_TEST(test_vec3_add)
DECLARE_TEST(test_vec3_add_scalar)
DECLARE_TEST(test_vec3_approximately_equal)
DECLARE_TEST(test_vec3_cross)
DECLARE_TEST(test_vec3_div_scalar)
DECLARE_TEST(test_vec3_dot)
DECLARE_TEST(test_vec3_magnitude)
DECLARE_TEST(test_vec3_mul_scalar)
DECLARE_TEST(test_vec3_negate)
DECLARE_TEST(test_vec3_normalize)
DECLARE_TEST(test_vec3_reflect)
DECLARE_TEST(test_vec3_refract)
DECLARE_TEST(test_vec3_rotate)
DECLARE_TEST(test_vec3_square_magnitude)
DECLARE_TEST(test_vec3_sub)
DECLARE_TEST(test_vec3_sub_scalar)
DECLARE_TEST(test_vec4_add)
DECLARE_TEST(test_vec4_add_scalar)
DECLARE_TEST(test_vec4_approximately_equal)
DECLARE_TEST(test_vec4_div_scalar)
DECLARE_TEST(test_vec4_dot)
DECLARE_TEST(test_vec4_magnitude)
DECLARE_TEST(test_vec4_mul_scalar)
DECLARE_TEST(test_vec4_negate)
DECLARE_TEST(test_vec4_normalize)
DECLARE_TEST(test_vec4_square_magnitude)
DECLARE_TEST(test_vec4_sub)
DECLARE_TEST(test_vec4_sub_scalar)
DECLARE_TEST(test_point2_add)
DECLARE_TEST(test_point2_add_scalar)
DECLARE_TEST(test_point2_approximately_equal)
DECLARE_TEST(test_point2_div_scalar)
DECLARE_TEST(test_point2_line_orientation)
DECLARE_TEST(test_point2_mul_scalar)
DECLARE_TEST(test_point2_negate)
DECLARE_TEST(test_point2_sub)
DECLARE_TEST(test_point2_sub_scalar)
DECLARE_TEST(test_point3_add)
DECLARE_TEST(test_point3_add_scalar)
DECLARE_TEST(test_point3_approximately_equal)
DECLARE_TEST(test_point3_div_scalar)
DECLARE_TEST(test_point3_mul_scalar)
DECLARE_TEST(test_point3_negate)
DECLARE_TEST(test_point3_rotate)
DECLARE_TEST(test_point3_sub)
DECLARE_TEST(test_point3_sub_scalar)
DECLARE_TEST(test_mat4_add)
DECLARE_TEST(test_mat4_add_scalar)
DECLARE_TEST(test_mat4_approximately_equal)
DECLARE_TEST(test_mat4_div_scalar)
DECLARE_TEST(test_mat4_mul)
DECLARE_TEST(test_mat4_mul_point3)
DECLARE_TEST(test_mat4_mul_scalar)
DECLARE_TEST(test_mat4_mul_vec3)
DECLARE_TEST(test_mat4_mul_vec4)
DECLARE_TEST(test_mat4_rotate)
DECLARE_TEST(test_mat4_rotate_x)
DECLARE_TEST(test_mat4_rotate_y)
DECLARE_TEST(test_mat4_rotate_z)
DECLARE_TEST(test_mat4_scale)
DECLARE_TEST(test_mat4_sub)
DECLARE_TEST(test_mat4_sub_scalar)
DECLARE_TEST(test_mat4_translate)
DECLARE_TEST(test_mat4_transpose)
END_TEST_SUITE
#endif

/*******************************************************************************
 test
*******************************************************************************/

BEGIN_TEST_SUITE(tester)
DECLARE_TEST(test_test_count_tests)
DECLARE_TEST(test_test_run_tests)
END_TEST_SUITE

/*******************************************************************************
 collections
*******************************************************************************/

#if DL_USE_CONTAINERS

BEGIN_TEST_SUITE(collections)
DECLARE_TEST(test_memory_swap)
DECLARE_TEST(test_init_linked_list)
DECLARE_TEST(test_linked_list_capacity)
DECLARE_TEST(test_linked_list_length)
DECLARE_TEST(test_linked_list_add)
DECLARE_TEST(test_linked_list_copy)
DECLARE_TEST(test_linked_list_copy_array)
DECLARE_TEST(test_linked_list_grow)
DECLARE_TEST(test_linked_list_shrink)
DECLARE_TEST(test_linked_list_resize)
DECLARE_TEST(test_linked_list_get)
DECLARE_TEST(test_linked_list_set)
DECLARE_TEST(test_linked_list_ref)
DECLARE_TEST(test_linked_list_index)
DECLARE_TEST(test_linked_list_destroy_range)
DECLARE_TEST(test_linked_list_swap)
DECLARE_TEST(test_init_destroy_vector)
DECLARE_TEST(test_init_vector_array)
DECLARE_TEST(test_vector_copy)
DECLARE_TEST(test_vector_get)
DECLARE_TEST(test_vector_grow)
DECLARE_TEST(test_vector_ref)
DECLARE_TEST(test_vector_resize)
DECLARE_TEST(test_vector_set)
DECLARE_TEST(test_vector_shrink)
DECLARE_TEST(test_collection_all)
DECLARE_TEST(test_collection_any)
DECLARE_TEST(test_collection_clear)
DECLARE_TEST(test_collection_count)
DECLARE_TEST(test_collection_destroy_all)
DECLARE_TEST(test_collection_destroy_at)
DECLARE_TEST(test_collection_destroy_first)
DECLARE_TEST(test_collection_destroy_last)
DECLARE_TEST(test_collection_drop)
DECLARE_TEST(test_collection_find)
DECLARE_TEST(test_collection_find_all)
DECLARE_TEST(test_collection_find_last)
DECLARE_TEST(test_collection_foldl)
DECLARE_TEST(test_collection_foldr)
DECLARE_TEST(test_collection_index_of)
DECLARE_TEST(test_collection_is_empty)
DECLARE_TEST(test_collection_map)
DECLARE_TEST(test_collection_next)
DECLARE_TEST(test_collection_peek)
DECLARE_TEST(test_collection_pop)
DECLARE_TEST(test_collection_prev)
DECLARE_TEST(test_collection_push)
DECLARE_TEST(test_collection_ref)
DECLARE_TEST(test_collection_ref_array)
DECLARE_TEST(test_collection_remove_all)
DECLARE_TEST(test_collection_remove_at)
DECLARE_TEST(test_collection_remove_first)
DECLARE_TEST(test_collection_remove_last)
DECLARE_TEST(test_collection_take)
DECLARE_TEST(test_collection_pop_destroy)
DECLARE_TEST(test_collection_zip)
DECLARE_TEST(test_collection_copy)
DECLARE_TEST(test_init_collection)
DECLARE_TEST(test_collection_fifo)
DECLARE_TEST(test_collection_remove_range)
DECLARE_TEST(test_collection_destroy_range)
DECLARE_TEST(test_collection_insert)
/*
DECLARE_TEST(test_collection_pop_forget)
DECLARE_TEST(test_collection_pop_destroy)
DECLARE_TEST(test_linked_list_remove)
DECLARE_TEST(test_linked_list_destroy)
DECLARE_TEST(test_collection_push_index)
*/
END_TEST_SUITE

#endif

#endif

int main(int argc, char **argv) {
#if DL_USE_TEST
  RUN_TEST_SUITE(tester);
  RUN_TEST_SUITE(math);
  RUN_TEST_SUITE(collections);
#endif
  
  return 0;
}
