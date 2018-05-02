#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    // create socket and get file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    clilen = sizeof(cli_addr);

    // bind the host address using bind() call
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("ERROR on binding\n");
        exit(1);
    }

    // start listening for the clients,
    // here process will go in sleep mode and will wait for the incoming connection
    listen(sockfd, 5);

    // accept actual connection from the client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    // inside this while loop, implemented communication with read/write or send/recv function
    printf("start");
    while (1) {
        bzero(buffer,256);
        n = read(newsockfd, buffer, 255);

        if (n < 0){
            perror("ERROR in reading from socket");
            exit(1);
        }

        printf("client said: %s \n", buffer);

        n = write(newsockfd, buffer, strlen(buffer));

        if (n < 0){
            perror("ERROR in writing to socket");
            exit(1);
        }

        // escape this loop, if the client sends message "quit"
        if (!bcmp(buffer, "quit", 4))
            break;
    }
    return 0;
}
