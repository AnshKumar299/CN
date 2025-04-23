// udp_client.c
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define BUF_SIZE 1000

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char server_ip[100], send_buf[BUF_SIZE], recv_buf[BUF_SIZE];
    socklen_t addrlen;
    unsigned short serv_port = 25000;

    printf("Enter server IP address: ");
    scanf("%s", server_ip);
    getchar();  // Clear newline from input buffer

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serv_port);

    if (inet_aton(server_ip, &servaddr.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(servaddr);

    while (1) {
        printf("You: ");
        fgets(send_buf, BUF_SIZE, stdin);
        send_buf[strcspn(send_buf, "\n")] = '\0'; // remove newline

        sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&servaddr, addrlen);

        if (strcasecmp(send_buf, "bye") == 0) {
            break;
        }

        n = recvfrom(sockfd, recv_buf, BUF_SIZE - 1, 0, NULL, NULL);
        if (n > 0) {
            recv_buf[n] = '\0';
            printf("Server: %s\n", recv_buf);

            if (strcasecmp(recv_buf, "bye") == 0) {
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
