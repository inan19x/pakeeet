#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);

    // Create a TCP socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the address
    int reuse_addr = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0) {
        perror("Set socket option failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Setup server address struct
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    // Bind the socket to the host and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Listening for incoming connections on %s:%d...\n", host, port);

    // Accept incoming connections and handle them
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;

    while (1) {
        // Wait for a connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;  // Skip to the next connection attempt
        }

        printf("Connection established from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char buffer[BUFFER_SIZE];
        int bytes_received;

        // Receive data from the client
        while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            // Null-terminate and print the received data
            buffer[bytes_received] = '\0';
            printf("Received data: %s\n", buffer);
        }

        if (bytes_received < 0) {
            perror("Error while receiving data");
        }

        // Close the connection
        close(client_socket);
        printf("Connection with %s:%d closed.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    // Clean up and close the server socket
    close(server_socket);
    printf("Server socket closed. Exiting program.\n");

    return 0;
}
