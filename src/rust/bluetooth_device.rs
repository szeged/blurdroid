use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_char};
use std::ptr;
use std::sync::Arc;
use utils;
use time;

use bluetooth_adapter::Adapter;

const NOT_SUPPORTED_ERROR: &'static str = "Error! Not supported platform!";

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
        println!("{} Device new", time::precise_time_ns());
        let device = unsafe { ffi::bluetooth_device_create_device(adapter.adapter(), address.as_ptr() as *const c_char) };
        if device == ptr::null_mut() {
            //return Err(Box::from("No device found!"));
            println!("#### NO DEVICE FOUND!!!!! {:?}", device);
        }
        println!("{} Device new {:?}", time::precise_time_ns(), device);
        let d = Device {
            i: Box::new(IDevice { device: Cell::new(device) } ),
            address: address,
        };
        d
    }

    /*pub fn get_address(&self) -> Result<String, Box<Error>> {
        println!("{} Device get_address {:?}", time::precise_time_ns(), self.device());
        let address = unsafe { ffi::bluetooth_device_get_address(self.device()) };
        let res = match utils::c_str_to_slice(&address) {
            Some(a) => a.to_string(),//Ok(a.to_string()),
            None => String::from("Error"),//Err(Box::from("No address!")),
        };
        println!("{} Device get_address {:?} {:?}", time::precise_time_ns(), address, res);
        unsafe { ffi::bluetooth_device_free_string(address) };
        res
    }*/

    pub fn get_id(&self) -> String {
        self.address.clone()
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        println!("{} Device get_address {}", time::precise_time_ns(), self.address);
        Ok(self.address.clone())
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

    pub fn connect(&self) -> Result<(), Box<Error>> {
        println!("{} Device connect {:?}", time::precise_time_ns(), self.device());
        unsafe {
            Ok(ffi::bluetooth_device_connect_gatt(self.device()))
        }
    }

    pub fn disconnect(&self) -> Result<(), Box<Error>> {
        println!("{} Device disconnect {:?}", time::precise_time_ns(), self.device());
        unsafe {
            Ok(ffi::bluetooth_device_disconnect(self.device()))
        }
    }

    pub fn is_connected(&self) -> Result<bool, Box<Error>> {
        println!("{} Device is_connected {:?}", time::precise_time_ns(), self.device());
        unsafe {
            Ok(ffi::bluetooth_device_is_connected(self.device()).is_positive())
        }
    }

    pub fn get_gatt_services(&self) -> Result<Vec<String>, Box<Error>> {
        println!("####GET_GATT_SERVICES####");
        let mut v = vec!();
        unsafe {
            let services = ffi::bluetooth_device_get_gatt_services(self.device());
            println!("#### SERVICE PTR {:?} ####", services);
            let max = ffi::bluetooth_device_get_gatt_services_size(self.device()) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*services.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### SERVICE LENGTH {:?} ####", max);
            for i in 0..max {
                println!("#### serv #{} ####", i);
                let s_ptr = *services.offset(i);
                println!("#### serv offset {:?} ####", s_ptr);
                let s = s_ptr as i32;
                println!("#### SERVICE #{}: {:?} ####", i, s);
                v.push(s.to_string());
            }
            ffi::bluetooth_device_free_int_array(services);
        }
        Ok(v)
    }

    pub fn get_icon(&self) -> Result<String, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_class(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_appearance(&self) -> Result<u16, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_uuids(&self) -> Result<Vec<String>, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_paired(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_trusted(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_blocked(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_alias(&self) -> Result<String, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_alias(&self, _value: String) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_legacy_pairing(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_vendor_id_source(&self) -> Result<String, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_vendor_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_product_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_device_id(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_modalias(&self) -> Result<(String, u32, u32, u32), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_rssi(&self) -> Result<i16, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_tx_power(&self) -> Result<i16, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn connect_profile(&self, _uuid: String) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn disconnect_profile(&self, _uuid: String) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn pair(&self) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn cancel_pairing(&self) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Device {
    fn clone(&self) -> Device {
        println!("{} Clone Device {:?}", time::precise_time_ns(), self.device());
        unsafe { ffi::bluetooth_device_inc_refcount(self.device()) };
        Device {
            i: self.i.clone(),
            address: self.address.clone(),
        }
    }
}

impl Drop for Device {
    fn drop(&mut self) {
        println!("{} Drop Device {:?}", time::precise_time_ns(), self.device());
        unsafe {
            ffi::bluetooth_device_dec_refcount(self.device());
            ffi::bluetooth_device_free_device(self.device());
        }
    }
}
