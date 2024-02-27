#include "SharedArena.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

SharedArena *SharedArena::createOrConnect(const char *arenaName, uint64_t ringBufferSize) {
    int fd = shm_open(arenaName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return nullptr;
    }
    auto arenaSize = sizeof(SharedArena) + sizeof(LogEntry) * ringBufferSize;
    ftruncate(fd, arenaSize);
    auto *arena = static_cast<SharedArena *>(
            mmap(nullptr, arenaSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (arena == MAP_FAILED) {
        int rc = shm_unlink(arenaName);
        if (rc == -1) {
            return nullptr;
        }
        return nullptr;
    }
    arena->ringBufferSize = ringBufferSize;
    return arena;
}

LogEntry *SharedArena::allocateNextEntry() {
    uint64_t index = nextRingEntry++;
    index %= ringBufferSize;
    return &entries[index];
}
