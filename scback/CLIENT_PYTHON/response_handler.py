import helpers_response
import helpers_request
import struct

class ResponseHandler:

    def __init__(self, socket):
        self.socket = socket
        self.server_version = 0
        self.status = 0
        self.name_len = 0
        self.file_name = ""
        self.size = 0

    def __str__(self):
        return f'server_version={self.server_version}, status={self.status}, ' \
               f'name_len={self.name_len}, filename={self.name_len}, size={self.size}'

    def read_response_status(self):
        data = self.sock.recv(helpers_response.VERSION + helpers_response.STATUS)

        if data:
            print('\nReceived Response', repr(data));
            self.version, self.status = struct.unpack('>BH', data)
            print(f'Version={self.version}, Opcode={self.status}')
            # General error response, bad client request
            if self.status == helpers_request.RESPONSE['F_ERROR']:
                print("Server Response with general error due to bad request or timeout")
                exit()
        else:
            print("Did not receive respond from the server in read_reponse_status(), shutting app")
            exit()

    def read_second_time_full_header(self):

        self.read_second_time_part_header()
        data = self.sock.recv(helpers_response.SIZE)
        if data:
            self.size = struct.unpack('>I', data)
        else:
            print("Did not receive respond from the server in read_second_time_full_header(), shutting app")
            exit()



    def read_second_time_part_header(self):

        data = self.sock.recv(helpers_response.NAME_LEN)
        if data:
            self.name_len = struct.unpack('>H', data)
            data2 = self.sock.recv(self.name_len)
            if data2:
                self.file_name = data2.decode("utf-8")
            else:
                print("Did not receive respond from the server in read_second_time_part_header(), shutting app")
                exit()
        else:
            print("Did not receive respond from the server in read_second_time_part_header(), shutting app")
            exit()
