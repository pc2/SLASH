#include "arg_parser.hpp"
#include "commands/list_command.hpp"
#include <iostream>
#include <getopt.h>
#include <regex>

ArgParser::ArgParser() {
    addCommand("query", [this]() { currentCommand = "query"; });
    addCommand("validate", [this]() { currentCommand = "validate"; });
    addCommand("report_utilization", [this]() { currentCommand = "report_utilization"; });
    addCommand("list", [this]() { currentCommand = "list"; });
}

void ArgParser::parse(int argc, char* argv[]) {
    static struct option long_options[] = {
        {"device", required_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "d:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'd':
                device = convertBdf(optarg);
                break;
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case '?':
                printHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    if (optind < argc) {
        std::string command = argv[optind];
        auto it = commands.find(command);
        if (it != commands.end()) {
            it->second();
        } else {
            std::cerr << "Error: Unknown command " << command << std::endl;
            printHelp();
            exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Error: No command specified" << std::endl;
        printHelp();
        exit(EXIT_FAILURE);
    }
}

std::string ArgParser::getDevice() const {
    return device;
}

bool ArgParser::isCommand(const std::string& command) const {
    return currentCommand == command;
}

void ArgParser::printHelp() const {
    std::cout << "Usage: v80-smi <command> [options]\n"
              << "Commands:\n"
              << "  query                Query the device\n"
              << "  validate             Validate the device\n"
              << "  report_utilization   Report device utilization for the current programmed shell\n"
              << "  list                 List V80s installed\n"
              << "Options:\n"
              << "  -d, --device <device>  Specify the device (e.g., 21:00.0)\n"
              << "  -h, --help             Show this help message\n";
}

void ArgParser::addCommand(const std::string& command, const std::function<void()>& handler) {
    commands[command] = handler;
}

std::string ArgParser::convertBdf(const std::string& bdf) const {
    std::regex pattern("^0000:(.*)");
    std::smatch match;
    if (std::regex_match(bdf, match, pattern)) {
        return match[1].str();
    }
    return strip(bdf);
}

std::string ArgParser::strip(const std::string& bdf) const {
    size_t colonPos = bdf.find(':');
    if (colonPos != std::string::npos) {
        return bdf.substr(0, colonPos);
    }
    return bdf;
}