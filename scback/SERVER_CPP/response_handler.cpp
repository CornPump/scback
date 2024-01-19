#include "response_handler.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


void ResponseHandler::set_status(const uint16_t status) {

	this->status = status;
}

uint16_t ResponseHandler::get_status() const {

	return status;
}

void ResponseHandler::set_name_len(const uint16_t len) {

	this->name_len = len;
}

uint16_t ResponseHandler::get_name_len() const {

	return name_len;
}

void ResponseHandler::set_filename(const std::string& name) {

	this->filename = name;
}

std::string ResponseHandler::get_filename() const {

	return filename;
}

void ResponseHandler::set_size(const uint32_t size) {

	this->size = size;
}

uint32_t ResponseHandler::get_size() const {
	return size;
}


void ResponseHandler::send_error_message(tcp::socket &sock, ResponseType error) {

	std::vector<uint8_t> message;
	message.push_back(this->server_version);
	uint16_t value = static_cast<int>(error);
	value = htons(value);

	message.insert(message.end(), reinterpret_cast<uint8_t*>(&value),
		reinterpret_cast<uint8_t*>(&value) + sizeof(uint16_t));

	if (error == ResponseType::F_ERROR) {
		std::cerr << "Invalid Request, Responding error " <<
			static_cast<int>(ResponseType::F_ERROR) << ".."<< std::endl;
	}

	if (error == ResponseType::F_DIR) {
		std::cerr << "Client has no files / directory on server \nResponding error " <<
			static_cast<int>(ResponseType::F_DIR) << "..";
	}

	// Send the message using boost::asio::write
	boost::asio::write(sock, boost::asio::buffer(message));

}