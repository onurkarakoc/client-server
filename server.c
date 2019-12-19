#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 256
#define SA struct sockaddr

void manageserverclient(int sockfd)
{
    char buff[MAX];
    while(1)
    {
        bzero(buff, sizeof(buff));

        // read the numbers from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));

        int result = 0;
        char *number;
        number = strtok(buff, " ");
        while (number != NULL)
        {
            result += atoi(number);
            number = strtok(NULL, " ");
        }
        bzero(buff, sizeof(buff));
        sprintf(buff, "%d", result);
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
    }
    printf("Server Exit...\n");
}

int main(int argc, char *argv[])
{
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    int connectionfd;
    socklen_t len;
    struct sockaddr_in simpleServer, client;
    if (2 != argc)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

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
    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);
    /* set up the address structure */
    /* use INADDR_ANY to bind to all local addresses
*/
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);
    /*
bind to the address and port with our socket
*/
    returnStatus = bind(simpleSocket,
                        (struct sockaddr *)&simpleServer,
                        sizeof(simpleServer));
    if (returnStatus == 0)
    {
        fprintf(stderr, "Bind completed!\n");
    }
    else
    {
        fprintf(stderr, "Could not bind to address!\n");
        close(simpleSocket);
        exit(1);
    }
    //let's listen on the socket for connections
    returnStatus = listen(simpleSocket, 5);
    if (returnStatus == -1)
    {
        fprintf(stderr, "Cannot listen on socket!\n");
        close(simpleSocket);
        exit(1);
    }

    connectionfd = accept(simpleSocket, (SA *)&client, &len);
    if (connectionfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    manageserverclient(connectionfd);
    close(simpleSocket);
    return 0;
}