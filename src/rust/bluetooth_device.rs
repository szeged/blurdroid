use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_char, c_int};
use std::ptr::{self};
use std::sync::Arc;
use utils;

use bluetooth_adapter::Adapter;

#[derive(Clone, Debug)]
struct IDevice {
    device: Cell<*mut ffi::BluetoothDevice>,
}

#[derive(Debug)]
pub struct Device {
    i: Box<IDevice>,
    address: String,
}

impl Device {
    #[inline(always)]
    pub fn device(&self) -> *mut ffi::BluetoothDevice {
        self.i.device.get()
    }

    pub fn new(adapter: Arc<Adapter>, address: String) -> Device {
        println!("#### device new {:?} {:?} {:?}", address, address.as_ptr(), address.len());
        let device = unsafe { ffi::bluetooth_device_create_device(adapter.adapter(), address.as_ptr() as *const c_char, address.len() as c_int) };
        Device {
            i: Box::new(IDevice { device: Cell::new(device) }),
            address: address,
        }
    }

    pub fn get_id(&self) -> String {
        self.address.clone()
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        Ok(self.address.clone())
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        let name = unsafe { ffi::bluetooth_device_get_name(self.device()) };
        let res = match utils::c_str_to_slice(&name) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No name!")),
        };
        unsafe { ffi::jni_free_string(name) };
        res
    }

    pub fn connect(&self) -> Result<(), Box<Error>> {
        unsafe {
            Ok(ffi::bluetooth_device_connect_gatt(self.device()))
        }
    }

    pub fn disconnect(&self) -> Result<(), Box<Error>> {
        unsafe {
            Ok(ffi::bluetooth_device_disconnect(self.device()))
        }
    }

    pub fn is_connected(&self) -> Result<bool, Box<Error>> {
        unsafe {
            Ok(ffi::bluetooth_device_is_connected(self.device()).is_positive())
        }
    }

    pub fn get_gatt_services(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let services = ffi::bluetooth_device_get_gatt_services(self.device());
            let max = ffi::bluetooth_device_get_gatt_services_size(self.device()) as isize;
            for i in 0..max {
                let s_ptr = *services.offset(i);
                let s = s_ptr as i32;
                v.push(s.to_string());
            }
            if max > 0 {
                ffi::jni_free_int_array(services);
            }
        }
        Ok(v)
    }

    pub fn get_uuids(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let uuids = ffi::bluetooth_device_get_uuids(self.device());
            let max = ffi::bluetooth_device_get_uuids_size(self.device()) as isize;
            for i in 0..max {
                let u_ptr = *uuids.offset(i);
                let u = match utils::c_str_to_slice(&u_ptr) {
                    None => continue,
                    Some(uuid) => uuid.to_owned(),
                };
                println!("##### {:?}", u);
                v.push(u.clone());
            }
            println!("#### {:?}", v);
            if max > 0 {
                ffi::jni_free_string_array(uuids, max as i32);
            }
        }
        Ok(v)
    }

    pub fn get_rssi(&self) -> Result<i16, Box<Error>> {
        let rssi = unsafe { ffi::bluetooth_device_get_rssi(self.device()) as i32 };
        Ok(rssi as i16)
    }

    pub fn get_tx_power(&self) -> Result<i16, Box<Error>> {
        let tx_power = unsafe { ffi::bluetooth_device_get_tx_power(self.device()) as i32 };
        Ok(tx_power as i16)
    }

    pub fn get_appearance(&self) -> Result<u16, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_icon(&self) -> Result<String, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_class(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn is_paired(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn is_trusted(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn is_blocked(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_alias(&self) -> Result<String, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn set_alias(&self, _value: String) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn is_legacy_pairing(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_vendor_id_source(&self) -> Result<String, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_vendor_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_product_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_device_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn get_modalias(&self) -> Result<(String, u32, u32, u32), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn connect_profile(&self, _uuid: String) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn disconnect_profile(&self, _uuid: String) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn pair(&self) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn cancel_pairing(&self) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Device {
    fn clone(&self) -> Device {
        unsafe { ffi::bluetooth_device_inc_refcount(self.device()) };
        Device {
            i: self.i.clone(),
            address: self.address.clone(),
        }
    }
}

impl Drop for Device {
    fn drop(&mut self) {
        unsafe {
            ffi::bluetooth_device_dec_refcount(self.device());
            ffi::bluetooth_device_free_device(self.device());
        }
    }
}
