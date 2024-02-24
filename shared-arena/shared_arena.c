#include <stddef.h>
#include "shared_arena.h"
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

log_entry_t *allocate_next_entry(shared_arena_t *arena) {
    assert(arena != NULL);
    uint64_t index = ++arena->next_ring_entry;
    index %= arena->ring_buffer_size;
    return &arena->entries[index];
}

shared_arena_t *create_or_connect_to_arena(const char *arena_name, uint64_t ring_buffer_size) {
    int fd = shm_open(arena_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return NULL;
    }
    size_t arena_size = sizeof(shared_arena_t) + sizeof(log_entry_t) * ring_buffer_size;
    ftruncate(fd, arena_size);
    shared_arena_t *arena = mmap(NULL, arena_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int rc = shm_unlink(arena_name);
    if (rc == -1) {
        return NULL;
    }
    if (arena == MAP_FAILED) {
        return NULL;
    }
    arena->ring_buffer_size = ring_buffer_size;
    return arena;
}