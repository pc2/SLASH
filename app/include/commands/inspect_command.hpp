#ifndef INSPECT_COMMAND_HPP
#define INSPECT_COMMAND_HPP

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string>

#include "utils/vrtbin.hpp"

#define INSPECT_SYSTEM_MAP_PATH "/tmp/system_map.xml"  ///< Path to the system map XML file
#define INSPECT_VERSION_PATH "/tmp/version.json"       ///< Path to the version JSON file

/**
 * @brief Class for inspecting VRTBIN files.
 *
 * The InspectCommand class provides functionality to inspect and analyze VRTBIN
 * files, extracting metadata.
 */
class InspectCommand {
   public:
    /**
     * @brief Constructor for InspectCommand.
     *
     * @param image_path Path to the VRTBIN file to inspect.
     */
    InspectCommand(const std::string &image_path);

    /**
     * @brief Executes the inspection command.
     *
     * This method processes the VRTBIN file and displays relevant information.
     */
    void execute();

   private:
    std::string imagePath;  ///< Path to the VRTBIN file being inspected.

    /**
     * @brief Queries metadata from the VRTBIN file.
     *
     * Extracts and processes metadata information from the VRTBIN file.
     */
    void queryMetadata();
};

#endif  // INSPECT_COMMAND_HPP