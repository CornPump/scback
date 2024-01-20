#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <boost/asio.hpp>
#include "helpers_request.h"
#include <filesystem>

std::string BACKUP_DIR_NAME("backup_server");

void write_names_to_file(const std::vector<std::string>& fileNames, const std::string& outputFileName) {
    std::ofstream file(outputFileName);

    if (!file.is_open()) {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        return;
    }

    for (const auto& fileName : fileNames) {
        file << fileName << "\n";
    }

    std::cout << "File write complete." << std::endl;
}

std::vector<std::string> get_dir_files(std::string dir_name) {

    std::vector<std::string> to_ret;

    for (const auto& entry : std::filesystem::directory_iterator(dir_name)) {

        if (!entry.is_directory()) {
            to_ret.push_back(entry.path().filename().string());
        }
    }
    return to_ret;
}


std::string generate_random_name(const int len) {

    std::string to_ret;

    const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int num_characters = sizeof(characters) - 1;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < len; i++) {


        int random_index = std::rand() % num_characters;
        to_ret += characters[random_index];

    }

    return to_ret;
}

std::string create_dir(const std::string& path, const std::string& new_name) {
    std::filesystem::path new_dir = std::filesystem::path(path) / new_name;

    if (!std::filesystem::exists(new_dir)) {
        std::filesystem::create_directory(new_dir);
    }

    return new_dir.string();
}


long long get_file_size(const std::string& filename) {
    try {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return -1;
        }
        else {
            std::streamsize file_size = file.tellg();

            return file_size;
        }
    }
    catch (...) {
        return -1;
    }
}

void send_file(const std::string& filename, boost::asio::ip::tcp::socket& socket, uint32_t size) {
    try {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        // Send file content in chunks 
        std::vector<char> buffer(MESSAGE_MAX_LENGTH);

        while (!file.eof()) {
            file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            boost::asio::write(socket, boost::asio::buffer(buffer.data(), file.gcount()));
        }

        std::cout << "File sent successfully: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending file: " << e.what() << std::endl;
    }
}

void receive_file(const std::string& filename, boost::asio::ip::tcp::socket& socket, uint32_t size) {
    try {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }

        std::vector<char> buffer(MESSAGE_MAX_LENGTH);

        std::size_t total_received = 0;
        while (total_received < static_cast<std::size_t>(size)) {

            std::size_t bytes_to_receive = std::min(static_cast<std::size_t>(MESSAGE_MAX_LENGTH), static_cast<std::size_t>(size - total_received));
            boost::asio::read(socket, boost::asio::buffer(buffer.data(), bytes_to_receive));
            file.write(buffer.data(), static_cast<std::streamsize>(bytes_to_receive));
            total_received += bytes_to_receive;

        }

        std::cout << "File received successfully: " << filename << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error receiving file: " << e.what() << std::endl;
    }
}

bool check_file_exist(std::filesystem::path file_path) {

    if (std::filesystem::exists(file_path)) {
        return true;
    }
    return false;

}