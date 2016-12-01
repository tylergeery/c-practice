#include "../csapp/csapp.h"

/**
 * Compiled with:
 * gcc ./sockets/echoclient.c csapp/csapp.c -o ./exec.socketclient
 */
int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    // validate CL args
    if(argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = atoi(argv[2]);
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        /**
         * Allow explicit "exit" command
         */
        if (strcmp(buf, "exit\n") == 0) {
            break;
        }

        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }

    /**
     * Good practice to explicitly Close
     * Will close when this process terminates though
     */
    Close(clientfd);
    exit(0);
}
