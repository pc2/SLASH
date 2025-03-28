#include "commands/inspect_command.hpp"

InspectCommand::InspectCommand(const std::string& image_path) : imagePath(image_path) {}

void InspectCommand::execute() {
    Vrtbin::extract(this->imagePath, "/tmp");
    queryMetadata();
}

void InspectCommand::queryMetadata() {
    xmlDocPtr document = xmlReadFile(INSPECT_SYSTEM_MAP_PATH, NULL, 0);
    if (document == NULL) {
        std::cerr << "Error: could not read system map file" << std::endl;
        return;
    }
    xmlNode* rootNode = xmlDocGetRootElement(document);
    if (rootNode == NULL) {
        std::cerr << "Error: could not get root element" << std::endl;
        return;
    }

    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "VRTBIN Information\n";
    std::cout << "--------------------------------------------------------------------\n";
    std::string platform, type, clockFrequency;

    for (xmlNode* kernelNode = rootNode->children; kernelNode; kernelNode = kernelNode->next) {
        if (kernelNode->type == XML_ELEMENT_NODE &&
            xmlStrcmp(kernelNode->name, BAD_CAST "Platform") == 0) {
            platform = (const char*)xmlNodeGetContent(kernelNode);
            std::cout << "Platform                    | " << platform << "\n";
        } else if (kernelNode->type == XML_ELEMENT_NODE &&
                   xmlStrcmp(kernelNode->name, BAD_CAST "Type") == 0) {
            type = (const char*)xmlNodeGetContent(kernelNode);
            std::cout << "Type                        | " << type << "\n";
        } else if (kernelNode->type == XML_ELEMENT_NODE &&
                   xmlStrcmp(kernelNode->name, BAD_CAST "ClockFrequency") == 0) {
            clockFrequency = (const char*)xmlNodeGetContent(kernelNode);
            std::cout << "Max clock Frequency         | " << clockFrequency << " Hz\n";
        }
    }
    std::cout << "\n";
    Vrtbin::extractAndPrintInfo(INSPECT_VERSION_PATH);

    for (xmlNode* kernelNode = rootNode->children; kernelNode; kernelNode = kernelNode->next) {
        if (kernelNode->type == XML_ELEMENT_NODE &&
            xmlStrcmp(kernelNode->name, BAD_CAST "Kernel") == 0) {
            std::string name, baseAddr, range;
            for (xmlNode* childNode = kernelNode->children; childNode;
                 childNode = childNode->next) {
                if (childNode->type == XML_ELEMENT_NODE) {
                    if (xmlStrcmp(childNode->name, BAD_CAST "Name") == 0) {
                        name = (char*)xmlNodeGetContent(childNode);
                    } else if (xmlStrcmp(childNode->name, BAD_CAST "BaseAddress") == 0) {
                        baseAddr = (char*)xmlNodeGetContent(childNode);
                    } else if (xmlStrcmp(childNode->name, BAD_CAST "Range") == 0) {
                        range = (char*)xmlNodeGetContent(childNode);
                    }
                }
            }
            std::cout << "--------------------------------------------------------------------\n";
            std::cout << "Kernel Information\n";
            std::cout << "--------------------------------------------------------------------\n";
            std::cout << "Kernel Name                 | " << name << "\n";
            std::cout << "Base Address                | " << baseAddr << "\n";
            std::cout << "Range                       | " << range << "\n\n";
        }
    }
}
