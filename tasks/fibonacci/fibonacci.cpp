#include "fibonacci.h"
#include <vector>

int64_t CalculateNthFibonacci(int64_t n) {
    std::vector<int64_t> fibonacci(n);
    if (n <= 2) {
        return 1;
    }
    fibonacci[0] = 1;
    fibonacci[1] = 1;
    for (int64_t i = 2; i < n; ++i) {
        fibonacci[i] = fibonacci[i - 2] + fibonacci[i - 1];
    }
    return fibonacci[n - 1];
}
