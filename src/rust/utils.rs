use std::ffi::{CStr};
use std::ptr::{self};
use std::os::raw::{c_char};
use std::str::from_utf8;

pub fn c_str_to_slice(c: & *const c_char) -> Option<&str> {
    println!("####c_str_to_slice {:?}####", c);
    if *c == ptr::null() {
        None
    } else {
        from_utf8( unsafe { CStr::from_ptr(*c).to_bytes() }).ok()
    }
}
