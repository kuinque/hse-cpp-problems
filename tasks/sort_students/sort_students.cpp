#include "sort_students.h"

#include <tuple>

bool operator<(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) <
           std::tie(right_date.year, right_date.month, right_date.day);
}

bool operator!=(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) !=
           std::tie(right_date.year, right_date.month, right_date.day);
}

bool CompareByDate(const Student& left_student, const Student& right_student) {
    return std::tie(left_student.birth_date, left_student.last_name, left_student.name) <
           std::tie(right_student.birth_date, right_student.last_name, right_student.name);
}

auto CompareByName(const Student& left_student, const Student& right_student) {
    return std::tie(left_student.last_name, left_student.name, left_student.birth_date) <
           std::tie(right_student.last_name, right_student.name, right_student.birth_date);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    if (sortKind == SortKind::Date) {
        std::sort(students.begin(), students.end(), CompareByDate);
    } else {
        std::sort(students.begin(), students.end(), CompareByName);
    }
}
