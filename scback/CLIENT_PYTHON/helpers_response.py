# Byte size for header fields, -1 mean byte size varies

VERSION = 1
STATUS = 2
NAME_LEN = 2
FILENAME = -1
SIZE = 4
PAYLOAD = -1

# Possible server response
RESPONSE = {
    # File succefully backed
    'S_BACKUP': 210,
    # DIR operation success
    'S_DIR': 211,
    # Transfer or Delete operation success
    'S_DELETE_OR_TRANSFER': 212,
    # File does not exist on server
    'F_NO_FILE': 1001,
    # Client has no files; no directory on the server to client
    'F_DIR':1002,
    # General error
    'F_ERROR': 1003
}
