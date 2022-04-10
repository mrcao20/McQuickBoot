#ifndef MEMLOAD_H
#define MEMLOAD_H

#include <stddef.h>

#if defined (__cplusplus) || defined (c_plusplus)
extern "C" {
#endif

void *memory_load_library(const void *buf, size_t size);

void *memory_get_procaddr(void *handle, const char *procname);

bool memory_free_library(void *handle);

char *memory_get_errormsg(void);

#if defined (__cplusplus) || defined (c_plusplus)
} /* End of extern "C" */
#endif

#endif // MEMLOAD_H
