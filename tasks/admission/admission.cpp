#include "admission.h"

#include <unordered_map>
#include <algorithm>

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
    std::unordered_map<std::string, size_t> university_position;
    std::vector<size_t> universities_student_count(universities.size());
    for (size_t pos = 0; pos < universities.size(); ++pos) {
        university_position[universities[pos].name] = pos;
    }
    std::vector<std::pair<Applicant, const Student*>> applicants_copy;
    for (size_t p = 0; p < applicants.size(); ++p) {
        applicants_copy.emplace_back(applicants[p], &applicants[p].student);
    }
    auto applicant_compare = [](Applicant& left_applicant, Applicant& right_applicant) -> bool {
        if (left_applicant.points > right_applicant.points) {
            return true;
        } else if (left_applicant.points != right_applicant.points) {
            return false;
        }
        if (left_applicant.student.birth_date < right_applicant.student.birth_date) {
            return true;
        } else if (left_applicant.student.birth_date != right_applicant.student.birth_date) {
            return false;
        }
        std::string left_student_name = left_applicant.student.name.substr(0, left_applicant.student.name.find(' '));
        std::string right_student_name = right_applicant.student.name.substr(0, right_applicant.student.name.find(' '));
        std::string left_student_surname = left_applicant.student.name.substr(
            left_student_name.size() + 1, left_applicant.student.name.size() - (left_student_name.size() + 1));
        std::string right_student_surname = right_applicant.student.name.substr(
            right_student_name.size() + 1, right_applicant.student.name.size() - (right_student_name.size() + 1));
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
    std::sort(applicants_copy.begin(), applicants_copy.end(),
              [&](std::pair<Applicant, const Student*>& left_applicant,
                  std::pair<Applicant, const Student*>& right_applicant) {
                  return applicant_compare(left_applicant.first, right_applicant.first);
              });
    AdmissionTable universities_students;
    for (std::pair<Applicant, const Student*>& applicant : applicants_copy) {
        for (std::string& wished_university : applicant.first.wish_list) {
            if (universities_student_count[university_position[wished_university]] ==
                universities[university_position[wished_university]].max_students) {
                continue;
            }
            universities_students[wished_university].push_back(applicant.second);
            ++universities_student_count[university_position[wished_university]];
            break;
        }
    }
    auto students_compare = [](const Student& left_student, const Student& right_student) -> bool {
        std::string left_student_name = left_student.name.substr(0, left_student.name.find(' '));
        std::string right_student_name = right_student.name.substr(0, right_student.name.find(' '));
        std::string left_student_surname = left_student.name.substr(
            left_student_name.size() + 1, left_student.name.size() - (left_student_name.size() + 1));
        std::string right_student_surname = right_student.name.substr(
            right_student_name.size() + 1, right_student.name.size() - (right_student_name.size() + 1));
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
        if (left_student.birth_date < right_student.birth_date) {
            return true;
        }
        return false;
    };
    for (auto& tmp : universities_students) {
        std::sort(tmp.second.begin(), tmp.second.end(), [&](const Student* left_student, const Student* right_student) {
            return students_compare(*left_student, *right_student);
        });
    }
    return universities_students;
}
