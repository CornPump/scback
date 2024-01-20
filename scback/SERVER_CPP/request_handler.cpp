#include "request_handler.h"
#include "response_handler.h"
#include "helpers_request.h" 
#include "helpers_response.h" 
#include "operation.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>


RequestHandler::RequestHandler(std::string backup_dir) {

    this->backup_dir = backup_dir;
}

void RequestHandler::clear(uint8_t message[], int length) {
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}

void RequestHandler::print() const{

    std::cout << "(user_id:" << this->user_id << ", c_version:" << static_cast<int>(this->c_version) << 
                " ,opcode:" << static_cast<int>(this->opcode) << "\n" << " ,name_len:" << this->name_len << 
                " ,file_name:" << this->file_name << " ,size:" << this->size << 
                ", backup_dir:" << this->backup_dir << ")" << std::endl;
}

void RequestHandler::save_and_backup(tcp::socket& sock) {

    std::cout << "In save_and_backup() for user_id:" << this->user_id << std::endl;

    std::string user_dir_name = std::to_string(this->user_id);
    std::filesystem::path full_path = std::filesystem::path(std::filesystem::current_path()) / BACKUP_DIR_NAME;
    std::string full_name = create_dir(full_path.string(), user_dir_name);

    std::filesystem::path file_full_path = std::filesystem::path (full_name) / this->file_name;
    bool file_exist = check_file_exist(file_full_path);

    // if the file already exist remove the old one
    if (file_exist) {
        
        std::filesystem::remove(file_full_path);
    }

    receive_file(file_full_path.string(), sock, this->size);

    ResponseHandler resh;

    resh.set_status(static_cast<uint8_t>(ResponseType::S_DELETE_OR_BACKUP));
    resh.set_name_len(static_cast<uint16_t>(this->file_name.length()));
    resh.set_filename(this->file_name);
    resh.send_success_header(sock);

}
void RequestHandler::retrieve_file(tcp::socket& sock) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::delete_file(tcp::socket& sock) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::list_files(tcp::socket& sock) {

    std::cout << "In list_files() for user_id:" << this->user_id << std::endl;
    bool found_dir_flag = false;

    // Search for directory with user_id as name
    for (const auto& entry : std::filesystem::directory_iterator(this->backup_dir)) {
        // Check if it's a regular file or a directory
        if (entry.is_directory()) {

            uint32_t dir_name = std::stoul(entry.path().filename().string());

            // If found fetch all files in the directory and write them into a file
            if(dir_name == this->user_id){

                const int len_size = 32;
                found_dir_flag = true;
                std::cout << "Found matching file: " << entry.path().filename() << std::endl;

                std::string file_name = generate_random_name(len_size);
                std::filesystem::path BACKUP_DIR_NAME;

                std::filesystem::path user_dir = BACKUP_DIR_NAME / entry;
                write_names_to_file(get_dir_files(user_dir.string()), file_name);

                ResponseHandler resh;

                resh.set_status(static_cast<uint8_t>(ResponseType::S_DIR));
                resh.set_name_len(len_size);
                resh.set_filename(file_name);

                std::filesystem::path full_path = std::filesystem::path(std::filesystem::current_path()) / file_name;

                uint32_t file_size = static_cast<uint32_t>(get_file_size(full_path.string()));
                resh.set_size(file_size);
                
                resh.print();
                resh.send_success_header(sock);
                send_file(full_path.string(),sock, file_size);
                break;
            }
        }
    }
    if (!found_dir_flag) {
        ResponseHandler resh;
        resh.send_error_message(sock, ResponseType::F_DIR);
    }
}

bool RequestHandler::validate_request_number(RequestType opcode, tcp::socket &sock, ResponseHandler& resh) {

    switch (opcode) {
    case RequestType::SAVE_FILE:
    case RequestType::RETRIEVE_FILE:
    case RequestType::DELETE_FILE:
        uint8_t data[MESSAGE_MAX_LENGTH];
        clear(data, MESSAGE_MAX_LENGTH);
        try {
            boost::asio::read(sock, boost::asio::buffer(data, NAME_LEN));
        }
        catch (...) {
            std::cout << "Timeout error in  read name_len" << std::endl;
            return false;

        }
        std::memcpy(&this->name_len, data, sizeof(uint16_t));

        if (this->name_len) {
            clear(data, MESSAGE_MAX_LENGTH);
            try {
                boost::asio::read(sock, boost::asio::buffer(data, this->name_len));
            }
            catch (...) {
                std::cout << "Timeout error in  read filename" << std::endl;
                return false;

            }
            this->file_name.assign(reinterpret_cast<const char*>(data), name_len);
        }
        else { return false; }

        if (opcode == RequestType::SAVE_FILE) {
            clear(data, MESSAGE_MAX_LENGTH);
            try {
                boost::asio::read(sock, boost::asio::buffer(data, SIZE_));
            }
            catch (...) {
                std::cout << "Timeout error in file size" << std::endl;
                return false;

            }
            std::memcpy(&this->size, data, sizeof(uint32_t));
        }
    case RequestType::DIR:
        return true;
    default:
        return false;
    }
}
// Function to create and handle requests
void RequestHandler::manage_request(RequestType opcode, tcp::socket& sock) {

    switch (opcode) {

    case RequestType::SAVE_FILE:
        save_and_backup(sock);
            break;

    case RequestType::RETRIEVE_FILE:
        retrieve_file(sock);
            break;

    case RequestType::DELETE_FILE:
        delete_file(sock);
            break;

    case RequestType::DIR:
        list_files(sock);
            break;
    }
}

uint8_t RequestHandler::validate_request_header(tcp::socket &sock,ResponseHandler &resh) {
        
        uint8_t data[MESSAGE_MAX_LENGTH];
        clear(data, MESSAGE_MAX_LENGTH);
        try{
            boost::asio::read(sock, boost::asio::buffer(data, MIN_REQUEST_SIZE));
        }
        // If error or time out send client error message and retorn 0
        catch (...) {
            std::cout << "Timeout error in first header read" << std::endl;
            return 0;

        }
        // if no error fetch minimum header
        std::memcpy(&this->user_id, data, sizeof(uint32_t));
        std::memcpy(&this->c_version, data + sizeof(uint32_t), sizeof(uint8_t));
        std::memcpy(&this->opcode, data + sizeof(uint32_t) + sizeof(uint8_t), sizeof(uint8_t));

        std::cout << "Recivied Request: (user_id:" << this->user_id << ", client_version:"
            << static_cast<int>(this->c_version) << ",opcode: " << static_cast<int>(this->opcode) <<
            ")" << std::endl;
        std::cout << "Validating full header.. " <<  std::endl;
        bool is_valid_header = validate_request_number(static_cast<RequestType>(opcode),sock,resh);
        if (!is_valid_header) { return 0; }
        

    return static_cast<uint8_t>(this->opcode);
}
