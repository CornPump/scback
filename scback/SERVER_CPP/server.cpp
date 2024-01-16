#include <iostream>
#include <boost/asio.hpp>
#include "request_handler.h"
#include "response_handler.h"
using boost::asio::ip::tcp;

void clear(uint8_t message[], int length) {
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}

int main() {
    int port = 1234;
    boost::asio::io_context io_context;
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Waiting for client \n";
    tcp::socket sock = a.accept(); // this waits for client to open session
    std::cout << "Accepting client \n";
    sock.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{5000});
    uint8_t data[MESSAGE_MAX_LENGTH];
    clear(data, MESSAGE_MAX_LENGTH);
    uint8_t opcode = 0;
    ResponseHandler resh;
    // Wait for the client to send data
    RequestHandler reqh;
    opcode = reqh.validate_request_header(sock,resh);
    reqh.print();
        
    return 0;
}


