#include "api/device.hpp"

namespace vrt {

    Device::Device(const std::string& bdf, const std::string& vrtbinPath, bool program, ProgramType programType) : vrtbin(vrtbinPath, bdf), clkWiz(nullptr, "", 0, 0, 0), pcieHandler(bdf) {
        this->bdf = bdf;
        this->allocator = new Allocator(4096);
        this->systemMap = this->vrtbin.getSystemMapPath();
        this->pdiPath = this->vrtbin.getPdiPath();
        this->programType = programType;
        this->qdmaIntf = QdmaIntf(bdf);
        this->zmqServer = new ZmqServer();
        findPlatform();
        if(platform == Platform::HARDWARE) {
            createAmiDev();
            findVrtbinType();
            if(program) {
                programDevice();
            }
            parseSystemMap();
            this->clkWiz.setRateHz(clockFreq, false);
        } else if(platform == Platform::EMULATION) {
            parseSystemMap();
            std::string emulationExecPath = this->vrtbin.getEmulationExec() + " >/dev/null";

            std::thread([emulationExecPath]() {
                std::system(emulationExecPath.c_str());
            }).detach();

        } else {
            throw std::runtime_error("Unsupported platform simulation");
        }
    }

    Device::~Device() {
        //destroyAmiDev();
    }

    void Device::parseSystemMap() {
        XMLParser parser(systemMap);
        parser.parseXML();
        clockFreq = parser.getClockFrequency();
        this->platform = parser.getPlatform();
        this->clkWiz = ClkWiz(dev, "clk_wiz", CLK_WIZ_BASE, CLK_WIZ_OFFSET, clockFreq);
        this->clkWiz.setPlatform(platform);
        kernels = parser.getKernels();
        for(auto& kernel : kernels) {
            kernel.second.setDevice(dev);
        }
        //this->vrtbinType = parser.getVrtbinType();
    }
    Kernel Device::getKernel(const std::string& name) {
        return kernels[name];
    }

    void Device::cleanup() {
        if(platform == Platform::HARDWARE) {
            ami_dev_delete(&dev);
        } else if(platform == Platform::EMULATION) {
            Json::Value exit;
            exit["command"] = "exit";
            zmqServer->sendCommand(exit);
        }
    }

    std::string Device::getBdf() {
        return bdf;
    }

    void Device::programDevice() {
        if(vrtbinType == VrtbinType::FLAT) {
            if(programType == ProgramType::FLASH) {
                char current_uuid[33];
                std::string logic_uuid = vrtbin.getUUID();
                int found_current_uuid = AMI_STATUS_ERROR;
                found_current_uuid = ami_dev_read_uuid(dev, current_uuid);
                if(found_current_uuid == AMI_STATUS_OK) {
                    std::string current_uuid_str(current_uuid);
                    current_uuid_str = current_uuid_str.substr(0, 32);
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Current UUID: {}", current_uuid_str);
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New UUID: {}", logic_uuid);
                    if(current_uuid_str == logic_uuid) {
                        utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Device already programmed with the same image");
                        bootDevice();
                        return;
                    }
                }
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Programming device {} in FLASH mode...This might take a while", bdf);
                if(ami_prog_download_pdi(dev, pdiPath.c_str(), 0, 1, nullptr, false) != AMI_STATUS_OK) {
                    throw std::runtime_error("Failed to program device");
                }
                bootDevice();
            } else {
                int found_current_uuid = AMI_STATUS_ERROR;
                char current_uuid[33];
                std::string logic_uuid = vrtbin.getUUID();
                found_current_uuid = ami_dev_read_uuid(dev, current_uuid);
                if(found_current_uuid == AMI_STATUS_OK) {
                    std::string current_uuid_str(current_uuid);
                    current_uuid_str = current_uuid_str.substr(0, 32);
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Current UUID: {}", current_uuid_str);
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New UUID: {}", logic_uuid);
                    if(current_uuid_str == logic_uuid) {
                        utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Device already programmed with the same image");
                        bootDevice();
                        return;
                    }
                }
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Programming device {} in JTAG mode...This might take a while", bdf);
                std::string cmd = JTAG_PROGRAM_PATH + pdiPath;
                system(cmd.c_str());
                bootDevice();
            }
        } else if(vrtbinType == VrtbinType::SEGMENTED) {
            utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Programming device {} in SEGMENTED mode...This might take a while", bdf);
            char current_uuid[33];
            std::string logic_uuid = vrtbin.getUUID();
            int found_current_uuid = AMI_STATUS_ERROR;
            found_current_uuid = ami_dev_read_uuid(dev, current_uuid);
            if(found_current_uuid == AMI_STATUS_OK) {
                std::string current_uuid_str(current_uuid);
                current_uuid_str = current_uuid_str.substr(0, 32);
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Current UUID: {}", current_uuid_str);
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New UUID: {}", logic_uuid);
                if(current_uuid_str == logic_uuid) {
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Device already programmed with the same image");
                    //bootDevice();
                    return;
                }
            }
            bootDevice();
        }
    }

    void Device::bootDevice() {
        utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Booting device...");
        if(vrtbinType == VrtbinType::FLAT) {
            if(programType == ProgramType::FLASH) {
                int ret = ami_prog_device_boot(&dev, 1);
                if(ret != AMI_STATUS_OK && geteuid() == 0) { // for root users this should not matter
                    throw std::runtime_error("Failed to boot device");
                }
                else {
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Booting into PDI...");
                    utils::Logger::log(utils::LogLevel::DEBUG, __PRETTY_FUNCTION__, "Writing PMC GPIO...");
                    ami_mem_bar_write(dev, 0, 0x1040000, 1);
                    destroyAmiDev();
                    pcieHandler.execute(PcieDriverHandler::Command::REMOVE);
                    pcieHandler.execute(PcieDriverHandler::Command::TOGGLE_SBR);
                    pcieHandler.execute(PcieDriverHandler::Command::RESCAN);
                    pcieHandler.execute(PcieDriverHandler::Command::HOTPLUG);
                    createAmiDev();
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New PDI booted successfully");
                    std::string cmd = "sudo " + std::string(QDMA_SETUP_QUEUES) + bdf;
                    system(cmd.c_str());
                    utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "QDMA queues setup successfully");
                }
            } else {
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Booting into PDI...");
                destroyAmiDev();
                pcieHandler.execute(PcieDriverHandler::Command::REMOVE);
                pcieHandler.execute(PcieDriverHandler::Command::RESCAN);
                pcieHandler.execute(PcieDriverHandler::Command::HOTPLUG);
                createAmiDev();
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New PDI booted successfully");
                std::string cmd = "sudo " + std::string(QDMA_SETUP_QUEUES) + bdf;
                system(cmd.c_str());
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "QDMA queues setup successfully");
            }
        } else if(vrtbinType == VrtbinType::SEGMENTED) {
            int ret = ami_prog_device_boot(&dev, 1); // make sure we are on partition one, this contains the segmented base pdi
            if(ret != AMI_STATUS_OK && geteuid() == 0) {
                throw std::runtime_error("Failed to boot into base segmented PDI");
            } else {
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Booting into base segmented PDI...");
                utils::Logger::log(utils::LogLevel::DEBUG, __PRETTY_FUNCTION__, "Writing PMC GPIO...");
                ami_mem_bar_write(dev, 0, 0x1040000, 1);
                destroyAmiDev();
                pcieHandler.execute(PcieDriverHandler::Command::REMOVE);
                pcieHandler.execute(PcieDriverHandler::Command::TOGGLE_SBR);
                pcieHandler.execute(PcieDriverHandler::Command::RESCAN);
                pcieHandler.execute(PcieDriverHandler::Command::HOTPLUG);
                createAmiDev();
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Base segmented PDI booted successfully");
                if(ami_prog_download_pdi(dev, pdiPath.c_str(), 0, 1, nullptr, true) != AMI_STATUS_OK) {
                    throw std::runtime_error("Failed to program partial device");
                }
                destroyAmiDev();
                pcieHandler.execute(PcieDriverHandler::Command::REMOVE);
                usleep(DELAY_PARTIAL_BOOT);
                pcieHandler.execute(PcieDriverHandler::Command::RESCAN);
                pcieHandler.execute(PcieDriverHandler::Command::HOTPLUG);
                createAmiDev();
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "PLD PDI booted successfully");
                std::string cmd = "sudo " + std::string(QDMA_SETUP_QUEUES) + bdf;
                system(cmd.c_str());
                utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "QDMA queues setup successfully");
            }
        }
    }

    void Device::getNewHandle() {
        ami_device *new_dev = NULL;
        int ret = AMI_STATUS_ERROR;
        ret = ami_dev_find_next(&new_dev, AMI_PCI_BUS(pci_bdf), AMI_PCI_DEV(pci_bdf), AMI_PCI_FUNC(pci_bdf), NULL);
        if(ret == AMI_STATUS_OK) {
            if(ami_sensor_discover(new_dev) == AMI_STATUS_OK) {
                dev = new_dev;
            } else {
                throw std::runtime_error("Failed to discover sensors");
            }
        } else {
            throw std::runtime_error("Failed to find device");
        }
    }

    void Device::createAmiDev() {
        if(ami_dev_find(bdf.c_str(), &dev) != AMI_STATUS_OK) {
            throw std::runtime_error("Failed to find device " + bdf);
        }
        ami_dev_get_pci_bdf(dev, &pci_bdf);
        if(ami_dev_request_access(dev) != AMI_STATUS_OK) {
            throw std::runtime_error("Failed to request elevated access to device");
        }
    }

    void Device::destroyAmiDev() {
        ami_dev_delete(&dev);
    }

    void Device::setFrequency(uint64_t freq) {
        if(platform == Platform::HARDWARE) {
            clkWiz.setRateHz(freq);
        }
    }

    ami_device* Device::getAmiDev() {
        return dev;
    }

    void Device::findVrtbinType() {
        XMLParser parser(systemMap);
        parser.parseXML();
        this->vrtbinType = parser.getVrtbinType();
    }

    void Device::findPlatform() {
        XMLParser parser(systemMap);
        parser.parseXML();
        this->platform = parser.getPlatform();
    }

    Platform Device::getPlatform() {
        return platform;
    }

    ZmqServer* Device::getZmqServer() {
        return zmqServer;
    }

    Allocator* Device::getAllocator() {
        return allocator;
    }
    
} // namespace vrt