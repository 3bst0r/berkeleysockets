//
// Created by johannes on 5/2/15.
//

#include "server.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int marked[9];

/* returns the degree as char or 'e' in case of a corrupted client */
char lookup(char* city) {

    if (strcmp(city,"BREGENZ") == 0) {
        if (marked[0]) return 'e';
        marked[0] = 1;
        return '5';
    }

    if (strcmp(city,"EISENSTADT") == 0) {
        if (marked[1]) return 'e';
        marked[1] = 1;
        return '7';
    }

    if (strcmp(city,"GRAZ") == 0) {
        if (marked[2]) return 'e';
        marked[2] = 1;
        return '5';
    }

    if (strcmp(city,"INNSBRUCK") == 0) {
        if (marked[3]) return 'e';
        marked[3] = 1;
        return '2';
    }

    if (strcmp(city,"KLAGENFURT") == 0) {
        if (marked[4]) return 'e';
        marked[4] = 1;
        return '7';
    }

    if (strcmp(city,"LINZ") == 0) {
        if (marked[5]) return 'e';
        marked[5] = 1;
        return '7';
    }

    if (strcmp(city,"SALZBURG") == 0) {
        if (marked[6]) return 'e';
        marked[6] = 1;
        return '3';
    }

    if (strcmp(city,"STPOELTEN") == 0) {
        if (marked[7]) return 'e';
        marked[7] = 1;
        return '4';
    }

    if (strcmp(city,"WIEN") == 0) {
        if (marked[8]) return 'e';
        marked[8] = 1;
        return '9';
    }

    return 'e';
}

int main() {

    struct sockaddr_in destAddr;
    struct sockaddr_in sourceAddr;

    for (int i = 0; i < 9; i++) marked[i] = 0;


    memset(&sourceAddr,0,sizeof(sourceAddr));
    memset(&destAddr,0,sizeof(destAddr));

    /* create socket */
    int sock;
    if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("%s\n","error creating socket");
        return -1;
    }

    /* bind socket */
    destAddr.sin_port = htons(5000);
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock,(struct sockaddr*)&destAddr,sizeof(destAddr)) < 0) {
        printf("%s\n","could not bind");
        return -1;
    }

    /* start listening */
    if (listen(sock,10) < 0) {
        printf("%s\n","error listening");
        return -1;
    }



    /* accept clients */
    for(;;) {
        int sock_client;
        socklen_t client_adress_size = sizeof(sourceAddr);
        printf("%s","now accepting clients\n");
        if ((sock_client = accept(sock,(struct sockaddr*)&sourceAddr,&client_adress_size)) < 0) {
            printf("%s\n","accept error");
        } else {
            printf("%s %s\n","accepted ",inet_ntoa(sourceAddr.sin_addr));
        }

        pid_t child_pid;
        if ((child_pid = fork()) < 0) {
            printf("%s\n","error forking server");
            return -1;
        } else if (child_pid == 0) { // client process handling
            printf("%s","forking the server\n");

            char buf[512];
            int length;

            for(;;) {
                if ((length = recv(sock_client,buf,512,0)) < 0) {
                    perror("recv: ");
                    printf("%s\n","error receiving message");
                } else if (length == 0) {
                    printf("closing\n");
                    return 1;
                } else {
                    char c = lookup(buf);
                    char sendbuf[3] = {c,'\0','\n'};
                    if (c == 'e') {
                        printf("hacker detected\n");
                        close(sock);
                        close(sock_client);
                        return 1;
                    }
                    if (send(sock_client,sendbuf,3,0) < 0) {
                        printf("error sending\n");
                    } else {
                        printf("sent\n");
                    }
                }
            }
        }
    }

    return 0;
}