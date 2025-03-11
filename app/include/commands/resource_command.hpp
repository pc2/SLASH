#ifndef RESOURCE_COMMAND_HPP
#define RESOURCE_COMMAND_HPP

#include <string>
#include <iomanip>
#include <iostream>
#include <regex>
#include <utility>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <cstdlib>

#define RESOURCE_UTILIZATION_FILE "%s/%s:00.0/report_utilization.xml"

struct Instance {
    std::string name;
    std::string module;
    std::pair<int, float> totalLUTs;
    std::pair<int, float> logicLUTs;
    std::pair<int, float> lutRAMs;
    std::pair<int, float> srls;
    std::pair<int, float> ffs;
    std::pair<int, float> ramb36;
    std::pair<int, float> ramb18;
    std::pair<int, float> uram;
    std::pair<int, float> dspBlocks;
    std::vector<Instance> children;
};


class ResourceCommand {
public:
    ResourceCommand(const std::string& device);
    void execute() const;

private:
    std::string device;
    Instance rootInstance;

    void parseXML(const std::string& filename);
    void parseInstance(xmlNode * a_node, Instance& instance);
    void printResources();
    void printInstance(const Instance& instance, int level, bool& headerPrinted) const;
    void printSpecificInstance(const Instance& instance, const std::string& targetName, bool headerPrinted) const;
    void printChildrenOfBaseLogic(const Instance& instance, const std::vector<std::string>& excludeList, bool& headerPrinted) const;
};

#endif // RESOURCE_COMMAND_HPP