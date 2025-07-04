import socket
import time

# Inspiration from https://github.com/Seconb/Aimmy-Arduino-Edition

class SocketArduinoMouse:
    def __init__(self):
        self.ip_address = "127.0.0.1"
        self.port = 9999

    def send_mouse_coordinates(self, x, y):
        if x != 0 or y != 0:
            message = f"{x},{y}"
            self._send_message(message)

    def send_mouse_click(self, click):
        if click in [0, 1]:
            message = str(click)
            self._send_message(message)

    def _send_message(self, message):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
                client.settimeout(0.5)
                client.connect((self.ip_address, self.port))
                client.sendall(message.encode("utf-8"))
        except (ConnectionRefusedError, socket.timeout):
            pass
