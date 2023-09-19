#include "reduce_fraction.h"
#include <numeric>

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    int64_t calc_gcd = std::gcd(numerator, denominator);
    while (calc_gcd != 1) {
        numerator /= calc_gcd;
        denominator /= calc_gcd;
        calc_gcd = std::gcd(numerator, denominator);
    }
    return numerator + denominator;
}
