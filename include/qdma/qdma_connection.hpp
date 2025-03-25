#ifndef QDMA_CONNECTION_HPP
#define QDMA_CONNECTION_HPP

#include <string>

namespace vrt {

    /**
     * @brief Enumeration for stream data direction.
     * 
     * This enum represents the different directions for data streaming between
     * host and device.
     */
    enum class StreamDirection {
        HOST_TO_DEVICE, ///< Data flow from host to device (H2C)
        DEVICE_TO_HOST  ///< Data flow from device to host (C2H)
    };

    /**
     * @brief Class for managing QDMA connections.
     * 
     * The QdmaConnection class provides functionality to manage connections
     * between the host and device using QDMA (Queue DMA) for data transfers.
     */
    class QdmaConnection {
    public:
        /**
         * @brief Constructor for QdmaConnection.
         * 
         * @param kernel The name of the kernel associated with this connection.
         * @param qid Queue ID for the QDMA operation.
         * @param interface The interface name for the connection.
         * @param direction String representation of the stream direction ("h2c" or "c2h").
         * 
         * Initializes a new QDMA connection with the specified parameters.
         */
        QdmaConnection(const std::string& kernel, uint32_t qid, const std::string& interface, const std::string& direction);

        /**
         * @brief Gets the kernel name.
         * 
         * @return The name of the kernel associated with this connection.
         */
        std::string getKernel() const;

        /**
         * @brief Gets the queue ID.
         * 
         * @return The queue ID for this QDMA connection.
         */
        uint32_t getQid() const;

        /**
         * @brief Gets the interface name.
         * 
         * @return The interface name for this connection.
         */
        std::string getInterface() const;

        /**
         * @brief Gets the stream direction.
         * 
         * @return The direction of data flow for this connection.
         */
        StreamDirection getDirection() const;

    private:
        std::string kernel;      ///< Name of the kernel associated with this connection.
        uint32_t qid;            ///< Queue ID for the QDMA operation.
        std::string interface;   ///< Interface name for the connection.
        StreamDirection direction; ///< Direction of data flow.
    };
}

#endif // QDMA_CONNECTION_HPP