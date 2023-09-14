#include <cstdint>
#include "is_leap.h"

const int16_t D1 = 4;
const int16_t D2 = 100;
const int16_t D3 = 400;

bool IsLeap(int year) {
    if ((year % D1 == 0 && year % D2 != 0) || year % D3 == 0) {
        return true;
    }
    return false;
}
