import random


# Byte size for header fields, -1 mean byte size varies
USER_ID_BYTES = 4
VERSION = 1
OP = 1
NAME_LEN = 2
FILE_NAME = -1
SIZE = 4
PAY_LOAD = -1

# User maximum possible ID
MAX_USER_ID = (2 ** (USER_ID_BYTES*8)) -1

CLIENT_VERSION = 1

MESSAGE_MAX_SIZE = 1024

# Possible client requests
REQUESTS = {
    # save and back up the file on the server
    'SAVE_FILE': 100,
    # retrieve file back from the server
    'RETRIEVE_FILE': 200,
    # delete the file from the server
    'DELETE_FILE': 201,
    # returns list of all files back-uped by the user on the server
    'DIR': 202
}





