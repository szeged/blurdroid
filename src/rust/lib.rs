#![cfg(target_os = "android")]

#[macro_use]
mod utils;
mod ffi;

pub use bluetooth_adapter::Adapter;
pub use bluetooth_device::Device;
pub use bluetooth_gatt_service::Service;
pub use bluetooth_gatt_characteristic::Characteristic;
pub use bluetooth_gatt_descriptor::Descriptor;
pub use bluetooth_discovery_session::DiscoverySession;

pub mod bluetooth_adapter;
pub mod bluetooth_device;
pub mod bluetooth_gatt_service;
pub mod bluetooth_gatt_characteristic;
pub mod bluetooth_gatt_descriptor;
pub mod bluetooth_discovery_session;

