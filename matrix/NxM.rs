use std::{env, thread};

const ROUTINE_COUNT: i32 = 3;

fn usage(file: &String) {
	println!("Usage: {} <operation>\n", file);
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

	let result_matrix = [
		[0, 0, 0],
		[0, 0, 0],
		[0, 0, 0]
	];

	let args: Vec<String> = env::args().collect();

	if args.len() < 2 {
		let ref file = &args[0];
		usage(file);
		return;
	}

	match &args[1] {
		"add" => add(), // Do nothing
		"subtract" => subtract(),
		"multiple" => muliply(),
		_ => { usage(&args[1]); return; }
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
