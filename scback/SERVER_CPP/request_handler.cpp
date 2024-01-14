#include "request_handler.h"
#include "helpers_request.h" 
#include "helpers_response.h" 
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>


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

bool RequestHandler::validate_request_number(RequestType opcode) {

    switch (opcode) {
    case RequestType::SAVE_FILE:
    case RequestType::RETRIEVE_FILE:
    case RequestType::DELETE_FILE:
    case RequestType::DIR:
        return true;
    default:
        return false;
    }
}
// Function to create and handle requests
void RequestHandler::create_request(RequestType opcode) {

    if (validate_request_number(opcode)) {
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
    else {
        throw std::invalid_argument("Invalid opcode: " + std::to_string(static_cast<int>(opcode)));
    }
}

int8_t RequestHandler::validate_request_header(tcp::socket sock) {
        
        int8_t ret_code = -1;
        uint32_t user_id;
        uint8_t c_version, opcode;

        uint8_t data[MESSAGE_MAX_LENGTH];
        boost::system::error_code ec;

        boost::asio::read(sock, boost::asio::buffer(data, MIN_REQUEST_SIZE), ec);

        if (!ec) {
            std::memcpy(&user_id, data, sizeof(uint32_t));
            std::memcpy(&c_version, data + sizeof(uint32_t), sizeof(uint8_t));
            std::memcpy(&opcode, data + sizeof(uint32_t) + sizeof(uint8_t), sizeof(uint8_t));

            std::cout << "Recivied Request: (user_id:" << user_id << ", client_version:"
                << static_cast<int>(c_version) << ",opcode: " << static_cast<int>(opcode) <<
                ")" << std::endl;
        }

        else { 
            std::cerr << "Bad Request " << ec.what() << "\nResponding error " << 
                static_cast<int>(ResponseType::F_ERROR) << "..";
            
            std::vector<uint8_t> message;
            message.push_back(SERVER_VERSION);
            uint16_t value = static_cast<int>(ResponseType::F_ERROR);
            value = htons(value);

            message.insert(message.end(), reinterpret_cast<uint8_t*>(&value),
                reinterpret_cast<uint8_t*>(&value) + sizeof(uint16_t));

            // Send the message using boost::asio::write
            boost::asio::write(sock, boost::asio::buffer(message));
        
        
        }

    return ret_code;
}
