#include "qdma/qdma_connection.hpp"

namespace vrt {
QdmaConnection::QdmaConnection(const std::string& kernel, uint32_t qid,
                               const std::string& interface, const std::string& direction)
    : kernel(kernel), interface(interface), qid(qid) {
    if (direction == "HostToDevice") {
        this->direction = StreamDirection::HOST_TO_DEVICE;
    } else if (direction == "DeviceToHost") {
        this->direction = StreamDirection::DEVICE_TO_HOST;
    }
}

std::string QdmaConnection::getKernel() const { return kernel; }

uint32_t QdmaConnection::getQid() const { return qid; }

std::string QdmaConnection::getInterface() const { return interface; }

StreamDirection QdmaConnection::getDirection() const { return direction; }

}  // namespace vrt