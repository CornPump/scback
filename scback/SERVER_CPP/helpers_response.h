#ifndef HELPERS_RESPONSE_H
#define HELPERS_RESPONSE_H

#include <cstdint>

enum class ResponseType :uint16_t {
    // File succefully backed
    S_BACKUP = 210,
    // DIR operation success
    S_DIR = 211,
    // Transer or Delete operation success
    S_DELETE_OR_TRANSFER = 212,
    // File does not exist
    F_NO_FILE = 1001,
    // Client has no files; no directory on the server to client
    F_DIR = 1002,
    // General error
    F_ERROR = 1003
};

// Byte size for header fields, -1 mean byte size varies

const int8_t SERVER_VERSION = 1;
const int8_t RESPONSE_STATUS = 2;

// server actual versio
const int8_t SERVER_CUR_VERSION = 1;
#endif //HELPERS_RESPONSE_H