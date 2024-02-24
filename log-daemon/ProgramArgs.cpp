#include "ProgramArgs.h"

ProgramArgs::ProgramArgs(int argc, char **argv) {
    initialized = false;
    if (argc != 6 || strcmp(argv[1], "-t") != 0 || strcmp(argv[3], "-f") != 0) {
        return;
    }

    char *pollFreqArgEnd = nullptr;
    long pollFrequencyNs = std::strtol(argv[2], &pollFreqArgEnd, 10);
    pollFrequency = std::chrono::nanoseconds(pollFrequencyNs);
    if (!(*argv[2] != '\0' && *pollFreqArgEnd == '\0')) {
        return;
    }

    std::string callListStr(argv[4]);
    if (!filterStringToCallIdsVector(callListStr, callList)) {
        return;
    }

    logFile = argv[5];
    initialized = true;
}

bool ProgramArgs::filterStringToCallIdsVector(const std::string &filter, std::vector<CallId> &callIds) {
    std::stringstream filterStream(filter);
    for (std::string call; std::getline(filterStream, call, ',');) {
        if (call == "open") callIds.push_back(CallId_open);
        else if (call == "close") callIds.push_back(CallId_close);
        else if (call == "lseek") callIds.push_back(CallId_lseek);
        else if (call == "read") callIds.push_back(CallId_read);
        else if (call == "write") callIds.push_back(CallId_write);
        else if (call == "malloc") callIds.push_back(CallId_malloc);
        else if (call == "realloc") callIds.push_back(CallId_realloc);
        else if (call == "free") callIds.push_back(CallId_free);
        else return false;
    }
    return true;
}
