#ifndef QDMA_LOGIC_HPP
#define QDMA_LOGIC_HPP
#include "api/kernel.hpp"
// Not used now. Might be needed for C2H stream future development
namespace vrt {

    class QdmaLogic : public Kernel {
    public:
        
        QdmaLogic(ami_device* device, const std::string& name, uint64_t baseAddr, uint64_t range);

        void setValues(uint16_t qid, uint32_t length);


    };

} // namespace vrt


#endif // QDMA_LOGIC_HPP