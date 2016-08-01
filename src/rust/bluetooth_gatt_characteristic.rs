use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_int};
use std::ptr;
use std::sync::Arc;
use utils;
use time;

use bluetooth_gatt_service::Service;

const NOT_SUPPORTED_ERROR: &'static str = "Error! Not supported platform!";

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
        println!("{} Characteristic new", time::precise_time_ns());
        let characteristic = unsafe { ffi::bluetooth_characteristic_create_characteristic(service.service(), id.parse::<i32>().unwrap()) };
        if characteristic == ptr::null_mut() {
            //return Err(Box::from("No service found!"));
            println!("#### NO SERVICE FOUND!!!!! {:?}", characteristic);
        }
        println!("{} Characteristic new {:?}", time::precise_time_ns(), characteristic);
        let c = Characteristic {
            i: Box::new(ICharacteristic { characteristic: Cell::new(characteristic) }),
            id: id,
        };
        c
    }

    pub fn get_id(&self) -> String {
        self.id.clone()
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        println!("{} Characteristic get_uuid {:?}", time::precise_time_ns(), self.characteristic());
        let uuid = unsafe { ffi::bluetooth_characteristic_get_uuid(self.characteristic()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        println!("{} Characteristic get_uuid {:?} {:?}", time::precise_time_ns(), uuid, res);
        unsafe { ffi::bluetooth_characteristic_free_string(uuid) };
        res
    }

    pub fn get_gatt_descriptors(&self) -> Result<Vec<String>, Box<Error>> {
        println!("####GET_GATT_DESCRIPTORS####");
        let mut v = vec!();
        unsafe {
            let descriptors = ffi::bluetooth_characteristic_get_gatt_descriptors(self.characteristic());
            println!("#### DESCRIPTOR PTR {:?} ####", descriptors);
            let max = ffi::bluetooth_characteristic_get_gatt_descriptors_size(self.characteristic()) as isize;
            println!("#### DESCRIPTOR LENGTH {:?} ####", max);
            for i in 0..max {
                let d_ptr = *descriptors.offset(i);
                let d = d_ptr as i32;
                println!("#### DESCRIPTOR #{}: {:?} ####", i, d);
                v.push(d.to_string());
            }
            ffi::bluetooth_characteristic_free_int_array(descriptors);
        }
        Ok(v)
    }

    pub fn get_value(&self) -> Result<Vec<u8>, Box<Error>> {
        println!("{} Characteristic get_value {:?}", time::precise_time_ns(), self.characteristic());
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_characteristic_get_value(self.characteristic());
            println!("#### values PTR {:?} ####", values);
            let max = ffi::bluetooth_characteristic_get_value_size(self.characteristic()) as isize;
            println!("#### values LENGTH {:?} ####", max);
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                println!("#### values #{}: {:?} ####", i, val);
                v.push(val);
            }
            ffi::bluetooth_characteristic_free_int_array(values);
        }
        Ok(v)
    }

    pub fn read_value(&self) -> Result<Vec<u8>, Box<Error>> {
        println!("{} Characteristic read_value {:?}", time::precise_time_ns(), self.characteristic());
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_characteristic_read_value(self.characteristic());
            println!("#### values PTR {:?} ####", values);
            let max = ffi::bluetooth_characteristic_get_value_size(self.characteristic()) as isize;
            println!("#### values LENGTH {:?} ####", max);
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                println!("#### values #{}: {:?} ####", i, val);
                v.push(val);
            }
            ffi::bluetooth_characteristic_free_int_array(values);
        }
        Ok(v)
    }

    pub fn write_value(&self, values: Vec<u8>) -> Result<(), Box<Error>> {
        println!("{} Characteristic write_value values: {:?} {:?}", time::precise_time_ns(), values, self.characteristic());
        let v = values.iter().map(|&x| x as i32).collect::<Vec<i32>>();
        unsafe  {
            ffi::bluetooth_characteristic_write_value(self.characteristic(), v.as_ptr() as *const c_int, v.len() as c_int)
        }
        Ok(())
    }

    pub fn is_notifying(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_flags(&self) -> Result<Vec<String>, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn start_notify(&self) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn stop_notify(&self) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Characteristic {
    fn clone(&self) -> Characteristic {
        println!("{} Clone Characteristic {:?}", time::precise_time_ns(), self.characteristic());
        unsafe { ffi::bluetooth_characteristic_inc_refcount(self.characteristic()) };
        Characteristic {
            i: self.i.clone(),
            id: self.id.clone(),
        }
    }
}

impl Drop for Characteristic {
    fn drop(&mut self) {
        println!("{} Drop Characteristic {:?}", time::precise_time_ns(), self.characteristic());
        unsafe {
            ffi::bluetooth_characteristic_dec_refcount(self.characteristic());
            ffi::bluetooth_characteristic_free_characteristic(self.characteristic());
        }
    }
}
