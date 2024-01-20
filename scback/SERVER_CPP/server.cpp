#include <iostream>
#include <boost/asio.hpp>
#include "request_handler.h"
#include "operation.h"
#include <filesystem>

using boost::asio::ip::tcp;

void handle_client(tcp::socket&& sock, const std::string& backup_dir) {
    uint8_t opcode = 1;

    while (opcode) {
        // response handler instance
        ResponseHandler resh;

        // Wait for the client to send data and validate the header
        RequestHandler reqh(backup_dir);
        opcode = reqh.validate_request_header(sock, resh);

        // if the header is incorrect send an error message
        if (!opcode) {
            resh.send_error_message(sock, ResponseType::F_ERROR);
        }
        else {
            reqh.print();
            reqh.manage_request(static_cast<RequestType>(opcode), sock);
        }
    }
}


int main() {
    
    // create backup dir 
    std::string working_dir = std::filesystem::current_path().string();
    std::string backup_dir = create_dir(working_dir, BACKUP_DIR_NAME);
    std::cout << "Setting backup directory: " << backup_dir << "\nSpawning a socket.." << std::endl;

    // set socket info
    int port = 1234;
    boost::asio::io_context io_context;
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Waiting for clients.. \n";

    while (true) {
        try {
            // Accept a new connection
            tcp::socket sock = a.accept();
            sock.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{5000});
            // Spawn a new thread for each client
            std::thread(handle_client, std::move(sock), backup_dir).detach();
        }
        catch (const std::exception& e) {
            std::cerr << "Exception in main: " << e.what() << std::endl;
        }
    }
}


