// udp_server.c
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 1000

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr, clientaddr;
    char recv_buf[BUF_SIZE], send_buf[BUF_SIZE];
    socklen_t addrlen;
    unsigned short serv_port = 25000;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(serv_port);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(clientaddr);

    while (1) {
        n = recvfrom(sockfd, recv_buf, BUF_SIZE - 1, 0, (struct sockaddr*)&clientaddr, &addrlen);
        if (n > 0) {
            recv_buf[n] = '\0';
            printf("Client: %s\n", recv_buf);

            if (strcasecmp(recv_buf, "bye") == 0) {
                break;
            }

            printf("You: ");
            fgets(send_buf, BUF_SIZE, stdin);
            send_buf[strcspn(send_buf, "\n")] = '\0'; // remove newline

            sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&clientaddr, addrlen);

            if (strcasecmp(send_buf, "bye") == 0) {
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
