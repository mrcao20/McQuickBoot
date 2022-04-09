#ifndef MEMLOAD_H
#define MEMLOAD_H

#include "mldll_global.h"

#if defined (__cplusplus) || defined (c_plusplus)
extern "C" {
#endif

MLDLL_EXPORT void * memory_load_library(const void *buf, size_t size);

MLDLL_EXPORT void * memory_get_procaddr(void *handle, const char *procname);

MLDLL_EXPORT bool memory_free_library(void *handle);

MLDLL_EXPORT char * memory_get_errormsg(void);

#if defined (__cplusplus) || defined (c_plusplus)
} /* End of extern "C" */
#endif

#endif // MEMLOAD_H
