#include "password.h"

const int8_t MINIMAL_ASCII = 33;
const int8_t MAXIMAL_ASCII = 126;
const int8_t MINIMAL_PASSWORD_SIZE = 8;
const int8_t MAXIMAL_PASSWORD_SIZE = 14;
const int8_t MINIMAL_CLASS_COUNT = 3;

bool ValidatePassword(const std::string& password) {
    if (password.size() < MINIMAL_PASSWORD_SIZE || password.size() > MAXIMAL_PASSWORD_SIZE) {
        return false;
    }
    for (const char8_t i : password) {
        if (i < MINIMAL_ASCII || i > MAXIMAL_ASCII) {
            return false;
        }
    }
    bool lower_letter_included = false;
    bool upper_letter_included = false;
    bool digit_included = false;
    bool other_char_included = false;
    for (const char8_t i : password) {
        if (isalpha(i)) {
            if (islower(i)) {
                lower_letter_included = true;
            } else {
                upper_letter_included = true;
            }
        } else if (isdigit(i)) {
            digit_included = true;
        } else {
            other_char_included = true;
        }
    }
    int8_t good_class_characters = 0;
    if (lower_letter_included) {
        ++good_class_characters;
    }
    if (upper_letter_included) {
        ++good_class_characters;
    }
    if (digit_included) {
        ++good_class_characters;
    }
    if (other_char_included) {
        ++good_class_characters;
    }
    if (good_class_characters >= MINIMAL_CLASS_COUNT) {
        return true;
    }
    return false;
}
