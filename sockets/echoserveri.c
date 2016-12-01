#include "../csapp/csapp.h"

/**
 * echo function stub
 * defined in echo.c
 */
void echo(int connfd);

/**
 * Compiled with:
 * gcc ./sockets/echoserveri.c csapp/csapp.c sockets/echo.c -o ./exec.socketserver
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;

    // validate CL args
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    listenfd = Open_listenfd(port);

    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /**
         * Determine the domain name and IP address of the client
         */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                            sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);

        echo(connfd);
        Close(connfd);
    }

    exit(0);
}
