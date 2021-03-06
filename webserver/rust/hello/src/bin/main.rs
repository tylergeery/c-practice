extern crate hello;

use std::fs::File;
use std::io::prelude::*;
use std::net::{Shutdown, TcpListener, TcpStream};
use std::{thread, time};
use hello::ThreadPool::ThreadPool;

fn main() {
    let listener = TcpListener::bind("127.0.0.1:8080").unwrap();
    let pool = ThreadPool::new(4);

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        pool.execute(|| {
            handle_connection(stream);
        });
    }
}

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 512];
    stream.read(&mut buffer).unwrap();

    let get = b"GET / HTTP/1.1\r\n";
    let sleep = b"GET /sleep HTTP/1.1\r\n";
    let shutdown = b"GET /shutdown HTTP1.1\r\n";

    // TODO: better match routing
    let (status_line, filename) = if buffer.starts_with(get) {
        ("HTTP/1.1 200 OK", "html/hello.html")
    } else if buffer.starts_with(sleep)  {
        thread::sleep(time::Duration::new(5, 0));
        ("HTTP/1.1 200 OK", "html/hello.html")
    } else if buffer.starts_with(shutdown) {
        stream.shutdown(Shutdown::Read);
        ("HTTP/1.1 500 OK", "html/500.html")
    } else {
        ("HTTP/1.1 404 NOT FOUND", "html/404.html")
    };

    let mut file = File::open(filename).unwrap();

    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();

    let response = format!("{}\r\n\r\n{}", status_line, contents);

    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();
}
