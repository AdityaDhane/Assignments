import socket
import sys
import threading

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

    def clientHandler(conn,addr):
        print('Connected with client socket number', addr[1])
        try:
            while True:
                data = conn.recv(1024).decode('UTF-8')
                data = str(eval(data)).encode('UTF-8')
                print('Sending reply: ',data)
                conn.sendall(data)

    while True:
        conn, addr = sock.accept()
        threading.Thread(target=clientHandler,args=(conn,addr)).start()

except socket.timeout:
    print('Error: Timeout !!')
except socket.error:
    print('Error: port is occupied !!')
finally:
    sock.close()
