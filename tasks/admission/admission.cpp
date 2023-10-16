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

bool ApplicantCompare(const Applicant* left_applicant, const Applicant* right_applicant) {
    if (left_applicant->points > right_applicant->points) {
        return true;
    } else if (left_applicant->points < right_applicant->points) {
        return false;
    }
    return std::tie(left_applicant->student.birth_date, left_applicant->student.name) <
           std::tie(right_applicant->student.birth_date, right_applicant->student.name);
}

bool StudentsCompare(const Student* left_student, const Student* right_student) {
    return std::tie(left_student->name, left_student->birth_date) <
           std::tie(right_student->name, right_student->birth_date);
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
    std::sort(applicants_copy.begin(), applicants_copy.end(), ApplicantCompare);
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
        std::sort(students.begin(), students.end(), StudentsCompare);
    }
    return universities_students;
}
