import socket
import random
import helpers_request
import request_handler as rh
import operation


if __name__ == "__main__":
    # create random user id
    user_id = random.randint(1, helpers_request.MAX_USER_ID)
    #user_id = 1162125029
    # user_id = 4261025877
    # create client class handler
    client = rh.RequestHandler(user_id)
    print('Created client: ',client,"\n")
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
    """
    req = client.create_request(helpers.REQUESTS['SAVE_FILE'], file_name, 3335, "")
    nreq = req[:8] + req[16:]
    print(nreq)
    category1, category2, category3, category4, category5 = struct.unpack('<IBBHI', nreq)
    print(f'{category1} {category2} {category3} {category4} {file_name} {category5}')
    
    req = client.create_request(helpers.REQUESTS['RETRIEVE_FILE'], file_name)
    category1, category2, category3,category4 = struct.unpack('<IBBH', req[:-8])
    print(f'{category1} {category2} {category3} {category4} {file_name}')
    """


    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        try:
            sock.connect((HOST, PORT))
        except:
            print("Could not open socket with ", HOST);
            exit()
        client.create_request(helpers_request.REQUESTS['SAVE_FILE'], 'test.txt', sock)
        #operation.send_file('just_got_it', sock)


