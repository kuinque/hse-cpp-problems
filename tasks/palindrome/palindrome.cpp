#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    int64_t pointer1 = 0;
    int64_t pointer2 = str.length() - 1;
    while (pointer1 < pointer2) {
        if (str[pointer1] == ' ') {
            ++pointer1;
            continue;
        } else if (str[pointer2] == ' ') {
            --pointer2;
        }
        if (str[pointer1] != str[pointer2]) {
            return false;
        } else {
            ++pointer1;
            --pointer2;
        }
    }
    return true;
}
