use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothGATTService {
    uuid: String,
}

impl BluetoothGATTService {
    pub fn new(uuid: String) ->BluetoothGATTService {
        println!("####NEW_SERVICE {}####", uuid);
        BluetoothGATTService {
            uuid: uuid,
        }
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        Ok(self.uuid.clone())
    }
}
