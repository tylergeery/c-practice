use std::{env, thread};
use std::thread::JoinHandle;

const ROUTINE_COUNT: i32 = 3;

fn usage(file: &String) {
	println!("Usage: {} <operation>\n", file);
}

fn add(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &[[i32; 3]; 3], mut row: i32) {
	while row < (n.len() as i32) {
		row = row + ROUTINE_COUNT;
	}
}

fn subtract(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &[[i32; 3]; 3], row: i32) {

}

fn multiply(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &[[i32; 3]; 3], row: i32) {

}

fn main() {
	let n = [
		[1, 2, 3],
		[0, 5, 0],
		[7, 7, 7]
	];

	let m = [
		[2, 2, 4],
		[0, 6, 3],
		[3, 1, 1]
	];

	let result_matrix  = [[0; 3]; 3];

	let args: Vec<String> = env::args().collect();

	if args.len() < 2 {
		let ref file = &args[0];
		usage(file);
		return;
	}

	let mut threads: Vec<JoinHandle<_>> = Vec::new();
	let cmd = &args[1];
	let func = match &cmd[..] {
		"add" => add, // Do nothing
		"subtract" => subtract,
		"multiply" => multiply,
		_ => { usage(&args[1]); return; }
	};

	for i in 1..(ROUTINE_COUNT + 1) {
		threads.push(thread::spawn(move || {
			func(&n, &m, &result_matrix, i);
		}));
	}

	for thread in threads.into_iter() {
		let _ = thread.join();
	}

	println!("{:?}", result_matrix);
}
