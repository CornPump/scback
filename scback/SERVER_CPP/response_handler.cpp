#include "response_handler.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>


void ResponseHandler::print() const {

	std::cout << "(server_version:" << this->server_version << ", status:" << static_cast<int>(this->status) <<
		" ,name_len:" << static_cast<int>(this->name_len) << "\n" << " ,file_name:" << this->filename <<
		" ,size:" << static_cast<int>(this->size) << ")" << std::endl;
}


uint8_t ResponseHandler::get_server_version() {
	return server_version;
}

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
	message.push_back(this->get_server_version());
	uint16_t value = static_cast<uint16_t>(error);
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

void ResponseHandler::send_success_header(tcp::socket& sock) {
	
	std::vector<uint8_t> message;

	message.push_back(this->get_server_version());

	status = htons(this->get_status());
	message.insert(message.end(), reinterpret_cast<uint8_t*>(&status),
	reinterpret_cast<uint8_t*>(&status) + sizeof(uint16_t));

	name_len = htons(this->get_name_len());
	message.insert(message.end(), reinterpret_cast<uint8_t*>(&name_len),
		reinterpret_cast<uint8_t*>(&name_len) + sizeof(uint16_t));

	message.insert(message.end(), this->filename.begin(), this->filename.end());


	if (this->get_size()) {
		size = htonl(this->size);
		message.insert(message.end(), reinterpret_cast<uint8_t*>(&size),
			reinterpret_cast<uint8_t*>(&size) + sizeof(uint32_t));
	}
	
	boost::asio::write(sock, boost::asio::buffer(message));

}