import socket
import sys
import select

if len(sys.argv) > 1:
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
else:
    HOST = '127.0.0.1'
    PORT = 65432

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, PORT))
    sock.settimeout(10)
    sock.listen(5)
    inputs = [sock]
    outputs = []
    mymap = {}
    while True:
        infds, outfds, errfds = select.select(inputs, outputs, [], 5)
        if infds:
            for fds in infds:
                if fds is sock:
                    conn, addr = fds.accept()
                    inputs.append(conn)
                    print('Connected with client socket number', addr[1])
                else:
                    data = fds.recv(1024).decode('UTF-8')
                    data = str(eval(data))
                    inputs.remove(fds)
                    mymap[fds] = data
                    outputs.append(fds)

        if outfds:
            for fds in outfds:
                reply = mymap[fds].encode('UTF-8')
                print('Sending reply: ',reply)
                fds.sendall(reply)
                outputs.remove(fds)
                del mymap[fds]

except socket.timeout:
    print('Error: Timeout !!')
except socket.error as err:
    print('Error: port is occupied !!')
finally:
    sock.close()
