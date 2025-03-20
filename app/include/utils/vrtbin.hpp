#ifndef VRTBIN_HPP
#define VRTBIN_HPP

#include <ami.h>
#include <ami_mem_access.h>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>
#include <iostream>
#include <string>
#include <ami_program.h>
#include <unistd.h>

class Vrtbin {
public:
    static void extract(std::string source, std::string destination);
    static void copy(const std::string& source, const std::string& destination);
    static void progressHandler(enum ami_event_status status, uint64_t ctr, void* data);
    static char print_progress_bar(uint32_t cur, uint32_t max, uint32_t width,
	    char left, char right, char fill, char empty, char state);
    static std::string extractUUID();
    static void extractAndPrintInfo(const std::string& path);
};

#endif // VRTBIN_HPP