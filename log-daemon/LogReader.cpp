#include "LogReader.h"
#include <thread>

LogReader::LogReader(
        std::vector<CallId> callIds,
        std::chrono::nanoseconds pollFrequency,
        SharedArena *arena
) :
        callIds(std::move(callIds)),
        pollFrequency(pollFrequency),
        arena(arena),
        ringBufferSize(arena->getRingBufferSize()){}

void LogReader::readTo(std::ofstream &logFile) {
    uint64_t lastBufferIndex = 0;
    while (true) {
        uint64_t nextBufferIndex = arena->getNextRingEntry();
        if (nextBufferIndex < lastBufferIndex || lastBufferIndex + ringBufferSize < nextBufferIndex) {
            lastBufferIndex = nextBufferIndex - ringBufferSize;
        }
        bool hasNewEntries = lastBufferIndex != nextBufferIndex;
        for (; lastBufferIndex != nextBufferIndex; lastBufferIndex++) {
            LogEntry *entry = arena->getRingEntry(lastBufferIndex);
            if (filterEntry(entry)) {
                logEntry(entry, logFile);
            }
        }
        if (hasNewEntries)logFile.flush();

        std::this_thread::sleep_for(pollFrequency);
    }
}

struct EntryFormatConfig {
    const char *name;
    int argsCount;
    bool showRetVal;
};
constexpr EntryFormatConfig config[] = {
        {"undefined", 0, true},
        {"open",      2, true},
        {"close",     1, true},
        {"lseek",     3, true},
        {"read",      3, true},
        {"write",     3, true},
        {"malloc",    1, true},
        {"realloc",   2, true},
        {"free",      1, false},
};

void LogReader::logEntry(LogEntry *entry, std::ofstream &ofstream) {
    auto entryFormatConfig = config[entry->id];
    ofstream
            << entry->procName << ":"
            << std::dec << entry->procPid << " "
            << entryFormatConfig.name << "(";
    for (int i = 0; i < entryFormatConfig.argsCount - 1; ++i) {
        ofstream << std::hex << entry->args[i] << ", ";
    }
    if (entryFormatConfig.argsCount != 0) {
        ofstream << std::hex << entry->args[entryFormatConfig.argsCount - 1];
    }
    ofstream << ")";
    if (entryFormatConfig.showRetVal) {
        ofstream << "=" << std::hex << entry->retVal;
    }
    ofstream << std::endl;

}

bool LogReader::filterEntry(LogEntry *entry) {
    return std::find(callIds.begin(), callIds.end(), entry->id) != callIds.end();
}

