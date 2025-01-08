#include "api/device.hpp"

namespace vrt {

    Device::Device(const std::string& bdf, const std::string& vrtbinPath, bool program, ProgramType programType) : vrtbin(vrtbinPath, bdf), clkWiz(nullptr, "", 0, 0, 0), pcieHandler(bdf), allocator(4096) {
        this->bdf = bdf;
        this->systemMap = this->vrtbin.getSystemMapPath();
        this->pdiPath = this->vrtbin.getPdiPath();
        this->programType = programType;
        this->qdmaIntf = QdmaIntf(bdf);
        createAmiDev();
        
        if(program) {
            programDevice();
        }
        parseSystemMap();
        this->clkWiz.setRateHz(clockFreq, false);
    }


    Device::~Device() {
        //destroyAmiDev();
    }

    void Device::parseSystemMap() {
        XMLParser parser(systemMap);
        parser.parseXML();
        clockFreq = parser.getClockFrequency();
        this->clkWiz = ClkWiz(dev, "clk_wiz", CLK_WIZ_BASE, CLK_WIZ_OFFSET, clockFreq);
        kernels = parser.getKernels();
        for(auto& kernel : kernels) {
            kernel.second.setDevice(dev);
        }
    }
    Kernel Device::getKernel(const std::string& name) {
        return kernels[name];
    }

    void Device::cleanup() {
        ami_dev_delete(&dev);
    }

    std::string Device::getBdf() {
        return bdf;
    }

    void Device::programDevice() {
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
            if(ami_prog_download_pdi(dev, pdiPath.c_str(), 0, 1, nullptr) != AMI_STATUS_OK) {
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
    }

    void Device::bootDevice() {
        utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "Booting device...");
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
                usleep(1000);
                pcieHandler.execute(PcieDriverHandler::Command::TOGGLE_SBR);
                usleep(5000000);
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
            usleep(1000);
            pcieHandler.execute(PcieDriverHandler::Command::RESCAN);
            pcieHandler.execute(PcieDriverHandler::Command::HOTPLUG);
            createAmiDev();
            utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "New PDI booted successfully");
            std::string cmd = "sudo " + std::string(QDMA_SETUP_QUEUES) + bdf;
            system(cmd.c_str());
            utils::Logger::log(utils::LogLevel::INFO, __PRETTY_FUNCTION__, "QDMA queues setup successfully");
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
        clkWiz.setRateHz(freq);
    }

    ami_device* Device::getAmiDev() {
        return dev;
    }

    Allocator& Device::getAllocator() {
        return allocator;
    }
    
} // namespace vrt