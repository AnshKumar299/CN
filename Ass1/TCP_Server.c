#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8000
#define BUFFER_SIZE 1024

int new_socket;
char buffer[BUFFER_SIZE];

void* send_messages(void* arg) {
    while (1) {
        printf("Enter message to send to client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character from the input

        // If the server types "bye", close the connection
        if (strcmp(buffer, "bye") == 0) {
            send(new_socket, buffer, strlen(buffer), 0);
            printf("You terminated the connection.\n");
            close(new_socket);
            exit(0);
        }

        // Send message to client
        send(new_socket, buffer, strlen(buffer), 0);
    }
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    // Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // Create a thread to handle sending messages to the client
    pthread_t send_thread;
    pthread_create(&send_thread, NULL, send_messages, NULL);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client disconnected or error occurred\n");
            break;
        }
        printf("Client: %s\n", buffer);

        // If the client sends "bye", terminate the connection
        if (strcmp(buffer, "bye") == 0) {
            printf("Client terminated the connection.\n");
            close(new_socket);
            break;
        }
    }

    // Wait for the send thread to finish
    pthread_join(send_thread, NULL);

    // Close socket
    close(server_fd);

    return 0;
}
