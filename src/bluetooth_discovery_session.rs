use std::error::Error;
use ffi;

#[derive(Debug)]
pub struct BluetoothDiscoverySession { }

impl BluetoothDiscoverySession {
    fn new() -> BluetoothDiscoverySession {
        BluetoothDiscoverySession { }
    }

    pub fn create_session(_adapter: String) -> Result<BluetoothDiscoverySession, Box<Error>> {
        Ok(BluetoothDiscoverySession::new())
    }

    pub fn start_discovery(&self) -> Result<(), Box<Error>> {
        unsafe {
            ffi::BluetoothAdapter_startDiscovery();
        }
        Ok(())
    }

    pub fn stop_discovery(&self) -> Result<(), Box<Error>> {
        unsafe {
            ffi::BluetoothAdapter_stopDiscovery();
        }
        Ok(())
    }
}
