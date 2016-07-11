use std::os::raw::{c_char, /*c_uchar,*/ c_int, c_void};
//use libc::{c_void, c_char, c_uint, c_int};

pub type BluetoothAdapter = c_void;
pub type BluetoothDevice = c_void;
pub type BluetoothService = c_void;
pub type BluetoothCharacteristic = c_void;
pub type BluetoothDescriptor = c_void;

#[link(name = "blurdroid")]
extern {
    pub fn bluetooth_adapter_get_adapter() -> *mut BluetoothAdapter;
    pub fn bluetooth_adapter_get_address(adapter: *mut BluetoothAdapter) -> *const c_char;
    pub fn bluetooth_adapter_get_name(adapter: *mut BluetoothAdapter) -> *const c_char;
    pub fn bluetooth_adapter_start_discovery(adapter: *mut BluetoothAdapter);
    pub fn bluetooth_adapter_stop_discovery(adapter: *mut BluetoothAdapter);
    pub fn bluetooth_adapter_get_devices(adapter: *mut BluetoothAdapter) -> *const *const c_char;
    pub fn bluetooth_adapter_get_devices_size(adapter: *mut BluetoothAdapter) -> c_int;
    pub fn bluetooth_adapter_free_adapter(adapter: *mut BluetoothAdapter);
    pub fn bluetooth_adapter_free_string(string: *const c_char);
    pub fn bluetooth_adapter_free_string_array(string: *const *const c_char);

    pub fn bluetooth_device_create_device(adapter: *mut BluetoothAdapter, address: *const c_char) -> *mut BluetoothDevice;
    //pub fn bluetooth_device_get_address(device: *mut BluetoothDevice) -> *const c_char;
    pub fn bluetooth_device_get_name(device: *mut BluetoothDevice) -> *const c_char;
    pub fn bluetooth_device_connect_gatt(device: *mut BluetoothDevice);
    pub fn bluetooth_device_is_connected(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_gatt_services(device: *mut BluetoothDevice) -> *const c_int;
    pub fn bluetooth_device_get_gatt_services_size(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_free_device(device: *mut BluetoothDevice);
    pub fn bluetooth_device_free_string(string: *const c_char);
    pub fn bluetooth_device_free_int_array(array: *const c_int);

    pub fn bluetooth_service_create_service(device: *mut BluetoothDevice, id: c_int) -> *mut BluetoothService;
    pub fn bluetooth_service_get_uuid(service: *mut BluetoothService) -> *const c_char;
    pub fn bluetooth_service_is_primary(service: *mut BluetoothService) -> c_int;
    pub fn bluetooth_service_get_gatt_characteristics(service: *mut BluetoothService) -> *const c_int;
    pub fn bluetooth_service_get_gatt_characteristics_size(service: *mut BluetoothService) -> c_int;
    pub fn bluetooth_service_free_service(service: *mut BluetoothService);
    pub fn bluetooth_service_free_string(string: *const c_char);
    pub fn bluetooth_service_free_int_array(array: *const c_int);

    pub fn bluetooth_characteristic_create_characteristic(service: *mut BluetoothService, id: c_int) -> *mut BluetoothCharacteristic;
    pub fn bluetooth_characteristic_get_uuid(characteristic: *mut BluetoothCharacteristic) -> *const c_char;
    pub fn bluetooth_characteristic_get_gatt_descriptors(characteristic: *mut BluetoothCharacteristic) -> *const c_int;
    pub fn bluetooth_characteristic_get_gatt_descriptors_size(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_free_characteristic(characteristic: *mut BluetoothCharacteristic);
    pub fn bluetooth_characteristic_free_string(string: *const c_char);
    pub fn bluetooth_characteristic_free_int_array(array: *const c_int);

    pub fn bluetooth_descriptor_create_descriptor(characteristic: *mut BluetoothCharacteristic, id: c_int) -> *mut BluetoothDescriptor;
    pub fn bluetooth_descriptor_get_uuid(descriptor: *mut BluetoothDescriptor) -> *const c_char;
    pub fn bluetooth_descriptor_free_descriptor(descriptor: *mut BluetoothDescriptor);
    pub fn bluetooth_descriptor_free_string(string: *const c_char);
}
