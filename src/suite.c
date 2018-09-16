#include <stdio.h>

#define DL_IMPLEMENTATION 1
#include "dl_all.h"

#if DL_IS_ATLEAST_C99

/*******************************************************************************
 math
*******************************************************************************/

dl_begin_test_suite(math)
dl_declare_test(test_approximately_equal)
dl_declare_test(test_ceil_to_integer)
dl_declare_test(test_clamp)
dl_declare_test(test_factorial)
dl_declare_test(test_floor_to_integer)
dl_declare_test(test_max)
dl_declare_test(test_min)
dl_declare_test(test_rationalize)
dl_declare_test(test_round_to_integer)
dl_declare_test(test_radian_to_degree)
dl_declare_test(test_degree_to_radian)
dl_declare_test(test_random)
dl_declare_test(test_random_degree)
dl_declare_test(test_random_radian)
dl_declare_test(test_random_range)
dl_declare_test(test_random_real)
dl_declare_test(test_random_real_range)
dl_declare_test(test_init_mat4)
dl_declare_test(test_init_point2)
dl_declare_test(test_init_point3)
dl_declare_test(test_init_vec2)
dl_declare_test(test_init_vec3)
dl_declare_test(test_init_vec4)
dl_declare_test(test_vec2_add)
dl_declare_test(test_vec2_add_scalar)
dl_declare_test(test_vec2_approximately_equal)
dl_declare_test(test_vec2_div_scalar)
dl_declare_test(test_vec2_dot)
dl_declare_test(test_vec2_magnitude)
dl_declare_test(test_vec2_mul_scalar)
dl_declare_test(test_vec2_negate)
dl_declare_test(test_vec2_normalize)
dl_declare_test(test_vec2_reflect)
dl_declare_test(test_vec2_refract)
dl_declare_test(test_vec2_square_magnitude)
dl_declare_test(test_vec2_sub)
dl_declare_test(test_vec2_sub_scalar)
dl_declare_test(test_vec3_add)
dl_declare_test(test_vec3_add_scalar)
dl_declare_test(test_vec3_approximately_equal)
dl_declare_test(test_vec3_cross)
dl_declare_test(test_vec3_div_scalar)
dl_declare_test(test_vec3_dot)
dl_declare_test(test_vec3_magnitude)
dl_declare_test(test_vec3_mul_scalar)
dl_declare_test(test_vec3_negate)
dl_declare_test(test_vec3_normalize)
dl_declare_test(test_vec3_reflect)
dl_declare_test(test_vec3_refract)
dl_declare_test(test_vec3_rotate)
dl_declare_test(test_vec3_square_magnitude)
dl_declare_test(test_vec3_sub)
dl_declare_test(test_vec3_sub_scalar)
dl_declare_test(test_vec4_add)
dl_declare_test(test_vec4_add_scalar)
dl_declare_test(test_vec4_approximately_equal)
dl_declare_test(test_vec4_div_scalar)
dl_declare_test(test_vec4_dot)
dl_declare_test(test_vec4_magnitude)
dl_declare_test(test_vec4_mul_scalar)
dl_declare_test(test_vec4_negate)
dl_declare_test(test_vec4_normalize)
dl_declare_test(test_vec4_square_magnitude)
dl_declare_test(test_vec4_sub)
dl_declare_test(test_vec4_sub_scalar)
dl_declare_test(test_point2_add)
dl_declare_test(test_point2_add_scalar)
dl_declare_test(test_point2_approximately_equal)
dl_declare_test(test_point2_div_scalar)
dl_declare_test(test_point2_line_orientation)
dl_declare_test(test_point2_mul_scalar)
dl_declare_test(test_point2_negate)
dl_declare_test(test_point2_sub)
dl_declare_test(test_point2_sub_scalar)
dl_declare_test(test_point3_add)
dl_declare_test(test_point3_add_scalar)
dl_declare_test(test_point3_approximately_equal)
dl_declare_test(test_point3_div_scalar)
dl_declare_test(test_point3_mul_scalar)
dl_declare_test(test_point3_negate)
dl_declare_test(test_point3_rotate)
dl_declare_test(test_point3_sub)
dl_declare_test(test_point3_sub_scalar)
dl_declare_test(test_mat4_add)
dl_declare_test(test_mat4_add_scalar)
dl_declare_test(test_mat4_approximately_equal)
dl_declare_test(test_mat4_div_scalar)
dl_declare_test(test_mat4_mul)
dl_declare_test(test_mat4_mul_point3)
dl_declare_test(test_mat4_mul_scalar)
dl_declare_test(test_mat4_mul_vec3)
dl_declare_test(test_mat4_mul_vec4)
dl_declare_test(test_mat4_rotate)
dl_declare_test(test_mat4_rotate_x)
dl_declare_test(test_mat4_rotate_y)
dl_declare_test(test_mat4_rotate_z)
dl_declare_test(test_mat4_scale)
dl_declare_test(test_mat4_sub)
dl_declare_test(test_mat4_sub_scalar)
dl_declare_test(test_mat4_translate)
dl_declare_test(test_mat4_transpose)
dl_end_test_suite

/*******************************************************************************
 test
*******************************************************************************/

dl_begin_test_suite(tester)
dl_declare_test(test_test_count_tests)
dl_declare_test(test_test_run_tests)
dl_end_test_suite

/*******************************************************************************
 dl_collections
*******************************************************************************/

dl_begin_test_suite(dl_collections)
dl_declare_test(test_memory_swap)
dl_declare_test(test_init_linked_list_fat)
dl_declare_test(test_linked_list_add_fat)
dl_declare_test(test_init_linked_list)
dl_declare_test(test_linked_list_capacity)
dl_declare_test(test_linked_list_length)
dl_declare_test(test_linked_list_add)
dl_declare_test(test_linked_list_copy)
dl_declare_test(test_linked_list_copy_array)
dl_declare_test(test_linked_list_grow)
dl_declare_test(test_linked_list_shrink)
dl_declare_test(test_linked_list_resize)
dl_declare_test(test_linked_list_get)
dl_declare_test(test_linked_list_set)
dl_declare_test(test_linked_list_ref)
dl_declare_test(test_linked_list_index)
dl_declare_test(test_linked_list_destroy_range)
dl_declare_test(test_linked_list_swap)
dl_declare_test(test_init_destroy_vector)
dl_declare_test(test_init_vector_array)
dl_declare_test(test_vector_copy)
dl_declare_test(test_vector_get)
dl_declare_test(test_vector_grow)
dl_declare_test(test_vector_ref)
dl_declare_test(test_vector_resize)
dl_declare_test(test_vector_set)
dl_declare_test(test_vector_shrink)
dl_declare_test(test_collection_all)
dl_declare_test(test_collection_any)
dl_declare_test(test_collection_clear)
dl_declare_test(test_collection_count)
dl_declare_test(test_collection_destroy_all)
dl_declare_test(test_collection_destroy_at)
dl_declare_test(test_collection_destroy_first)
dl_declare_test(test_collection_destroy_last)
dl_declare_test(test_collection_drop)
dl_declare_test(test_collection_find)
dl_declare_test(test_collection_find_all)
dl_declare_test(test_collection_find_last)
dl_declare_test(test_collection_foldl)
dl_declare_test(test_collection_foldr)
dl_declare_test(test_collection_index_of)
dl_declare_test(test_collection_is_empty)
dl_declare_test(test_collection_map)
dl_declare_test(test_collection_next)
dl_declare_test(test_collection_peek)
dl_declare_test(test_collection_pop)
dl_declare_test(test_collection_prev)
dl_declare_test(test_collection_push)
dl_declare_test(test_collection_ref)
dl_declare_test(test_collection_ref_array)
dl_declare_test(test_collection_remove_all)
dl_declare_test(test_collection_remove_at)
dl_declare_test(test_collection_remove_first)
dl_declare_test(test_collection_remove_last)
dl_declare_test(test_collection_take)
dl_declare_test(test_collection_pop_destroy)
dl_declare_test(test_collection_zip)
dl_declare_test(test_collection_copy)
dl_declare_test(test_init_collection)
dl_declare_test(test_collection_fifo)
dl_declare_test(test_collection_remove_range)
dl_declare_test(test_collection_destroy_range)
dl_declare_test(test_collection_insert)
dl_declare_test(test_collection_fuzz)
/*
dl_declare_test(test_collection_pop_forget)
dl_declare_test(test_collection_pop_destroy)
dl_declare_test(test_linked_list_remove)
dl_declare_test(test_linked_list_destroy)
dl_declare_test(test_collection_push_index)
*/
dl_end_test_suite


int main(int argc, char **argv) {
  dl_run_test_suite(tester);
  dl_run_test_suite(math);
  dl_run_test_suite(dl_collections);
  
  return 0;
}

#else

int main(int argc, char **argv) {
  dl_unused(argc);
  dl_unused(argv);
  return 0;
}

#endif