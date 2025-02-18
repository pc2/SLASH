#ifndef ZMQ_SERVER_HPP
#define ZMQ_SERVER_HPP

#include "utils/logger.hpp"
#include <zmq.hpp>
#include <vector>
#include <json/json.h>
#include <memory>

namespace vrt {

    class ZmqServer {
    private:
        zmq::context_t context;
        zmq::socket_t socket;
        std::string address = "tcp://localhost:5555";
        

    public:
        ZmqServer();
        void sendBuffer(const std::string& name, const std::vector<uint8_t>& buffer);
        void sendCommand(const Json::Value& command);
        uint32_t fetchScalar(const std::string& function, const std::string& argIdx);
        std::vector<uint8_t> fetchBuffer(const std::string& name);
        void sendStream(const std::string& name, const std::vector<uint8_t>& buffer);
        std::vector<uint8_t> fetchStream(const std::string& name, size_t size);
        ZmqServer(const ZmqServer&) = delete;
        ZmqServer& operator=(const ZmqServer&) = delete;

        ZmqServer(ZmqServer&&) = default;
        ZmqServer& operator=(ZmqServer&&) = default;
    };


} // namespace vrt

#endif