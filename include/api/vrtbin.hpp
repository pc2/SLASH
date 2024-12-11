#ifndef VRTBIN_HPP
#define VRTBIN_HPP

#include <string>
#include <cstdlib>
#include <array>
#include <fstream>
#include <iostream>
#include <memory>

namespace vrt {
    class Vrtbin {
        std::string vrtbinPath;
        std::string systemMapPath;
        std::string pdiPath;
        std::string uuid;
        std::string tempExtractPath = "/tmp";
        void copy(const std::string& source, const std::string& destination);
    public:
        Vrtbin(std::string vrtbinPath, const std::string& bdf);
        void extract();
        std::string getSystemMapPath();
        std::string getPdiPath();
        std::string getUUID();
        void extractUUID();
    };
}

#endif // VRTBIN_HPP