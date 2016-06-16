use std::os::raw::{c_char, c_int};

#[link(name = "blurdroid")]
extern {
    pub fn BluetoothAdapter_init();
    pub fn BluetoothAdapter_getAddress() -> *const c_char;
    pub fn BluetoothAdapter_getName() -> *const c_char;
    pub fn BluetoothAdapter_getBondedDevices() -> *const *const c_char;
    pub fn Helper_arraySize(array: *const *const c_char) -> c_int;
}
