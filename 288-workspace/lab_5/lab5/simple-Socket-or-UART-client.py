import socket

# Set the IP address and Port of the CyBot server
HOST = "192.168.1.1"  # IP address of the CyBot server
PORT = 288            # Port number for CyBot server

# Create a TCP socket and connect to the CyBot server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
cybot_socket.connect((HOST, PORT))

# Instructions for controlling the CyBot
print("Use 'w' to move forward, 's' to move backward, 'a' to turn left, and 'd' to turn right. Enter 'quit' to exit.")

while True:
    command = input("Enter a command: ")  # Get movement command from the user
    if command in ['w', 'a', 's', 'd']:
        cybot_socket.sendall(command.encode())  # Send command to CyBot
    elif command == 'quit':
        cybot_socket.sendall(command.encode())
        break
    else:
        print("Invalid command. Please use 'w', 'a', 's', 'd'.")

cybot_socket.close()  # Close the socket connection
