#include "reduce_fraction.h"

int64_t GCD(int64_t a, int64_t b) {
    while (a > 0 && b > 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    if (a == 0) {
        return b;
    }
    return a;
}

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    while (GCD(numerator, denominator) != 1) {
        int64_t temp = GCD(numerator, denominator);
        numerator /= temp;
        denominator /= temp;
    }
    return numerator + denominator;
}
