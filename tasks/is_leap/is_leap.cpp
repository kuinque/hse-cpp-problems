#include <cstdint>
#include "is_leap.h"

bool IsLeap(int year) {
    static const int16_t D1 = 4;
    static const int16_t D2 = 100;
    static const int16_t D3 = 400;
    if ((year % D1 == 0 && year % D2 != 0) || year % D3 == 0) {
        return true;
    }
    return false;
}
