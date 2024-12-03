#ifndef REGISTER_HPP
#define REGISTER_HPP
#include <string>
#include <cstdint>

namespace vrt {

    class Register {
    std::string registerName;
    uint32_t offset;
    uint32_t width;
    std::string rw;
    std::string description;
    public:
        Register(std::string registerName, uint32_t offset, uint32_t width, std::string rw, std::string description);
        Register() = default;
        std::string getRegisterName();
        uint32_t getOffset();
        uint32_t getWidth();
        std::string getRW();
        std::string getDescription();
        void setRegisterName(std::string registerName);
        void setOffset(uint32_t offset);
        void setWidth(uint32_t width);
        void setRW(std::string rw);
        void setDescription(std::string description);
    };
} // namespace vrt

#endif // REGISTER_HPP