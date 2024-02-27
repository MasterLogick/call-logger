#ifndef PROXY_SHARED_ARENA_H
#define PROXY_SHARED_ARENA_H

#include <sys/types.h>
#include <stdint.h>
#include <atomic>

enum CallId {
    CallId_Undefined = 0,
    CallId_open,
    CallId_close,
    CallId_lseek,
    CallId_read,
    CallId_write,
    CallId_malloc,
    CallId_realloc,
    CallId_free
};

struct LogEntry {
    pid_t procPid;
    char procName[64];

    CallId id;
    uint64_t args[6];
    uint64_t retVal;

    union CallSpecificData {
        char openPath[64];
    } callSpecificData;
};

class SharedArena {
public:
    SharedArena() = delete;

    static SharedArena *createOrConnect(const char *arenaName = "/log_arena", uint64_t ringBufferSize = 512);

    LogEntry *allocateNextEntry();

    uint64_t getNextRingEntry() const {
        return nextRingEntry;
    }

    uint64_t getRingBufferSize() const {
        return ringBufferSize;
    }

    LogEntry *getRingEntry(uint64_t index) {
        return &entries[index % ringBufferSize];
    }

private:
    uint64_t ringBufferSize;
    std::atomic_uint64_t nextRingEntry;
    LogEntry entries[0];
};

#endif //PROXY_SHARED_ARENA_H
