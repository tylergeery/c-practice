use std::time::SystemTime;
use rot_13_derive::Rot13;

pub trait Rot13 {
    fn rot13(&self);
}

#[derive(Rot13)]
struct Message {
    note: String,
    author: String,
    created_at: u64
}

fn main() {
    let msg = Message {
        note: String::from("Hello, keep this a secret"),
        author: String::from("Mr encrypt"),
        created_at: SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_secs()
    };

    msg.rot13();

    println!("{} ({}) - {}", msg.note, msg.author, msg.created_at);

    msg.rot13();

    println!("{} ({}) - {}", msg.note, msg.author, msg.created_at);
}
