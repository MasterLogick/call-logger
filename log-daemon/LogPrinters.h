#ifndef LOGGER_LOGPRINTERS_H
#define LOGGER_LOGPRINTERS_H

#include "SharedArena.h"
#include <fstream>

typedef void(*CustomLogPrinter)(LogEntry *, std::ofstream &);

/**
 * Description for a call log entry text format
 */
struct EntryFormatConfig {
    const char *name;
    int argsCount;
    bool showRetVal;
    CustomLogPrinter logPrinter;
};

void logEntry(LogEntry *entry, std::ofstream &ofstream);

void logOpen(LogEntry *entry, std::ofstream &ofstream);

/**
 * Describes text format for all intercepted calls. See also CallId enum
 */
constexpr EntryFormatConfig config[] = {
        {"undefined", 0, true,  logEntry},
        {"open",      2, true,  logOpen},
        {"close",     1, true,  logEntry},
        {"lseek",     3, true,  logEntry},
        {"read",      3, true,  logEntry},
        {"write",     3, true,  logEntry},
        {"malloc",    1, true,  logEntry},
        {"realloc",   2, true,  logEntry},
        {"free",      1, false, logEntry},
};

#endif //LOGGER_LOGPRINTERS_H
