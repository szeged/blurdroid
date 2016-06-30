use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothGATTService {
    address: String,
    id: i32,
}

impl BluetoothGATTService {
    pub fn new(address: String, id: i32) -> BluetoothGATTService {
        println!("####NEW_SERVICE {} {}####", address, id);
        BluetoothGATTService {
            address: address,
            id: id,
        }
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        println!("####GET_UUID####");
        unsafe {
            let uuid = ffi::BluetoothGattService_getUuid(self.address.clone().as_ptr() as *const c_char, self.id);
            Ok(utils::c_str_to_slice(&uuid).unwrap().to_owned())
        }
    }

    pub fn get_instance_id(&self) -> i32 {
        println!("####GET_INSTANCE_ID####");
        self.id
    }

    pub fn is_primary(&self) -> Result<bool, Box<Error>> {
        println!("####IS_PRIMARY####");
        unsafe {
            Ok(ffi::BluetoothGattService_isPrimary(
                self.address.clone().as_ptr() as *const c_char, self.id).is_positive())
        }
    }

    pub fn get_device(&self) -> String {
        println!("####GET_DEVICE####");
        self.address.clone()
    }

    pub fn get_gatt_characteristics(&mut self) -> Result<Vec<i32>, Box<Error>> {
        println!("####GET_GATT_CHARACTERISTICS####");
        let mut v = vec!();
        unsafe {
            //println!("####  discoverServices result: {:?}", ffi::BluetoothGatt_discoverServices());
            let characteristics = ffi::BluetoothGattService_getCharacteristics(self.address.clone().as_ptr() as *const c_char, self.id);
            println!("#### CHARACTERISTICS PTR {:?} ####", characteristics);
            let max = ffi::BluetoothDevice_getCharacteristicsSize(self.address.clone().as_ptr() as *const c_char) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### CHARACTERISTICS LENGTH {:?} ####", max);
            for i in 0..max {
                let c_ptr = *characteristics.offset(i);
                /*let s = match utils::c_str_to_slice(&s_ptr) {
                    None => break,
                    Some(service) => service.to_owned(),
                };*/
                let c = c_ptr as i32;
                println!("#### CHARACTERISTICS #{}: {:?} ####", i, c);
                v.push(c);
            }
            ffi::Helper_freeIntArray(characteristics);
        }
        Ok(v)
    }

}
