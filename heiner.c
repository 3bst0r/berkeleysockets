//
// Created by johannes on 5/2/15.
//

#include "heiner.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

/*char** parse_input(char** result, char* str) {
    char* p = strtok(str,' ');

    for (int i = 0; p != NULL; p = strtok(NULL,' ')) {
        result[i] = p;
        i++;
    }
    result[i] = null;
    return result;
}*/

int main(int args, char** argv) {

    char* server_ip = argv[1];

    /* create socket */
    int sock;
    if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("%s\n","error creating socket");
        return -1;
    }

    /* bind socket */
    struct sockaddr_in destAddr;
    memset(&destAddr,0,sizeof(destAddr));
    destAddr.sin_port = htons(5000);
    destAddr.sin_family = AF_INET;


    if (inet_aton(server_ip,&destAddr.sin_addr) < 0) {
      printf("%s\n","error parsing server ip adress");
    }

    if (connect(sock,(struct sockaddr *)&destAddr, sizeof(destAddr)) < 0) {
        printf("%s\n","error connecting to server");
    } else {
        printf("%s %s\n","connected to ",inet_ntoa(destAddr.sin_addr));
    }

    /* now readwrite */

    char buf[512];
    for (int i = 2; i < args; i++) {

        if (send(sock,strcat(argv[i],"\0\n"),strlen(argv[i])+1,0) < 0) {
            printf("%s\n","error sending");
        } else {
            printf("sent\n");
        }
        int length;
        if ((length = recv(sock,(char*)&buf,512*sizeof(char),0)) < 0) {
            perror("recv: ");
            printf("%s\n", "error receiving");
        } else if (length == 0) {
            printf("hacking attack aborted\n");
            return 1;
        } else {
           buf[length] = '\0';
           printf("%s: %s\n",argv[i],buf);
        }


    }
    close(sock);

    return 0;
}