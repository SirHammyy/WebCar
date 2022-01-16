from websocket_server import WebsocketServer


def new_client(client, server):
    print("New connection from: " + str(client['address'][0]) + ":" + str(client['address'][1]))

def recieve_message(client, server, message):
    if message == 'esp8266 Connected':
        client['id'] = 999


    if client['id'] != 999:
        carClient = None
        for oneClient in server.clients:
            if oneClient['id'] == 999:
                carClient = oneClient
                break
        if carClient is not None:
            server.send_message(carClient, message)
        print("web request: " + message)
    server.send_message(client, message)

def client_left(client, server):
    print("Connection lost: " + str(client['address'][0]) + ":" + str(client['address'][1]))

server = WebsocketServer(host='0.0.0.0', port=42422)
server.set_fn_new_client(new_client)
server.set_fn_message_received(recieve_message)
server.set_fn_client_left(client_left)
server.run_forever()