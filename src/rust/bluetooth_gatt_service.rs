use bluetooth_device::Device;
use ffi;
use std::cell::Cell;
use std::error::Error;
use std::sync::Arc;
use utils;

#[derive(Clone, Debug)]
struct IService {
    service: Cell<*mut ffi::BluetoothService>,
}

#[derive(Debug)]
pub struct Service {
    i: Box<IService>,
    id: String,
}

impl Service {
    #[inline(always)]
    pub fn service(&self) -> *mut ffi::BluetoothService {
        self.i.service.get()
    }

    pub fn new(device: Arc<Device>, id: String) -> Service {
        let service = unsafe { ffi::bluetooth_service_create_service(device.device(), id.parse::<i32>().unwrap()) };
        Service {
            i: Box::new(IService { service: Cell::new(service) }),
            id: id,
        }
    }

    pub fn get_id(&self) -> String {
        self.id.clone()
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        let uuid = unsafe { ffi::bluetooth_service_get_uuid(self.service()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        unsafe { ffi::bluetooth_service_free_string(uuid) };
        res
    }

    pub fn is_primary(&self) -> Result<bool, Box<Error>> {
        unsafe {
            Ok(ffi::bluetooth_service_is_primary(self.service()).is_positive())
        }
    }

    pub fn get_gatt_characteristics(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let characteristics = ffi::bluetooth_service_get_gatt_characteristics(self.service());
            let max = ffi::bluetooth_service_get_gatt_characteristics_size(self.service()) as isize;
            for i in 0..max {
                let c_ptr = *characteristics.offset(i);
                let c = c_ptr as i32;
                v.push(c.to_string());
            }
            ffi::bluetooth_service_free_int_array(characteristics);
        }
        Ok(v)
    }

    pub fn get_includes(&self) -> Result<Vec<String>, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Service {
    fn clone(&self) -> Service {
        unsafe { ffi::bluetooth_service_inc_refcount(self.service()) };
        Service {
            i: self.i.clone(),
            id: self.id.clone(),
        }
    }
}


impl Drop for Service {
    fn drop(&mut self) {
        unsafe {
            ffi::bluetooth_service_dec_refcount(self.service());
            ffi::bluetooth_service_free_service(self.service());
        }
    }
}