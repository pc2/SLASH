#ifndef LIST_COMMAND_HPP
#define LIST_COMMAND_HPP

#include <string>

/**
 * @brief Class for listing available V80 devices.
 *
 * The ListCommand class provides functionality to list available devices
 * that match the specified vendor and device IDs.
 */
class ListCommand {
   public:
    /**
     * @brief Constructor for ListCommand.
     *
     * @param vendorId The vendor ID to filter devices by.
     * @param deviceId The device ID to filter devices by.
     */
    ListCommand(uint16_t vendorId, uint16_t deviceId);

    /**
     * @brief Executes the list command.
     *
     * This method lists all available devices that match the specified
     * vendor and device IDs.
     */
    void execute() const;

   private:
    uint16_t vendorId;  ///< The vendor ID to filter devices by.
    uint16_t deviceId;  ///< The device ID to filter devices by.

    /**
     * @brief Lists all matching devices.
     *
     * This method performs the actual listing of devices that match the
     * specified vendor and device IDs.
     */
    void listDevices() const;
};

#endif  // LIST_COMMAND_HPP