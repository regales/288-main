import socket

# Set the IP address and Port for the CyBot server
HOST = "127.0.0.1"  # IP address for the CyBot server
PORT = 288            # Port number for CyBot server

# Setting up a server socket
cybot_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cybot_server_socket.bind((HOST, PORT))
cybot_server_socket.listen()

print(f"Listening on {HOST}:{PORT}")

conn, addr = cybot_server_socket.accept()
print(f"Connected by {addr}")

while True:
    data = conn.recv(1024)  # Receive data from client
    if not data:
        break
    command = data.decode()  # Decode the received command

    print(f"Received command: {command}")

    if command == 'quit':
        break

    # Process the received command (for simplicity, this just echoes back)
    conn.sendall(command.encode())

conn.close()  # Close connection
cybot_server_socket.close()  # Close server
