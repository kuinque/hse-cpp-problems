#include "password.h"

bool ValidatePassword(const std::string& password) {
    static const char8_t MINIMAL_ASCII = 33;
    static const char8_t MAXIMAL_ASCII = 126;
    static const size_t MINIMAL_PASSWORD_SIZE = 8;
    static const size_t MAXIMAL_PASSWORD_SIZE = 14;
    static const int8_t MINIMAL_CLASS_COUNT = 3;
    if (password.size() < MINIMAL_PASSWORD_SIZE || password.size() > MAXIMAL_PASSWORD_SIZE) {
        return false;
    }
    for (const char8_t i : password) {
        if (i < MINIMAL_ASCII || i > MAXIMAL_ASCII) {
            return false;
        }
    }
    bool has_lower_letter = false;
    bool has_upper_letter = false;
    bool has_digit = false;
    bool has_other_char = false;
    for (const char8_t i : password) {
        if (std::isalpha(i)) {
            if (std::islower(i)) {
                has_lower_letter = true;
            } else {
                has_upper_letter = true;
            }
        } else if (std::isdigit(i)) {
            has_digit = true;
        } else {
            has_other_char = true;
        }
    }
    int8_t good_class_characters = 0;
    for (bool i : {has_lower_letter, has_upper_letter, has_digit, has_other_char}) {
        if (i) {
            ++good_class_characters;
        }
    }
    if (good_class_characters >= MINIMAL_CLASS_COUNT) {
        return true;
    }
    return false;
}
