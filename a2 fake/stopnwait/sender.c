// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 25000
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 3
#define LOSS_PROBABILITY 0.3

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE], recv_buf[BUFFER_SIZE];
    int seq_num = 0;

    srand(time(NULL));  // Initialize random for simulating noisy channel

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to receiver.\n");

    while (1) {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcasecmp(buffer, "bye") == 0) {
            write(sockfd, buffer, strlen(buffer));
            printf("Connection closed.\n");
            break;
        }

        char send_frame[BUFFER_SIZE];
        snprintf(send_frame, sizeof(send_frame), "%d %s", seq_num, buffer);

        write(sockfd, send_frame, strlen(send_frame));
        printf("Sent Frame %d: %s\n", seq_num, buffer);
        sleep(1);  // delay after sending

        fd_set readfds;
        struct timeval timeout;
        int ack;

        while (1) {
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = 0;

            int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
            if (activity > 0 && FD_ISSET(sockfd, &readfds)) {
                int n = read(sockfd, recv_buf, sizeof(recv_buf) - 1);
                recv_buf[n] = '\0';

                if (sscanf(recv_buf, "%d ACK", &ack) == 1 && ack == seq_num) {
                    printf("Received ACK %d\n", ack);
                    seq_num = 1 - seq_num;
                    sleep(1);  // delay after ACK
                    break;
                }
            } else {
                printf("Timeout! Resending Frame %d\n", seq_num);
                sleep(1);  // delay before resend
                write(sockfd, send_frame, strlen(send_frame));
            }
        }
    }

    close(sockfd);
    return 0;
}
