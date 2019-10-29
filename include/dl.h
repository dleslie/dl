/* Preprocessor defines for machine information, and memory functions */
#include "dl_core.h"

/* Basic time methods */
#include "dl_time.h"

#if DL_USE_LOGGING
/* STDIO logging */
#include "dl_logging.h"

/* Define and execute test suites */
#include "dl_test.h"
#endif

/* Generic abstractions over data structures */
#include "dl_algorithms.h"

/* Interface for data structres */
#include "dl_container.h"

/* Linked List */
#include "dl_linked_list.h"

/* Linked List transformer for Container interface */
#include "dl_linked_list_container.h"

/* Vector */
#include "dl_vector.h"

/* Vector transformer for Container interface */
#include "dl_vector_container.h"

/* Lerping, Splines, and some pleasant-looking in/out tweens */
#include "dl_tween.h"

/* Vector, Matrix et al */
#include "dl_math.h"
