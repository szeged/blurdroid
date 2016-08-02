use std::ffi::{CStr};
use std::os::raw::{c_char};
use std::ptr::{self};
use std::str::from_utf8;

pub const NOT_SUPPORTED_ERROR: &'static str = "Error! Not supported platform!";

pub fn c_str_to_slice(c: & *const c_char) -> Option<&str> {
    if *c == ptr::null() {
        None
    } else {
        from_utf8( unsafe { CStr::from_ptr(*c).to_bytes() }).ok()
    }
}
