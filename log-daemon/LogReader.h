#ifndef LOGGER_LOGREADER_H
#define LOGGER_LOGREADER_H


#include <vector>
#include <chrono>
#include <fstream>
#include "SharedArena.h"

class LogReader {

public:
    /**
     * Constructs new log reader
     * @param callIds whitelist of calls to print
     * @param pollFrequency ring buffer polling frequency
     * @param arena pointer to shared ring buffer of logs
     */
    LogReader(std::vector<CallId> callIds, std::chrono::nanoseconds pollFrequency, SharedArena* arena);

    /**
     * Reads data to provided ostream infinitely
     * @param logFile output stream for call logs
     */
    void readTo(std::ofstream &logFile);

private:
    /**
     * Whitelist of calls
     */
    std::vector<CallId> callIds;
    /**
     * Ringbuffer poll frequency
     */
    std::chrono::nanoseconds pollFrequency;
    /**
     * Shared ring buffer
     */
    SharedArena *arena;
    /**
     * Saved size of ring buffer
     */
    uint64_t ringBufferSize;

    /**
     * @param entry log entry to check for filters
     * @return True if entry call is in callIds, false otherwise
     */
    bool filterEntry(LogEntry *entry);
};


#endif //LOGGER_LOGREADER_H
