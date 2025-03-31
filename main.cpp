#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>


int main() {
	int fd = open("/dev/pcie_hotplug", O_WRONLY);
    if(fd < 0) {
        std::cerr << "Could not open device\n";
        return -1;
    }

    std::string cmd = "hotplug";
    if(write(fd, cmd.c_str(), cmd.size()) < 0) {
        std::cerr << "Could not write to device\n";
        return -1;
    }
    close(fd);

}
