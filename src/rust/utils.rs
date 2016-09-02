use std::error::Error;
use std::ffi::{CStr};
use std::os::raw::{c_char};
use std::ptr::{self};
use std::str::from_utf8;

pub const NOT_SUPPORTED_ERROR: &'static str = "Error! Not supported function!";

macro_rules! check_null(
    ($obj:expr, $err:expr) => (
        if *$obj == ptr::null_mut() {
            return Err(Box::from($err));
        }
    );
);

pub fn c_str_to_slice(c: & *const c_char) -> Option<&str> {
    if *c == ptr::null() {
        None
    } else {
        from_utf8( unsafe { CStr::from_ptr(*c).to_bytes() }).ok()
    }
}

pub fn convert_cstr(c: & *const c_char, err: &str) -> Result<String, Box<Error>> {
    let res = match from_utf8( unsafe { CStr::from_ptr(*c).to_bytes() }) {
        Ok(a) => Ok(a.to_string()),
        Err(_) => Err(Box::from(err)),
    };
    res
}
