#ifndef INSPECT_COMMAND_HPP
#define INSPECT_COMMAND_HPP

#include <string>
#include "utils/vrtbin.hpp"
#include <libxml/parser.h>
#include <libxml/tree.h>

#define INSPECT_SYSTEM_MAP_PATH "/tmp/system_map.xml"
#define INSPECT_VERSION_PATH "/tmp/version.json"


class InspectCommand {
public:
    InspectCommand(const std::string& image_path);
    void execute();

private:
    std::string imagePath;
    void queryMetadata();
};

#endif // INSPECT_COMMAND_HPP