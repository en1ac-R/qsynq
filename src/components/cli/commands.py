from cli.socket_client import UnixSocketClient


class CommandDispatcher:
    def __init__(self):
        self.client = UnixSocketClient()

    def execute(self, payload: dict):

        response = self.client.send(payload)

        print(response)
