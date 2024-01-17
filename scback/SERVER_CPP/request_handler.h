#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include "helpers_request.h"  
#include "response_handler.h" 
#include <string>

using boost::asio::ip::tcp;

class RequestHandler {

    uint32_t user_id;
    uint8_t c_version;
    RequestType opcode;
    uint16_t name_len = 0;
    std::string file_name;
    uint32_t size = 0;
    std::string backup_dir;


    void save_and_backup(RequestType opcode, uint16_t name_len, std::string file_name, uint32_t size);
    void retrieve_file(RequestType opcode, uint16_t name_len, std::string file_name);
    void delete_file(RequestType opcode, uint16_t name_len, std::string file_name);
    void list_files(tcp::socket& sock, ResponseHandler& resh);

public:

    RequestHandler(std::string backup_dir);

    // Function to create and handle requests
    static void clear(uint8_t message[], int length);
    void manage_request(RequestType opcode, tcp::socket& sock, ResponseHandler& resh);
    bool validate_request_number(RequestType opcode, tcp::socket &sock, ResponseHandler& resh);
    uint8_t validate_request_header(tcp::socket &sock, ResponseHandler &resh);
    void print() const;
};

#endif //REQUEST_HANDLER_H
