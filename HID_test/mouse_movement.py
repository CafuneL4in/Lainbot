import time
import sys
import socket
import threading
from .mouse_instruct import MouseInstruct, DeviceNotFoundError

def handle_client_connection(client_socket):
    global m

    while True:
        message = client_socket.recv(1024).decode('utf-8')
        if message:
            if ',' in message:
                x, y = map(int, message.split(','))
                m.move(x, y)

            elif message == "1":
                m.press()
            elif message == "0":
                m.release()
        else:
            break

    client_socket.close()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("localhost", 9999))
server.listen(30)
m = None

def getMouse():
    global m
    try:
        m = MouseInstruct.getMouse()
        print("[+] Mouse device found!")
    except DeviceNotFoundError:
        sys.exit()
    return m

def main():
    global m
    m = getMouse()
    
    server.settimeout(1)
    while True:
        try:
            client_sock, address = server.accept()
            client_handler = threading.Thread(
                target=handle_client_connection,
                args=(client_sock,),
                daemon=True
            )
            client_handler.start()
        except socket.timeout:
            continue
