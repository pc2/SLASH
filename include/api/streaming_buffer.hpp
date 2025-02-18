#ifndef STREAMING_BUFFER_HPP
#define STREAMING_BUFFER_HPP

#include "api/device.hpp"
#include "qdma/qdma_intf.hpp"
#include "utils/platform.hpp"
#include "utils/zmq_server.hpp"
#include "qdma/qdma_connection.hpp"
#include <regex>

namespace vrt {

    /**
     * @brief Class representing a streaming buffer.
     *
     * This class provides an interface for managing a streaming buffer in a device.
     * It supports memory-mapped and streaming QDMA connections.
     *
     * @tparam T The type of the elements in the buffer.
     */
    template<typename T>
    class StreamingBuffer {
    public:
        /**
         * @brief Constructs a StreamingBuffer object.
         *
         * @param device The device associated with the buffer.
         * @param kernel The kernel associated with the buffer.
         * @param portName The name of the port associated with the buffer.
         * @param size The size of the buffer.
         */
        StreamingBuffer(Device device, Kernel kernel, const std::string& portName, size_t size);

        /**
         * @brief Destructs the StreamingBuffer object.
         */
        ~StreamingBuffer();

        /**
         * @brief Gets a pointer to the buffer.
         *
         * @return A pointer to the buffer.
         */
        T* get() const;

        /**
         * @brief Accesses an element in the buffer.
         *
         * @param index The index of the element to access.
         * @return A reference to the element at the specified index.
         * @throws std::out_of_range If the index is out of range.
         */
        T& operator[](size_t index);

        /**
         * @brief Accesses an element in the buffer (const version).
         *
         * @param index The index of the element to access.
         * @return A const reference to the element at the specified index.
         * @throws std::out_of_range If the index is out of range.
         */
        const T& operator[](size_t index) const;

        /**
         * @brief Gets the name of the buffer.
         *
         * @return The name of the buffer.
         */
        std::string getName() const;

        /**
         * @brief Synchronizes the buffer with the device.
         */
        void sync();

    private:
        T* localBuffer; ///< Pointer to the local buffer.
        size_t size; ///< Size of the buffer.
        StreamDirection syncType; ///< Synchronization type (direction).
        Device device; ///< Device associated with the buffer.
        Kernel kernel; ///< Kernel associated with the buffer.
        std::size_t index; ///< Index of the buffer.
        std::string name; ///< Name of the buffer.
        std::string portName; ///< Name of the port associated with the buffer.
        QdmaIntf* qdmaInterface; ///< Pointer to the QDMA interface.
    };
} // namespace vrt

#include "api/streaming_buffer.tpp"

#endif // STREAMING_BUFFER_HPP