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
               f'name_len={self.name_len}, filename={self.file_name}, size={self.size}'

    def read_response_status(self):
        data = self.socket.recv(helpers_response.VERSION + helpers_response.STATUS)

        if data:
            self.server_version, self.status = struct.unpack('>BH', data)
            # General error response, bad client request
            if self.status == helpers_response.RESPONSE['F_ERROR']:
                print("Server Response with general error due to bad request or timeout: Response ",self.status)
        else:
            print("Did not receive respond from the server in read_reponse_status(), shutting app")
            exit()

    def read_second_time_full_header(self):

        self.read_second_time_part_header()
        data = self.socket.recv(helpers_response.SIZE)
        if data:
            self.size = struct.unpack('>I', data)[0]
        else:
            print("Did not receive respond from the server in read_second_time_full_header(), shutting app")
            exit()



    def read_second_time_part_header(self):

        data = self.socket.recv(helpers_response.NAME_LEN)
        if data:
            self.name_len = struct.unpack('>H', data)[0]
            data2 = self.socket.recv(self.name_len)
            if data2:
                self.file_name = data2.decode("utf-8")
            else:
                print("Did not receive respond from the server in read_second_time_part_header(), shutting app")
                exit()
        else:
            print("Did not receive respond from the server in read_second_time_part_header(), shutting app")
            exit()
