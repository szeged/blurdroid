use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothGATTCharacteristic {
    address: String,
    id: i32,
}

impl BluetoothGATTCharacteristic {
    pub fn new(address: String, id: i32) -> BluetoothGATTCharacteristic {
        println!("####NEW_CHARACTERISTIC {} {}####", address, id);
        BluetoothGATTCharacteristic {
            address: address,
            id: id,
        }
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        println!("####GET_UUID####");
        unsafe {
            let uuid = ffi::BluetoothGattCharacteristic_getUuid(self.address.clone().as_ptr() as *const c_char, self.id);
            Ok(utils::c_str_to_slice(&uuid).unwrap().to_owned())
        }
    }

    pub fn get_instance_id(&self) -> i32 {
        println!("####GET_INSTANCE_ID####");
        self.id
    }

    pub fn get_device(&self) -> String {
        println!("####GET_DEVICE####");
        self.address.clone()
    }

    pub fn get_gatt_descriptors(&mut self) -> Result<Vec<String>, Box<Error>> {
        println!("####GET_GATT_DESCRIPTORS####");
        let mut v = vec!();
        unsafe {
            //println!("####  discoverdescriptors result: {:?}", ffi::BluetoothGatt_discoverdescriptors());
            let descriptors = ffi::BluetoothGattCharacteristic_getDescriptors(self.address.clone().as_ptr() as *const c_char, self.id);
            println!("#### DESCRIPTOR PTR {:?} ####", descriptors);
            let max = ffi::BluetoothDevice_getDescriptorsSize(self.address.clone().as_ptr() as *const c_char) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### DESCRIPTOR LENGTH {:?} ####", max);
            for i in 0..max {
                let d_ptr = *descriptors.offset(i);
                let d = match utils::c_str_to_slice(&d_ptr) {
                    None => break,
                    Some(descriptor) => descriptor.to_owned(),
                };
                println!("#### DESCRIPTOR #{}: {:?} ####", i, d);
                v.push(d.clone());
            }
            ffi::Helper_freeCharArray(descriptors, max as i32);
        }
        Ok(v)
    }
}
