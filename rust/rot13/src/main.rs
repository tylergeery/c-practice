#![feature(custom_attribute)]
#![recursion_limit="256"]
use std::time::SystemTime;
use rot_13_derive::Rot13;

pub trait Rot13 {
    type S;

    fn rot13(&self) -> Self::S;
}

#[derive(Rot13)]
struct Message {
    note: String,
    author: String,
    #[skip_rot13] created_at: u64,
}

#[derive(Rot13)]
struct Text {
    value: String,
}

fn main() {
    let msg = Message {
        note: String::from("Hello, keep this a secret"),
        author: String::from("Mr encrypt"),
        created_at: SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs()
    };

    let new = msg.rot13();

    println!("new: {} ({}) - {}", new.note, new.author, new.created_at);

    let orig = new.rot13();

    println!("orig: {} ({}) - {}", orig.note, orig.author, orig.created_at);

    let txt = Text { value: String::from("jbeyq") };

    println!("hello {}", txt.rot13().value);
}
