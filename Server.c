#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

void createClientInstance(int socketfd) {
   
    char buffer[256];
    printf("");
    int n = read(socketfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s", buffer);

}

int main(int argc, char *argv[]) {
    FILE *file_ptr = fopen("passwd.txt", "r");
    char fileEntries[25][20];
    char ch;
    int index = 0;
    for (int i = 0; i < 25; i++) {
        index = 0;
        while ((ch = fgetc(file_ptr)) != EOF) {
            if (ch == ';')
            break;
            fileEntries[i][index] = ch;
            index ++;
        }
    }
    for (int i = 0; i < 25; i++) {
        printf("%s\n", fileEntries[i]);
    }
   int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    int fds[3];

    for (int i = 0; i < 3; i++) {
        newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, &clilen);
        if (newsockfd < 0)
        error("ERROR on accept");
        fds[i] = newsockfd;
    }

    createClientInstance(fds[0]);
    createClientInstance(fds[1]);
    createClientInstance(fds[2]);

}