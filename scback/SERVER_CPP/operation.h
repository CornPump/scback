#ifndef OPERATION_H
#define OPERATION_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <boost/asio.hpp>
#include "helpers_request.h"


extern std::string BACKUP_DIR_NAME;

void write_names_to_file(const std::vector<std::string>& fileNames, const std::string& outputFileName);
std::vector<std::string> get_dir_files(std::string dir_name);
std::string generate_random_name(const int len);
std::string create_dir(const std::string& path, const std::string& new_name);
long long get_file_size(const std::string& filename);
void send_file(const std::string& filename, tcp::socket& socket, uint32_t size);
void receive_file(const std::string& filename, tcp::socket& socket, uint32_t size);

#endif // OPERATION_H
