#ifndef QDMA_LOGIC_HPP
#define QDMA_LOGIC_HPP
#include "api/kernel.hpp"

namespace vrt {

/**
 * @brief Class for managing QDMA logic operations.
 *
 * The QdmaLogic class extends the Kernel class to provide functionality specific
 * to QDMA operations, allowing for control of QDMA queues and data transfers in streaming mode.
 * 
 * @note Not used now. Might be needed for C2H stream future development
 */
class QdmaLogic : public Kernel {
   public:
    /**
     * @brief Constructor for QdmaLogic.
     *
     * @param device Pointer to the AMI device.
     * @param name Name of the QDMA kernel.
     * @param baseAddr Base address of the QDMA kernel in device memory.
     * @param range Memory range allocated to the QDMA kernel.
     */
    QdmaLogic(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range);

    /**
     * @brief Sets QDMA queue parameters.
     *
     * @param qid Queue ID to configure.
     * @param length Length of the data transfer.
     *
     * This method configures the specified QDMA queue with the given parameters.
     */
    void setValues(uint16_t qid, uint32_t length);
};

}  // namespace vrt

#endif  // QDMA_LOGIC_HPP