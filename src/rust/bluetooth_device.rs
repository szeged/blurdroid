use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_char};
use std::ptr;
use utils;
use time;

use bluetooth_adapter::Adapter;

#[derive(Clone, Debug)]
struct IDevice {
    device: Cell<*mut ffi::BluetoothDevice>,
}

#[derive(Clone, Debug)]
pub struct Device {
    i: Box<IDevice>,
}

impl Device {
    #[inline(always)]
    pub fn device(&self) -> *mut ffi::BluetoothDevice {
        self.i.device.get()
    }

    pub fn new(adapter: Adapter, address: String) -> Device {
        println!("{} Device new", time::precise_time_ns());
        let device = unsafe { ffi::bluetooth_device_create_device(adapter.adapter(), address.as_ptr() as *const c_char) };
        if device == ptr::null_mut() {
            //return Err(Box::from("No device found!"));
            println!("#### NO DEVICE FOUND!!!!! {:?}", device);
        }
        println!("{} Device new {:?}", time::precise_time_ns(), device);
        let d = Device { i: Box::new(IDevice {
            device: Cell::new(device),
        })};
        d
    }

//    pub fn get_address(&self) -> Result<String, Box<Error>> {
    pub fn get_address(&self) -> String {
        println!("{} Device get_address {:?}", time::precise_time_ns(), self.device());
        let address = unsafe { ffi::bluetooth_device_get_address(self.device()) };
        let res = match utils::c_str_to_slice(&address) {
            Some(a) => a.to_string(),//Ok(a.to_string()),
            None => String::from("Error"),//Err(Box::from("No address!")),
        };
        println!("{} Device get_address {:?} {:?}", time::precise_time_ns(), address, res);
        unsafe { ffi::bluetooth_device_free_string(address) };
        res
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        println!("{} Device get_name {:?}", time::precise_time_ns(), self.device());
        let name = unsafe { ffi::bluetooth_device_get_name(self.device()) };
        let res = match utils::c_str_to_slice(&name) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No name!")),
        };
        println!("{} Device get_name {:?} {:?}", time::precise_time_ns(), name, res);
        unsafe { ffi::bluetooth_device_free_string(name) };
        res
    }
}

impl Drop for Device {
    fn drop(&mut self) {
        println!("{} Drop Device {:?}", time::precise_time_ns(), self.device());
        /*unsafe {
            ffi::bluetooth_adapter_free_adapter(self.adapter());
        }*/
    }
}
