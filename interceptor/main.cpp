#include <dlfcn.h>
#include <cstring>
#include <cerrno>
#include "SharedArena.h"

extern "C" pid_t getpid();

SharedArena *arena = SharedArena::createOrConnect();

typedef uint64_t(*hooked_func)(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                               uint64_t arg5);

#define INTERCEPT(FUNC_NAME)                                                  \
static hooked_func FUNC_NAME##Func = nullptr;                                 \
                                                                              \
void __attribute__((constructor)) FUNC_NAME##FindFunc() {                     \
    FUNC_NAME##Func = reinterpret_cast<hooked_func>                           \
            (dlsym(RTLD_NEXT, #FUNC_NAME));                                   \
}                                                                             \
                                                                              \
extern "C" uint64_t FUNC_NAME##Hook(                                          \
        uint64_t arg0, uint64_t arg1, uint64_t arg2,                          \
        uint64_t arg3, uint64_t arg4, uint64_t arg5) {                        \
    uint64_t res = FUNC_NAME##Func(arg0, arg1, arg2, arg3, arg4, arg5);       \
                                                                              \
    LogEntry *entry = arena->allocateNextEntry();                             \
    strncpy(entry->procName, program_invocation_short_name,                   \
        sizeof(entry->procName));                                             \
    entry->procName[sizeof(entry->procName) - 1] = 0;                         \
    entry->procPid = getpid();                                                \
    entry->args[0] = arg0;                                                    \
    entry->args[1] = arg1;                                                    \
    entry->args[2] = arg2;                                                    \
    entry->args[3] = arg3;                                                    \
    entry->args[4] = arg4;                                                    \
    entry->args[5] = arg5;                                                    \
    entry->retVal = res;                                                      \
    entry->id = CallId_##FUNC_NAME;                                           \
    return res;                                                               \
}                                                                             \
asm (".globl "#FUNC_NAME"\n"                                                  \
     ".set "#FUNC_NAME","#FUNC_NAME"Hook");

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
