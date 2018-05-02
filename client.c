#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    struct hostent *server;

    char buffer[256];
    portno = 5001;

    // create socket and get file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname("127.0.0.1");

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);


    // connect to server with server address which is set above (serv_addr)

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR while connecting");
        exit(1);
    }

    // inside this while loop, implement communicating with read/write or send/recv function
    while (1) {
        printf("What do you want to say? ");
        bzero(buffer,256);
        scanf("%s", buffer);

        n = write(sockfd,buffer,strlen(buffer));

        if (n < 0){
            perror("ERROR while writing to socket");
            exit(1);
        }

        bzero(buffer,256);
        n = read(sockfd, buffer, 255);

        if (n < 0){
            perror("ERROR while reading from socket");
            exit(1);
        }
        printf("server replied: %s \n", buffer);

        // escape this loop, if the server sends message "quit"

        if (!bcmp(buffer, "quit", 4))
            break;
    }
    return 0;
}
