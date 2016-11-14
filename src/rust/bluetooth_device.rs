use ffi;
use std::cell::Cell;
use std::collections::HashMap;
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
        check_null!(&name, "No name found!");
        let res = try!(utils::convert_cstr(&name, "No name found!"));
        unsafe { ffi::jni_free_string(name) };
        Ok(res)
    }

    pub fn connect(&self) -> Result<(), Box<Error>> {
        let connect = unsafe { ffi::bluetooth_device_connect_gatt(self.device()) };
        if !connect.is_positive() {
            return Err(Box::from("Connect error!"));
        }
        Ok(())
    }

    pub fn disconnect(&self) -> Result<(), Box<Error>> {
        let disconnect = unsafe { ffi::bluetooth_device_disconnect(self.device()) };
        if !disconnect.is_positive() {
            return Err(Box::from("Disconnect error!"));
        }
        Ok(())
    }

    pub fn is_connected(&self) -> Result<bool, Box<Error>> {
        Ok( unsafe { ffi::bluetooth_device_is_connected(self.device()).is_positive() })
    }

    pub fn get_gatt_services(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let services = ffi::bluetooth_device_get_gatt_services(self.device());
            check_null!(&services, "No services found!");
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
            check_null!(&uuids, "No uuids found!");
            let max = ffi::bluetooth_device_get_uuids_size(self.device()) as isize;
            for i in 0..max {
                let u_ptr = *uuids.offset(i);
                let u = match utils::c_str_to_slice(&u_ptr) {
                    None => continue,
                    Some(uuid) => uuid.to_owned(),
                };
                v.push(u.clone());
            }
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

    pub fn get_manufacturer_data(&self) -> Result<HashMap<u16, Vec<u8>>, Box<Error>> {
        let mut m = HashMap::new();
        unsafe {
            // Map keys
            let keys = ffi::bluetooth_device_get_manufacturer_data_keys(self.device());
            check_null!(&keys, "No manufacturer data found!");
            let max = ffi::bluetooth_device_get_manufacturer_data_keys_size(self.device()) as isize;
            let mut k = vec!();
            for i in 0..max {
                let val_ptr = *keys.offset(i);
                let val = (val_ptr as i32) as u16;
                k.push(val);
            }
            if max > 0 {
                ffi::jni_free_int_array(keys);
            }
            // Map values from key
            for key in k {
                let values = ffi::bluetooth_device_get_manufacturer_data_values(self.device(), key as i32);
                check_null!(&values, "No manufacturer data found!");
                let max = ffi::bluetooth_device_get_manufacturer_data_values_size(self.device(), key as i32) as isize;
                let mut v = vec!();
                for i in 0..max {
                    let val_ptr = *values.offset(i);
                    let val = (val_ptr as i32) as u8;
                    v.push(val);
                }
                if max > 0 {
                    ffi::jni_free_int_array(values);
                }
                m.insert(key, v);
            }
        }
        Ok(m)
    }

    pub fn get_service_data(&self) -> Result<HashMap<String, Vec<u8>>, Box<Error>> {
        let mut m = HashMap::new();
        unsafe {
            // Map keys
            let keys = ffi::bluetooth_device_get_service_data_keys(self.device());
            check_null!(&keys, "No service data found!");
            let max = ffi::bluetooth_device_get_service_data_keys_size(self.device()) as isize;
            let mut k = vec!();
            for i in 0..max {
                let u_ptr = *keys.offset(i);
                let u = match utils::c_str_to_slice(&u_ptr) {
                    None => continue,
                    Some(uuid) => uuid.to_owned(),
                };
                k.push(u.clone());
            }
            if max > 0 {
                ffi::jni_free_string_array(keys, max as i32);
            }
            // Map values from key
            for key in k {
                let values = ffi::bluetooth_device_get_service_data_values(self.device(), key.as_ptr() as *const c_char, key.len() as c_int);
                check_null!(&values, "No service data found!");
                let max = ffi::bluetooth_device_get_service_data_values_size(self.device(), key.as_ptr() as *const c_char, key.len() as c_int) as isize;
                let mut v = vec!();
                for i in 0..max {
                    let val_ptr = *values.offset(i);
                    let val = (val_ptr as i32) as u8;
                    v.push(val);
                }
                if max > 0 {
                    ffi::jni_free_int_array(values);
                }
                m.insert(key, v);
            }
        }
        Ok(m)
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
