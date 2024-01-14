#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include "helpers_request.h"  
#include <string>

using boost::asio::ip::tcp;

class RequestHandler {

    void save_and_backup(RequestType opcode, uint16_t name_len, std::string file_name, uint32_t size, std::string Payload);
    void retrieve_file(RequestType opcode, uint16_t name_len, std::string file_name);
    void delete_file(RequestType opcode, uint16_t name_len, std::string file_name);
    void list_files(RequestType opcode);

public:
    // Function to create and handle requests
    void create_request(RequestType opcode);
    bool validate_request_number(RequestType opcode);
    int8_t validate_request_header(tcp::socket sock);

};

#endif //REQUEST_HANDLER_H
