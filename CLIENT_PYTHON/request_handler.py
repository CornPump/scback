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

        if size == helpers.VERSION or size == helpers.OP:
            try:
                to_ret = struct.pack("<B", num)
            except:
                print(num)
        return to_ret

    def save_and_backup(self, opcode, name_len: int, file_name: str, size: int, Payload):
        pass

    def retrieve_file(self, opcode, name_len: int, file_name: str):
        pass

    def delete_file(self, opcode, name_len: int, file_name: str):
        pass

    def list_files(self ,opcode):
        print("opcode= ",opcode)
        header = RequestHandler.convert_to_little_endian(self.user_id, helpers.USER_ID_BYTES) + \
                RequestHandler.convert_to_little_endian(self.version, helpers.VERSION) + \
                 RequestHandler.convert_to_little_endian(opcode, helpers.OP)

        print(header)
        return header
