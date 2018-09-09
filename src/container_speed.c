#define DL_IMPLEMENTATION 1
#include "dl.h"

const dl_natural test_size = 1000;

dl_byte collection_types[4] = {
    DL_COLLECTION_TYPE_LIST,
    DL_COLLECTION_TYPE_QUEUE,
    DL_COLLECTION_TYPE_SORTED_LIST,
    DL_COLLECTION_TYPE_SET
};

int main(int argc, char const *argv[])
{
    dl_iterator iter;
    dl_integer next_value;
    dl_natural type_idx, container_idx;
    dl_collection test_collection;
    dl_random_state r;

#if defined(DL_TEST_LINKED_LIST)
    dl_storage_type storage = DL_STORAGE_TYPE_LINKED_LIST;
#else //if defined(DL_TEST_VECTOR)
    dl_storage_type storage = DL_STORAGE_TYPE_VECTOR;
#endif

    dl_init_random_time(&r);

    for (type_idx = 0; type_idx < 4; ++type_idx)
    {
        dl_byte type = collection_types[type_idx];
        dl_init_collection(&test_collection, type, storage, NULL, NULL, sizeof(dl_integer));

        // Add 10,000 values in random order
        for (container_idx = 0; container_idx < test_size; container_idx += 2)
        {
            iter = dl_collection_index(&test_collection, container_idx / 2);
            next_value = dl_random_integer(&r, 32767);
            dl_collection_push(&test_collection, &next_value);
            next_value = dl_random_integer(&r, 32767);
            dl_collection_insert(&test_collection, &iter, &next_value);
        }

        // Sort values
	if (!dl_collection_is_sorted(&test_collection))
	  dl_collection_sort(&test_collection);

        // Remove values at random, calling resize whenever possible
        while (!dl_collection_is_empty(&test_collection))
        {
            iter = dl_collection_index(&test_collection, dl_random_integer(&r, dl_collection_count(&test_collection)));
            dl_collection_destroy_at(&test_collection, &iter);
            dl_collection_pop_destroy(&test_collection);

            dl_collection_compact(&test_collection);
        }

        // Add 10,000 values in reverse order
        for (container_idx = 0; container_idx < test_size; ++container_idx)
        {
            next_value = test_size - container_idx;
            dl_collection_push(&test_collection, &next_value);
        }

        // Sort values
	if (!dl_collection_is_sorted(&test_collection))
	  dl_collection_sort(&test_collection);

        // Remove values at random, calling resize whenever possible
        while (!dl_collection_is_empty(&test_collection))
        {
            iter = dl_collection_index(&test_collection, dl_random_integer(&r, dl_collection_count(&test_collection)));
            dl_collection_destroy_at(&test_collection, &iter);
            dl_collection_pop_destroy(&test_collection);

            dl_collection_compact(&test_collection);
        }

        // Add 10,000 values in pseudo-sorted order
        for (container_idx = 0; container_idx < test_size; container_idx += 2)
        {
            iter = dl_collection_index(&test_collection, container_idx / 2);
            next_value = container_idx;
            dl_collection_push(&test_collection, &next_value);
            next_value = container_idx + 1;
            dl_collection_insert(&test_collection, &iter, &next_value);
        }

        // Sort values
	if (!dl_collection_is_sorted(&test_collection))
	  dl_collection_sort(&test_collection);

        // Remove values at random, calling resize whenever possible
        while (!dl_collection_is_empty(&test_collection))
        {
            iter = dl_collection_index(&test_collection, dl_random_integer(&r, dl_collection_count(&test_collection)));
            dl_collection_destroy_at(&test_collection, &iter);
            dl_collection_pop_destroy(&test_collection);

            dl_collection_compact(&test_collection);
        }

        dl_destroy_collection(&test_collection);
    }

    return 0;
}
