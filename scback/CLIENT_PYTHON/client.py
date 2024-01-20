import socket
import random
import helpers_request
import request_handler as rh
import operation


def main():
    # create random user id
    user_id = random.randint(1, helpers_request.MAX_USER_ID)

    # create client class handler
    client = rh.RequestHandler(user_id)
    print('Created client: ', client, "\n")

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

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        try:
            sock.connect((HOST, PORT))
        except:
            print("Could not open socket with ", HOST);
            exit()
        client.create_request(helpers_request.REQUESTS['DIR'], sock)
        client.create_request(helpers_request.REQUESTS['SAVE_FILE'], files_lst[0], sock)
        client.create_request(helpers_request.REQUESTS['SAVE_FILE'], files_lst[1], sock)
        client.create_request(helpers_request.REQUESTS['DIR'], sock)
        client.create_request(helpers_request.REQUESTS['RETRIEVE_FILE'], files_lst[0], sock)
        client.create_request(helpers_request.REQUESTS['DELETE_FILE'], files_lst[0], sock)
        client.create_request(helpers_request.REQUESTS['RETRIEVE_FILE'], files_lst[0], sock)


if __name__ == "__main__":
    main()



