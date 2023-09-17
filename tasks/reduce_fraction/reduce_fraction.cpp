#include "reduce_fraction.h"
#include <numeric>

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    while (std::gcd(numerator, denominator) != 1) {
        int64_t temp = std::gcd(numerator, denominator);
        numerator /= temp;
        denominator /= temp;
    }
    return numerator + denominator;
}
