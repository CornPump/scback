#include <iostream>
#include <boost/asio.hpp>
#include "request_handler.h"

using boost::asio::ip::tcp;

void clear(uint8_t message[], int length) {
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}
/*
int main() {
    int port = 1234;
    boost::asio::io_context io_context;
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Waiting for client \n";
    tcp::socket sock = a.accept(); // this waits for client to open session
    std::cout << "Accepting client \n";
    uint8_t data[MESSAGE_MAX_LENGTH];
    clear(data, MESSAGE_MAX_LENGTH);
    try {
        // Wait for the client to send data
        size_t length = boost::asio::read(sock, boost::asio::buffer(data, MESSAGE_MAX_LENGTH));
        RequestHandler rh;
        uint8_t opcode = rh.validate_request_header(length, data);

    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error reading from socket: " << e.what() << std::endl;


    return 0;
    }
    
    try {
        // Wait for the client to send data
        size_t length = boost::asio::read(sock, boost::asio::buffer(data, max_Length));

        // Interpret the received data as per the little-endian structure
        uint32_t category1;
        uint8_t category2, category3;

        std::memcpy(&category1, data, sizeof(uint32_t));
        std::memcpy(&category2, data + sizeof(uint32_t), sizeof(uint8_t));
        std::memcpy(&category3, data + sizeof(uint32_t) + sizeof(uint8_t), sizeof(uint8_t));

        // Print the interpreted values
        std::cout << "Category 1: " << category1 << std::endl;
        std::cout << "Category 2: " << static_cast<int>(category2) << std::endl;
        std::cout << "Category 3: " << static_cast<int>(category3) << std::endl;

    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error reading from socket: " << e.what() << std::endl;
    }
}*/


