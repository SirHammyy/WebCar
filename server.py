from asyncio.streams import start_server
import websockets
import asyncio

HOST = ''
PORT = 42423

async def server(websocket, path):
    while True:
        data = await websocket.recv()
        print(data)
        await websocket.send("repeated: " + data)
    
start_server = websockets.serve(server, HOST, PORT)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()