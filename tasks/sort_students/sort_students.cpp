#include "sort_students.h"

bool operator<(const Date& left_date, const Date& right_date) {
    if (left_date.year < right_date.year) {
        return true;
    } else if (left_date.year > right_date.year) {
        return false;
    }
    if (left_date.month < right_date.month) {
        return true;
    } else if (left_date.month > right_date.month) {
        return false;
    }
    if (left_date.day < right_date.day) {
        return true;
    }
    return false;
}

bool operator!=(const Date& left_date, const Date& right_date) {
    return (left_date.day != right_date.day || left_date.month != right_date.month ||
            left_date.year != right_date.year);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    auto comparator_by_date = [](const Student& left_student, const Student& right_student) -> bool {
        if (left_student.birth_date < right_student.birth_date) {
            return true;
        } else if (left_student.birth_date != right_student.birth_date) {
            return false;
        }
        if (left_student.last_name < right_student.last_name) {
            return true;
        } else if (left_student.last_name != right_student.last_name) {
            return false;
        }
        if (left_student.name < right_student.name) {
            return true;
        } else {
            return false;
        }
    };
    auto comparator_by_name = [](const Student& left_student, const Student& right_student) -> bool {
        if (left_student.last_name < right_student.last_name) {
            return true;
        } else if (left_student.last_name != right_student.last_name) {
            return false;
        }
        if (left_student.name < right_student.name) {
            return true;
        } else if (left_student.name != right_student.name) {
            return false;
        }
        if (left_student.birth_date < right_student.birth_date) {
            return true;
        }
        return false;
    };
    if (sortKind == SortKind::Date) {
        std::sort(students.begin(), students.end(), comparator_by_date);
    } else {
        std::sort(students.begin(), students.end(), comparator_by_name);
    }
}
