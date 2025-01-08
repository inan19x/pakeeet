import socket

# Prompt the user for the server address and port
SERVER_HOST = input("Enter the host to send data to: ")
SERVER_PORT = int(input("Enter the port to send data to: "))

# Create a TCP/IP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the server's address and port
try:
    client_socket.connect((SERVER_HOST, SERVER_PORT))
    print(f"Connected to {SERVER_HOST} on port {SERVER_PORT}")
    
    while True:
        # Prompt the user to enter the data to send
        data = input("Enter the message to send (or 'exit' to quit): ")
        
        # Exit the loop if the user types 'exit'
        if data.lower() == 'exit':
            print("Exiting the program.")
            break
        
        # Send the entered data to the server
        client_socket.sendall(data.encode('utf-8'))
        print(f"Sent data: {data}")

except Exception as e:
    print(f"Error: {e}")

finally:
    # Close the socket
    client_socket.close()
    print("Connection closed.")
