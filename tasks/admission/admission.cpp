#include <unordered_map>
#include <algorithm>

#include "admission.h"

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

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::unordered_map<std::string, size_t> universities_student_left;
    for (const University& university : universities) {
        universities_student_left[university.name] = university.max_students;
    }
    std::vector<const Applicant*> applicants_copy;
    for (const Applicant& applicant : applicants) {
        applicants_copy.emplace_back(&applicant);
    }
    auto applicant_compare = [](const Applicant* left_applicant, const Applicant* right_applicant) -> bool {
        if (left_applicant->points > right_applicant->points) {
            return true;
        } else if (left_applicant->points != right_applicant->points) {
            return false;
        }
        if (left_applicant->student.birth_date < right_applicant->student.birth_date) {
            return true;
        } else if (left_applicant->student.birth_date != right_applicant->student.birth_date) {
            return false;
        }
        std::string left_student_name = left_applicant->student.name.substr(0, left_applicant->student.name.find(' '));
        std::string right_student_name =
            right_applicant->student.name.substr(0, right_applicant->student.name.find(' '));
        std::string left_student_surname = left_applicant->student.name.substr(left_student_name.size() + 1);
        std::string right_student_surname = right_applicant->student.name.substr(right_student_name.size() + 1);
        if (left_student_surname < right_student_surname) {
            return true;
        } else if (left_student_surname != right_student_surname) {
            return false;
        }
        if (left_student_name < right_student_name) {
            return true;
        }
        return false;
    };
    std::sort(applicants_copy.begin(), applicants_copy.end(), applicant_compare);
    AdmissionTable universities_students;
    for (const Applicant* applicant : applicants_copy) {
        for (const std::string& wished_university : applicant->wish_list) {
            if (!universities_student_left[wished_university]) {
                continue;
            }
            universities_students[wished_university].push_back(&applicant->student);
            --universities_student_left[wished_university];
            break;
        }
    }
    auto students_compare = [](const Student* left_student, const Student* right_student) -> bool {
        std::string left_student_name = left_student->name.substr(0, left_student->name.find(' '));
        std::string right_student_name = right_student->name.substr(0, right_student->name.find(' '));
        std::string left_student_surname = left_student->name.substr(left_student_name.size() + 1);
        std::string right_student_surname = right_student->name.substr(right_student_name.size() + 1);
        if (left_student_surname < right_student_surname) {
            return true;
        } else if (left_student_surname != right_student_surname) {
            return false;
        }
        if (left_student_name < right_student_name) {
            return true;
        } else if (left_student_name != right_student_name) {
            return false;
        }
        if (left_student->birth_date < right_student->birth_date) {
            return true;
        }
        return false;
    };
    for (auto& [university, students] : universities_students) {
        std::sort(students.begin(), students.end(), students_compare);
    }
    return universities_students;
}
