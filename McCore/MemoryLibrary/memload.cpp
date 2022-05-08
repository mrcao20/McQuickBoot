#include "memload.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef Q_OS_LINUX
#include <linux/memfd.h>
#endif
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#define MEMLOAD_SHM_NAME "memload_dyso_shm"

#ifndef __NR_memfd_create
#define __NR_memfd_create (319)
#endif

#if defined (__cplusplus) || defined (c_plusplus)
extern "C" {
#endif

static inline int my_memfd_create(const char *name, unsigned int flags)
{
    return syscall(__NR_memfd_create, name, flags);
}

bool kernel_version_ge37(void) {
    struct utsname buffer;
    uname(&buffer);

    char *token = NULL;
    const char *separator = ".";

    token = strtok(buffer.release, separator);
    if (atoi(token) > 3){
        return true;
    } else {
        if (atoi(token) < 3) {
            return false;
        } else {
            token = strtok(NULL, separator);
            if (atoi(token) >= 17) {
                return true;
            }
            else {
                return false;
            }
        }
    }
}

int open_ramfs(void)
{
    int shm_fd = 0;

    if (kernel_version_ge37()) {
        shm_fd = my_memfd_create(MEMLOAD_SHM_NAME, 1);

    } else {
        shm_fd = shm_open(MEMLOAD_SHM_NAME, O_RDWR | O_CREAT, S_IRWXU);
    }

    return shm_fd;
}

bool write_data(int shm_fd, const void *ptr, size_t nmemb) {
    if (write(shm_fd, ptr, nmemb) < 0) {
        return false;
    }
    return true;
}

void * load_dyso(int shm_fd)
{
    char path[1024] = {'\0'};
    void * handle = NULL;

    if (kernel_version_ge37()) {
        snprintf(path, 1024, "/proc/%d/fd/%d", getpid(), shm_fd);
    } else {
        snprintf(path, 1024, "/dev/shm/%s", MEMLOAD_SHM_NAME);
    }

    handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
        fprintf(stderr,"[-] Dlopen failed with error: %s\n", dlerror());
    }
    return handle;
}

void * memory_load_library(const void *buf, size_t size)
{
    if (buf == NULL || size == 0) {
        return NULL;
    }

    int shm_fd = open_ramfs();
    if (shm_fd < 0) {
        fprintf(stderr, "[-] Could not open file descriptor\n");
        return NULL;
    }

    if (! write_data(shm_fd, buf, size)) {
        fprintf(stderr, "[-] Could not write file\n");
        close(shm_fd);
        return NULL;
    }

    void *handle = load_dyso(shm_fd);
    close(shm_fd);
    return handle;
}

void *memory_get_procaddr(void *handle, const char *procname)
{
    return dlsym(handle, procname);
}

bool memory_free_library(void *handle)
{
    if (handle != NULL) {
        dlclose(handle);
        return true;
    } else {
        return false;
    }
}

char *memory_get_errormsg()
{
    return dlerror();
}

#if defined (__cplusplus) || defined (c_plusplus)
} /* End of extern "C" */
#endif
