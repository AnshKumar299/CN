#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    short int n = 0, m = 0;
    int len = 100;
    int addrlen;
    unsigned short serv_port = 25000;
    short sockfd;
    char serv_ip[] = "127.0.0.1"; // Using localhost IP for testing
    char mesg[1000];
    struct sockaddr_in servaddr, clientaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(serv_ip, &servaddr.sin_addr);  // Convert IP address to binary format
    servaddr.sin_port = htons(serv_port);

    // Bind the socket to the IP and port
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("Bind failed\n");
        close(sockfd);
        exit(1);
    }

    addrlen = sizeof(clientaddr); // Initialize addrlen to the size of the clientaddr structure
    n = recvfrom(sockfd, mesg, len, 0, (struct sockaddr *)&clientaddr, &addrlen);
    if (n != -1) {
        printf("\n\t\t\t*** receive success ***\n");
    } else {
        printf("\n\t\t\t*** receive failure ***\n");
    }

    m = sendto(sockfd, mesg, n, 0, (struct sockaddr *)&clientaddr, addrlen);
    if (m != -1) {
        printf("\n\t\t\t*** send success ***\n");
    } else {
        printf("\n\t\t\t*** send failure ***\n");
    }

    close(sockfd);  // Close the socket when done
    return 0;
}
