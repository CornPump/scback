import socket
import random
import helpers
import struct
import request_handler as rh


if __name__ == "__main__":

    # create random user id
    user_id = random.randint(1, helpers.MAX_USER_ID)
    # create client class handler
    client = rh.RequestHandler(user_id)

    file_name = 'test.txt'
    # pull host port info
    with open('server.info', 'r') as f:
        s = f.readline().split(':')

    HOST, PORT = s[0], int(s[1])

    # pull files to backup and append to list
    files_lst = []
    with open('backup.info', 'r') as f:
        for line in f:
            line = line.strip()
            if line:
                files_lst.append(line)
    print(client)

    req = client.create_request(helpers.REQUESTS['RETRIEVE_FILE'], file_name)
    category1, category2, category3,category4 = struct.unpack('<IBBH', req[:-8])
    print(f'{category1} {category2} {category3} {category4} {file_name}')
    """
    req = client.create_request(helpers.REQUESTS['DIR'])
    #unpack helpers.REQUESTS['DIR']
    category1, category2, category3 = struct.unpack('<IBB', req)
    print(f'{category1} {category2} {category3}')
    """

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
            client.connect((HOST, PORT))

            message = req.ljust(1024, b'\0')
            client.sendall(message[:])
            print('MSG SENT:   ', message);
            data = client.recv(1024)
    except:
            print("Could not open socket with ", HOST);
            exit()
    if data:
        print('Received', repr(data));
        version,opcode = struct.unpack('>BH', data)
        print(f'Version={version}, Opcode={opcode}')


