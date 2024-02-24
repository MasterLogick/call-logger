#ifndef LOGGER_LOGREADER_H
#define LOGGER_LOGREADER_H


#include <vector>
#include <chrono>
#include <fstream>
#include "SharedArena.h"

class LogReader {

public:
    LogReader(std::vector<CallId> callIds, std::chrono::nanoseconds pollFrequency, SharedArena* arena);

    void readTo(std::ofstream &logFile);

private:
    std::vector<CallId> callIds;
    std::chrono::nanoseconds pollFrequency;
    SharedArena *arena;
    uint64_t ringBufferSize;

    bool filterEntry(LogEntry *entry);

    void logEntry(LogEntry *entry, std::ofstream &ofstream);
};


#endif //LOGGER_LOGREADER_H
