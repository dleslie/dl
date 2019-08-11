#ifndef DL_VECTOR_CONTAINER_H
#define DL_VECTOR_CONTAINER_H

#include "dl_container.h"

#ifdef __cplusplus
extern "C" {
#endif

dl_api dl_container *dl_make_vector_container(dl_natural element_size, dl_natural capacity);

#ifdef __cplusplus
}
#endif

#if defined(DL_IMPLEMENTATION)
#include "dl_vector.h"

#endif /* DL_IMPLEMENTATION */
#endif /* DL_VECTOR_CONTAINER_H */