#include "is_leap.h"

bool IsLeap(int year) {
    static const int LEAP_MOD_1_CRITERIA = 4;
    static const int LEAP_MOD_2_CRITERIA = 100;
    static const int LEAP_MOD_3_CRITERIA = 400;
    return (year % LEAP_MOD_1_CRITERIA == 0 && year % LEAP_MOD_2_CRITERIA != 0) || year % LEAP_MOD_3_CRITERIA == 0;
}
