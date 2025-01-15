#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char SERVER_HOST[100];
    int SERVER_PORT;
    int client_socket;
    struct sockaddr_in server_addr;
    char data[1024];

    // Prompt the user for the server address and port
    printf("Enter the host to send data to: ");
    fgets(SERVER_HOST, sizeof(SERVER_HOST), stdin);
    SERVER_HOST[strcspn(SERVER_HOST, "\n")] = 0; // Remove newline character

    printf("Enter the port to send data to: ");
    scanf("%d", &SERVER_PORT);
    getchar(); // To consume the newline left by scanf

    // Create a TCP/IP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    // Connect the socket to the server's address and port
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }
    printf("Connected to %s on port %d\n", SERVER_HOST, SERVER_PORT);

    // Communicate with the server
    while (1) {
        // Prompt the user to enter the data to send
        printf("Enter the message to send (or 'exit' to quit): ");
        fgets(data, sizeof(data), stdin);
        data[strcspn(data, "\n")] = 0; // Remove newline character from input

        // Exit the loop if the user types 'exit'
        if (strcasecmp(data, "exit") == 0) {
            printf("Exiting the program.\n");
            break;
        }

        // Send the entered data to the server
        if (send(client_socket, data, strlen(data), 0) == -1) {
            perror("Send failed");
            break;
        }
        printf("Sent data: %s\n", data);
    }

    // Close the socket
    close(client_socket);
    printf("Connection closed.\n");

    return 0;
}
