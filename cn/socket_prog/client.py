import socket
import sys

if len(sys.argv) > 1:
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
else:
    HOST = '127.0.0.1'  # The server's hostname or IP address
    PORT = 65432        # The port used by the server

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))
    print('\n + Connected to Server !!')
    while True:
        print('\n<- Please enter the message to the server: ',end=' ')
        exp = input().encode('UTF-8')

        sock.sendall(exp)
        data = sock.recv(1024).decode('UTF-8')

        print('\n-> Server replied:', data)

except KeyboardInterrupt:
    print('\n\n # Error: keyboardInterrupt !!')
except ConnectionRefusedError:
    print('\n\n # Error: ConnectionRefused !!')
finally:
    sock.close()
