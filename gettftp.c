#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#define MAXSIZE 512


int main(int argc, char **argv) {
// Vérification du nbre d'arguments

    if(argc !=4){
        printf("Wrong usage. \nPlease use %s hostname port file", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Welcome to 2g2td1tp1's implementation of a tftp client\n");
    struct addrinfo * result;
    struct addrinfo hints;
    memset (&hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
    hints.ai_protocol=IPPROTO_UDP;
    hints.ai_socktype=SOCK_DGRAM;

    int status;
    status = getaddrinfo(argv[1],argv[2], &hints, &result);
    if(status==-1){
        printf("It seems like i ain't find the host %s\n",argv[1]);
    }
    char buffer [MAXSIZE]={0};
    buffer[1]=2;
    sprintf(buffer+2, "%s", "file.txt");
    sprintf(buffer +3 + strlen("file.txt"), "NETASCII");
    int sock = socket(result->ai_family, result->ai_socktype,result->ai_protocol);
    if(sock==-1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    printf("Sending %s to the host :%s@%s\n",argv[3],argv[1],argv[2]);

    //Construction RRQ et envoi du fichier
    char RRQ[128]={0};
    RRQ[1] = 1;
    sprintf(RRQ+2,"%s",argv[3]);
    sprintf(RRQ+ strlen(argv[3])+3,"NETASCII");
    sendto(sock, RRQ, 12+ strlen(argv[3]),0, result->ai_addr, result->ai_addrlen);
    //réception du fichier
    int numberReadBytes;
    char receivedBuffer[1024];
    struct sockaddr socketReceived;
    int socketReceivedSize;
    numberReadBytes= recvfrom(sock,receivedBuffer,1024,0,&socketReceived,&socketReceivedSize);

    if(receivedBuffer[1]==5){
        printf("Error code received from the server : \n%s\n",receivedBuffer+4);
        exit(EXIT_SUCCESS);
    }
    // nbre de bytes reçu
    if(receivedBuffer[1]==3){
        printf("We received %d bytes",numberReadBytes-4);
        char ack[4]={0,4,0,0};
        ack[3]=receivedBuffer[3];
        sendto(sock,ack,4,0,&socketReceived,socketReceivedSize);
    }

    return 0;
}
