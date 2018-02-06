# object files
OBJS=./webserver/render.o ./webserver/helpers.o ./csapp/csapp.o

all:	tiny_threads

tiny_render:	$(OBJS)
	$(CC) -o ./exec/tinyserver webserver/tiny.c webserver/helpers.c csapp/csapp.c

tiny_processes:	$(OBJS)
	$(CC) -o ./exec/tiny_processes webserver/tiny_processes.c webserver/helpers.c csapp/csapp.c

tiny_threads:	$(OBJS)
	$(CC) -o ./exec/tiny_threads webserver/tiny_threads.c webserver/render_threads.c webserver/helpers_threads.c csapp/csapp.c
