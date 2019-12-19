#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 256

void manageclientserver(int sockfd)
{
    char buff[MAX];
    //We'll use n for buffer index.
    int n;
    while (1)
    {
        bzero(buff, sizeof(buff));
        printf("TO EXIT PLEASE TYPE 'exit'!!!\n");
        printf("Enter two numbers i.e 5 10: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        // Send the buffer to the server.
        if(strcmp(buff, "exit\n") == 0)
            break;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        //Read the data from the server.
        read(sockfd, buff, sizeof(buff));
        printf("Addition of two number is : %s\n", buff);
    }
     printf("Client Exit...\n");
}

int main(int argc, char *argv[])
{
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    if (3 != argc)
    {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }
    /* create a streaming socket*/
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (simpleSocket == -1)
    {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "Socket created!\n");
    }
    /* retrieve the port number for connecting */
    simplePort = atoi(argv[2]);
    /* set up the address structure */
    /* use the IP address argument for the server address*/
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    inet_addr(argv[1]);
    simpleServer.sin_port = htons(simplePort);
    /*connect to the address and port with our socket*/
    returnStatus = connect(simpleSocket,
                           (struct sockaddr *)&simpleServer,
                           sizeof(simpleServer));
    if (returnStatus == 0)
    {
        fprintf(stderr, "Connect successful!\n");
    }
    else
    {
        fprintf(stderr, "Could not connect to address!\n");
        close(simpleSocket);
        exit(1);
    }
    /* get the message from the server*/
    manageclientserver(simpleSocket);
    close(simpleSocket);
    return 0;
}