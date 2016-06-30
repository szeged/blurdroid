/*use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;*/

#[derive(Clone, Debug)]
pub struct BluetoothGATTDescriptor {
    address: String,
    uuid: String,
}

impl BluetoothGATTDescriptor {
    pub fn new(address: String, uuid: String) -> BluetoothGATTDescriptor {
        println!("####NEW_DESCRIPTOR {} {}####", address, uuid);
        BluetoothGATTDescriptor {
            address: address,
            uuid: uuid,
        }
    }

    pub fn get_uuid(&self) -> String {
        println!("####GET_UUID_{}####", self.uuid);
        self.uuid.clone()
    }
}
