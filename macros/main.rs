macro_rules! not_12_macro {
    ($n:ident) => {{
        if $n != 12 {
            print!("macro: {} is not 12 but is instead {}\n", stringify!($n), $n);
        }
    }};
}

fn not_12(n: u64) {
    if n != 12 {
        print!("fn: {} is not 12 but is instead {}\n", stringify!(n), n);
    }
}

pub fn main() {
    let val_from_main = 15;

    not_12_macro!(val_from_main);
    not_12(val_from_main);
}
