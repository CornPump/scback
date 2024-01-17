#include "request_handler.h"
#include "helpers_request.h" 
#include "helpers_response.h" 
#include "response_handler.h"
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

void RequestHandler::save_and_backup(tcp::socket& sock, ResponseHandler& resh) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::retrieve_file(tcp::socket& sock, ResponseHandler& resh) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::delete_file(tcp::socket& sock, ResponseHandler& resh) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::list_files(tcp::socket& sock, ResponseHandler& resh) {

    std::cout << "In list_files() for user_id:" << this->user_id << std::endl;
    bool found_dir_flag = false;
    for (const auto& entry : std::filesystem::directory_iterator(this->backup_dir)) {
        // Check if it's a regular file or a directory
        if (entry.is_directory()) {

            uint32_t tmp = std::stoul(entry.path().filename().string());
            if(tmp == this->user_id){
                found_dir_flag = true;
                std::cout << "Found matching file: " << entry.path().filename() << std::endl;




                break;
            }
        }
    }
    if (!found_dir_flag) {
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
void RequestHandler::manage_request(RequestType opcode, tcp::socket& sock, ResponseHandler& resh) {

    switch (opcode) {

    case RequestType::SAVE_FILE:
        save_and_backup(sock, resh);
            break;

    case RequestType::RETRIEVE_FILE:
        retrieve_file(sock, resh);
            break;

    case RequestType::DELETE_FILE:
        delete_file(sock, resh);
            break;

    case RequestType::DIR:
        list_files(sock, resh);
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
        std::cout << ", Validating full header.. " <<  std::endl;
        bool is_valid_header = validate_request_number(static_cast<RequestType>(opcode),sock,resh);
        if (!is_valid_header) { return 0; }
        

    return static_cast<uint8_t>(this->opcode);
}
