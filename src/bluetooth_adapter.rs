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

    pub fn get_bonded_devices(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_BONDED_DEVICES####");
            let devices = ffi::BluetoothAdapter_getBondedDevices();
            println!("#### DEVICES PTR {:?} ####", devices);
            //println!("#### {:?} ####", utils::c_str_to_slice(&devices).unwrap());
            //for device in de
            //v.push(utils::c_str_to_slice(&name).unwrap().to_owned());
            let mut i = 0;
            let max = ffi::Helper_arraySize(devices) as isize;
            println!("#### DEVICES LENGTH {:?} ####", max);
            loop {
                let d_ptr = *devices.offset(i);
                let d = match utils::c_str_to_slice(&d_ptr) {
                    None => break,
                    Some(dev) => dev.to_owned(),
                };
                println!("#### DEVICE #{}: {:?} ####", i, d);
                v.push(d.clone());
                i += 1;
                if i == max {
                    break;
                }
            }
            //(0..argc).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
        }
        Ok(v)
    }
}
