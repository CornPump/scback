#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <filesystem>

std::string BACKUP_DIR_NAME("backup_server");

std::string create_dir(const std::string& path, const std::string& new_name) {
    std::filesystem::path new_dir = std::filesystem::path(path) / new_name;

    if (!std::filesystem::exists(new_dir)) {
        std::filesystem::create_directory(new_dir);
    }

    return new_dir.string();
}
	

#endif // OPERATION_H
