use std::{env, thread};

fn main() {
	let args: Vec<String> = env::args().collect();
		
	if args.len() < 2 {
		let ref file = &args[0];
		usage(file);
		return;
	}

	let handle = thread::spawn(|| {
		for i in 1..10 {
			println!("printing {} from spawned thread", i);
		}
	});

	let _ = handle.join();

	for j in 1..5 {
		println!("printing {} from subsequent loop", j);
	}
}

fn usage(file: &String) {
	println!("Usage: {} <operation>\n", file);
}
