#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "api/kernel.hpp" // Include the Kernel class
#include "utils/platform.hpp"
#include "qdma/qdma_connection.hpp"

namespace vrt {
    class Kernel;

    /**
     * @brief Enum class for the different types of VRT bins.
     */
    enum class VrtbinType {
        FLAT,
        SEGMENTED
        // PARTIAL when implemented
    };

    /**
     * @brief Class for parsing XML files to extract kernel information.
     */
    class XMLParser {
        std::string filename; ///< The name of the XML file to parse.
        xmlDocPtr document; ///< Pointer to the parsed XML document.
        xmlNode* rootNode; ///< Pointer to the root node of the XML document.
        xmlNode* workingNode; ///< Pointer to the current working node in the XML document.
        std::map<std::string, Kernel> kernels; ///< Map of kernel names to Kernel objects.
        uint64_t clockFrequency; ///< The clock frequency of the device.
        VrtbinType vrtbinType; ///< The VRT bin type of the device.
        Platform platform; ///< The platform of the device.
        std::vector<QdmaConnection> qdmaConnections; ///< Vector of QDMA connections.

    public:
        /**
         * @brief Constructor for XMLParser.
         * @param file The name of the XML file to parse.
         */
        XMLParser(const std::string& file);

        /**
         * @brief Parses the XML file.
         */
        void parseXML();

        /**
         * @brief Converts an xmlChar pointer to a std::string.
         * @param xmlCharPtr The xmlChar pointer to convert.
         * @return The converted std::string.
         */
        static std::string convertFromXmlCharPtr(const xmlChar* xmlCharPtr);

        /**
         * @brief Gets the map of kernels parsed from the XML file.
         * @return The map of kernel names to Kernel objects.
         */
        std::map<std::string, Kernel> getKernels();

        /**
         * @brief Gets the clock frequency of the device.
         * @return The clock frequency of the device.
         */
        uint64_t getClockFrequency();

        /**
         * @brief Gets the VRT bin type of the device.
         * @return The VRT bin type of the device.
         */
        VrtbinType getVrtbinType();

        /**
         * @brief Gets the platform of the device.
         * @return The platform of the device.
         */
        Platform getPlatform();

        /**
         * @brief Gets the vector of QDMA connections.
         * @return The vector of QDMA connections.
         */
        std::vector<QdmaConnection> getQdmaConnections();

        /**
         * @brief Destructor for XMLParser.
         */
        ~XMLParser();
    };

} // namespace vrt

#endif // XML_PARSER_HPP