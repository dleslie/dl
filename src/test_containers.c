#include "test_containers.h"

#if DL_USE_MALLOC
_container_info infos[] = {
  {"vector", &dl_vector_container_interface, DL_CONTAINER_TRAIT_RANDOM_ACCESS},
  {"linked list", &dl_linked_list_container_interface, DL_CONTAINER_TRAIT_RANDOM_INSERT}};
dl_natural info_count = 2;
#else
_container_info infos[] = {};
dl_natural info_count = 0;
#endif
