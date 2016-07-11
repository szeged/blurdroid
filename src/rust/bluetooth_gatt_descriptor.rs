use ffi;
use std::cell::Cell;
use std::error::Error;
use std::ptr;
use utils;
use time;

use bluetooth_gatt_characteristic::Characteristic;

#[derive(Clone, Debug)]
struct IDescriptor {
    descriptor: Cell<*mut ffi::BluetoothDescriptor>,
}

#[derive(Clone, Debug)]
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
}

impl Drop for Descriptor {
    fn drop(&mut self) {
        println!("{} Drop Descriptor {:?}", time::precise_time_ns(), self.descriptor());
        /*unsafe {
            ffi::bluetooth_descriptor_free_descriptor(self.descriptor());
        }*/
    }
}
