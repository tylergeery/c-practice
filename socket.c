#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

typedef struct sockaddr SA;

int open_clientfd(char *hostname, int port) {
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1; /* Check errno for cause of error */
    }

    /* Fill in the server’s IP address and port */
    if ((hp = gethostbyname(hostname)) == 0) {
        return -2; /* Check h_errno for cause of error */
    }

    printf("hp hname: %s\n", hp->h_name);

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    /* Establish a connection with the server */
    if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    return clientfd;
}

int main(int argc, char *argv[]) {
    int socket_desc, read_size, sock;
    char *message;

    if (argc != 3) {
        printf("\nUsage: ./socket <server-name> <message>\n\n");
        return -1;
    }

    int fd = open_clientfd("dev.tradesy.com", 80);

    if (fd > 0) {
        ssize_t success = write(fd, argv[2], strlen(argv[2]));
        printf("successfully wrote(%zd): %s\n", success, argv[2]);

        //Receive a message from client
        while( (read_size = recv(sock , message , 2000 , 0)) > 0 )
        {
            //Send the message back to client
            printf("server replied: %s\n", message);
        }
    }

    return 1;
}
