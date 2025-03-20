#ifndef QUERY_COMMAND_HPP
#define QUERY_COMMAND_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <ami.h>
#include <ami_device.h>
#include <ami_mfg_info.h>
#include "utils/vrtbin.hpp"

#define SYSTEM_MAP_PATH "%s/%s:00.0/system_map.xml"
#define VERSION_PATH "%s/%s:00.0/version.json"
#define QDMA_QUEUE_PATH "/dev/qdma%s001-MM-0" // /dev/qdma<bus>:<dev>.<func>-MM-<queue>
#define QDMA_QMAX_PATH "/sys/bus/pci/devices/0000:%s:00.1/qdma/qmax"
#define BUFFER_SIZE 1024

#define META_MAX_STR_LEN 256
#define MFG_TIMESTAMP_BASE 16
#define MFG_DATE_TM_YEAR 70 // Base year 1970

class QueryCommand {
public:
    QueryCommand(const std::string& device);
    void execute();

private:
    std::string device;

    void queryDevice();
    void queryKernels(const std::string& bdf);
    void queryQueues(const std::string& bdf);
    void printAmiDetails();
    void formatManufacturingDate(long);
};

#endif // QUERY_COMMAND_HPP