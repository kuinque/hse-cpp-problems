#include "password.h"

bool ValidatePassword(const std::string& password) {
    static const char8_t MINIMAL_ASCII = 33;
    static const char8_t MAXIMAL_ASCII = 126;
    static const size_t MINIMAL_PASSWORD_SIZE = 8;
    static const size_t MAXIMAL_PASSWORD_SIZE = 14;
    static const int MINIMAL_CLASS_COUNT = 3;
    if (std::clamp(password.size(), MINIMAL_PASSWORD_SIZE, MAXIMAL_PASSWORD_SIZE) != password.size()) {
        return false;
    }
    int has_lower_letter = 0;
    int has_upper_letter = 0;
    int has_digit = 0;
    int has_other_char = 0;
    for (const char8_t i : password) {
        if (std::clamp(i, MINIMAL_ASCII, MAXIMAL_ASCII) != i) {
            return false;
        }
        if (std::islower(i)) {
            has_lower_letter = 1;
        } else if (std::isupper(i)) {
            has_upper_letter = 1;
        } else if (std::isdigit(i)) {
            has_digit = 1;
        } else {
            has_other_char = 1;
        }
    }
    if (has_lower_letter + has_upper_letter + has_digit + has_other_char >= MINIMAL_CLASS_COUNT) {
        return true;
    }
    return false;
}
