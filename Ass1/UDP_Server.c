#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    short int n=0,m=0;
    int len=100;
    int addrlen;
    unsigned short serv_port=25000;
    short sockfd;
    char serv_ip[]="192.168.3.1";char mesg[1000];
    char *client;
    struct sockaddr_in servaddr, clientaddr; client=(char *)malloc(sizeof(servaddr));
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
    {
        printf("failed");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    inet_aton(serv_ip,&servaddr.sin_addr);
    servaddr.sin_port=htons(serv_port);
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    n=recvfrom(sockfd,mesg,len,0,(struct sockaddr *) & clientaddr,&addrlen);
    if(n!=-1)
        printf("\n\t\t\t*** receive success ***\n");
    else
        printf("\n\t\t\t*** receive failure ***\n");
    m=sendto(sockfd,mesg,n,0,(struct sockaddr *) & clientaddr,sizeof(clientaddr));
    if (m!=-1)
        printf("\n\t\t\t*** send success ***\n");
    else
        printf("\n\t\t\t*** send failure ***\n");
    return 0;
}


