#ifndef LOGGER_PROGRAMARGS_H
#define LOGGER_PROGRAMARGS_H


#include <cstring>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>
#include "SharedArena.h"

class ProgramArgs {

public:
    ProgramArgs(int argc, char **argv);

    const std::chrono::nanoseconds &getPollFrequency() const {
        return pollFrequency;
    }

    const std::vector<CallId> &getCallList() const {
        return callList;
    }

    const std::string &getLogFile() const {
        return logFile;
    }

    bool isInitialized() const {
        return initialized;
    }
private:
    std::chrono::nanoseconds pollFrequency;
    std::vector<CallId> callList;
    std::string logFile;
    bool initialized;
    static bool filterStringToCallIdsVector(const std::string &filter, std::vector<CallId> &callIds);
};


#endif //LOGGER_PROGRAMARGS_H
