#ifndef PROXY_SHARED_ARENA_H
#define PROXY_SHARED_ARENA_H

#ifdef __cplusplus
extern "C"{
#endif

#include <sys/types.h>
#include <stdint.h>

typedef enum call_id {
    call_id_undefined = 0,
    call_id_open,
    call_id_close,
    call_id_lseek,
    call_id_read,
    call_id_write,
    call_id_malloc,
    call_id_realloc,
    call_id_free
} call_id_t;

typedef struct log_entry {
    pid_t proc_pid;
    char proc_name[64];

    call_id_t id;
    uint64_t args[6];
    uint64_t ret_val;
} log_entry_t;

typedef struct shared_arena {
    uint64_t ring_buffer_size;
    _Atomic uint64_t next_ring_entry;
    log_entry_t entries[0];
} shared_arena_t;

log_entry_t *allocate_next_entry(shared_arena_t *arena);

#define DEFAULT_ARENA_NAME "/log_arena"
#define DEFAULT_ARENA_BUFFER_SIZE 512

shared_arena_t *create_or_connect_to_arena(const char *arena_name, uint64_t ring_buffer_size);

#ifdef __cplusplus
}
#endif

#endif //PROXY_SHARED_ARENA_H
