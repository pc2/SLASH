#include "driver/qdma_logic.hpp"

namespace vrt {

    QdmaLogic::QdmaLogic(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range) : 
        Kernel(device, name, baseAddr, range, std::vector<Register>{}){}

    void QdmaLogic::setValues(uint16_t qid, uint32_t length) {
        uint32_t regVal = 0;
        regVal |= (qid & 0xFFF);
        regVal |= ((length & 0xFFFF) << 12);
        write(0x00, regVal);
    }
    
} // namespace vrt