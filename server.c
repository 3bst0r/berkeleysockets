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

/* returns the gefährdung as char */
char lookup(char* city) {

}

int main(int args, char** argv) {

    struct sockaddr_in destAddr;
    struct sockaddr_in sourceAddr;

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
        if ((sock_client = accept(sock,(struct sockaddr*)&sourceAddr,&client_adress_size) < 0)) {
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
            if ((length = recv(sock_client,buf,512,0)) < 0) {
                printf("%s\n","error receiving message");
            } else {
                buf[length] = '\0';
                printf("%s\n",buf);

            /*if (send(sock,"answer",strlen(argv[i]),0) < 0) {
                printf("%s\n","error sending");
            } else {
                printf("%s %s %s %zu\n","sent message: ",argv[i]," of length: ",strlen(argv[i]));
            }*/

                /* now send back */
            }



        }
    }

    return 0;
}