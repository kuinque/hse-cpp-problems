#include "search.h"

#include <cmath>
#include <unordered_map>
#include <set>
#include <sstream>

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::set<std::string> query_words;
    std::string word;
    for (size_t i = 0; i <= query.size(); ++i) {
        if (i < query.size() && std::isalpha(query[i])) {
            word += static_cast<char>(std::tolower(query[i]));
        } else {
            if (!word.empty()) {
                query_words.insert(word);
            }
            word.clear();
        }
    }
    std::unordered_map<std::string, double> unique_word_count;
    size_t line_count = 0;
    for (size_t text_pointer = 0; text_pointer < text.size(); ++text_pointer) {
        size_t next_text_pointer = text_pointer;
        while (next_text_pointer < text.size() && text[next_text_pointer] != '\n') {
            ++next_text_pointer;
        }
        std::string_view line = text.substr(text_pointer, next_text_pointer - text_pointer);
        text_pointer = next_text_pointer;
        word.clear();
        std::set<std::string> unique_words;
        bool has_words = false;
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i < line.size() && std::isalpha(line[i])) {
                word += static_cast<char>(std::tolower(line[i]));
                has_words = true;
            } else {
                if (!word.empty() && query_words.find(word) != query_words.end()) {
                    unique_words.insert(word);
                }
                word.clear();
            }
        }
        if (has_words) {
            ++line_count;
        }
        for (const std::string& unique_word : unique_words) {
            ++unique_word_count[unique_word];
        }
    }
    std::vector<std::pair<double, size_t>> index_answer_strings;
    std::set<std::tuple<double, size_t, std::string_view>> answer_strings_set;
    for (size_t text_pointer = 0, line_number = 0; text_pointer < text.size(); ++text_pointer) {
        size_t next_text_pointer = text_pointer;
        while (next_text_pointer < text.size() && text[next_text_pointer] != '\n') {
            ++next_text_pointer;
        }
        std::string_view line = text.substr(text_pointer, next_text_pointer - text_pointer);
        text_pointer = next_text_pointer;
        std::unordered_map<std::string, double> word_occurrence;
        word.clear();
        double word_count = 0;
        for (size_t i = 0; i <= line.size(); ++i) {
            if (i < line.size() && std::isalpha(line[i])) {
                word += static_cast<char>(std::tolower(line[i]));
            } else {
                if (!word.empty()) {
                    ++word_count;
                    if (query_words.find(word) != query_words.end()) {
                        ++word_occurrence[word];
                    }
                }
                word.clear();
            }
        }
        if (word_count > 0) {
            ++word_count;
        }
        double best_line_relevant = 0;
        for (const std::string& query_word : query_words) {
            double relevant = (word_occurrence[query_word] / word_count) *
                              log(static_cast<double>(line_count) / unique_word_count[query_word]);
            best_line_relevant = std::max(best_line_relevant, relevant);
        }
        if (best_line_relevant > 0) {
            answer_strings_set.insert(std::make_tuple(best_line_relevant, line_count - line_number, line));
        }
        if (answer_strings_set.size() > results_count) {
            answer_strings_set.erase(answer_strings_set.begin());
        }
    }
    std::vector<std::string_view> answer_lines;
    for (auto iterator = answer_strings_set.begin(); iterator != answer_strings_set.end(); ++iterator) {
        answer_lines.push_back(get<2>(*iterator));
    }
    return answer_lines;
}
