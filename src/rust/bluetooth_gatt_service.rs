use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_char};
use std::ptr;
use utils;
use time;

use bluetooth_device::Device;

#[derive(Clone, Debug)]
struct IService {
    service: Cell<*mut ffi::BluetoothService>,
}

#[derive(Clone, Debug)]
pub struct Service {
    i: Box<IService>,
}

impl Service {
    #[inline(always)]
    pub fn service(&self) -> *mut ffi::BluetoothService {
        self.i.service.get()
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
}