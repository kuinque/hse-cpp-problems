#include "unixpath.h"

#include <vector>

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string> last_dirs;
    std::string current_dir;
    for (int pos = 1; pos < current_working_dir.size(); ++pos) {
        if (current_working_dir[pos] == '/') {
            last_dirs.push_back(current_dir);
            current_dir.clear();
        } else {
            current_dir += current_working_dir[pos];
        }
    }
    last_dirs.push_back(current_dir);
    current_dir.clear();
    for (int pos = 0; pos < path.size(); ++pos) {
        if (path[pos] == '/') {
            if (!current_dir.empty()) {
                last_dirs.push_back(current_dir);
                current_dir.clear();
            }
            continue;
        } else if (path[pos] == '.' && (pos + 1 == path.size() || path[pos + 1] != '.')) {
            continue;
        } else if (path[pos] == '.') {
            if (!last_dirs.empty()) {
                last_dirs.pop_back();
            }
            ++pos;
        } else {
            current_dir += path[pos];
        }
    }
    if (!current_dir.empty()) {
        last_dirs.push_back(current_dir);
        current_dir.clear();
    }
    std::string full_path;
    for (std::string& dir : last_dirs) {
        full_path += "/";
        full_path += dir;
    }
    if (full_path.empty()) {
        full_path += "/";
    }
    return full_path;
}
