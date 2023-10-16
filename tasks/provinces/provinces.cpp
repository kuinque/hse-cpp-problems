#include "provinces.h"

int64_t CountPassports(const std::vector<int>& provinces) {
    int64_t released_passports = 0;
    std::multiset<int64_t> merged_provinces;
    for (const int& province : provinces) {
        merged_provinces.insert(province);
    }
    while (merged_provinces.size() > 1) {
        int64_t new_passports = 0;
        for (int i = 0; i < 2; ++i) {
            new_passports += *merged_provinces.begin();
            merged_provinces.erase(merged_provinces.begin());
        }
        merged_provinces.insert(new_passports);
        released_passports += new_passports;
    }
    return released_passports;
}
