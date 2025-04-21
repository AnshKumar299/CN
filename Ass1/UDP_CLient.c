#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
    int sockfd,returnvalue,len=100,n;
    unsigned int addrlen;
    unsigned short serv_port=25000;
    char serv_ip[]="127.0.0.1";
    char text[]="abcde";
    char rtext[100];
    struct sockaddr_in servaddr;
    addrlen=sizeof(servaddr);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(serv_port);
    inet_aton(serv_ip,&servaddr.sin_addr);
    sockfd=socket(PF_INET,SOCK_DGRAM,0);
    returnvalue=sendto(sockfd,text,strlen(text),0,(struct sockaddr *) & servaddr,sizeof(servaddr));
    if(returnvalue!=-1)
        printf("\n\t\t\t*** message sent successfully ***\n");
    else
        printf("\n\t\t\t*** message sent failure ***\n");

    n=recvfrom(sockfd,rtext,len,0,(struct sockaddr *) &servaddr,&addrlen);
    if(n!=-1)
    {
        rtext[n]='\0';
        printf("\n\t\t\t*** %s ***\n",rtext);
    }
    close(sockfd);
    return 0;
}

