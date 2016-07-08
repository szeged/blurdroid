//extern crate libc;
extern crate time;

pub use bluetooth_adapter::Adapter;
pub use bluetooth_device::Device;
/*pub use bluetooth_gatt_service::BluetoothGATTService;
pub use bluetooth_gatt_characteristic::BluetoothGATTCharacteristic;
pub use bluetooth_gatt_descriptor::BluetoothGATTDescriptor;
*/
pub use bluetooth_discovery_session::DiscoverySession;

pub mod bluetooth_adapter;
pub mod bluetooth_device;
/*pub mod bluetooth_gatt_service;
pub mod bluetooth_gatt_characteristic;
pub mod bluetooth_gatt_descriptor;
*/
pub mod bluetooth_discovery_session;

mod ffi;
mod utils;
