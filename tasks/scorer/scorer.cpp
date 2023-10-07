#include <unordered_map>

#include "scorer.h"

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::vector<const Event*> sorted_events;
    for (const Event& event : events) {
        sorted_events.push_back(&event);
    }
    std::sort(sorted_events.begin(), sorted_events.end(),
              [](const Event* left_event, const Event* right_event) { return left_event->time < right_event->time; });
    ScoreTable score_table;
    std::map<StudentName, std::map<TaskName, std::pair<bool, bool>>> students_tasks_status;
    for (const Event* event : sorted_events) {
        if (event->time > score_time) {
            break;
        }
        switch (event->event_type) {
            case EventType::CheckFailed:
                students_tasks_status[event->student_name][event->task_name].first = false;
                break;
            case EventType::CheckSuccess:
                students_tasks_status[event->student_name][event->task_name].first = true;
                break;
            case EventType::MergeRequestOpen:
                students_tasks_status[event->student_name][event->task_name].second = true;
                break;
            case EventType::MergeRequestClosed:
                students_tasks_status[event->student_name][event->task_name].second = false;
                break;
        }
    }
    for (const auto& [student_name, student_tasks_status] : students_tasks_status) {
        for (const auto& [task_name, task_status] : student_tasks_status) {
            if (task_status.first && !task_status.second) {
                score_table[student_name].insert(task_name);
            }
        }
    }
    return score_table;
}
