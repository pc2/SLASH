#ifndef VRTBIN_HPP
#define VRTBIN_HPP

#include <ami.h>
#include <ami_mem_access.h>
#include <ami_program.h>
#include <unistd.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

/**
 * @brief Class for managing VRTBIN files.
 *
 * The Vrtbin class provides utilities for handling VRTBIN files, including
 * extraction, copying, and information retrieval.
 */
class Vrtbin {
   public:
    /**
     * @brief Extracts the contents of a VRTBIN file.
     *
     * @param source Path to the source VRTBIN file.
     * @param destination Path where the contents will be extracted.
     */
    static void extract(std::string source, std::string destination);

    /**
     * @brief Copies a VRTBIN file.
     *
     * @param source Path to the source VRTBIN file.
     * @param destination Path where the file will be copied.
     */
    static void copy(const std::string& source, const std::string& destination);

    /**
     * @brief Handler for progress events during VRTBIN operations.
     *
     * @param status The current status of the operation.
     * @param ctr Counter value indicating progress.
     * @param data Additional data for the handler.
     */
    static void progressHandler(enum ami_event_status status, uint64_t ctr, void* data);

    /**
     * @brief Generates a progress bar string.
     *
     * @param cur Current progress value.
     * @param max Maximum progress value.
     * @param width Width of the progress bar in characters.
     * @param left Character for the left end of the progress bar.
     * @param right Character for the right end of the progress bar.
     * @param fill Character for filled portions of the progress bar.
     * @param empty Character for empty portions of the progress bar.
     * @param state Character indicating the current state.
     * @return A character representing the current state of the progress bar.
     */
    static char print_progress_bar(uint32_t cur, uint32_t max, uint32_t width, char left,
                                   char right, char fill, char empty, char state);

    /**
     * @brief Extracts the UUID from a VRTBIN file.
     *
     * @return The UUID as a string.
     */
    static std::string extractUUID();

    /**
     * @brief Extracts and prints information about a VRTBIN file.
     *
     * @param path Path to the VRTBIN file.
     */
    static void extractAndPrintInfo(const std::string& path);
};

#endif  // VRTBIN_HPP