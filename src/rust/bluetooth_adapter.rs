use ffi;
use std::cell::Cell;
use std::error::Error;
use std::ptr;
use utils;
use time;
//use std::collections::HashMap;

#[derive(Clone, Debug)]
struct IAdapter {
    adapter: Cell<*mut ffi::BluetoothAdapter>,
}

#[derive(Clone, Debug)]
pub struct Adapter {
    i: Box<IAdapter>,
    //devices: Option<HashMap<Device>>,
}

unsafe impl Send for Adapter {}

impl Adapter {
    #[inline(always)]
    pub fn adapter(&self) -> *mut ffi::BluetoothAdapter {
        self.i.adapter.get()
    }

    pub fn get_adapter() -> Result<Adapter, Box<Error>> {
        println!("{} Adapter get_adapter", time::precise_time_ns());
        let adapter = unsafe { ffi::bluetooth_adapter_get_adapter() };
        if adapter == ptr::null_mut() {
            return Err(Box::from("No adapter found!"));
        }
        println!("{} Adapter get_adapter {:?}", time::precise_time_ns(), adapter);
        let a = Adapter { i: Box::new(IAdapter {
            adapter: Cell::new(adapter),
            //devices: None,
        })};
        Ok(a)
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        println!("{} Adapter get_address {:?}", time::precise_time_ns(), self.adapter());
        let address = unsafe { ffi::bluetooth_adapter_get_address(self.adapter()) };
        let res = match utils::c_str_to_slice(&address) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No address!")),
        };
        println!("{} Adapter get_address {:?} {:?}", time::precise_time_ns(), address, res);
        unsafe { ffi::bluetooth_adapter_free_string(address) };
        res
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        println!("{} Adapter get_name {:?}", time::precise_time_ns(), self.adapter());
        let name = unsafe { ffi::bluetooth_adapter_get_name(self.adapter()) };
        let res = match utils::c_str_to_slice(&name) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No name!")),
        };
        println!("{} Adapter get_name {:?} {:?}", time::precise_time_ns(), name, res);
        unsafe { ffi::bluetooth_adapter_free_string(name) };
        res
    }

    pub fn get_devices(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_DEVICES####");
            let devices = ffi::bluetooth_adapter_get_devices(self.adapter());
            println!("#### DEVICES PTR {:?} ####", devices);
            let max = ffi::bluetooth_adapter_get_devices_size(self.adapter()) as isize;
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
            ffi::bluetooth_adapter_free_string_array(devices);
        }
        Ok(v)
    }
}

impl Drop for Adapter {
    fn drop(&mut self) {
        println!("{} Drop Adapter {:?}", time::precise_time_ns(), self.adapter());
        /*unsafe {
            ffi::bluetooth_adapter_free_adapter(self.adapter());
        }*/
    }
}
