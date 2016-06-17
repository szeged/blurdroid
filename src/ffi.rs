use std::os::raw::{c_char, c_int};
#[allow(non_camel_case_types)]
pub type c_bool = c_int;

#[link(name = "blurdroid")]
extern {
    pub fn BluetoothAdapter_init();
    pub fn BluetoothAdapter_getAddress() -> *const c_char;
    pub fn BluetoothAdapter_getName() -> *const c_char;
    pub fn BluetoothAdapter_getBondedDevices() -> *const *const c_char;
    pub fn BluetoothAdapter_isEnabled() -> c_bool;
    //pub fn BluetoothAdapter_enable() -> c_bool;
    //pub fn BluetoothAdapter_disable() -> c_bool;

    pub fn BluetoothDevice_getAddress() -> *const c_char;
    pub fn BluetoothDevice_getName() -> *const c_char;

    pub fn Helper_arraySize(array: *const *const c_char) -> c_int;
    pub fn Helper_setRemoteDevice(address: *const c_char) -> c_bool;

}
