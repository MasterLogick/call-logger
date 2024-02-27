#include <cstring>
#include "LogPrinters.h"

void logEntry(LogEntry *entry, std::ofstream &ofstream) {
    auto entryFormatConfig = config[entry->id];
    char name[sizeof(entry->procName)];
    memcpy(name, entry->procName, sizeof(name));
    name[sizeof(name) - 1] = 0;
    ofstream
            << name << ":"
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

void logOpen(LogEntry *entry, std::ofstream &ofstream) {
    char name[sizeof(entry->procName)];
    memcpy(name, entry->procName, sizeof(name));
    name[sizeof(name) - 1] = 0;
    ofstream
            << name << ":"
            << std::dec << entry->procPid << " "
            << "open(";
    char filename[sizeof(entry->callSpecificData.openPath)];
    memcpy(filename, entry->callSpecificData.openPath, sizeof(filename));
    filename[sizeof(filename) - 1] = 0;
    ofstream << filename << ", " << std::hex << entry->args[1] << ")=" << entry->retVal << std::endl;
}
