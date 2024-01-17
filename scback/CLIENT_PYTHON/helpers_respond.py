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

REPONSE_MIN_SIZE = 3