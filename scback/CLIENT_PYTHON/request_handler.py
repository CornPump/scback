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

    def save_and_backup(self, opcode, file_name: str, sock):
        print("Creating request for backup on server for file: ",file_name)

        size = operation.get_file_size(file_name)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name + \
                 RequestHandler.convert_to_little_endian(size, helpers_request.SIZE)

        try:
            sock.sendall(header)
        except:
            print("couldn't send message")
            exit()

        operation.send_file(file_name, sock)

        resh = response_handler.ResponseHandler(sock)
        resh.read_response_status()

        if resh.status == helpers_response.RESPONSE['S_DELETE_OR_BACKUP']:
            resh.read_second_time_part_header()
            print(f"Server Response = {resh}")
            print(f"File {resh.file_name} successfully backed-up on the server")

        elif resh.status != helpers_response.RESPONSE['F_ERROR']:
            print("Unrecognized Response type..")


    def retrieve_file(self, opcode, file_name: str, sock):

        print("Creating request retrieve_file for file: ", file_name)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name

        print(header)
        try:
            sock.sendall(header)
        except:
            print("couldn't send message")
            exit()

        resh = response_handler.ResponseHandler(sock)
        resh.read_response_status()

        if resh.status == helpers_response.RESPONSE['S_RETRIEVE']:
            resh.read_second_time_full_header()
            print(f"Server Response = {resh}")
            # switch tmp with resh.file_name to get the actual name
            operation.receive_file('tmp', sock, resh.size)
            print(f"File {resh.file_name} successfully retrieved from the server")


        if resh.status == helpers_response.RESPONSE['F_NO_FILE']:
            resh.read_second_time_part_header()
            print(f"Server Response = {resh}")
            print(f"File: {resh.file_name} doesn't exist on the server, retrieve request denied")


    def delete_file(self, opcode, file_name: str, sock):
        print("Creating request delete_file on server for file: ", file_name)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers_request.NAME_LEN) + \
                 binary_file_name

        try:
            sock.sendall(header)
        except:
            print("couldn't send message")
            exit()

        resh = response_handler.ResponseHandler(sock)
        resh.read_response_status()

        if resh.status == helpers_response.RESPONSE['S_DELETE_OR_BACKUP']:

            resh.read_second_time_part_header()
            print(f"Server Response = {resh}")
            print(f"File {resh.file_name} successfully deleted from the server")

        elif resh.status == helpers_response.RESPONSE['F_NO_FILE']:

            resh.read_second_time_part_header()
            print(f"Server Response = {resh}")
            print(f"File: {resh.file_name} doesn't exist on the server, delete request denied")

        elif resh.status != helpers_response.RESPONSE['F_ERROR']:
            print("Unrecognized Response type..")

    def list_files(self, opcode, sock):
        print("Creating list_files() request..")
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers_request.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers_request.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers_request.OP)

        try:
            sock.sendall(header)
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
        elif resh.status != helpers_response.RESPONSE['F_ERROR']:
            print("Unrecognized Response type..")

