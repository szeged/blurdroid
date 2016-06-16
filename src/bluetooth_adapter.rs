use std::error::Error;
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothAdapter { }

impl BluetoothAdapter {
    fn new() -> BluetoothAdapter {
        BluetoothAdapter { }
    }

    pub fn init() -> Result<BluetoothAdapter, Box<Error>> {
        unsafe {
            println!("####INIT####");
            ffi::BluetoothAdapter_init();
            Ok(BluetoothAdapter::new())
        }
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        unsafe {
            println!("####GET_ADDRESS####");
            let address = ffi::BluetoothAdapter_getAddress();
            Ok(utils::c_str_to_slice(&address).unwrap().to_owned())
        }
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        unsafe {
            println!("####GET_NAME####");
            let name = ffi::BluetoothAdapter_getName();
            Ok(utils::c_str_to_slice(&name).unwrap().to_owned())
        }
    }
}
