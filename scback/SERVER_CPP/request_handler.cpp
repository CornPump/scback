#include "request_handler.h"
#include "helpers_request.h"  
#include <string>
#include <iostream>
#include <stdexcept>

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

uint8_t RequestHandler::validate_request_header(size_t length, uint8_t (&data)[MESSAGE_MAX_LENGTH]) {
    
    std::cout << length;
    if (length < MIN_REQUEST_SIZE) {
        
        throw std::runtime_error("A runtime error occurred");
    }
    uint32_t user_id;
    uint8_t client_version, opcode;

    std::memcpy(&user_id, data, sizeof(uint32_t));
    std::memcpy(&client_version, data + sizeof(uint32_t), sizeof(uint8_t));
    std::memcpy(&opcode, data + sizeof(uint32_t) + sizeof(uint8_t), sizeof(uint8_t));

    // Print the interpreted values
    std::cout << "user_id: " << user_id << std::endl;
    std::cout << "client_version: " << static_cast<int>(client_version) << std::endl;
    std::cout << "opcode: " << static_cast<int>(opcode) << std::endl;

    return opcode;
}

/*
int main() {
        RequestHandler rh;

        uint8_t opcode = static_cast<uint8_t>(RequestType::SAVE_FILE);
        RequestType tmp = static_cast<RequestType>(8);

        rh.create_request(static_cast<RequestType>(opcode));
        opcode = static_cast<uint8_t>(RequestType::RETRIEVE_FILE);
        rh.create_request(static_cast<RequestType>(opcode));
        opcode = static_cast<uint8_t>(RequestType::DELETE_FILE);
        rh.create_request(static_cast<RequestType>(opcode));
        opcode = static_cast<uint8_t>(RequestType::DIR);
        rh.create_request(static_cast<RequestType>(opcode));
        try{
            rh.create_request(static_cast<RequestType>(tmp));
        }
        catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        return 0;
}*/
