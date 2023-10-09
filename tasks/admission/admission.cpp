#include "admission.h"

#include <algorithm>
#include <tuple>
#include <unordered_map>

bool operator!=(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) !=
           std::tie(right_date.year, right_date.month, right_date.day);
}

bool operator<(const Date& left_date, const Date& right_date) {
    return std::tie(left_date.year, left_date.month, left_date.day) <
           std::tie(right_date.year, right_date.month, right_date.day);
}

bool applicant_compare(const Applicant* left_applicant, const Applicant* right_applicant) {
    if (left_applicant->points > right_applicant->points) {
        return true;
    } else if (left_applicant->points < right_applicant->points) {
        return false;
    }
    std::string left_student_name = left_applicant->student.name.substr(0, left_applicant->student.name.find(' '));
    std::string right_student_name = right_applicant->student.name.substr(0, right_applicant->student.name.find(' '));
    std::string left_student_surname = left_applicant->student.name.substr(left_student_name.size() + 1);
    std::string right_student_surname = right_applicant->student.name.substr(right_student_name.size() + 1);
    return std::tie(left_applicant->student.birth_date, left_student_surname, left_student_name) <
           std::tie(right_applicant->student.birth_date, right_student_surname, right_student_name);
}

bool students_compare(const Student* left_student, const Student* right_student) {
    std::string left_student_name = left_student->name.substr(0, left_student->name.find(' '));
    std::string right_student_name = right_student->name.substr(0, right_student->name.find(' '));
    std::string left_student_surname = left_student->name.substr(left_student_name.size() + 1);
    std::string right_student_surname = right_student->name.substr(right_student_name.size() + 1);
    return std::tie(left_student_surname, left_student_name, left_student->birth_date) <
           std::tie(right_student_surname, right_student_name, right_student->birth_date);
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
    for (auto& [university, students] : universities_students) {
        std::sort(students.begin(), students.end(), students_compare);
    }
    return universities_students;
}
