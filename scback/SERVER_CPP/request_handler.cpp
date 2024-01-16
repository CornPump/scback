#include "request_handler.h"
#include "helpers_request.h" 
#include "helpers_response.h" 
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>


void RequestHandler::clear(uint8_t message[], int length) {
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}

void RequestHandler::print() const{

    std::cout << "(user_id:" << this->user_id << ", c_version:" << static_cast<int>(this->c_version) << 
                " ,opcode:" << static_cast<int>(this->opcode) << "\n" << " ,name_len:" << this->name_len << 
                " ,file_name:" << this->file_name << " ,size:" << this->size << ")" << std::endl;
}

void RequestHandler::save_and_backup(RequestType opcode, uint16_t name_len, std::string file_name, uint32_t size) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::retrieve_file(RequestType opcode, uint16_t name_len, std::string file_name) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::delete_file(RequestType opcode, uint16_t name_len, std::string file_name) {
    std::cout << "USE FUNCTION";
}
void RequestHandler::list_files(RequestType opcode) {
    std::cout << "USE FUNCTION";
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
void RequestHandler::manage_request(RequestType opcode) {

    switch (opcode) {

    case RequestType::SAVE_FILE:
        save_and_backup(opcode, this->name_len, this->file_name, this->size);
            break;

    case RequestType::RETRIEVE_FILE:
        retrieve_file(opcode, this->name_len, this->file_name);
            break;

    case RequestType::DELETE_FILE:
        delete_file(opcode, this->name_len, this->file_name);
            break;

    case RequestType::DIR:
        list_files(opcode);
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
