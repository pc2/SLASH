#include "commands/partial_program_command.hpp"

PartialProgramCommand::PartialProgramCommand(const std::string& device, const std::string& image_path) {
    this->device = device;
    this->imagePath = image_path;
    this->dev = nullptr;
    if (ami_dev_find(device.c_str(), &dev) != AMI_STATUS_OK) {
        std::cerr << "Error finding ami device: " << device << std::endl;
        throw std::runtime_error("Error finding device");
    }
}

void PartialProgramCommand::execute() {
    int found_current_uuid = AMI_STATUS_ERROR;
    int found_new_uuid = AMI_STATUS_ERROR;
    std::string new_uuid;
    char current_uuid[AMI_LOGIC_UUID_SIZE] = {0};

    Vrtbin::extract(this->imagePath, "/tmp");
    std::string ami_path = std::string(std::getenv("AMI_HOME"));
    std::string create_path = "mkdir -p " + ami_path + "/" + device + ":00.0/";
    std::string basePath = ami_path + "/" + device + ":00.0/";
    system(create_path.c_str());
    Vrtbin::copy("/tmp/system_map.xml", basePath + "system_map.xml");
    Vrtbin::copy("/tmp/version.json", basePath + "version.json");
    Vrtbin::copy("/tmp/report_utilization.xml", basePath + "report_utilization.xml");
    imagePath = "/tmp/design.pdi";


    uint16_t dev_bdf;
    ami_dev_get_pci_bdf(dev, &dev_bdf);

    found_current_uuid = ami_dev_read_uuid(dev, current_uuid);
    found_new_uuid = Vrtbin::extractUUID().empty() ? AMI_STATUS_ERROR : AMI_STATUS_OK;
    new_uuid = Vrtbin::extractUUID().substr(0, 32);
    	printf(
		"----------------------------------------------\r\n"
		"Device | %02x:%02x.%01x\r\n"
		"----------------------------------------------\r\n"
		"Current Configuration\r\n"
		"----------------------------------------------\r\n"
		"UUID   | %s\r\n"
		"----------------------------------------------\r\n"
		"Incoming Configuration\r\n"
		"----------------------------------------------\r\n"
		"UUID      | %s\r\n"
		"Path      | %s\r\n"
		"----------------------------------------------\r\n",
		AMI_PCI_BUS(dev_bdf),
		AMI_PCI_DEV(dev_bdf),
		AMI_PCI_FUNC(dev_bdf),
		((found_current_uuid != AMI_STATUS_OK) ? ("N/A") : (current_uuid)),
		((found_new_uuid != AMI_STATUS_OK) ? ("N/A") : (new_uuid.c_str())),
		imagePath.c_str()
	);

    if (ami_dev_request_access(dev) != AMI_STATUS_OK) {
        std::cerr << "Error requesting access to ami device: " << device << std::endl;
        throw std::runtime_error("Error requesting access to device");
    }

    if (ami_prog_download_pdi(dev, imagePath.c_str(), 0, 0, Vrtbin::progressHandler, true) != AMI_STATUS_OK) {
        std::cerr << "Error downloading image to ami device: " << device << std::endl;
        throw std::runtime_error("Error downloading image to device");
    }
    ami_dev_delete(&dev);
    PcieDriverHandler pcieDriverHandler(device + ":00.0");
    pcieDriverHandler.execute(PcieDriverHandler::Command::REMOVE);
    usleep(DELAY_PARTIAL_BOOT);
    pcieDriverHandler.execute(PcieDriverHandler::Command::RESCAN);

    std::cout << "\nPartial Program Command executed successfully\n";

}