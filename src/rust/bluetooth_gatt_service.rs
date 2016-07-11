use ffi;
use std::cell::Cell;
use std::error::Error;
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
    id: i32,
}

impl Service {
    #[inline(always)]
    pub fn service(&self) -> *mut ffi::BluetoothService {
        self.i.service.get()
    }

    pub fn new(device: Device, id: i32) -> Service {
        println!("{} Service new", time::precise_time_ns());
        let service = unsafe { ffi::bluetooth_service_create_service(device.device(), id) };
        if service == ptr::null_mut() {
            //return Err(Box::from("No device found!"));
            println!("#### NO SERVICE FOUND!!!!! {:?}", service);
        }
        println!("{} Service new {:?}", time::precise_time_ns(), service);
        let s = Service {
            i: Box::new(IService { service: Cell::new(service) }),
            id: id,
        };
        s
    }

    pub fn get_id(&self) -> i32 {
        self.id
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        println!("{} Service get_uuid {:?}", time::precise_time_ns(), self.service());
        let uuid = unsafe { ffi::bluetooth_service_get_uuid(self.service()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        println!("{} Service get_uuid {:?} {:?}", time::precise_time_ns(), uuid, res);
        unsafe { ffi::bluetooth_service_free_string(uuid) };
        res
    }

    pub fn is_primary(&mut self) -> Result<bool, Box<Error>> {
        println!("{} Service is_primary {:?}", time::precise_time_ns(), self.service());
        unsafe {
            Ok(ffi::bluetooth_service_is_primary(self.service()).is_positive())
        }
    }

    pub fn get_gatt_characteristics(&mut self) -> Result<Vec<i32>, Box<Error>> {
        println!("####GET_GATT_CHARACTERISTICS####");
        let mut v = vec!();
        unsafe {
            let characteristics = ffi::bluetooth_service_get_gatt_characteristics(self.service());
            println!("#### CHARACTERISTICS PTR {:?} ####", characteristics);
            let max = ffi::bluetooth_service_get_gatt_characteristics_size(self.service()) as isize;
            println!("#### CHARACTERISTICS LENGTH {:?} ####", max);
            for i in 0..max {
                let c_ptr = *characteristics.offset(i);
                let c = c_ptr as i32;
                println!("#### CHARACTERISTICS #{}: {:?} ####", i, c);
                v.push(c);
            }
            ffi::bluetooth_service_free_int_array(characteristics);
        }
        Ok(v)
    }
}

impl Drop for Service {
    fn drop(&mut self) {
        println!("{} Drop Service {:?}", time::precise_time_ns(), self.service());
        /*unsafe {
            ffi::bluetooth_service_free_service(self.service());
        }*/
    }
}