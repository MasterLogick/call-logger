#include "LogReader.h"
#include <thread>

LogReader::LogReader(
        std::vector<call_id_t> callIds,
        std::chrono::nanoseconds pollFrequency,
        const char *arenaName,
        uint64_t arenaBufferSize
) :
        callIds(std::move(callIds)),
        pollFrequency(pollFrequency) {
    arena = create_or_connect_to_arena(arenaName, arenaBufferSize);
}

void LogReader::readTo(std::ofstream &logFile) {
    uint64_t lastBufferIndex = 0;
    while (true) {
        uint64_t nextBufferIndex = arena->next_ring_entry;
        if (nextBufferIndex < lastBufferIndex || lastBufferIndex < nextBufferIndex - ringBufferSize) {
            lastBufferIndex = nextBufferIndex - ringBufferSize;
        }
        for (; lastBufferIndex != nextBufferIndex; lastBufferIndex++) {
            log_entry_t *entry = &arena->entries[lastBufferIndex % ringBufferSize];
            if (filterEntry(entry)) {
                logEntry(entry);
            }
        }

        std::this_thread::sleep_for(pollFrequency);
    }
}

void LogReader::logEntry(log_entry_t *entry) {

}

bool LogReader::filterEntry(log_entry_t *entry) {
    return std::find(callIds.begin(), callIds.end(), entry->id) != callIds.end();
}

