#include <tuple>

#include "sort_students.h"

bool operator<(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) <
           std::tie(right_date.year, right_date.month, right_date.day);
}

bool operator!=(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) !=
           std::tie(right_date.year, right_date.month, right_date.day);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    auto comparator_by_date = [](const Student& left_student, const Student& right_student) -> bool {
        std::tuple<const Date&, const std::string&, const std::string&> left_student_tuple =
            std::tie(left_student.birth_date, left_student.last_name, left_student.name);
        std::tuple<const Date&, const std::string&, const std::string&> right_student_tuple =
            std::tie(right_student.birth_date, right_student.last_name, right_student.name);
        return left_student_tuple < right_student_tuple;
    };
    auto comparator_by_name = [](const Student& left_student, const Student& right_student) -> bool {
        std::tuple<const std::string&, const std::string&, const Date&> left_student_tuple =
            std::tie(left_student.last_name, left_student.name, left_student.birth_date);
        std::tuple<const std::string&, const std::string&, const Date&> right_student_tuple =
            std::tie(right_student.last_name, right_student.name, right_student.birth_date);
        return left_student_tuple < right_student_tuple;
    };
    if (sortKind == SortKind::Date) {
        std::sort(students.begin(), students.end(), comparator_by_date);
    } else {
        std::sort(students.begin(), students.end(), comparator_by_name);
    }
}
