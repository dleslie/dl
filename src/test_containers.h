#ifndef TEST_CONTAINERS_H
#define TEST_CONTAINERS_H

#include "dl.h"

typedef struct {
  const char *type_name;
  dl_container_interface *interface;
  dl_natural traits;
} _container_info;

extern _container_info infos[];
extern dl_natural info_count;

#endif
