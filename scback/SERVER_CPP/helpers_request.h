#ifndef HELPERS_REQUEST_H
#define HELPERS_REQUEST_H
#include <cstdint>

// Request codes
enum class RequestType :uint8_t{
    SAVE_FILE = 100,
    RETRIEVE_FILE = 200,
    DELETE_FILE = 201,
    DIR = 202
};

// Byte size for header fields, -1 mean byte size varies
const int8_t USER_ID_BYTES = 4;
const int8_t VERSION = 1;
const int8_t OP = 1;
const int8_t NAME_LEN = 2;
const int8_t FILE_NAME = -1;
const int8_t SIZE_ = 4;
const int8_t PAY_LOAD = -1;


const size_t MIN_REQUEST_SIZE = 6;
const int MAX_FILE_NAME = pow(NAME_LEN, sizeof(int8_t)) * 2;
const int MESSAGE_MAX_LENGTH = 1024;
#endif //HELPERS_REQUEST_H

