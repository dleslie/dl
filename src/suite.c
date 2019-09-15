#include <stdio.h>

#define DL_IMPLEMENTATION 1
#include "dl.h"

/* clang-format off */

#if DL_IS_ATLEAST_C99


/*******************************************************************************
 math
*******************************************************************************/

dl_begin_test_suite(math);
dl_declare_test(test_approximately_equal);
dl_declare_test(test_ceil_to_integer);
dl_declare_test(test_clamp);
dl_declare_test(test_factorial);
dl_declare_test(test_floor_to_integer);
dl_declare_test(test_max);
dl_declare_test(test_min);
dl_declare_test(test_rationalize);
dl_declare_test(test_round_to_integer);
dl_declare_test(test_radian_to_degree);
dl_declare_test(test_degree_to_radian);
dl_declare_test(test_random);
dl_declare_test(test_random_degree);
dl_declare_test(test_random_radian);
dl_declare_test(test_random_range);
dl_declare_test(test_random_real);
dl_declare_test(test_random_real_range);
dl_declare_test(test_init_mat4);
dl_declare_test(test_init_point2);
dl_declare_test(test_init_point3);
dl_declare_test(test_init_vec2);
dl_declare_test(test_init_vec3);
dl_declare_test(test_init_vec4);
dl_declare_test(test_vec2_add);
dl_declare_test(test_vec2_add_scalar);
dl_declare_test(test_vec2_approximately_equal);
dl_declare_test(test_vec2_div_scalar);
dl_declare_test(test_vec2_dot);
dl_declare_test(test_vec2_magnitude);
dl_declare_test(test_vec2_mul_scalar);
dl_declare_test(test_vec2_negate);
dl_declare_test(test_vec2_normalize);
dl_declare_test(test_vec2_reflect);
dl_declare_test(test_vec2_refract);
dl_declare_test(test_vec2_square_magnitude);
dl_declare_test(test_vec2_sub);
dl_declare_test(test_vec2_sub_scalar);
dl_declare_test(test_vec3_add);
dl_declare_test(test_vec3_add_scalar);
dl_declare_test(test_vec3_approximately_equal);
dl_declare_test(test_vec3_cross);
dl_declare_test(test_vec3_div_scalar);
dl_declare_test(test_vec3_dot);
dl_declare_test(test_vec3_magnitude);
dl_declare_test(test_vec3_mul_scalar);
dl_declare_test(test_vec3_negate);
dl_declare_test(test_vec3_normalize);
dl_declare_test(test_vec3_reflect);
dl_declare_test(test_vec3_refract);
dl_declare_test(test_vec3_rotate);
dl_declare_test(test_vec3_square_magnitude);
dl_declare_test(test_vec3_sub);
dl_declare_test(test_vec3_sub_scalar);
dl_declare_test(test_vec4_add);
dl_declare_test(test_vec4_add_scalar);
dl_declare_test(test_vec4_approximately_equal);
dl_declare_test(test_vec4_div_scalar);
dl_declare_test(test_vec4_dot);
dl_declare_test(test_vec4_magnitude);
dl_declare_test(test_vec4_mul_scalar);
dl_declare_test(test_vec4_negate);
dl_declare_test(test_vec4_normalize);
dl_declare_test(test_vec4_square_magnitude);
dl_declare_test(test_vec4_sub);
dl_declare_test(test_vec4_sub_scalar);
dl_declare_test(test_point2_add);
dl_declare_test(test_point2_add_scalar);
dl_declare_test(test_point2_approximately_equal);
dl_declare_test(test_point2_div_scalar);
dl_declare_test(test_point2_line_orientation);
dl_declare_test(test_point2_mul_scalar);
dl_declare_test(test_point2_negate);
dl_declare_test(test_point2_sub);
dl_declare_test(test_point2_sub_scalar);
dl_declare_test(test_point3_add);
dl_declare_test(test_point3_add_scalar);
dl_declare_test(test_point3_approximately_equal);
dl_declare_test(test_point3_div_scalar);
dl_declare_test(test_point3_mul_scalar);
dl_declare_test(test_point3_negate);
dl_declare_test(test_point3_rotate);
dl_declare_test(test_point3_sub);
dl_declare_test(test_point3_sub_scalar);
dl_declare_test(test_mat4_add);
dl_declare_test(test_mat4_add_scalar);
dl_declare_test(test_mat4_approximately_equal);
dl_declare_test(test_mat4_div_scalar);
dl_declare_test(test_mat4_mul);
dl_declare_test(test_mat4_mul_point3);
dl_declare_test(test_mat4_mul_scalar);
dl_declare_test(test_mat4_mul_vec3);
dl_declare_test(test_mat4_mul_vec4);
dl_declare_test(test_mat4_rotate);
dl_declare_test(test_mat4_rotate_x);
dl_declare_test(test_mat4_rotate_y);
dl_declare_test(test_mat4_rotate_z);
dl_declare_test(test_mat4_scale);
dl_declare_test(test_mat4_sub);
dl_declare_test(test_mat4_sub_scalar);
dl_declare_test(test_mat4_translate);
dl_declare_test(test_mat4_transpose);
dl_end_test_suite

/*******************************************************************************;
 test
*******************************************************************************/

dl_begin_test_suite(tester);
dl_declare_test(test_test_count_tests);
dl_declare_test(test_test_run_tests);
dl_end_test_suite

/*******************************************************************************;
 vector
*******************************************************************************/

dl_begin_test_suite(vector);
dl_declare_test(test_dl_init_vector);
dl_declare_test(test_dl_vector_push_pop);
dl_declare_test(test_dl_vector_grow);
dl_declare_test(test_dl_vector_get_set_ref);
dl_declare_test(test_dl_vector_insert_remove);
dl_end_test_suite

/*******************************************************************************;
 linked_list
*******************************************************************************/

dl_begin_test_suite(linked_list);
dl_declare_test(test_dl_init_linked_list);
dl_declare_test(test_dl_linked_list_push_pop);
dl_declare_test(test_dl_linked_list_get_set_ref);
dl_declare_test(test_dl_linked_list_insert_remove);
dl_end_test_suite

/*******************************************************************************;
 container
*******************************************************************************/

dl_begin_test_suite(container);
dl_declare_test(test_dl_make_container);
dl_declare_test(test_dl_container_element_size);
dl_declare_test(test_dl_container_length);
dl_declare_test(test_dl_container_is_empty);
dl_declare_test(test_dl_container_index);
dl_declare_test(test_dl_container_first);
dl_declare_test(test_dl_container_last);
dl_declare_test(test_dl_container_push);
dl_declare_test(test_dl_container_pop);
dl_declare_test(test_dl_container_traits);
dl_declare_test(test_dl_iterator_is_valid);
dl_declare_test(test_dl_iterator_get);
dl_declare_test(test_dl_iterator_ref);
dl_declare_test(test_dl_iterator_set);
dl_declare_test(test_dl_iterator_swap);
dl_declare_test(test_dl_iterator_insert);
dl_declare_test(test_dl_iterator_remove);
dl_declare_test(test_dl_iterator_next);
dl_declare_test(test_dl_iterator_prev);
dl_declare_test(test_dl_iterator_next_ref);
dl_declare_test(test_dl_iterator_prev_ref);
dl_declare_test(test_dl_iterator_equal);
dl_declare_test(test_dl_iterator_compare);
dl_declare_test(test_dl_iterator_index);
dl_end_test_suite

/*******************************************************************************;
 algorithms
*******************************************************************************/

dl_begin_test_suite(algorithm);
dl_declare_test(test_dl_count);
dl_declare_test(test_dl_find);
dl_declare_test(test_dl_find_reverse);
dl_declare_test(test_dl_find_all);
dl_declare_test(test_dl_foldl);
dl_declare_test(test_dl_foldr);
dl_declare_test(test_dl_all);
dl_declare_test(test_dl_any);
dl_declare_test(test_dl_map);
dl_declare_test(test_dl_zip);
dl_declare_test(test_dl_take);
dl_declare_test(test_dl_drop);
dl_declare_test(test_dl_remove);
dl_declare_test(test_dl_remove_reverse);
dl_declare_test(test_dl_quick_sort);
dl_declare_test(test_dl_make_handler);
dl_declare_test(test_dl_make_zipper);
dl_declare_test(test_dl_make_filter);
dl_declare_test(test_dl_make_folder);
dl_declare_test(test_dl_make_comparator);
dl_end_test_suite
    
int  main(int argc, char **argv) {
  dl_run_test_suite(tester);
  dl_run_test_suite(math);
  dl_run_test_suite(vector);
  dl_run_test_suite(linked_list);
  dl_run_test_suite(container);
  dl_run_test_suite(algorithm);
  return 0;
}

#else

int main(int argc, char **argv) {
  dl_unused(argc);
  dl_unused(argv);
  return 0;
}

#endif
