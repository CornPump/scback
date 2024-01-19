#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include "helpers_response.h"
#include <boost/asio.hpp>
#include <filesystem>

using boost::asio::ip::tcp;


class ResponseHandler {

	uint8_t server_version = SERVER_CUR_VERSION;
	uint16_t status;
	uint16_t name_len;
	std::string filename;
	uint32_t size =0;

public:

	uint8_t get_server_version();
	void set_status(const uint16_t status);
	uint16_t get_status() const;
	void set_name_len(const uint16_t len);
	uint16_t get_name_len() const;
	void set_filename(const std::string& name);
	std::string get_filename() const;
	void set_size(const uint32_t size);
	uint32_t get_size() const;
	void send_error_message(tcp::socket& sock, ResponseType error);
	void send_success_header(tcp::socket& sock);
	void print() const;


};


#endif // RESPONSE_HANDLER_H