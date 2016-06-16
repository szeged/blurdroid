use std::os::raw::{c_char};

#[link(name = "blurdroid")]
extern {
    pub fn BluetoothAdapter_init();
    pub fn BluetoothAdapter_getAddress() -> *const c_char;
    pub fn BluetoothAdapter_getName() -> *const c_char;
}
