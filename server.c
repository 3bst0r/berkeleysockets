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

    /* create socket */
    int sock;
    if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("%s\n","error creating socket");
        return -1;
    }

    /* bind socket */
    struct sockaddr_in svr_sock_addr;
    memset(&svr_sock_addr,0,sizeof(svr_sock_addr));
    svr_sock_addr.sin_port = htons(5000);
    svr_sock_addr.sin_family = AF_INET;
    svr_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock,(struct sockaddr*)&svr_sock_addr,sizeof(svr_sock_addr)) < 0) {
        printf("%s\n","could not bind");
        return -1;
    }

    /* start listening */
    if (listen(sock,10) < 0) {
        printf("%s\n","error listening");
        return -1;
    }

    struct sockaddr_in address_client;
    memset(&address_client,0,sizeof(address_client));

    /* accept clients */
    for(;;) {
        int sock_client;
        socklen_t client_adress_size = sizeof(address_client);
        printf("%s","now accepting clients\n");
        if ((sock_client = accept(sock,(struct sockaddr*)&address_client,&client_adress_size) < 0)) {
            printf("%s\n","accept error");
        }

        pid_t child_pid;
        if ((child_pid = fork()) < 0) {
            printf("%s\n","error forking server");
            return -1;
        } else if (child_pid == 0) { // client process handling
            printf("%s","forking the server\n");
            char buf[50];
            int length;
            if ((length = recv(sock,&buf,5,0)) < 0) {
                printf("%s\n","error receiving message");
            } else {
                buf[length] = '\0';
                printf("%s\n",buf);


                /* now send back */
            }



        }
    }

    return 0;
}