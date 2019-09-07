#include "test_containers.h"

_container_info infos[] = {
  {"vector", &dl_vector_container_interface, DL_CONTAINER_TRAIT_RANDOM_ACCESS},
  {"linked list", &dl_linked_list_container_interface, DL_CONTAINER_TRAIT_RANDOM_INSERT}};
dl_natural info_count = 2;
