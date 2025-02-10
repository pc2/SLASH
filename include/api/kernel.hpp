#ifndef KERNEL_HPP
#define KERNEL_HPP

#include "register/register.hpp"
#include "utils/logger.hpp"
#include "utils/platform.hpp"
#include "utils/zmq_server.hpp"

#include <string>
#include <ami.h>
#include <ami_mem_access.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <regex>
#include <string>
#include <memory>
#include <json/json.h>
#include <type_traits>


namespace vrt {
    class Device;
    template<typename T> class Buffer;

    /**
     * @brief Class representing a kernel.
     */
    class Kernel {
        static constexpr uint64_t BASE_BAR_ADDR = 0x20100000000; ///< Base BAR address
        uint8_t bar = 0; ///< Base Address Register (BAR)
        ami_device* dev = nullptr; ///< Pointer to the AMI device
        std::string name; ///< Name of the kernel
        uint64_t baseAddr; ///< Base address of the kernel
        uint64_t range; ///< Address range of the kernel
        std::vector<Register> registers; ///< List of registers in the kernel
        size_t currentRegisterIndex = 4; ///< Index of the current register being processed
        std::string deviceBdf; ///< BDF of the device
        Platform platform; ///< Platform of the device
        ZmqServer* server; ///< Pointer to ZeroMQ server for communication
    public:
        /**
         * @brief Constructor for Kernel.
         * @param device Pointer to the AMI device.
         * @param name The name of the kernel.
         * @param baseAddr The base address of the kernel.
         * @param range The address range of the kernel.
         * @param registers The list of registers in the kernel.
         */
        Kernel(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range, const std::vector<Register>& registers);

        /**
         * @brief Default constructor for Kernel.
         */
        Kernel() = default;

        /**
         * @brief Constructor for Kernel using a Device object.
         * @param device The Device object.
         * @param kernelName The name of the kernel.
         */
        Kernel(vrt::Device device, const std::string& kernelName);

        /**
         * @brief Sets the device for the kernel.
         * @param device Pointer to the AMI device.
         */
        void setDevice(ami_device* device);

        /**
         * @brief Writes a value to a register.
         * @param offset The offset of the register.
         * @param value The value to write.
         */
        void write(uint32_t offset, uint32_t value);

        /**
         * @brief Reads a value from a register.
         * @param offset The offset of the register.
         * @return The value read from the register.
         */
        uint32_t read(uint32_t offset);

        /**
         * @brief Waits for the kernel to complete.
         */
        void wait();

        /**
         * @brief Starts the kernel.
         * @param autorestart Flag indicating whether to enable autorestart.
         */
        void startKernel(bool autorestart=false);

        /**
         * @brief Sets the platform for the kernel.
         * @param platform The platform to set.
         */
        void setPlatform(Platform platform);

        /**
         * @brief Calls the kernel.
         */
        template<typename... Args>
            void call(Args... args) {
                currentRegisterIndex = 4;
                if(platform == Platform::HARDWARE) {
                    (processArg(args), ...);
                    this->startKernel();
                    this->wait();
                } else if(platform == Platform::EMULATION) {
                    Json::Value command;
                    command["command"] = "call";
                    command["function"] = name;
                    int argIdx = 0;
                    (processEmuArg(args, command, argIdx), ...);
                    server->sendCommand(command);
                }
            }

        /**
         * @brief Async kernel call.
         */
        template<typename... Args>
            void start(Args... args) {
                currentRegisterIndex = 4;
                if(platform == Platform::HARDWARE) {
                    (processArg(args), ...);
                    this->startKernel();

                } else if(platform == Platform::EMULATION) {
                    Json::Value command;
                    command["command"] = "call";
                    command["function"] = name;
                    int argIdx = 0;
                    (processEmuArg(args, command, argIdx), ...);
                    server->sendCommand(command);
                }
            }
        /**
         * @brief Helper method which processes an argument.
         * @tparam T The type of the argument.
         * @param arg The argument to process.
         */
        template<typename T>
        void processArg(T arg) {
            if (currentRegisterIndex < registers.size()) {
                std::regex re(".*_\\d+$"); // Regular expression to match strings ending with _nr
                if (std::regex_match(registers.at(currentRegisterIndex).getRegisterName(), re)) {
                    this->write(registers.at(currentRegisterIndex).getOffset(), arg & 0xFFFFFFFF);
                    this->write(registers.at(currentRegisterIndex + 1).getOffset(), static_cast<uint32_t>((static_cast<uint64_t>(arg) >> 32) & 0xFFFFFFFF));
                    // std::cout << "Register: " << registers.at(currentRegisterIndex).getRegisterName() << " Value: " << std::hex << (arg & 0xFFFFFFFF) << std::endl;
                    // std::cout << "Register: " << registers.at(currentRegisterIndex + 1).getRegisterName() << " Value: " << std::hex <<((arg >> 32) & 0xFFFFFFFF) << std::endl;
                    currentRegisterIndex+=2;
                } else {
                    this->write(registers.at(currentRegisterIndex).getOffset(), arg);
                    currentRegisterIndex++;
                }
                
            } else {
                //utils::Logger::log(utils::LogLevel::ERROR, __PRETTY_FUNCTION__, "Not enough registers to process all arguments.");
                throw std::runtime_error("Not enough registers to process all arguments.");
            }
        }

        /**
         * @brief Helper method which processes an argument for emulation.
         * @tparam T The type of the argument.
         * @param arg The argument to process.
         * @param command The JSON command to update.
         * @param argIndex The index of the argument.
         */
        template<typename T>
        void processEmuArg(T arg, Json::Value& command, int& argIndex) {
            if (currentRegisterIndex < registers.size()) {
                std::regex re(".*_\\d+$"); // Regular expression to match strings ending with _nr
                if (std::regex_match(registers.at(currentRegisterIndex).getRegisterName(), re)) {
                    command["args"]["arg" + std::to_string(argIndex)]["type"] = "buffer";
                    command["args"]["arg" + std::to_string(argIndex)]["name"] = std::to_string(arg);
                    currentRegisterIndex+=2;
                } else {
                    command["args"]["arg" + std::to_string(argIndex)]["type"] = "scalar";
                    command["args"]["arg" + std::to_string(argIndex)]["value"] = arg;
                    currentRegisterIndex++;
                }
                argIndex++;
            } else {
                throw std::runtime_error("Not enough registers to process all arguments.");
            }
        }

         /**
         * @brief Destructor for Kernel.
         */
        ~Kernel();
    };

} // namespace vrt

#endif // KERNEL_HPP