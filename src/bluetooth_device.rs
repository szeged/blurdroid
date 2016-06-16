use std::error::Error;
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothDevice {
    address: String
}

impl BluetoothDevice {
    pub fn new(address: String) ->BluetoothDevice {
        BluetoothDevice {
            address: address
        }
    }

    /*pub fn create_device(address: String) -> Result<BluetoothDevice, Box<Error>> {
        Ok(BluetoothDevice::new(address))
    }*/

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        Ok("blurdroid_address".to_owned())
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        Ok("blurdroid_name".to_owned())
    }
}
