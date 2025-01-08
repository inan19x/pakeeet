import socket
import argparse

# Set up argument parsing
parser = argparse.ArgumentParser(description='TCP server to listen for incoming data')
parser.add_argument('--host', type=str, required=True, help='Host to bind the server to')
parser.add_argument('--port', type=int, required=True, help='Port to listen on')
args = parser.parse_args()

# Retrieve host and port from arguments
HOST = args.host
PORT = args.port

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Set socket options to reuse the address
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to the specified host and port
server_socket.bind((HOST, PORT))

# Enable the socket to listen for incoming connections
server_socket.listen(5)

print(f"Listening for incoming connections on {HOST}:{PORT}...")

try:
    # Accept incoming connections and process the data
    while True:
        # Wait for a connection
        client_socket, client_address = server_socket.accept()
        print(f"Connection established from {client_address}")

        try:
            # Receive and print data from the client
            while True:
                data = client_socket.recv(1024)  # Buffer size is 1024 bytes
                if not data:
                    break  # No more data, close the connection

                # Print the human-readable data
                print(f"Received data: {data.decode('utf-8', errors='replace')}")

        except Exception as e:
            print(f"Error while receiving data: {e}")

        finally:
            # Close the client connection
            client_socket.close()
            print(f"Connection with {client_address} closed.")

except KeyboardInterrupt:
    print("\nServer interrupted. Shutting down gracefully...")
finally:
    # Clean up the server socket before exiting
    server_socket.close()
    print("Server socket closed. Exiting program.")
