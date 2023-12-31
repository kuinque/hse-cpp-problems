#include "tests_checking.h"

#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string> tests_deque;
    for (const StudentAction& student_action : student_actions) {
        if (student_action.side == Side::Bottom) {
            tests_deque.push_back(student_action.name);
        } else {
            tests_deque.push_front(student_action.name);
        }
    }
    std::vector<std::string> queries_answers(queries.size());
    for (size_t query_number = 0; query_number < queries.size(); ++query_number) {
        queries_answers[query_number] = tests_deque[queries[query_number] - 1];
    }
    return queries_answers;
}
