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

bool operator<(const Student& left_student, const Student& right_student) {
    if (left_student.birth_date < right_student.birth_date) {
        return true;
    } else if (left_student.birth_date != right_student.birth_date) {
        return false;
    }
    if (left_student.name < right_student.name) {
        return true;
    }
    return false;
}

bool operator>(const Applicant& left_applicant, const Applicant& right_applicant) {
    if (left_applicant.points > right_applicant.points) {
        return true;
    } else if (left_applicant.points < right_applicant.points) {
        return false;
    }
    return left_applicant.student < right_applicant.student;
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::unordered_map<std::string, size_t> university_position;
    std::vector<size_t> universities_student_count(universities.size());
    for (size_t pos = 0; pos < universities.size(); ++pos) {
        university_position[universities[pos].name] = pos;
    }
    std::vector<Applicant> applicants_copy = applicants;
    std::sort(applicants_copy.begin(), applicants_copy.end(),
        [&](Applicant& left_applicant, Applicant& right_applicant) {
            return (left_applicant > right_applicant);
        });
    std::vector<University> universities_copy = universities;
    sort(universities_copy.begin(), universities_copy.end(),
         [&](University left_university, University right_university) {
             return left_university.name > right_university.name;
        });
    AdmissionTable universities_students;
    for (auto& university : universities_copy) {
        universities_students[university.name] = {};
    }
    for (Applicant& applicant : applicants_copy) {
        for (std::string& wished_university : applicant.wish_list) {
            if (universities_student_count[university_position[wished_university]] ==
                universities[university_position[wished_university]].max_students) {
                continue;
            }
            Student* p = &applicant.student;
            universities_students[wished_university].push_back(p);
            ++universities_student_count[university_position[wished_university]];
            break;
        }
    }
    for (auto& tmp : universities_students) {
        std::sort(tmp.second.begin(), tmp.second.end(),
            [&](const Student* left_student, const Student* right_student) {
                return &left_student < &right_student;
            });
    }
    return universities_students;
}
