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

    pub fn get_devices(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_DEVICES####");
            let devices = ffi::BluetoothAdapter_getDevices();
            println!("#### DEVICES PTR {:?} ####", devices);
            let max = ffi::BluetoothAdapter_getDevicesSize() as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### DEVICES LENGTH {:?} ####", max);
            for i in 0..max {
                println!("#### dev #{} ####", i);
                let d_ptr = *devices.offset(i);
                println!("#### dev offset {:?} ####", d_ptr);
                let d = match utils::c_str_to_slice(&d_ptr) {
                    None => continue,
                    Some(dev) => dev.to_owned(),
                };
                println!("#### DEVICE #{}: {:?} ####", i, d);
                v.push(d.clone());
            }
            ffi::Helper_freeCharArray(devices, max as i32);
        }
        Ok(v)
    }
}
