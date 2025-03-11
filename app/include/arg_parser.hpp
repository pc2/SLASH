#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <functional>

class ArgParser {
public:
    ArgParser();
    void parse(int argc, char* argv[]);
    std::string getDevice() const;
    bool isCommand(const std::string& command) const;
    void printHelp() const;

private:
    std::unordered_map<std::string, std::function<void()>> commands;
    std::string device;
    std::string currentCommand;

    void addCommand(const std::string& command, const std::function<void()>& handler);
    std::string convertBdf(const std::string& bdf) const;
    std::string strip(const std::string& bdf) const;
};

#endif // ARG_PARSER_HPP