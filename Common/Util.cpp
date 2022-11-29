#include "Common/Util.h"

#include <cmath>
#include <tuple>

uint32_t alignUpBit(uint32_t bytes, uint32_t align) {
    // 返回n+7清除最低三位
    return ((bytes + align - 1) & ~(align - 1));
}

uint8_t bound(uint8_t min, int val, uint8_t max) {
    if (val <= min) return min;
    if (val >= max) return max;
    return val;
}

std::tuple<uint8_t, uint8_t, uint8_t> RGB2YUV(uint8_t const &R,
                                              uint8_t const &G,
                                              uint8_t const &B) {
    int Y = round(0.257f * R + 0.504f * G + 0.098f * B + 16);
    int U = round(-0.148f * R - 0.291f * G + 0.439f * B + 128);
    int V = round(0.439f * R - 0.368f * G - 0.071f * B + 128);
    uint8_t y = bound(16, Y, 235);
    uint8_t u = bound(16, U, 239);
    uint8_t v = bound(16, V, 239);
    return std::make_tuple(y, u, v);
}

std::tuple<uint8_t, uint8_t, uint8_t> YUV2RGB(uint8_t const &Y, uint8_t const &U, uint8_t const &V) {
    int R, G, B;
    R = round(1.164 * (Y - 16) + 1.596 * (V - 128));
    G = round(1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128));
    B = round(1.164 * (Y - 16) + 2.018 * (U - 128));
    uint8_t r = bound(0, R, 255);
    uint8_t g = bound(0, G, 255);
    uint8_t b = bound(0, B, 255);
    return std::make_tuple(r, g, b);
}