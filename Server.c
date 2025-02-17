#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

char fileEntries[25][20];
const char *accessMessage = "Welcome to CPSC445-Comp Networking Class\nYou are invited to use %s's Machine!";

void* createClientInstance(void* args) {
   
    int *socketfd = (int *)args;
    char userNamebuffer[256];
    char passwdbuffer[256];
    char msgbuffer[256];
    int n;
    
    for(int i = 0; i < 3; i++) {
    bzero(userNamebuffer, 256);
    bzero(passwdbuffer, 256);
    printf("");
   
    n = read(*socketfd, userNamebuffer, 255);
    if (n < 0) error("ERROR reading from socket");
    n = read(*socketfd, passwdbuffer, 255);
    if (n < 0) error("ERROR reading from socket");

    if ((memcmp(userNamebuffer, fileEntries[0], 5) == 0) && (memcmp(passwdbuffer, fileEntries[1], 4) == 0)) {
        snprintf(msgbuffer, sizeof(msgbuffer), accessMessage, fileEntries[2]);
        n = write(*socketfd, msgbuffer, 255);
        return NULL;
    }
    else if ((memcmp(userNamebuffer, fileEntries[5], 5) == 0) && (memcmp(passwdbuffer, fileEntries[6], 4) == 0)) {
        snprintf(msgbuffer, sizeof(msgbuffer), accessMessage, fileEntries[7]);
        n = write(*socketfd, msgbuffer, 255);
        return NULL;
    }
    else if ((memcmp(userNamebuffer, fileEntries[10], 5) == 0) && (memcmp(passwdbuffer, fileEntries[11], 4) == 0)) {
        snprintf(msgbuffer, sizeof(msgbuffer), accessMessage, fileEntries[12]);
        n = write(*socketfd, msgbuffer, 255);
        return NULL;
    }
    else if ((memcmp(userNamebuffer, fileEntries[15], 5) == 0) && (memcmp(passwdbuffer, fileEntries[16], 4) == 0)) {
        snprintf(msgbuffer, sizeof(msgbuffer), accessMessage, fileEntries[17]);
        n = write(*socketfd, msgbuffer, 255);
        return NULL;
    }
    else if ((memcmp(userNamebuffer, fileEntries[20], 5) == 0) && (memcmp(passwdbuffer, fileEntries[21], 4) == 0)) {
        snprintf(msgbuffer, sizeof(msgbuffer), accessMessage, fileEntries[22]);
        n = write(*socketfd, msgbuffer, 255);
        return NULL;
    }
    else if (i < 2){
    n = write(*socketfd, "You have entered your username or password incorrectly\nPlease try again", 255);
    }
    else {
    n = write(*socketfd, "You have exceeded the maximum number of allowed attemps to login.\nOnly for ESU CPSC Students taking CPSC445.\nYou are not yet invited.\n", 255);
    return NULL;
    }

    }
   return NULL;
}

 

int main(int argc, char *argv[]) {
    FILE *file_ptr = fopen("passwd.txt", "r");
    char ch;
    int index = 0;
    for (int i = 0; i < 25; i++) {
        index = 0;
        while ((ch = fgetc(file_ptr)) != EOF) {
            if (ch == '\n') continue;
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

   
   pthread_t threads[3];
   
    for(int i = 0; i < 3; i++) {
        newsockfd = accept(sockfd, (struct sockaddr*) & cli_addr, &clilen);
        if (newsockfd < 0)
        error("ERROR on accept");
        int* newsockfd_ptr = malloc(sizeof(int));
        *newsockfd_ptr = newsockfd;
        pthread_create(&threads[i], NULL, createClientInstance, newsockfd_ptr);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    close(sockfd);
    return 0;
}