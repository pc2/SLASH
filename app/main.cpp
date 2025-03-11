#include "arg_parser.hpp"
#include "commands/list_command.hpp"
#include "commands/query_command.hpp"
#include "commands/validate_command.hpp"
#include "commands/resource_command.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    ArgParser parser;
    parser.parse(argc, argv);

    if (parser.isCommand("query")) {
        std::string device = parser.getDevice();
        if (!device.empty()) {
            QueryCommand queryCommand(device);
            queryCommand.execute();
        } else {
            std::cerr << "Error: Device not specified" << std::endl;
            parser.printHelp();
            return EXIT_FAILURE;
        }
    } else if (parser.isCommand("validate")) {
        std::cout << "Validating device..." << std::endl;
        ValidateCommand validateCommand(parser.getDevice());
        validateCommand.execute();
    } else if (parser.isCommand("report_utilization")) {
        std::cout << "Reporting device utilization..." << std::endl;
    } else if (parser.isCommand("list")) {
        ListCommand listCommand(0x10ee, 0x50b4);
        listCommand.execute();
    } if (parser.isCommand("report_utilization")) {
        ResourceCommand resourceCommand(parser.getDevice());
        // resourceCommand.execute();
    } else {
        parser.printHelp();
    }

    return EXIT_SUCCESS;
}