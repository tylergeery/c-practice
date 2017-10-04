## Basic C Systems Practice Scripts

Following along with Addison/Wesley Systems Textbook. This takes some of the latter exercises and is my attempt to implement/understand/tweak them.

Most of this code is from their book.

The book is wonderful

### Compiling and Running
```
# Basic Tiny w/ Renderer Process
gcc -o ./exec/render csapp/csapp.c
gcc -o ./exec/tinyserver webserver/tiny.c webserver/helpers.c csapp/csapp.c
./exec/tiny_server <port>

# Tiny Server as processes w/ Renderer Process
gcc -o ./exec/render csapp/csapp.c
gcc -o ./exec/tiny_processes webserver/tiny_processes.c webserver/helpers.c csapp/csapp.c
./exec/tinyserver_processes <port>

# Multi-threaded Tiny server
gcc -o ./exec/tiny_threads webserver/tiny_threads.c webserver/render_threads.c webserver/helpers_threads.c csapp/csapp.c
./exec/tiny_threads <port>

# Pre-threaded Multi-threaded Tiny Server

```

### Makefiles
To come someday


## Rust Practice Scripts

Following along with https://doc.rust-lang.org/book/second-edition/

This book is also wonderful
