use bluetooth_gatt_service::Service;
use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_int};
use std::sync::Arc;
use utils;

#[derive(Clone, Debug)]
struct ICharacteristic {
    characteristic: Cell<*mut ffi::BluetoothCharacteristic>,
}

#[derive(Debug)]
pub struct Characteristic {
    i: Box<ICharacteristic>,
    id: String,
}

impl Characteristic {
    #[inline(always)]
    pub fn characteristic(&self) -> *mut ffi::BluetoothCharacteristic {
        self.i.characteristic.get()
    }

    pub fn new(service: Arc<Service>, id: String) -> Characteristic {
        let characteristic = unsafe { ffi::bluetooth_characteristic_create_characteristic(service.service(), id.parse::<i32>().unwrap()) };
        Characteristic {
            i: Box::new(ICharacteristic { characteristic: Cell::new(characteristic) }),
            id: id,
        }
    }

    pub fn get_id(&self) -> String {
        self.id.clone()
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        let uuid = unsafe { ffi::bluetooth_characteristic_get_uuid(self.characteristic()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        unsafe { ffi::jni_free_string(uuid) };
        res
    }

    pub fn get_gatt_descriptors(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let descriptors = ffi::bluetooth_characteristic_get_gatt_descriptors(self.characteristic());
            let max = ffi::bluetooth_characteristic_get_gatt_descriptors_size(self.characteristic()) as isize;
            for i in 0..max {
                let d_ptr = *descriptors.offset(i);
                let d = d_ptr as i32;
                v.push(d.to_string());
            }
            if max > 0 {
                ffi::jni_free_int_array(descriptors);
            }
        }
        Ok(v)
    }

    pub fn get_value(&self) -> Result<Vec<u8>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_characteristic_get_value(self.characteristic());
            let max = ffi::bluetooth_characteristic_get_value_size(self.characteristic()) as isize;
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                v.push(val);
            }
            if max > 0 {
                ffi::jni_free_int_array(values);
            }
        }
        Ok(v)
    }

    pub fn read_value(&self) -> Result<Vec<u8>, Box<Error>> {
        unsafe {
            if !ffi::bluetooth_characteristic_read_value(self.characteristic()).is_positive() {
                return Err(Box::from("Something went wrong..."));
            }
        }
        self.get_value()
    }

    pub fn write_value(&self, values: Vec<u8>) -> Result<(), Box<Error>> {
        let v = values.iter().map(|&x| x as i32).collect::<Vec<i32>>();
        unsafe  {
            if !ffi::bluetooth_characteristic_write_value(self.characteristic(), v.as_ptr() as *const c_int, v.len() as c_int).is_positive() {
                return Err(Box::from("Something went wrong..."));
            }
        }
        Ok(())
    }

    pub fn get_flags(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let flags = ffi::bluetooth_characteristic_get_flags(self.characteristic());
            let max = ffi::bluetooth_characteristic_get_flags_size(self.characteristic()) as isize;
            for i in 0..max {
                let f_ptr = *flags.offset(i);
                let f = match utils::c_str_to_slice(&f_ptr) {
                    None => continue,
                    Some(flag) => flag.to_owned(),
                };
                v.push(f.clone());
            }
            if max > 0 {
                ffi::jni_free_string_array(flags, max as i32);
            }
        }
        Ok(v)
    }

    pub fn is_notifying(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn start_notify(&self) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }

    pub fn stop_notify(&self) -> Result<(), Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Characteristic {
    fn clone(&self) -> Characteristic {
        unsafe { ffi::bluetooth_characteristic_inc_refcount(self.characteristic()) };
        Characteristic {
            i: self.i.clone(),
            id: self.id.clone(),
        }
    }
}

impl Drop for Characteristic {
    fn drop(&mut self) {
        unsafe {
            ffi::bluetooth_characteristic_dec_refcount(self.characteristic());
            ffi::bluetooth_characteristic_free_characteristic(self.characteristic());
        }
    }
}
