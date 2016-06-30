use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothDevice {
    address: String,
}

impl BluetoothDevice {
    pub fn new(address: String) ->BluetoothDevice {
        println!("####NEW_DEVICE####");
        BluetoothDevice {
            address: address,
        }
    }

    pub fn get_address(&mut self) -> String {
        println!("####GET_ADDRESS####");
        self.address.clone()
    }

    pub fn get_name(&mut self) -> Result<String, Box<Error>> {
        println!("####GET_NAME####");
        unsafe {
            let name = ffi::BluetoothDevice_getName(self.address.clone().as_ptr() as *const c_char);
            
            match utils::c_str_to_slice(&name) {
                Some(name) => Ok(name.to_owned()),
                None => Err(Box::from("Error!")),
            }
        }
    }

    pub fn connect(&mut self) -> Result<(), Box<Error>> {
        println!("####CONNECT####");
        unsafe {
            Ok(ffi::BluetoothDevice_connectGatt(self.address.clone().as_ptr() as *const c_char))
        }
    }

    pub fn get_gatt_services(&mut self) -> Result<Vec<i32>, Box<Error>> {
        println!("####GET_GATT_SERVICES####");
        let mut v = vec!();
        unsafe {
            //println!("####  discoverServices result: {:?}", ffi::BluetoothGatt_discoverServices());
            let services = ffi::BluetoothGatt_getServices(self.address.clone().as_ptr() as *const c_char);
            println!("#### SERVICE PTR {:?} ####", services);
            let max = ffi::BluetoothDevice_getServicesSize(self.address.clone().as_ptr() as *const c_char) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### SERVICES LENGTH {:?} ####", max);
            for i in 0..max {
                let s_ptr = *services.offset(i);
                /*let s = match utils::c_str_to_slice(&s_ptr) {
                    None => break,
                    Some(service) => service.to_owned(),
                };*/
                let s = s_ptr as i32;
                println!("#### SERVICE #{}: {:?} ####", i, s);
                v.push(s);
            }
            ffi::Helper_freeIntArray(services);
        }
        Ok(v)
    }

}
