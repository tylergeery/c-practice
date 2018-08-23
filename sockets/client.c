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
    int sockfd = 0, n = 0, port;
    struct sockaddr_in serv_addr;
    char input[1024];
    char host[100];

    if (argc != 3) {
        usage(argv[0]);
        return 0;
    }

    strcpy(host, argv[1]);
    port = atoi(argv[2]);

    if (strcmp(host, "") == 0 || port <= 80) {
        usage(argv[0]);
        return 0;
    }

    memset(input, '0', sizeof(input));

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      printf("\n Error : Could not create socket \n");
      return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        fprintf(stderr, "Could not connect to server: errno %d\n", errno);
        return -1;
    }

    while((n = read(sockfd, input, sizeof(input) - 1)) > 0) {
        printf("loop\n");
        input[n] = 0;
        if (fputs(input, stdout) == EOF) {
            printf("\n Error : Fputs error");
        }
        printf("\n");
    }

    if (n < 0) {
        printf("\nFound nothing to read\n");
    }

    return 0;
}
