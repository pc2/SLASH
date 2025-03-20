#ifndef ARG_PARSER_HPP
#define ARG_PARSER_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>

class ArgParser {
public:
    ArgParser();
    void parse(int argc, char* argv[]);
    std::string getDevice() const;
    std::string getImagePath() const;
    uint8_t getPartition() const;
    bool isCommand(const std::string& command) const;
    void printHelp() const;
    static bool endsWith(const std::string& str, const std::string& suffix);

private:
    std::unordered_map<std::string, std::function<void()>> commands;
    std::string device;
    std::string image;
    uint8_t partition = -1;
    std::string currentCommand;

    void addCommand(const std::string& command, const std::function<void()>& handler);
    std::string convertBdf(const std::string& bdf) const;
    std::string strip(const std::string& bdf) const;
};

#endif // ARG_PARSER_HPP