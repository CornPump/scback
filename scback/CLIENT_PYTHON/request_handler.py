import helpers_request
import struct
import helpers_response
import response_handler
import operation

class RequestHandler:

    def __init__(self, user_id):

        self.user_id = user_id
        self.version = helpers_request.VERSION

        self.requests = {
            helpers_request.REQUESTS['SAVE_FILE']: self.save_and_backup,
            helpers_request.REQUESTS['RETRIEVE_FILE']: self.retrieve_file,
            helpers_request.REQUESTS['DELETE_FILE']: self.delete_file,
            helpers_request.REQUESTS['DIR']: self.list_files
        }

    def __str__(self):
        return f'User_id={self.user_id}, Client_version={self.version}'

    # function to guide user request into the right function to handle the user request
    def create_request(self, opcode, *args, **kwargs):
        if opcode in self.requests:
            return self.requests[opcode](opcode,*args, **kwargs)
        else:
            raise ValueError(f"Invalid opcode: {opcode}")

    # converts int into little endian
    @staticmethod
    def convert_to_little_endian(num: int, size: int):
        if size == helpers_request.USER_ID_BYTES:
            to_ret = struct.pack("<I", num)

        if size == helpers_request.VERSION:
            try:
                to_ret = struct.pack("<B", num)
            except:
                print(num)
        if size == helpers_request.NAME_LEN:
            to_ret = struct.pack("<H", num)

        return to_ret

    def save_and_backup(self, opcode, file_name: str, size: int, Payload, sock):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name + \
                 RequestHandler.convert_to_little_endian(size, helpers_request.SIZE)

        print(header)
        return header

    def retrieve_file(self, opcode, file_name: str, sock):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name

        print(header)
        return header

    def delete_file(self, opcode, file_name: str, sock):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name

        print(header)
        return header

    def list_files(self, opcode, sock):
        print("Creating list_files() request..")
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP)

        # unpack helpers_request.REQUESTS['DIR']
        user_id, c_version, opcode = struct.unpack('<IBB', header)

        message = header
        try:
            sock.sendall(message)
        except:
            print("couldn't send message")
            exit()
        resh = response_handler.ResponseHandler(sock)
        resh.read_response_status()
        # Client has no directory on server
        if resh.status == helpers_response.RESPONSE['F_DIR']:
            print(f"Received Server Response {helpers_response.RESPONSE['F_DIR']}: no directory for client {self.user_id}\n"
                  f"Need to create backup files before sending {helpers_request.REQUESTS['DIR']} Request")
            return
        elif resh.status == helpers_response.RESPONSE['S_DIR']:
            resh.read_second_time_full_header()
            print(f"Server Response = {resh}")
            operation.receive_file(resh.file_name,sock,resh.size)
            print("List of files backed by the server:")
            operation.print_server_dir_file(resh.file_name)
        else:
            print("Unrecognized Response type exiting client app..")
            exit()

