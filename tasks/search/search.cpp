#include "search.h"

#include <cmath>
#include <unordered_map>
#include <set>
#include <sstream>

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::istringstream query_stream(static_cast<std::string>(query));
    std::set<std::string> query_words;
    for (std::string s; getline(query_stream, s, ' ');) {
        query_words.insert(s);
    }
    std::unordered_map<std::string, double> unique_word_count;
    std::istringstream text_stream((static_cast<std::string>(text)));
    double line_count = 0;
    for (std::string line; getline(text_stream, line, '\n');) {
        ++line_count;
        std::string word;
        std::set<std::string> unique_words;
        line += " ";
        for (size_t i = 0; i < line.size(); ++i) {
            if (std::isalpha(line[i])) {
                word += static_cast<char>(std::tolower(line[i]));
            } else {
                if (!word.empty() && query_words.find(word) != query_words.end()) {
                    unique_words.insert(word);
                }
                word = "";
            }
        }
        for (const std::string& unique_word : unique_words) {
            ++unique_word_count[unique_word];
        }
    }
    std::vector<std::string_view> answer_lines;
    std::vector<size_t> index_answer_strings;
    double best_relevant = 0;
    text_stream.clear();
    for (size_t text_pointer = 0; text_pointer < text.size(); ++text_pointer) {
        size_t next_text_pointer = text_pointer;
        while (text_pointer < text.size() && text[next_text_pointer] != '\n') {
            ++next_text_pointer;
        }
        std::string_view line = text.substr(text_pointer, next_text_pointer - text_pointer);
        text_pointer = next_text_pointer;
        std::unordered_map<std::string, double> word_occurrence;
        std::string word;
        double word_count = 0;
        for (size_t i = 0; i < line.size(); ++i) {
            if (std::isalpha(line[i])) {
                word += static_cast<char>(std::tolower(line[i]));
            } else {
                if (!word.empty()) {
                    ++word_count;
                    if (query_words.find(word) != query_words.end()) {
                        ++word_occurrence[word];
                    }
                }
                word = "";
            }
        }
        if (!word.empty()) {
            ++word_count;
            if (query_words.find(word) != query_words.end()) {
                ++word_occurrence[word];
            }
        }
        double best_line_relevant = 0;
        for (const std::string& query_word : query_words) {
            double relevant =
                (word_occurrence[query_word] / word_count) * log(line_count / unique_word_count[query_word]);
            best_line_relevant = std::max(best_line_relevant, relevant);
        }
        if (best_line_relevant > best_relevant) {
            answer_lines.clear();
            best_relevant = best_line_relevant;
        }
        if (best_line_relevant > 0 && best_relevant == best_line_relevant && answer_lines.size() < results_count) {
            answer_lines.push_back(line);
        }
    }
    return answer_lines;
}
