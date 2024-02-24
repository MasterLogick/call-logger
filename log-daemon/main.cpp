#include <iostream>
#include <fstream>
#include "shared_arena.h"
#include "ProgramArgs.h"
#include "LogReader.h"

int main(int argc, char **argv) {
    ProgramArgs args(argc, argv);
    if (!args.isInitialized()) {
        std::cout << "Usage: log-daemon -t poll_frequency_ns -f comma_separated_list_of_calls log_file" << std::endl;
        return -1;
    }
    std::ofstream logFile(args.getLogFile());
    LogReader reader(args.getCallList(), args.getPollFrequency(), DEFAULT_ARENA_NAME, DEFAULT_ARENA_BUFFER_SIZE);
    reader.readTo(logFile);
    return 0;
}
