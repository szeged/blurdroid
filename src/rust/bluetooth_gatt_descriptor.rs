use ffi;
use std::cell::Cell;
use std::error::Error;
use std::os::raw::{c_int};
use std::ptr;
use utils;
use time;

use bluetooth_gatt_characteristic::Characteristic;

#[derive(Clone, Debug)]
struct IDescriptor {
    descriptor: Cell<*mut ffi::BluetoothDescriptor>,
}

#[derive(Debug)]
pub struct Descriptor {
    i: Box<IDescriptor>,
    id: i32,
}

impl Descriptor {
    #[inline(always)]
    pub fn descriptor(&self) -> *mut ffi::BluetoothDescriptor {
        self.i.descriptor.get()
    }

    pub fn new(characteristic: Characteristic, id: i32) -> Descriptor {
        println!("{} Descriptor new", time::precise_time_ns());
        let descriptor = unsafe { ffi::bluetooth_descriptor_create_descriptor(characteristic.characteristic(), id) };
        if descriptor == ptr::null_mut() {
            //return Err(Box::from("No characteristic found!"));
            println!("#### NO DESCRIPTOR FOUND!!!!! {:?}", descriptor);
        }
        println!("{} Descriptor new {:?}", time::precise_time_ns(), descriptor);
        let d = Descriptor {
            i: Box::new(IDescriptor { descriptor: Cell::new(descriptor) }),
            id: id,
        };
        d
    }

    pub fn get_id(&self) -> i32 {
        self.id
    }

    pub fn get_uuid(&self) -> Result<String, Box<Error>> {
        println!("{} Descriptor get_uuid {:?}", time::precise_time_ns(), self.descriptor());
        let uuid = unsafe { ffi::bluetooth_descriptor_get_uuid(self.descriptor()) };
        let res = match utils::c_str_to_slice(&uuid) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No uuid!")),
        };
        println!("{} Descriptor get_uuid {:?} {:?}", time::precise_time_ns(), uuid, res);
        unsafe { ffi::bluetooth_descriptor_free_string(uuid) };
        res
    }

    pub fn get_value(&self) -> Result<Vec<u8>, Box<Error>> {
        println!("{} Descriptor get_value {:?}", time::precise_time_ns(), self.descriptor());
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_descriptor_get_value(self.descriptor());
            println!("#### values PTR {:?} ####", values);
            let max = ffi::bluetooth_descriptor_get_value_size(self.descriptor()) as isize;
            println!("#### values LENGTH {:?} ####", max);
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                println!("#### values #{}: {:?} ####", i, val);
                v.push(val);
            }
            ffi::bluetooth_descriptor_free_int_array(values);
        }
        Ok(v)
    }

    pub fn read_value(&self) -> Result<Vec<u8>, Box<Error>> {
        println!("{} Descriptor read_value {:?}", time::precise_time_ns(), self.descriptor());
        let mut v = vec!();
        unsafe {
            let values = ffi::bluetooth_descriptor_read_value(self.descriptor());
            println!("#### values PTR {:?} ####", values);
            let max = ffi::bluetooth_descriptor_get_value_size(self.descriptor()) as isize;
            println!("#### values LENGTH {:?} ####", max);
            for i in 0..max {
                let val_ptr = *values.offset(i);
                let val = (val_ptr as i32) as u8;
                println!("#### values #{}: {:?} ####", i, val);
                v.push(val);
            }
            ffi::bluetooth_descriptor_free_int_array(values);
        }
        Ok(v)
    }

    pub fn write_value(&self, values: Vec<u8>) -> Result<(), Box<Error>> {
        println!("{} Descriptor write_value values: {:?} {:?}", time::precise_time_ns(), values, self.descriptor());
        unsafe  {
            ffi::bluetooth_descriptor_write_value(self.descriptor(), values.as_ptr() as *const c_int, values.len() as c_int)
        }
        Ok(())
    }
}

impl Clone for Descriptor {
    fn clone(&self) -> Descriptor {
        println!("{} Clone Descriptor {:?}", time::precise_time_ns(), self.descriptor());
        unsafe { ffi::bluetooth_descriptor_inc_refcount(self.descriptor()) };
        Descriptor {
            i: self.i.clone(),
            id: self.id,
        }
    }
}

impl Drop for Descriptor {
    fn drop(&mut self) {
        println!("{} Drop Descriptor {:?}", time::precise_time_ns(), self.descriptor());
        unsafe {
            ffi::bluetooth_descriptor_dec_refcount(self.descriptor());
            ffi::bluetooth_descriptor_free_descriptor(self.descriptor());
        }
    }
}
