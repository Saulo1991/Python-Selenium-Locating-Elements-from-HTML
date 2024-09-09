import socket
import threading

def handle_client(client_socket, clients):
    while True:
        try:
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break
            print(f'Received: {message}')
            
            for client in clients:
                if client != client_socket:
                    client.send(message.encode('utf-8'))
        except ConnectionResetError:
            break
    
    client_socket.close()
    clients.remove(client_socket)

def server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 5555))
    server_socket.listen(5)
    print('Server initiating and waiting for connections...')
    
    clients = []
    
    while True:
        client_socket, addr = server_socket.accept()
        print(f"Client connected to {addr}")
        clients.append(client_socket)
        client_thread = threading.Thread(target=handle_client, args=(client_socket, clients))
        client_thread.start()
        
if __name__ == "__main__":
    server()