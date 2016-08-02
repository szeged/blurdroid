use bluetooth_gatt_characteristic::Characteristic;
use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_int};
use std::sync::Arc;
use utils;

#[derive(Clone, Debug)]
struct IDescriptor {
    descriptor: Cell<*mut ffi::BluetoothDescriptor>,
}

#[derive(Debug)]
pub struct Descriptor {
    i: Box<IDescriptor>,
    id: String,
}

impl Descriptor {
    #[inline(always)]
    pub fn descriptor(&self) -> *mut ffi::BluetoothDescriptor {
        self.i.descriptor.get()
    }

    pub fn new(characteristic: Arc<Characteristic>, id: String) -> Descriptor {
        let descriptor = unsafe { ffi::bluetooth_descriptor_create_descriptor(characteristic.characteristic(), id.parse::<i32>().unwrap()) };
        Descriptor {
            i: Box::new(IDescriptor { descriptor: Cell::new(descriptor) }),
            id: id.clone(),
        }
    }

    pub fn get_id(&self) -> String {
        self.id.clone()
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        let uuid = unsafe { ffi::bluetooth_descriptor_get_uuid(self.descriptor()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        unsafe { ffi::bluetooth_descriptor_free_string(uuid) };
        res
    }

    pub fn get_value(&self) -> Result<Vec<u8>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_descriptor_get_value(self.descriptor());
            let max = ffi::bluetooth_descriptor_get_value_size(self.descriptor()) as isize;
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                v.push(val);
            }
            ffi::bluetooth_descriptor_free_int_array(values);
        }
        Ok(v)
    }

    pub fn read_value(&self) -> Result<Vec<u8>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_descriptor_read_value(self.descriptor());
            let max = ffi::bluetooth_descriptor_get_value_size(self.descriptor()) as isize;
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                v.push(val);
            }
            ffi::bluetooth_descriptor_free_int_array(values);
        }
        Ok(v)
    }

    pub fn write_value(&self, values: Vec<u8>) -> Result<(), Box<Error>> {
        let v = values.iter().map(|&x| x as i32).collect::<Vec<i32>>();
        unsafe  {
            ffi::bluetooth_descriptor_write_value(self.descriptor(), v.as_ptr() as *const c_int, v.len() as c_int)
        }
        Ok(())
    }

    pub fn get_flags(&self) -> Result<Vec<String>, Box<Error>> {
        Err(Box::from(utils::NOT_SUPPORTED_ERROR))
    }
}

impl Clone for Descriptor {
    fn clone(&self) -> Descriptor {
        unsafe { ffi::bluetooth_descriptor_inc_refcount(self.descriptor()) };
        Descriptor {
            i: self.i.clone(),
            id: self.id.clone(),
        }
    }
}

impl Drop for Descriptor {
    fn drop(&mut self) {
        unsafe {
            ffi::bluetooth_descriptor_dec_refcount(self.descriptor());
            ffi::bluetooth_descriptor_free_descriptor(self.descriptor());
        }
    }
}
