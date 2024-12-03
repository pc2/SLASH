#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <string>
#include <map>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "api/kernel.hpp" // Include the Kernel class

namespace vrt {
    class XMLParser {
        std::string filename;
        xmlDocPtr document;
        xmlNode* rootNode;
        xmlNode* workingNode;
        std::map<std::string, Kernel> kernels;

        public:
        XMLParser(const std::string& file);
        void parseXML();
        static std::string convertFromXmlCharPtr(const xmlChar*);
        std::map<std::string, Kernel> getKernels();
        ~XMLParser();
    };
} // namespace vrt

#endif // XML_PARSER_HPP