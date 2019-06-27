#ifndef DL_CONTAINERS_H
#define DL_CONTAINERS_H 1

#include "dl.h"
#include "dl_vector.h"
#include "dl_linked_list.h"
#include "dl_iterator.h"



#ifdef __cplusplus
extern "C" {
    #endif

    /*****************************************************************************
     **  Generic Interface
     ****************************************************************************/

    typedef enum {
        DL_CONTAINER_TYPE_VECTOR,
        DL_CONTAINER_TYPE_LINKED_LIST
    } dl_container_type;
  
    typedef struct {
        dl_container_type type;
        union {
            dl_linked_list list;
            dl_vector vector;
            dl_any container;
        } data;
    } dl_container;

    dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity);
    dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count);
    dl_api void dl_destroy_container(dl_container *target, dl_handler *deconstruct_entry);
    
    dl_api dl_natural dl_container_length(const dl_container *container);
    
    dl_api dl_iterator dl_container_index(const dl_container *container, dl_natural position);
    dl_api dl_iterator dl_container_first(dl_container *target);
    dl_api dl_iterator dl_container_last(dl_container *target);

    dl_api dl_bool dl_container_push(dl_container *v, dl_any value);
    dl_api dl_any dl_container_pop(dl_container *v, dl_any out);

    ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

/*******************************************************************************
 ** Generic Interface
 ******************************************************************************/

dl_api dl_container *dl_init_container(dl_container *target, dl_container_type type, dl_natural element_size, dl_natural capacity) {
    if (dl_safety(target == NULL || element_size == 0 || capacity == 0))
        return NULL;

    target->type = type;
    switch (type) {
        default:
            return NULL;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            if (NULL == dl_init_linked_list(&target->data.list, element_size, capacity))
                return NULL;
            return target;
        case DL_CONTAINER_TYPE_VECTOR:
            if (NULL == dl_init_vector(&target->data.vector, element_size, capacity))
                return NULL;
            return target;
    }
}

dl_api dl_container *dl_init_container_array(dl_container *target, dl_byte *array_data, dl_natural element_size, dl_natural count) {
    if (dl_safety(target == NULL || element_size == 0 || count == 0 || array_data == NULL))
        return NULL;

    target->type = DL_CONTAINER_TYPE_VECTOR;
    if (NULL == dl_init_vector_array(&target->data.vector, array_data, element_size, count))
        return NULL;
  
    return target;
}

dl_api dl_natural dl_container_copy(dl_iterator target,  const dl_iterator original, dl_natural count) {
    if (dl_safety(!dl_iterator_is_valid(target) || !dl_iterator_is_valid(original)))
        return 0;
  
    switch (target.container->type) {
        default:
            return 0;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            return dl_linked_list_copy((dl_linked_list *)target.container, target.data.node, (const dl_linked_list *)original.container, original.data.node, count);
        case DL_CONTAINER_TYPE_VECTOR:
            return dl_vector_copy((dl_vector *)target.container, target.data.index, (dl_vector *)original.container, original.data.index, count);
    }
}

dl_api void dl_destroy_container(dl_container *target, dl_handler *deconstruct_entry) {
    if (dl_safety(target == NULL || target->data.container == NULL))
        return;

    switch (target->type) {
        default:
            break;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            dl_destroy_linked_list(&target->data.list, deconstruct_entry);
            break;
        case DL_CONTAINER_TYPE_VECTOR:
            dl_destroy_vector(&target->data.vector, deconstruct_entry);
            break;
    }

    target->data.container = NULL;
}

dl_api dl_natural dl_container_length(const dl_container *target) {
    if (dl_safety(target == NULL || target->data.container == NULL))
        return 0;

    switch (target->type) {
        default:
            return 0;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            return dl_linked_list_length(&target->data.list);
        case DL_CONTAINER_TYPE_VECTOR:
            return dl_vector_capacity(&target->data.vector);
    }
}

dl_api dl_iterator dl_container_index(const dl_container *target, dl_natural position) {
    dl_iterator iter;
  
    if (dl_safety(target == NULL || target->data.container == NULL))
        return dl_make_invalid_iterator();

    switch(target->type) {
        default:
            return dl_make_invalid_iterator();
        case DL_CONTAINER_TYPE_LINKED_LIST:
            iter.container = (dl_container *)target;
            iter.data.node = dl_linked_list_index(&target->data.list, position);
            return iter;
        case DL_CONTAINER_TYPE_VECTOR:
            if (dl_safety(position >= dl_vector_capacity(&target->data.vector)))
                return dl_make_invalid_iterator();
            iter.container = (dl_container *)target;
            iter.data.index = position;
            return iter;
    }
}

dl_api dl_iterator dl_container_first(dl_container *target) {
    dl_iterator iter;
  
    if (dl_safety(target == NULL || target->data.container == NULL))
        return dl_make_invalid_iterator();

    switch(target->type) {
        default:
            return dl_make_invalid_iterator();
        case DL_CONTAINER_TYPE_LINKED_LIST:
            iter.container = target;
            iter.data.node = target->data.list.first;
            return iter;
        case DL_CONTAINER_TYPE_VECTOR:
            if (dl_safety(target->data.vector.array == NULL))
                return dl_make_invalid_iterator();
            iter.container = target;
            iter.data.index = 0;
            return iter;
    }
}

dl_api dl_iterator dl_container_last(dl_container *target) {
    dl_iterator iter;

    if (dl_safety(target == NULL || target->data.container == NULL))
        return dl_make_invalid_iterator();

    switch(target->type) {
        default:
            return dl_make_invalid_iterator();
        case DL_CONTAINER_TYPE_LINKED_LIST:
            iter.container = target;
            iter.data.node = target->data.list.last;
            return iter;
        case DL_CONTAINER_TYPE_VECTOR:
            if (dl_safety(target->data.vector.array == NULL))
                return dl_make_invalid_iterator();
            iter.container = target;
            iter.data.index = target->data.vector.capacity - 1;
            return iter;
    }
}

dl_api dl_bool dl_container_push(dl_container *c, dl_any value) {
    if (dl_safety(c == NULL))
        return false;

    switch (c->type) {
        default:
            return false;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            return dl_linked_list_push(&c->data.list, value);
        case DL_CONTAINER_TYPE_VECTOR:
            return dl_vector_push(&c->data.vector, value);
    }
}

dl_api dl_any dl_container_pop(dl_container *c, dl_any out) {
    if (dl_safety(c == NULL))
        return false;

    switch (c->type) {
        default:
            return false;
        case DL_CONTAINER_TYPE_LINKED_LIST:
            return dl_linked_list_pop(&c->data.list, out);
        case DL_CONTAINER_TYPE_VECTOR:
            return dl_vector_pop(&c->data.vector, out);
    }
}



#endif /* DL_IMPLEMENTATION */

#endif
