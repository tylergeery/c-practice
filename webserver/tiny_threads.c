#include "../csapp/csapp.h"

void *webserve(void *vargp);
void read_request_headers(rio_t *rp);
int parse_uri(char *uri, char *filename, char *dynamic_args);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *dynamic_args);
void clienterror(int fd, char *cause, char *errnum,
                    char *shortmsg, char *longmsg);

int main(int argc, char **argv)
{
    int listenfd, *connfdp, port;
    socklen_t clientlen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;
    pthread_t tid;

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

     // listen on port
     listenfd = Open_listenfd(port);

     /**
      * Start neverending webserver
      */
     while (1) {
         connfdp = Malloc(sizeof(int));
         *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);

         // create child thread to handle request
         Pthread_create(&tid, NULL, webserve, connfdp);
     }
}

/**
 * Thread routine
 * Parses request and kicks off response process
 */
void *webserve(void *vargp)
{
    int is_static;
    int fd = *((int *)vargp);
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], dynamic_args[MAXLINE];
    rio_t rio;

    // Detach from parent thread
    Pthread_detach(pthread_self());

    // Free up passed memory
    Free(vargp);

    /**
     * Read request line and headers
     */
     Rio_readinitb(&rio, fd);
     Rio_readlineb(&rio, buf, MAXLINE);
     sscanf(buf, "%s %s %s", method, uri, version);

     if (strcasecmp(method, "GET")) {
         clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
         return NULL;
     }

     read_request_headers(&rio);

     /**
      * Parse URI from GET request
      */
     is_static = parse_uri(uri, filename, dynamic_args);

     if (stat(filename, &sbuf) < 0) {
         clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
         return NULL;
     }

     if (is_static) {
         /**
          * Serve static content
          */
         if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
             return NULL;
         }

         serve_static(fd, filename, sbuf.st_size);
     } else {
         /**
          * Serve dynamic content
          */
         if (!(S_ISREG(sbuf.st_mode))) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the dynamic program");
             return NULL;
         }

         if (!(S_IXUSR & sbuf.st_mode)) {
             clienterror(fd, filename, "403", "Forbidden",
                        "Tiny did not have permissions to run the program");
             return NULL;
         }

         serve_dynamic(fd, filename, dynamic_args);
     }

     // Close FD
     Close(fd);

     // return value if ever joined
     return NULL;
}
