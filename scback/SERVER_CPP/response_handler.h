#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

#include "helpers_response.h"
#include <boost/asio.hpp>
#include <filesystem>

using boost::asio::ip::tcp;


class ResponseHandler {

public:
	uint8_t server_version = SERVER_CUR_VERSION;

	void send_error_message(tcp::socket &sock, ResponseType error);


};


#endif // RESPONSE_HANDLER_H