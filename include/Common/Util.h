#include <stdint.h>
#include <tuple>

#define __FILENAME__                                                           \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define INFO(fmt, ...) fprintf(stderr, "file:%s line:%d info:" fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)

uint32_t alignUpBit(uint32_t bytes, uint32_t align);
uint8_t bound(uint8_t min, int val, uint8_t max);
std::tuple<uint8_t, uint8_t, uint8_t> RGB2YUV(uint8_t const &R, uint8_t const &G, uint8_t const &B);
std::tuple<uint8_t, uint8_t, uint8_t> YUV2RGB(uint8_t const &Y, uint8_t const &U, uint8_t const &V);