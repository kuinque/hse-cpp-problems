#include "reduce_fraction.h"
#include <numeric>

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    return (numerator + denominator) / std::gcd(numerator, denominator);
}
