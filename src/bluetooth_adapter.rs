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

    fn is_present(&self) -> bool {
        println!("####IS_PRESENT####");
        return unsafe { ffi::BluetoothAdapter_isEnabled().is_positive() };
    }

    fn check_present(&self) -> Result<(), Box<Error>> {
        if self.is_present() {
            println!("####ADAPTER_PRESENT####");
            return Ok(())
        }
        Err(Box::from("Adapter Error: not present!"))
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        unsafe {
            println!("####GET_ADDRESS####");
            try!(self.check_present());
            let address = ffi::BluetoothAdapter_getAddress();
            Ok(utils::c_str_to_slice(&address).unwrap().to_owned())
        }
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        unsafe {
            println!("####GET_NAME####");
            try!(self.check_present());
            let name = ffi::BluetoothAdapter_getName();
            Ok(utils::c_str_to_slice(&name).unwrap().to_owned())
        }
    }

    pub fn get_bonded_devices(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_BONDED_DEVICES####");
            try!(self.check_present());
            let devices = ffi::BluetoothAdapter_getBondedDevices();
            println!("#### DEVICES PTR {:?} ####", devices);
            let max = ffi::Helper_arraySize(devices) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### DEVICES LENGTH {:?} ####", max);
            for i in 0..max {
                let d_ptr = *devices.offset(i);
                let d = match utils::c_str_to_slice(&d_ptr) {
                    None => break,
                    Some(dev) => dev.to_owned(),
                };
                println!("#### DEVICE #{}: {:?} ####", i, d);
                v.push(d.clone());
            }
        }
        Ok(v)
    }
}
