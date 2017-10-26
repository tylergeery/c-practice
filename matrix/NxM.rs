use std::{env, thread};
use std::sync::{Arc, Mutex};
use std::thread::JoinHandle;

const ROUTINE_COUNT: i32 = 3;

fn usage(file: &String) {
	println!("Usage: {} <add|subtract|multiply>\n", file);
}

fn output(result: &[[i32; 3]; 3]) {
    print!("[ ", );
    let mut iter = result.iter();

    while iter.len() as i32 > 0  {
        let row = iter.next();

        for v in row.iter() {
            print!("{:?} ", *v);
        }

        if iter.len() as i32 > 0 {
            println!("");
        }
    }

	println!("]");
}

fn get_result_matrix() -> Arc<Mutex<[[i32; 3]; 3]>> {
	let result: Arc<Mutex<[[i32; 3]; 3]>> = Arc::new(Mutex::new([[0; 3]; 3]));

	result
}

fn add(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &mut Arc<Mutex<[[i32; 3]; 3]>>, mut row: i32) {
    let mut dest = result.lock().unwrap();

	while row < (n.len() as i32) {
		let r: usize = row as usize;
		let mut col: i32 = 0;

		while col < (n[r].len() as i32) {
			let c: usize = col as usize;

			dest[r][c] = n[r][c] + m[r][c];
            col += 1;
		}

		row = row + ROUTINE_COUNT;
	}
}

fn subtract(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &mut Arc<Mutex<[[i32; 3]; 3]>>, mut row: i32) {
    let mut dest = result.lock().unwrap();

	while row < (n.len() as i32) {
		let r: usize = row as usize;
		let mut col: i32 = 0;

		while col < (n[r].len() as i32) {
			let c: usize = col as usize;

			dest[r][c] = n[r][c] - m[r][c];
            col += 1;
		}

		row = row + ROUTINE_COUNT;
	}
}

/**
 * [ n00 n01 n02   [ m00 m01 m02   [ zrc]
 *   n10 n11 n12     m10 m11 m12
 *   n20 n21 n22 ]   m20 m21 m22 ] */
fn multiply(n: &[[i32; 3]; 3], m: &[[i32; 3]; 3], result: &mut Arc<Mutex<[[i32; 3]; 3]>>, mut row: i32) {
    let mut dest = result.lock().unwrap();

    while row < (n.len() as i32) {
		let r: usize = row as usize;
		let mut col: i32 = 0;

		while col < (n[r].len() as i32) {
			let c: usize = col as usize;
            let mut sum = 0;

            let mut zeta = 0;
            while zeta < m.len() as i32 {
                let z = zeta as usize;
                sum += n[r][z] * m[z][c];
                zeta += 1;
            }

            dest[r][c] = sum;
            col += 1;
		}

		row = row + ROUTINE_COUNT;
	}
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

	let ref result_matrix = get_result_matrix();

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

	for i in 0..(ROUTINE_COUNT) {
        let mut dest = result_matrix.clone();
		threads.push(thread::spawn(move || {
			func(&n, &m, &mut dest, i);
		}));
	}

	for thread in threads.into_iter() {
		let _ = thread.join();
	}

    output(&result_matrix.lock().unwrap());
}
