#include <iostream>
#include <boost/asio.hpp>
#include "request_handler.h"
#include "response_handler.h"
#include "operation.h"
#include <filesystem>

using boost::asio::ip::tcp;

int main() {

    // create backup dir 
    std::string working_dir = std::filesystem::current_path().string();
    std::string backup_dir = create_dir(working_dir, BACKUP_DIR_NAME);
    std::cout << "Setting backup directory: " << backup_dir << "\Spawning a socket.." << std::endl;

    // set socket info
    int port = 1234;
    boost::asio::io_context io_context;
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Waiting for client.. \n";
    tcp::socket sock = a.accept(); // this waits for client to open session
    std::cout << "Accepting client \n";
    sock.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{5000});

    // response handler instance
    ResponseHandler resh;

    // Wait for the client to send data and validate the header
    RequestHandler reqh;
    uint8_t opcode = reqh.validate_request_header(sock,resh);

    // if the header is incorrect send error message 
    if (!opcode) { resh.send_error_message(sock, ResponseType::F_ERROR); }

    else {

        reqh.print();
        reqh.manage_request(static_cast<RequestType>(opcode));
    }
    return 0;
}


