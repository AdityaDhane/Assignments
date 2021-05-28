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
    sock.bind((HOST, PORT))
    sock.settimeout(10)
    sock.listen(0)
    print(' $ Starting server')
    while True:
        try:
            conn, addr = sock.accept()
            print('\n + Connected with client socket number', addr[1])
            while True:
                data = conn.recv(1024).decode('UTF-8')
                if(data):
                    print('\n-> Client socket ' + str(addr[1]) + ' sent message: ', data)
                    try:
                        data = str(eval(data))
                    except SyntaxError:
                        data = 'Invalid Input'
                    print('\n<- Sending reply: ', data)
                    conn.sendall(data.encode('UTF-8'))
                else:
                    print('\n - Connection Closed with client socket number', addr[1])
                    break
        except KeyboardInterrupt:
            print(' ! Stopping server !!')
            break

except socket.timeout:
    print('\n # Error: Timeout !!')
except socket.error:
    print('\n # Error: port is occupied !!')
finally:
    sock.close()
