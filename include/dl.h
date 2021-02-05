/* Preprocessor defines for machine information, and memory functions */
#include "dl_core.h"

/* Extra preprocessor macros */
#include "dl_macro.h"

/* Memory methods */
#include "dl_memory.h"

/* Vector, Matrix et al */
#include "dl_math.h"

/* Lerping, Splines, and some pleasant-looking in/out tweens */
#include "dl_tween.h"

/* Generic abstractions over data structures */
#include "dl_algorithms.h"

/* Interface for data structres */
#include "dl_container.h"

/* Basic wrapper around C arrays */
#include "dl_array_container.h"

#if DL_IS_LINUX || DL_IS_APPLE || DL_IS_WINDOWS
/* Basic time methods */
#include "dl_time.h"
#endif /* DL_IS_LINUX || DL_IS_APPLE || DL_IS_WINDOWS */

#if DL_USE_LOGGING
/* STDIO logging */
#include "dl_logging.h"

/* Define and execute test suites */
#include "dl_test.h"
#endif /* DL_USE_LOGGING */

#if DL_USE_MALLOC
/* Linked List */
#include "dl_linked_list.h"

/* Linked List transformer for Container interface */
#include "dl_linked_list_container.h"

/* Vector */
#include "dl_vector.h"

/* Vector transformer for Container interface */
#include "dl_vector_container.h"
#endif /* DL_USE_MALLOC */
