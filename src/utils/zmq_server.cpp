#include "utils/zmq_server.hpp"

namespace vrt {

    ZmqServer::ZmqServer() : context(1), socket(context, ZMQ_REQ) {

        socket.connect(address);
    }

    void ZmqServer::sendBuffer(const std::string& name, const std::vector<uint8_t>& buffer) {
        Json::Value command;
        command["command"] = "populate";
        command["name"] = name;
        command["size"] = static_cast<Json::UInt64>(buffer.size());

        zmq::message_t request(command.toStyledString().size());
        memcpy(request.data(), command.toStyledString().c_str(), command.toStyledString().size());
        socket.send(request, zmq::send_flags::sndmore);

        zmq::message_t data(buffer.size());
        memcpy(data.data(), buffer.data(), buffer.size());
        socket.send(data, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply);
        std::string replyStr(static_cast<char*>(reply.data()), reply.size());
    }

    void ZmqServer::sendCommand(const Json::Value& command) {
        Json::StreamWriterBuilder writer;
        std::string commandStr = Json::writeString(writer, command);

        zmq::message_t request(commandStr.size());
        memcpy(request.data(), commandStr.data(), commandStr.size());
        socket.send(request, zmq::send_flags::none);
        zmq::message_t reply;
        socket.recv(reply);
        std::string replyStr(static_cast<char*>(reply.data()), reply.size());
    }

    uint32_t ZmqServer::fetchScalar(const std::string& function, const std::string& argIdx) {
        Json::Value command;
        command["command"] = "fetch";
        command["type"] = "scalar";
        command["function"] = function;
        command["arg"] = argIdx;

        Json::StreamWriterBuilder writer;
        std::string commandStr = Json::writeString(writer, command);

        zmq::message_t request(commandStr.size());
        memcpy(request.data(), commandStr.c_str(), commandStr.size());
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply);
        std::string replyStr(static_cast<char*>(reply.data()), reply.size());

        Json::Value response;
        Json::Reader reader;
        reader.parse(replyStr, response);

        return response.asUInt();
    }

    std::vector<uint8_t> ZmqServer::fetchBuffer(const std::string& name) {
        Json::Value command;
        command["command"] = "fetch";
        command["type"] = "buffer";
        command["name"] = name;

        Json::StreamWriterBuilder writer;
        std::string commandStr = Json::writeString(writer, command);

        zmq::message_t request(commandStr.size());
        memcpy(request.data(), commandStr.c_str(), commandStr.size());
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply);
        std::string replyStr(static_cast<char*>(reply.data()), reply.size());

        Json::Value response;
        Json::Reader reader;
        reader.parse(replyStr, response);

        std::vector<uint8_t> byteArray;
        for (const auto& byte : response) {
            byteArray.push_back(static_cast<uint8_t>(byte.asUInt()));
        }

        // Return the byteArray
        return byteArray;
    }

} // namespace vrt