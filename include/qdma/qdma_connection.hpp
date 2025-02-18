#ifndef QDMA_CONNECTION_HPP
#define QDMA_CONNECTION_HPP

#include <string>

namespace vrt {

    enum class StreamDirection {
        HOST_TO_DEVICE,
        DEVICE_TO_HOST
    };

    class QdmaConnection {
    public:
        QdmaConnection(const std::string& kernel, uint32_t qid, const std::string& interface, const std::string& direction);

        std::string getKernel() const;

        uint32_t getQid() const;

        std::string getInterface() const;

        StreamDirection getDirection() const;

    private:
        std::string kernel;
        uint32_t qid;
        std::string interface;
        StreamDirection direction;
    };
}

#endif // QDMA_CONNECTION_HPP