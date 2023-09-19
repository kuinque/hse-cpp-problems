#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    size_t left_pointer = 0;
    size_t right_pointer = str.length() - 1;
    while (left_pointer < right_pointer) {
        if (str[left_pointer] == ' ') {
            ++left_pointer;
            continue;
        } else if (str[right_pointer] == ' ') {
            --right_pointer;
            continue;
        }
        if (str[left_pointer] != str[right_pointer]) {
            return false;
        } else {
            ++left_pointer;
            --right_pointer;
        }
    }
    return true;
}
