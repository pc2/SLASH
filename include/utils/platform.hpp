#ifndef PLATFORM_HPP
#define PLATFORM_HPP

namespace vrt {
/**
 * @brief Enumeration for execution platforms.
 *
 * This enum represents the different execution platforms that can be used
 * for running applications and kernels.
 */
enum class Platform {
    HARDWARE,    ///< Hardware platform (actual FPGA device)
    EMULATION,   ///< Emulation platform (software emulation of hardware)
    SIMULATION,  ///< Simulation platform (software simulation)
    UNKNOWN      ///< Unknown or unspecified platform
};

}  // namespace vrt

#endif  // PLATFORM_HPP