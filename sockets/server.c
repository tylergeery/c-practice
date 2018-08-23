#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void usage(char *filename)
{
    printf("Usage - %s <host> <port>\n", filename);
}

int main(int argc, char **argv)
{
    int socket_desc, connfd;
    struct sockaddr_in server;
    char output[1025];

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1) {
        fprintf(stderr, "Could not create socket: errno %d\n", errno);
        return -1;
    }

    memset(&server, '0', sizeof(server));
    memset(output, '0', sizeof(output));

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) == -1) {
        fprintf(stderr, "Could not bind to socket: errno %d\n", errno);
        return -1;
    }

    if(listen(socket_desc, 10) == -1) {
        fprintf(stderr, "Could not listen on socket: errno %d\n", errno);
        return -1;
    }

    while(1) {
        connfd = accept(socket_desc, (struct sockaddr*)NULL ,NULL); // accept awaiting request

        strcpy(output, "Message from server");

        sleep(rand() % 5);

        strcpy(&output[strlen("Message from server")], "\nAnd some more");
        write(connfd, output, strlen(output));

        close(connfd);
    }

    return 0;
}
