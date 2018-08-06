#include <stdio.h>

#define DL_IMPLEMENTATION 1
#include "dl.h"

#if DL_USE_TEST

/*******************************************************************************
 math
*******************************************************************************/

#if DL_USE_MATH
DL_BEGIN_TEST_SUITE(math)
DL_DECLARE_TEST(test_approximately_equal)
DL_DECLARE_TEST(test_ceil_to_integer)
DL_DECLARE_TEST(test_clamp)
DL_DECLARE_TEST(test_factorial)
DL_DECLARE_TEST(test_floor_to_integer)
DL_DECLARE_TEST(test_max)
DL_DECLARE_TEST(test_min)
DL_DECLARE_TEST(test_rationalize)
DL_DECLARE_TEST(test_round_to_integer)
DL_DECLARE_TEST(test_radian_to_degree)
DL_DECLARE_TEST(test_degree_to_radian)
DL_DECLARE_TEST(test_random)
DL_DECLARE_TEST(test_random_degree)
DL_DECLARE_TEST(test_random_radian)
DL_DECLARE_TEST(test_random_range)
DL_DECLARE_TEST(test_random_real)
DL_DECLARE_TEST(test_random_real_range)
DL_DECLARE_TEST(test_init_mat4)
DL_DECLARE_TEST(test_init_point2)
DL_DECLARE_TEST(test_init_point3)
DL_DECLARE_TEST(test_init_vec2)
DL_DECLARE_TEST(test_init_vec3)
DL_DECLARE_TEST(test_init_vec4)
DL_DECLARE_TEST(test_vec2_add)
DL_DECLARE_TEST(test_vec2_add_scalar)
DL_DECLARE_TEST(test_vec2_approximately_equal)
DL_DECLARE_TEST(test_vec2_div_scalar)
DL_DECLARE_TEST(test_vec2_dot)
DL_DECLARE_TEST(test_vec2_magnitude)
DL_DECLARE_TEST(test_vec2_mul_scalar)
DL_DECLARE_TEST(test_vec2_negate)
DL_DECLARE_TEST(test_vec2_normalize)
DL_DECLARE_TEST(test_vec2_reflect)
DL_DECLARE_TEST(test_vec2_refract)
DL_DECLARE_TEST(test_vec2_square_magnitude)
DL_DECLARE_TEST(test_vec2_sub)
DL_DECLARE_TEST(test_vec2_sub_scalar)
DL_DECLARE_TEST(test_vec3_add)
DL_DECLARE_TEST(test_vec3_add_scalar)
DL_DECLARE_TEST(test_vec3_approximately_equal)
DL_DECLARE_TEST(test_vec3_cross)
DL_DECLARE_TEST(test_vec3_div_scalar)
DL_DECLARE_TEST(test_vec3_dot)
DL_DECLARE_TEST(test_vec3_magnitude)
DL_DECLARE_TEST(test_vec3_mul_scalar)
DL_DECLARE_TEST(test_vec3_negate)
DL_DECLARE_TEST(test_vec3_normalize)
DL_DECLARE_TEST(test_vec3_reflect)
DL_DECLARE_TEST(test_vec3_refract)
DL_DECLARE_TEST(test_vec3_rotate)
DL_DECLARE_TEST(test_vec3_square_magnitude)
DL_DECLARE_TEST(test_vec3_sub)
DL_DECLARE_TEST(test_vec3_sub_scalar)
DL_DECLARE_TEST(test_vec4_add)
DL_DECLARE_TEST(test_vec4_add_scalar)
DL_DECLARE_TEST(test_vec4_approximately_equal)
DL_DECLARE_TEST(test_vec4_div_scalar)
DL_DECLARE_TEST(test_vec4_dot)
DL_DECLARE_TEST(test_vec4_magnitude)
DL_DECLARE_TEST(test_vec4_mul_scalar)
DL_DECLARE_TEST(test_vec4_negate)
DL_DECLARE_TEST(test_vec4_normalize)
DL_DECLARE_TEST(test_vec4_square_magnitude)
DL_DECLARE_TEST(test_vec4_sub)
DL_DECLARE_TEST(test_vec4_sub_scalar)
DL_DECLARE_TEST(test_point2_add)
DL_DECLARE_TEST(test_point2_add_scalar)
DL_DECLARE_TEST(test_point2_approximately_equal)
DL_DECLARE_TEST(test_point2_div_scalar)
DL_DECLARE_TEST(test_point2_line_orientation)
DL_DECLARE_TEST(test_point2_mul_scalar)
DL_DECLARE_TEST(test_point2_negate)
DL_DECLARE_TEST(test_point2_sub)
DL_DECLARE_TEST(test_point2_sub_scalar)
DL_DECLARE_TEST(test_point3_add)
DL_DECLARE_TEST(test_point3_add_scalar)
DL_DECLARE_TEST(test_point3_approximately_equal)
DL_DECLARE_TEST(test_point3_div_scalar)
DL_DECLARE_TEST(test_point3_mul_scalar)
DL_DECLARE_TEST(test_point3_negate)
DL_DECLARE_TEST(test_point3_rotate)
DL_DECLARE_TEST(test_point3_sub)
DL_DECLARE_TEST(test_point3_sub_scalar)
DL_DECLARE_TEST(test_mat4_add)
DL_DECLARE_TEST(test_mat4_add_scalar)
DL_DECLARE_TEST(test_mat4_approximately_equal)
DL_DECLARE_TEST(test_mat4_div_scalar)
DL_DECLARE_TEST(test_mat4_mul)
DL_DECLARE_TEST(test_mat4_mul_point3)
DL_DECLARE_TEST(test_mat4_mul_scalar)
DL_DECLARE_TEST(test_mat4_mul_vec3)
DL_DECLARE_TEST(test_mat4_mul_vec4)
DL_DECLARE_TEST(test_mat4_rotate)
DL_DECLARE_TEST(test_mat4_rotate_x)
DL_DECLARE_TEST(test_mat4_rotate_y)
DL_DECLARE_TEST(test_mat4_rotate_z)
DL_DECLARE_TEST(test_mat4_scale)
DL_DECLARE_TEST(test_mat4_sub)
DL_DECLARE_TEST(test_mat4_sub_scalar)
DL_DECLARE_TEST(test_mat4_translate)
DL_DECLARE_TEST(test_mat4_transpose)
DL_END_TEST_SUITE
#endif

/*******************************************************************************
 test
*******************************************************************************/

DL_BEGIN_TEST_SUITE(tester)
DL_DECLARE_TEST(test_test_count_tests)
DL_DECLARE_TEST(test_test_run_tests)
DL_END_TEST_SUITE

/*******************************************************************************
 dl_collections
*******************************************************************************/

#if DL_USE_CONTAINERS

DL_BEGIN_TEST_SUITE(dl_collections)
DL_DECLARE_TEST(test_memory_swap)
DL_DECLARE_TEST(test_init_linked_list_fat)
DL_DECLARE_TEST(test_linked_list_add_fat)
DL_DECLARE_TEST(test_init_linked_list)
DL_DECLARE_TEST(test_linked_list_capacity)
DL_DECLARE_TEST(test_linked_list_length)
DL_DECLARE_TEST(test_linked_list_add)
DL_DECLARE_TEST(test_linked_list_copy)
DL_DECLARE_TEST(test_linked_list_copy_array)
DL_DECLARE_TEST(test_linked_list_grow)
DL_DECLARE_TEST(test_linked_list_shrink)
DL_DECLARE_TEST(test_linked_list_resize)
DL_DECLARE_TEST(test_linked_list_get)
DL_DECLARE_TEST(test_linked_list_set)
DL_DECLARE_TEST(test_linked_list_ref)
DL_DECLARE_TEST(test_linked_list_index)
DL_DECLARE_TEST(test_linked_list_destroy_range)
DL_DECLARE_TEST(test_linked_list_swap)
DL_DECLARE_TEST(test_init_destroy_vector)
DL_DECLARE_TEST(test_init_vector_array)
DL_DECLARE_TEST(test_vector_copy)
DL_DECLARE_TEST(test_vector_get)
DL_DECLARE_TEST(test_vector_grow)
DL_DECLARE_TEST(test_vector_ref)
DL_DECLARE_TEST(test_vector_resize)
DL_DECLARE_TEST(test_vector_set)
DL_DECLARE_TEST(test_vector_shrink)
DL_DECLARE_TEST(test_collection_all)
DL_DECLARE_TEST(test_collection_any)
DL_DECLARE_TEST(test_collection_clear)
DL_DECLARE_TEST(test_collection_count)
DL_DECLARE_TEST(test_collection_destroy_all)
DL_DECLARE_TEST(test_collection_destroy_at)
DL_DECLARE_TEST(test_collection_destroy_first)
DL_DECLARE_TEST(test_collection_destroy_last)
DL_DECLARE_TEST(test_collection_drop)
DL_DECLARE_TEST(test_collection_find)
DL_DECLARE_TEST(test_collection_find_all)
DL_DECLARE_TEST(test_collection_find_last)
DL_DECLARE_TEST(test_collection_foldl)
DL_DECLARE_TEST(test_collection_foldr)
DL_DECLARE_TEST(test_collection_index_of)
DL_DECLARE_TEST(test_collection_is_empty)
DL_DECLARE_TEST(test_collection_map)
DL_DECLARE_TEST(test_collection_next)
DL_DECLARE_TEST(test_collection_peek)
DL_DECLARE_TEST(test_collection_pop)
DL_DECLARE_TEST(test_collection_prev)
DL_DECLARE_TEST(test_collection_push)
DL_DECLARE_TEST(test_collection_ref)
DL_DECLARE_TEST(test_collection_ref_array)
DL_DECLARE_TEST(test_collection_remove_all)
DL_DECLARE_TEST(test_collection_remove_at)
DL_DECLARE_TEST(test_collection_remove_first)
DL_DECLARE_TEST(test_collection_remove_last)
DL_DECLARE_TEST(test_collection_take)
DL_DECLARE_TEST(test_collection_pop_destroy)
DL_DECLARE_TEST(test_collection_zip)
DL_DECLARE_TEST(test_collection_copy)
DL_DECLARE_TEST(test_init_collection)
DL_DECLARE_TEST(test_collection_fifo)
DL_DECLARE_TEST(test_collection_remove_range)
DL_DECLARE_TEST(test_collection_destroy_range)
DL_DECLARE_TEST(test_collection_insert)
/*
DL_DECLARE_TEST(test_collection_pop_forget)
DL_DECLARE_TEST(test_collection_pop_destroy)
DL_DECLARE_TEST(test_linked_list_remove)
DL_DECLARE_TEST(test_linked_list_destroy)
DL_DECLARE_TEST(test_collection_push_index)
*/
DL_END_TEST_SUITE

#endif

#endif

int main(int argc, char **argv) {
#if DL_USE_TEST
  DL_RUN_TEST_SUITE(tester);
  DL_RUN_TEST_SUITE(math);
  DL_RUN_TEST_SUITE(dl_collections);
#endif
  
  return 0;
}
