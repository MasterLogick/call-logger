#define _GNU_SOURCE

#include <stddef.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include "shared_arena.h"

extern pid_t getpid();

shared_arena_t *arena;

typedef uint64_t(*hooked_func)(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                               uint64_t arg5);

#define INTERCEPT(FUNC_NAME)                                             \
static hooked_func FUNC_NAME##_func = NULL;                              \
                                                                         \
void __attribute__((constructor)) find_##FUNC_NAME##_func() {            \
    FUNC_NAME##_func = dlsym(RTLD_NEXT, #FUNC_NAME);                     \
}                                                                        \
                                                                         \
uint64_t FUNC_NAME(uint64_t arg0, uint64_t arg1, uint64_t arg2,          \
                   uint64_t arg3, uint64_t arg4, uint64_t arg5) {        \
    uint64_t res = FUNC_NAME##_func(arg0, arg1, arg2, arg3, arg4, arg5); \
                                                                         \
    log_entry_t *entry = allocate_next_entry(arena);                     \
    strncpy(entry->proc_name, program_invocation_short_name,             \
        sizeof(entry->proc_name));                                       \
    entry->proc_pid = getpid();                                          \
    entry->args[0] = arg0;                                               \
    entry->args[1] = arg1;                                               \
    entry->args[2] = arg2;                                               \
    entry->args[3] = arg3;                                               \
    entry->args[4] = arg4;                                               \
    entry->args[5] = arg5;                                               \
    entry->ret_val = res;                                                \
    entry->id = call_id_##FUNC_NAME;                                     \
    return res;                                                          \
}

INTERCEPT(open)
//int open(const char *__file, int __oflag, ...);

INTERCEPT(close)
//int close(int __fd);

INTERCEPT(lseek)
//__off_t lseek(int __fd, __off_t __offset, int __whence);

INTERCEPT(read)
//ssize_t read(int __fd, void *__buf, size_t __nbytes);

INTERCEPT(write)
//ssize_t write(int __fd, const void *__buf, size_t __n);

INTERCEPT(malloc)
//void *malloc(size_t __size);

INTERCEPT(realloc)
//void *realloc(void *__ptr, size_t __size);

INTERCEPT(free)
//void free(void *__ptr);

void __attribute__((constructor)) init_proxy_lib() {
    arena = create_or_connect_to_arena(DEFAULT_ARENA_NAME, DEFAULT_ARENA_BUFFER_SIZE);
}
