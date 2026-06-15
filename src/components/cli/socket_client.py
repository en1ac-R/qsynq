import json
import socket

from constants import SOCKET_PATH


class UnixSocketClient:
    def send(self, payload: dict) -> str:

        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

        try:
            sock.connect(SOCKET_PATH)

            message = json.dumps(payload)

            sock.sendall(message.encode())

            response = sock.recv(4096)

            return response.decode()

        finally:
            sock.close()
