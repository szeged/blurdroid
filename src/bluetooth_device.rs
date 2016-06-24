use std::error::Error;
use std::os::raw::{c_char};
use utils;
use ffi;

#[derive(Clone, Debug)]
pub struct BluetoothDevice {
    address: String,
    present: bool,
}

impl BluetoothDevice {
    pub fn new(address: String) ->BluetoothDevice {
        println!("####NEW_DEVICE####");
        BluetoothDevice {
            present: BluetoothDevice::create_device(&address),
            address: address,
        }
    }

    fn create_device(address: &String) -> bool {
        println!("####CREATE_DEVICE####");
        return unsafe { ffi::Helper_setRemoteDevice(address.as_ptr() as *const c_char).is_positive()}
    }

    // TODO remove this, we check isPresent in java
    fn check_present(&mut self) -> Result<(), Box<Error>> {
        println!("####CHECK_PRESENT####");
        if !self.present {
            println!("####DEVICE_NOT_PRESENT####");
            self.present = BluetoothDevice::create_device(&self.address);
        }
        if self.present {
            println!("####DEVICE_PRESENT_{}####", self.address);
            return Ok(())
        }
        Err(Box::from("Device Error: not present!"))
    }

    pub fn get_address(&mut self) -> Result<String, Box<Error>> {
        println!("####GET_ADDRESS####");
        try!(self.check_present());
        unsafe {
            let address = ffi::BluetoothDevice_getAddress();
            Ok(utils::c_str_to_slice(&address).unwrap().to_owned())
        }
    }

    pub fn get_name(&mut self) -> Result<String, Box<Error>> {
        println!("####GET_NAME####");
        try!(self.check_present());
        unsafe {
            let name = ffi::BluetoothDevice_getName();
            Ok(utils::c_str_to_slice(&name).unwrap().to_owned())
        }
    }

    pub fn connect(&mut self) -> Result<(), Box<Error>> {
        println!("####CONNECT####");
        try!(self.check_present());
        unsafe {
            Ok(ffi::BluetoothDevice_connectGatt())
        }
    }

    pub fn get_gatt_services(&mut self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_GATT_SERVICES####");
            try!(self.check_present());
            println!("####  discoverServices result: {:?}", ffi::BluetoothGatt_discoverServices());
            let services = ffi::BluetoothGatt_getServices();
            println!("#### SERVICE PTR {:?} ####", services);
            let max = ffi::Helper_arraySize(services) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### SERVICES LENGTH {:?} ####", max);
            for i in 0..max {
                let s_ptr = *services.offset(i);
                let s = match utils::c_str_to_slice(&s_ptr) {
                    None => break,
                    Some(service) => service.to_owned(),
                };
                println!("#### SERVICE #{}: {:?} ####", i, s);
                v.push(s.clone());
            }
        }
        Ok(v)
    }

}
