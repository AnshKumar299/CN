// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 25000
#define BUFFER_SIZE 1024
#define LOSS_PROBABILITY 0.3

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int expected_seq = 0;
    socklen_t addrlen = sizeof(address);

    srand(time(NULL));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Receiver listening on port %d...\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Sender connected!\n");

    while (1) {
        int n = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (n <= 0) break;
        buffer[n] = '\0';

        if (strcasecmp(buffer, "bye") == 0) {
            printf("Sender closed the connection.\n");
            break;
        }

        int seq_num;
        char data[BUFFER_SIZE];
        sscanf(buffer, "%d %[^\n]", &seq_num, data);

        double r = (double)rand() / RAND_MAX;
        if (r < LOSS_PROBABILITY) {
            printf("Simulating frame loss for Frame %d: %s\n", seq_num, data);
            continue; // simulate loss
        }

        if (seq_num == expected_seq) {
            printf("Received Frame %d: %s\n", seq_num, data);
            sleep(1);  // delay after receiving
            expected_seq = 1 - expected_seq;
        } else {
            printf("Duplicate Frame %d received. Ignored.\n", seq_num);
            sleep(1);
        }

        char ack_msg[BUFFER_SIZE];
        snprintf(ack_msg, sizeof(ack_msg), "%d ACK", seq_num);
        write(new_socket, ack_msg, strlen(ack_msg));
        printf("Sent ACK %d\n", seq_num);
        sleep(1);  // delay after ACK
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
