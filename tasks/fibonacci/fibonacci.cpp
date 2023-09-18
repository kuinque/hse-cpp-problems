#include "fibonacci.h"

int64_t CalculateNthFibonacci(int64_t n) {
    int64_t current_fib_number = 0;
    int64_t next_fib_number = 1;
    for (int64_t i = 0; i < n; ++i) {
        int64_t calc_next_fib_number = current_fib_number + next_fib_number;
        current_fib_number = next_fib_number;
        next_fib_number = calc_next_fib_number;
    }
    return current_fib_number;
}
