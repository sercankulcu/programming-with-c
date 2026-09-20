// main.rs
use std::os::raw::{c_double, c_int, c_void};

#[link(name = "mathlib")]
extern "C" {
    fn ml_add(a: c_int, b: c_int) -> c_int;
    fn ml_mean(values: *const c_double, count: usize,
               status: *mut c_int) -> c_double;
    fn ml_make_range(start: c_double, step: c_double,
                     count: usize) -> *mut c_double;
    fn ml_free(p: *mut c_double);
    fn ml_acc_create() -> *mut c_void;
    fn ml_acc_destroy(acc: *mut c_void);
    fn ml_acc_add(acc: *mut c_void, value: c_double) -> c_int;
    fn ml_acc_mean(acc: *const c_void) -> c_double;
}

fn main() {
    // Every call is unsafe: Rust cannot verify C's contracts.
    unsafe {
        println!("ml_add(3, 4) = {}", ml_add(3, 4));

        let data = [1.0_f64, 2.0, 3.0, 4.0, 5.0];
        let mut status: c_int = 0;
        let mean = ml_mean(data.as_ptr(), data.len(), &mut status);
        println!("ml_mean = {mean} status {status}");

        let p = ml_make_range(0.0, 0.5, 6);
        let slice = std::slice::from_raw_parts(p, 6);
        println!("range = {slice:?}");
        ml_free(p);                       // C's allocator, not Rust's

        let acc = ml_acc_create();
        for v in [10.0, 20.0, 30.0] {
            ml_acc_add(acc, v);
        }
        println!("accumulator mean = {}", ml_acc_mean(acc));
        ml_acc_destroy(acc);
    }
}
