#include <iostream>

#include "arg_parser.hpp"
#include "commands/inspect_command.hpp"
#include "commands/list_command.hpp"
#include "commands/partial_program_command.hpp"
#include "commands/program_command.hpp"
#include "commands/query_command.hpp"
#include "commands/reload_command.hpp"
#include "commands/reset_command.hpp"
#include "commands/resource_command.hpp"
#include "commands/validate_command.hpp"

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
        ResourceCommand resourceCommand(parser.getDevice());
    } else if (parser.isCommand("list")) {
        ListCommand listCommand(0x10ee, 0x50b4);
        listCommand.execute();
    } else if (parser.isCommand("program")) {
        ProgramCommand programCommand(parser.getDevice(), parser.getImagePath(),
                                      parser.getPartition());
        programCommand.execute();
    } else if (parser.isCommand("partial_program")) {
        PartialProgramCommand partialProgramCommand(parser.getDevice(), parser.getImagePath());
        partialProgramCommand.execute();
    } else if (parser.isCommand("inspect")) {
        InspectCommand inspectCommand(parser.getImagePath());
        inspectCommand.execute();
    } else if (parser.isCommand("reload")) {
        ReloadCommand reloadCommand(parser.getDevice());
        reloadCommand.execute();
    } else if (parser.isCommand("reset")) {
        ResetCommand resetCommand(parser.getDevice());
        resetCommand.execute();
    } else {
        parser.printHelp();
    }

    return EXIT_SUCCESS;
}