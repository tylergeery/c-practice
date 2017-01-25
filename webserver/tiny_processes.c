#include "../csapp/csapp.h"

void webserve(int fd);
void read_request_headers(rio_t *rp);
int parse_uri(char *uri, char *filename, char *dynamic_args);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *dynamic_args);
void clienterror(int fd, char *cause, char *errnum,
                    char *shortmsg, char *longmsg);

/*
 * Checks on child process statuses for cleanup
 */
void sigchld_handler(int sig) {
    /**
     * WNOHANG will prevent current execution waiting for child process status
     */
    while (waitpid(-1, 0, WNOHANG) > 0) {
        // do nothing until child completes
    }

    return;
}

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;

    /**
     * Validate CL args
     */
     if (argc != 2) {
         fprintf(stderr, "usage: %s <port>\n", argv[0]);
         exit(1);
     }

     /**
      * Open file descriptor at port
      */
     port = atoi(argv[1]);

     // reap children
     Signal(SIGCHLD, sigchld_handler);

     // listen on port
     listenfd = Open_listenfd(port);

     /**
      * Start neverending webserver
      */
     while (1) {
         connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
         if (Fork() == 0) {
             Close(listenfd); // Child closes its listenting socket
             webserve(connfd); // child services client
             Close(connfd); // Child closes connection with client
             exit(0); // Child process exits
         }
         Close(connfd); // Parent closes socket connection
     }
}

/**
 * Parses request and kicks off response process
 */
void webserve(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], dynamic_args[MAXLINE];
    rio_t rio;

    /**
     * Read request line and headers
     */
     Rio_readinitb(&rio, fd);
     Rio_readlineb(&rio, buf, MAXLINE);
     sscanf(buf, "%s %s %s", method, uri, version);

     if (strcasecmp(method, "GET")) {
         clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
         return;
     }

     read_request_headers(&rio);

     /**
      * Parse URI from GET request
      */
     is_static = parse_uri(uri, filename, dynamic_args);

     if (stat(filename, &sbuf) < 0) {
         clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
         return;
     }

     if (is_static) {
         /**
          * Serve static content
          */
         if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
             return;
         }

         serve_static(fd, filename, sbuf.st_size);
     } else {
         /**
          * Serve dynamic content
          */
         if (!(S_ISREG(sbuf.st_mode))) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the dynamic program");
             return;
         }

         if (!(S_IXUSR & sbuf.st_mode)) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny did not have permissions to run the program");
             return;
         }

         serve_dynamic(fd, filename, dynamic_args);
     }
}
