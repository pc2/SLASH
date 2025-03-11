#ifndef LIST_COMMAND_HPP
#define LIST_COMMAND_HPP

#include <string>

class ListCommand {
public:
    ListCommand(uint16_t vendorId, uint16_t deviceId);
    void execute() const;

private:
    uint16_t vendorId;
    uint16_t deviceId;

    void listDevices() const;
};

#endif // LIST_COMMAND_HPP