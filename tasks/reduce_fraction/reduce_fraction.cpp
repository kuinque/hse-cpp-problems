#include "reduce_fraction.h"
#include <set>

std::multiset<int64_t> FindPrimeDivisors(int64_t number){
    std::multiset<int64_t> prime_divisors;
    for (int64_t i = 2; i * i <= number; ++i) {
        while (number % i == 0){
            number /= i;
            prime_divisors.insert(i);
        }
    }
    if (number > 1) {
        prime_divisors.insert(number);
    }
    return prime_divisors;
}

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    std::multiset<int64_t> numerator_divisors = FindPrimeDivisors(numerator);
    std::multiset<int64_t> denominator_divisors = FindPrimeDivisors(denominator);
    int64_t reduced_numerator = 1;
    int64_t reduced_denominator = 1;
    while(!numerator_divisors.empty() || !denominator_divisors.empty()) {
        if (*numerator_divisors.begin() == *denominator_divisors.begin()) {
            numerator_divisors.erase(numerator_divisors.begin());
            denominator_divisors.erase(denominator_divisors.begin());
        } else if (*numerator_divisors.begin() < *denominator_divisors.begin()) {
            reduced_numerator *= *numerator_divisors.begin();
            numerator_divisors.erase(numerator_divisors.begin());
        } else {
            reduced_denominator *= *denominator_divisors.begin();
            denominator_divisors.erase(denominator_divisors.begin());
        }
    }
    return reduced_numerator + reduced_denominator;
}
