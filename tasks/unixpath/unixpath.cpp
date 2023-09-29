#include "unixpath.h"

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::string full_path = static_cast<std::string>(current_working_dir);
    if (!path.empty() && path[0] == '/') {
        full_path = "/";
    }
    full_path += "/";
    full_path += static_cast<std::string>(path);
    size_t full_path_pointer = 0;
    full_path += "/";
    for (size_t path_pointer = 0; path_pointer < full_path.size() - 1; ++path_pointer) {
        if (full_path[path_pointer] == '/' && full_path[path_pointer + 1] == '.' &&
            full_path[path_pointer + 2] == '.' && full_path[path_pointer + 3] == '/') {
            while (full_path[full_path_pointer] != '/') {
                --full_path_pointer;
            }
            path_pointer += 2;
        } else if (full_path[path_pointer] == '/' && full_path[path_pointer + 1] == '.' &&
                   full_path[path_pointer + 2] == '/') {
            ++path_pointer;
            continue;
        } else if (full_path[path_pointer + 1] != '/') {
            ++path_pointer;
            if (full_path[full_path_pointer] != '/') {
                ++full_path_pointer;
                full_path[full_path_pointer] = '/';
            }
            while (full_path[path_pointer] != '/') {
                ++full_path_pointer;
                full_path[full_path_pointer] = full_path[path_pointer];
                ++path_pointer;
            }
            --path_pointer;
        }
    }
    return full_path.substr(0, full_path_pointer + 1);
}
