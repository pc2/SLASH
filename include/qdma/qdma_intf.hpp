#ifndef QDMA_INTF_HPP
#define QDMA_INTF_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <string>

#define RW_MAX_SIZE	0x7ffff000
#define GB_DIV 1000000000
#define MB_DIV 1000000
#define KB_DIV 1000
#define NSEC_DIV 1000000000
#define QMAX_PATH "/sys/bus/pci/devices/0000:%s:00.1/qdma/qmax"
#define QDMA_QUEUE_NAME "qdma%s001"
#define QDMA_DEFAULT_QUEUE "/dev/qdma%s001-MM-0"



class QdmaIntf {
    static uint8_t queueIdx;
    std::string bdf;
    std::string queueName;
    ssize_t write_from_buffer(const char* dev, char* buffer, uint64_t size, uint64_t base);
    ssize_t read_to_buffer(const char *fname, char *buffer, uint64_t size, uint64_t base);
    char* strip(const char* bdf);
    char* create_qdma_queue(const char* bdf);
    int delete_qdma_queue(const char* bdf);
    QdmaIntf(const QdmaIntf&) = delete;
    QdmaIntf& operator=(const QdmaIntf&) = delete;
public:
    static QdmaIntf& getInstance(const std::string& bdf = "21:00.0");
    QdmaIntf(const std::string& bdf);
    QdmaIntf();
    void write_buff(char* buffer, uint64_t start_addr, uint64_t size);
    void read_buff(char* buffer, uint64_t start_addr, uint64_t size);
    ~QdmaIntf();
};



#endif // QDMA_INTF_HPP