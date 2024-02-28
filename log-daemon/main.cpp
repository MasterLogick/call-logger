#include <iostream>
#include <fstream>
#include "SharedArena.h"
#include "ProgramArgs.h"
#include "LogReader.h"

int main(int argc, char **argv) {
    ProgramArgs args(argc, argv);
    if (!args.isInitialized()) {
        std::cout << "Usage: log-daemon -t poll_frequency -f comma_separated_list_of_calls log_file" << std::endl;
        return -1;
    }
    std::ofstream logFile(args.getLogFile());
    SharedArena *arena = SharedArena::createOrConnect();
    if (arena == nullptr) {
        std::cout << "Failed to connect to shared log ring buffer" << std::endl;
        return 1;
    }
    LogReader reader(args.getCallList(), args.getPollFrequency(), arena);
    reader.readTo(logFile);
    return 0;
}
