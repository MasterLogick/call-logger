#include "LogReader.h"
#include <thread>
#include "LogPrinters.h"

LogReader::LogReader(
        std::vector<CallId> callIds,
        std::chrono::nanoseconds pollFrequency,
        SharedArena *arena
) :
        callIds(std::move(callIds)),
        pollFrequency(pollFrequency),
        arena(arena),
        ringBufferSize(arena->getRingBufferSize()) {}

void LogReader::readTo(std::ofstream &logFile) {
    uint64_t lastBufferIndex = 0;
    while (true) {
        uint64_t nextBufferIndex = arena->getNextRingEntry();
        // Move our iterator back to the last filled element if it is outside of sliding window
        if (nextBufferIndex < lastBufferIndex || lastBufferIndex + ringBufferSize < nextBufferIndex) {
            lastBufferIndex = nextBufferIndex - ringBufferSize;
        }
        bool hasNewEntries = lastBufferIndex != nextBufferIndex;
        for (; lastBufferIndex != nextBufferIndex; lastBufferIndex++) {
            LogEntry *entry = arena->getRingEntry(lastBufferIndex);
            if (filterEntry(entry)) {
                if (entry->id < sizeof(config) / sizeof(config[0])) {
                    config[entry->id].logPrinter(entry, logFile);
                }
            }
        }
        if (hasNewEntries)logFile.flush();

        std::this_thread::sleep_for(pollFrequency);
    }
}

bool LogReader::filterEntry(LogEntry *entry) {
    return std::find(callIds.begin(), callIds.end(), entry->id) != callIds.end();
}
