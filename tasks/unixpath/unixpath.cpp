#include <vector>

#include "unixpath.h"

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string> last_dirs;
    std::string current_dir;
    for (size_t pos = 1; pos < current_working_dir.size(); ++pos) {
        if (current_working_dir[pos] == '/') {
            last_dirs.push_back(current_dir);
            current_dir.clear();
        } else {
            current_dir += current_working_dir[pos];
        }
    }
    if (!current_dir.empty()) {
        last_dirs.push_back(current_dir);
        current_dir.clear();
    }
    if (!path.empty() && path[0] == '/') {
        last_dirs.clear();
    }
    std::vector<std::string> minor_dirs;
    for (size_t pos = 0; pos < path.size(); ++pos) {
        if (path[pos] == '/') {
            minor_dirs.push_back(current_dir);
            current_dir.clear();
        } else {
            current_dir += path[pos];
        }
    }
    minor_dirs.push_back(current_dir);
    current_dir.clear();
    for (std::string& dir : minor_dirs) {
        if (dir.empty() || dir == ".") {
            continue;
        } else if (dir == "..") {
            if (!last_dirs.empty()) {
                last_dirs.pop_back();
            }
        } else {
            last_dirs.push_back(dir);
        }
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
