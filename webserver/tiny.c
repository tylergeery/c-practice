#include "../csapp.csapp.h"

void webserve(int fd);
void read_request_headers(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                    char *shortmsg, char *longmsg);


int main(int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
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
     listenfd = Open_listenfd(port);

     /**
      * Start neverending webserver
      */
     while (1) {
         clientlen = sizeof(clientaddr);
         connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
         webserve(connfd);
         Close(connfd);
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
    char filename[MAXLINE], cgiargs[MAXLINE];
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
     is_static = parse_uri(uri, filename, cgiargs);

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
         if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
             return;
         }

         serve_dynamic(fd, filename, cgiargs);
     }
}
