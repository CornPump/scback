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

    std::cout << "user_id:" << this->user_id << "\n";
    std::cout << "c_version:" << static_cast<int>(this->c_version) << "\n";
    std::cout << "opcode:" << static_cast<int>(this->opcode) << "\n";
    std::cout << "name_len:" << this->name_len << "\n";
    std::cout << "file_name:" << this->file_name << "\n";
}

void RequestHandler::save_and_backup(RequestType opcode, uint16_t name_len, std::string file_name, uint32_t size, std::string Payload) {
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
        catch (const boost::system::system_error& e) {
            std::cout << "Timeout error in  read name_len" << std::endl;
            resh.send_error_message(sock, ResponseType::F_ERROR);
            return false;

        }
        std::memcpy(&this->name_len, data, sizeof(uint16_t));
        std::cout << "Name_len: " << this->name_len << std::endl;

        if (this->name_len) {
            clear(data, MESSAGE_MAX_LENGTH);
            try {
                boost::asio::read(sock, boost::asio::buffer(data, this->name_len));
            }
            catch (const boost::system::system_error& e) {
                std::cout << "Timeout error in  read filename" << std::endl;
                resh.send_error_message(sock, ResponseType::F_ERROR);
                return false;

            }
            this->file_name.assign(reinterpret_cast<const char*>(data), name_len);
            std::cout << "Name_len: " << this->file_name << std::endl;
        }
        else { resh.send_error_message(sock, ResponseType::F_ERROR);}
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
        std::cout << std::to_string(static_cast<int>(opcode)) + "\n";
            break;
    case RequestType::RETRIEVE_FILE:
        std::cout << std::to_string(static_cast<int>(opcode)) + "\n";
            break;
    case RequestType::DELETE_FILE:
        std::cout << std::to_string(static_cast<int>(opcode)) + "\n";
            break;
    case RequestType::DIR:
        std::cout << std::to_string(static_cast<int>(opcode)) + "\n";
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
        catch (const boost::system::system_error& e) {
            std::cout << "Timeout error in first header read" << std::endl;
            resh.send_error_message(sock, ResponseType::F_ERROR);
            return 0;

        }
        // if no error fetch minimum header
        std::memcpy(&this->user_id, data, sizeof(uint32_t));
        std::memcpy(&this->c_version, data + sizeof(uint32_t), sizeof(uint8_t));
        std::memcpy(&this->opcode, data + sizeof(uint32_t) + sizeof(uint8_t), sizeof(uint8_t));

        std::cout << "Recivied Request: (user_id:" << this->user_id << ", client_version:"
            << static_cast<int>(this->c_version) << ",opcode: " << static_cast<int>(this->opcode) <<
            ")" << std::endl;
        bool is_valid_header = validate_request_number(static_cast<RequestType>(opcode),sock,resh);
        std::cout << "is_valid_header: " << is_valid_header << std::endl;

    return static_cast<uint8_t>(this->opcode);
}
