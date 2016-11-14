use std::os::raw::{c_char, c_int, c_void};

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
    pub fn bluetooth_adapter_start_discovery(adapter: *mut BluetoothAdapter) -> c_int;
    pub fn bluetooth_adapter_stop_discovery(adapter: *mut BluetoothAdapter) -> c_int;
    pub fn bluetooth_adapter_get_devices(adapter: *mut BluetoothAdapter) -> *const *const c_char;
    pub fn bluetooth_adapter_get_devices_size(adapter: *mut BluetoothAdapter) -> c_int;
    pub fn bluetooth_adapter_inc_refcount(adapter: *mut BluetoothAdapter);
    pub fn bluetooth_adapter_dec_refcount(adapter: *mut BluetoothAdapter);
    pub fn bluetooth_adapter_free_adapter(adapter: *mut BluetoothAdapter);

    pub fn bluetooth_device_create_device(adapter: *mut BluetoothAdapter, address: *const c_char, length: c_int) -> *mut BluetoothDevice;
    pub fn bluetooth_device_get_name(device: *mut BluetoothDevice) -> *const c_char;
    pub fn bluetooth_device_get_uuids (device: *mut BluetoothDevice) -> *const *const c_char;
    pub fn bluetooth_device_get_uuids_size (device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_rssi (device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_tx_power (device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_manufacturer_data_keys (device: *mut BluetoothDevice) -> *const c_int;
    pub fn bluetooth_device_get_manufacturer_data_keys_size (device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_manufacturer_data_values (device: *mut BluetoothDevice, key: c_int) -> *const c_int;
    pub fn bluetooth_device_get_manufacturer_data_values_size (device: *mut BluetoothDevice, key: c_int) -> c_int;
    pub fn bluetooth_device_get_service_data_keys (device: *mut BluetoothDevice) -> *const *const c_char;
    pub fn bluetooth_device_get_service_data_keys_size (device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_service_data_values (device: *mut BluetoothDevice, key: *const c_char, length: c_int) -> *const c_int;
    pub fn bluetooth_device_get_service_data_values_size (device: *mut BluetoothDevice, key: *const c_char, length: c_int) -> c_int;
    pub fn bluetooth_device_connect_gatt(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_disconnect(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_is_connected(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_get_gatt_services(device: *mut BluetoothDevice) -> *const c_int;
    pub fn bluetooth_device_get_gatt_services_size(device: *mut BluetoothDevice) -> c_int;
    pub fn bluetooth_device_inc_refcount(device: *mut BluetoothDevice);
    pub fn bluetooth_device_dec_refcount(device: *mut BluetoothDevice);
    pub fn bluetooth_device_free_device(device: *mut BluetoothDevice);

    pub fn bluetooth_service_create_service(device: *mut BluetoothDevice, id: c_int) -> *mut BluetoothService;
    pub fn bluetooth_service_get_uuid(service: *mut BluetoothService) -> *const c_char;
    pub fn bluetooth_service_is_primary(service: *mut BluetoothService) -> c_int;
    pub fn bluetooth_service_get_gatt_characteristics(service: *mut BluetoothService) -> *const c_int;
    pub fn bluetooth_service_get_gatt_characteristics_size(service: *mut BluetoothService) -> c_int;
    pub fn bluetooth_service_inc_refcount(service: *mut BluetoothService);
    pub fn bluetooth_service_dec_refcount(service: *mut BluetoothService);
    pub fn bluetooth_service_free_service(service: *mut BluetoothService);

    pub fn bluetooth_characteristic_create_characteristic(service: *mut BluetoothService, id: c_int) -> *mut BluetoothCharacteristic;
    pub fn bluetooth_characteristic_get_uuid(characteristic: *mut BluetoothCharacteristic) -> *const c_char;
    pub fn bluetooth_characteristic_get_flags (characteristic: *mut BluetoothCharacteristic) -> *const *const c_char;
    pub fn bluetooth_characteristic_get_flags_size (characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_get_gatt_descriptors(characteristic: *mut BluetoothCharacteristic) -> *const c_int;
    pub fn bluetooth_characteristic_get_gatt_descriptors_size(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_get_value(characteristic: *mut BluetoothCharacteristic) -> *const c_int;
    pub fn bluetooth_characteristic_get_value_size(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_read_value(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_write_value(characteristic: *mut BluetoothCharacteristic, array: *const c_int, length: c_int) -> c_int;
    pub fn bluetooth_characteristic_start_notify(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_stop_notify(characteristic: *mut BluetoothCharacteristic) -> c_int;
    pub fn bluetooth_characteristic_inc_refcount(characteristic: *mut BluetoothCharacteristic);
    pub fn bluetooth_characteristic_dec_refcount(characteristic: *mut BluetoothCharacteristic);
    pub fn bluetooth_characteristic_free_characteristic(characteristic: *mut BluetoothCharacteristic);

    pub fn bluetooth_descriptor_create_descriptor(characteristic: *mut BluetoothCharacteristic, id: c_int) -> *mut BluetoothDescriptor;
    pub fn bluetooth_descriptor_get_uuid(descriptor: *mut BluetoothDescriptor) -> *const c_char;
    pub fn bluetooth_descriptor_get_value(descriptor: *mut BluetoothDescriptor) -> *const c_int;
    pub fn bluetooth_descriptor_get_value_size(descriptor: *mut BluetoothDescriptor) -> c_int;
    pub fn bluetooth_descriptor_read_value(descriptor: *mut BluetoothDescriptor) -> c_int;
    pub fn bluetooth_descriptor_write_value(descriptor: *mut BluetoothDescriptor, array: *const c_int, length: c_int) -> c_int;
    pub fn bluetooth_descriptor_inc_refcount(descriptor: *mut BluetoothDescriptor);
    pub fn bluetooth_descriptor_dec_refcount(descriptor: *mut BluetoothDescriptor);
    pub fn bluetooth_descriptor_free_descriptor(descriptor: *mut BluetoothDescriptor);

    pub fn jni_free_string(string: *const c_char);
    pub fn jni_free_string_array(string: *const *const c_char, length: c_int);
    pub fn jni_free_int_array(array: *const c_int);
}
