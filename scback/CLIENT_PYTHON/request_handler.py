import helpers
import struct

class RequestHandler:

    def __init__(self, user_id):

        self.user_id = user_id
        self.version = helpers.VERSION

        self.requests = {
            helpers.REQUESTS['SAVE_FILE']: self.save_and_backup,
            helpers.REQUESTS['RETRIEVE_FILE']: self.retrieve_file,
            helpers.REQUESTS['DELETE_FILE']: self.delete_file,
            helpers.REQUESTS['DIR']: self.list_files
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
        if size == helpers.USER_ID_BYTES:
            to_ret = struct.pack("<I", num)

        if size == helpers.VERSION:
            try:
                to_ret = struct.pack("<B", num)
            except:
                print(num)
        if size == helpers.NAME_LEN:
            to_ret = struct.pack("<H", num)

        return to_ret

    def save_and_backup(self, opcode, file_name: str, size: int, Payload):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers.NAME_LEN) + \
                 binary_file_name + \
                 RequestHandler.convert_to_little_endian(size, helpers.SIZE)

        print(header)
        return header

    def retrieve_file(self, opcode,file_name: str):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers.NAME_LEN) + \
                 binary_file_name

        print(header)
        return header

    def delete_file(self, opcode, file_name: str):
        print("opcode= ", opcode)
        binary_file_name = file_name.encode('utf-8')
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers.OP) + \
                 RequestHandler.convert_to_little_endian(len(file_name), helpers.NAME_LEN) + \
                 binary_file_name

        print(header)
        return header

    def list_files(self ,opcode):
        print("opcode= ",opcode)
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers.USER_ID_BYTES) + \
                 RequestHandler.convert_to_little_endian(self.version, helpers.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers.OP)

        print(header)
        return header
