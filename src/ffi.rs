use std::os::raw::{c_char, c_int};
#[allow(non_camel_case_types)]
pub type c_bool = c_int;

#[link(name = "blurdroid")]
extern {
    pub fn BluetoothAdapter_init();
    //pub fn BluetoothAdapter_enable() -> c_bool;
    //pub fn BluetoothAdapter_disable() -> c_bool;
    //pub fn BluetoothAdapter_isEnabled() -> c_bool;
    pub fn BluetoothAdapter_getAddress() -> *const c_char;
    pub fn BluetoothAdapter_getName() -> *const c_char;
    pub fn BluetoothAdapter_getDevices() -> *const *const c_char;
    pub fn BluetoothAdapter_getDevicesSize() -> c_int;

    pub fn BluetoothDevice_getName(address: *const c_char) -> *const c_char;
    pub fn BluetoothDevice_connectGatt(address: *const c_char);
    pub fn BluetoothDevice_getServicesSize(address: *const c_char) -> c_int;
    pub fn BluetoothDevice_getCharacteristicsSize(address: *const c_char) -> c_int;
    pub fn BluetoothDevice_getDescriptorsSize(address: *const c_char) -> c_int;

    //pub fn BluetoothGatt_discoverServices() -> c_bool;
    pub fn BluetoothGatt_getServices(address: *const c_char) -> *const c_int;

    pub fn BluetoothGattService_getUuid(address: *const c_char, id: c_int) -> *const c_char;
    //pub fn BluetoothGattService_getType(address: *const c_char, id: c_int) -> c_int;
    pub fn BluetoothGattService_isPrimary(address: *const c_char, id: c_int) -> c_bool;
    pub fn BluetoothGattService_getCharacteristics(address: *const c_char, id: c_int) -> *const c_int;

    pub fn BluetoothGattCharacteristic_getUuid(address: *const c_char, id: c_int) -> *const c_char;
    pub fn BluetoothGattCharacteristic_getDescriptors(address: *const c_char, id: c_int) -> *const *const c_char;

    pub fn Helper_freeCharArray(array: *const *const c_char, num: c_int);
    pub fn Helper_freeIntArray(array: *const c_int);

}
