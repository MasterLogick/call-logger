#ifndef PROXY_SHARED_ARENA_H
#define PROXY_SHARED_ARENA_H

#include <sys/types.h>
#include <stdint.h>
#include <atomic>

/**
 * This enum lists all intercepted calls
 */
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

/**
 * Description of one intercepted call in binary format
 */
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

/**
 * This class represents shared memory range and manages access to log entries inside it
 */
class SharedArena {
public:
    SharedArena() = delete;

    SharedArena(const SharedArena &) = delete;

    SharedArena(SharedArena &&) = delete;

    /**
     * Unmaps shared arena from
     */
    ~SharedArena();

    /**
     * Opens new connection to shared ring buffer arena of creates a new one with specified size
     * @param arenaName name for ring buffer arena
     * @param ringBufferSize entry count in ring buffer
     * @return pointer to SharedArena if connection is established or nullptr if failed
     */
    static SharedArena *createOrConnect(const char *arenaName = "/log_arena", uint64_t ringBufferSize = 512);

    /**
     * Takes next entry in ring buffer and returns pointer to it. If all entries are used, overwrites last entry
     * @return pointer to log entry
     */
    LogEntry *allocateNextEntry();

    /**
     * @return index of the next used entry in ring buffer.
     */
    uint64_t getNextRingEntry() const {
        return nextRingEntry;
    }

    /**
     * @return entry count in ring buffer
     */
    uint64_t getRingBufferSize() const {
        return ringBufferSize;
    }

    /**
     * @param index index of entry in ring buffer
     * @return pointer to specified entry
     */
    LogEntry *getRingEntry(uint64_t index) {
        return &entries[index % ringBufferSize];
    }

private:
    /**
     * Entry count in ring buffer
     */
    uint64_t ringBufferSize;
    /**
     * Atomic index of the next used entry in the ring buffer
     */
    std::atomic_uint64_t nextRingEntry;
    /**
     * Start of the ring buffer
     */
    LogEntry entries[0];
};

#endif //PROXY_SHARED_ARENA_H
