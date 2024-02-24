#ifndef LOGGER_LOGREADER_H
#define LOGGER_LOGREADER_H


#include <vector>
#include <chrono>
#include <fstream>
#include "shared_arena.h"

class LogReader {

public:
    LogReader(std::vector<call_id_t> callIds, std::chrono::nanoseconds pollFrequency, const char *arenaName,
              uint64_t arenaBufferSize);

    void readTo(std::ofstream &logFile);

private:
    std::vector<call_id_t> callIds;
    std::chrono::nanoseconds pollFrequency;
    shared_arena_t *arena;
    uint64_t ringBufferSize;

    bool filterEntry(log_entry_t *entry);

    void logEntry(log_entry_t *entry);
};


#endif //LOGGER_LOGREADER_H
