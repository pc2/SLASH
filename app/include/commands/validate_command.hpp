#ifndef VALIDATE_CMD_HPP
#define VALIDATE_CMD_HPP

#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#define ADDR_START_DDR 0x50080000000
#define ADDR_START_DIMM_DDR 0x60000000000
#define ADDR_START_HBM 0x4000000000
#define SIZE_PER_HBM_CHANNEL 0x40000000
#define DEFAULT_TEST_SIZE 255000000
#define DEFAULT_COUNT_TIMES 50

#define RW_MAX_SIZE	0x7ffff000
#define GB_DIV 1000000000
#define MB_DIV 1000000
#define KB_DIV 1000
#define NSEC_DIV 1000000000

/**
 * @brief Enumeration for test types.
 */
typedef enum {
    TEST_TYPE_READ = 0, /**< Read test type. */
    TEST_TYPE_WRITE     /**< Write test type. */
} test_type;

/**
 * @brief Structure to hold thread data.
 */
typedef struct {
    std::string devname;    /**< Device name. */
    uint64_t addr;          /**< Address to test. */
    uint64_t size;          /**< Size of the memory region. */
    uint64_t offset;        /**< Offset within the memory region. */
    uint64_t count;         /**< Number of iterations. */
    int verbose;            /**< Verbose flag. */
} thread_data_t;

class ValidateCommand {
    public:
    ValidateCommand(const std::string& device);
    void execute();

private:
    std::string device;
    void test_dma(const std::string& devname, uint64_t addr, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    std::size_t write_from_buffer(const std::string& fname, int fd, char* buffer, uint64_t size, uint64_t base);
    std::size_t read_to_buffer(const std::string& fname, int fd, char* buffer, uint64_t size, uint64_t base);
    std::size_t test_dma_write(const std::string& devname, uint64_t addr, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    std::size_t test_dma_read(const std::string& devname, uint64_t addr, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    void print_results(const std::string& devname, uint64_t addr, double total_time, double avg_time, double size, double result, test_type type, int verbose);
    void print_pci_bandwidth(double pci_bw_result);
    void dma_read_thread(thread_data_t* data);
    void dma_write_thread(thread_data_t* data);
    int run_sim_seq_rw_threads(const std::string& devname, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    int run_sim_rw_per_memory(const std::string& devname, uint64_t addr, uint64_t size, uint64_t offset, uint64_t count, int verbose, const std::string& mem_type);
    int run_pcie_bw_test(const std::string& devname, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    double test_dma_write_pcie(const std::string& devname, uint64_t addr, uint64_t size, uint64_t offset, uint64_t count, int verbose);
    void dma_write_thread_pcie(thread_data_t* data);
    void timespec_sub(struct timespec *t1, struct timespec *t2);
    int timespec_check(struct timespec *t);
};

#endif