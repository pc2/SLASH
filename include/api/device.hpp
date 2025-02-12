#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ami.h>
#include <ami_mem_access.h>
#include <ami_program.h>
#include <ami_sensor.h>

#include "parser/xml_parser.hpp"
#include "allocator/allocator.hpp"
#include "api/vrtbin.hpp"
#include "api/kernel.hpp"
#include "qdma/pcie_driver_handler.hpp"
#include "qdma/qdma_intf.hpp"
#include "driver/clk_wiz.hpp"
#include "utils/logger.hpp"
#include "utils/platform.hpp"
#include "utils/zmq_server.hpp"

#include <map>
#include <fcntl.h>
#include <unistd.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <thread>
#include <json/json.h>

namespace vrt {

    enum class ProgramType {
        FLASH,
        JTAG
    };


    #define JTAG_PROGRAM_PATH "/usr/local/vrt/jtag_program.sh "
    #define QDMA_SETUP_QUEUES "/usr/local/vrt/setup_queues.sh "
    #define DELAY_PARTIAL_BOOT (4 * 1000 * 1000)
    /**
     * @brief Class representing a device.
     */
    class Device {
        static constexpr uint64_t CLK_WIZ_BASE = 0x20100010000; ///< Base address for the clock wizard
        static constexpr uint32_t CLK_WIZ_OFFSET = 0x10000;
        ami_device* dev = nullptr; ///< Pointer to the AMI device
        uint8_t bar = 0; ///< Base Address Register (BAR)
        uint64_t offset = 0; ///< Offset for memory operations
        uint16_t pci_bdf = 0; ///< PCI Bus:Device.Function identifier
        std::string systemMap; ///< Path to the system map file
        std::string bdf; ///< Bus:Device.Function identifier
        std::string pdiPath; ///< Path to the PDI file
        Vrtbin vrtbin; ///< Vrtbin object for handling VRTBIN operations
        ClkWiz clkWiz; ///< Clock Wizard object for handling clock wizard operations
        uint64_t clockFreq; ///< Clock frequency
        ProgramType programType; ///< Type of programming
        std::map<std::string, Kernel> kernels; ///< Map of kernel names to Kernel objects
        PcieDriverHandler pcieHandler; ///< PCIe driver handler object
        Allocator* allocator; ///< Allocator object
        VrtbinType vrtbinType; ///< Type of VRTBIN
        Platform platform; ///< Platform information
        ZmqServer* zmqServer; ///< ZeroMQ server object
    public:

        QdmaIntf qdmaIntf; ///< QDMA interface object

        /**
         * @brief Constructor for Device.
         * @param bdf The Bus:Device.Function identifier.
         * @param vrtbinPath The path to the VRTBIN file.
         * @param program Flag indicating whether to program the device.
         */
        Device(const std::string& bdf, const std::string& vrtbinPath, bool program, ProgramType programType);

        Device() = default;
        /**
         * @brief Gets a kernel by name.
         * @param name The name of the kernel.
         * @return The Kernel object.
         */
        vrt::Kernel getKernel(const std::string& name);

        /**
         * @brief Gets the Bus:Device.Function identifier.
         * @return The Bus:Device.Function identifier.
         */
        std::string getBdf();

        /**
         * @brief Programs the device.
         */
        void programDevice();

        /**
         * @brief Sends a command to the PCIe driver.
         * @param cmd The command to send.
         */
        void sendPcieDriverCmd(std::string cmd);

        /**
         * @brief Boots the device.
         */
        void bootDevice();

        /**
         * @brief Gets a new handle for the device.
         */
        void getNewHandle();

        /**
         * @brief Creates the AMI device.
         */
        void createAmiDev();

        /**
         * @brief Destroys the AMI device.
         */
        void destroyAmiDev();

        /**
         * @brief Destructor for Device.
         */
        ~Device();

        /**
         * @brief Parses the system map file.
         */
        void parseSystemMap();

        /**
         * @brief Cleans up the device.
         */
        void cleanup();
        /**
         * @brief Sets clk_wiz frequency.
         */
        void setFrequency(uint64_t freq);

        /**
         * @brief Gets ami device.
         */
        ami_device* getAmiDev();

        /**
         * @brief Finds the VRTBIN type from system map.
         */
        void findVrtbinType();

        /**
         * @brief Finds the platform from system map.
         */
        void findPlatform();

        /**
         * @brief Gets the platform.
         */
        Platform getPlatform();

        /**
         * @brief Gets the ZMQ server.
         */
        ZmqServer* getZmqServer();

        /**
         * @brief Gets the Allocator instance.
         */
        Allocator* getAllocator();

    };

} // namespace vrt

#endif // DEVICE_HPP