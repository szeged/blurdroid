use ffi;
use std::cell::Cell;
use std::error::Error;
use std::ptr;
use utils;
use time;

use bluetooth_gatt_service::Service;

#[derive(Clone, Debug)]
struct ICharacteristic {
    characteristic: Cell<*mut ffi::BluetoothCharacteristic>,
}

#[derive(Clone, Debug)]
pub struct Characteristic {
    i: Box<ICharacteristic>,
    id: i32,
}

impl Characteristic {
    #[inline(always)]
    pub fn characteristic(&self) -> *mut ffi::BluetoothCharacteristic {
        self.i.characteristic.get()
    }

    pub fn new(service: Service, id: i32) -> Characteristic {
        println!("{} Characteristic new", time::precise_time_ns());
        let characteristic = unsafe { ffi::bluetooth_characteristic_create_characteristic(service.service(), id) };
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

    pub fn get_id(&self) -> i32 {
        self.id
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

    pub fn get_gatt_descriptors(&mut self) -> Result<Vec<i32>, Box<Error>> {
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
                v.push(d);
            }
            ffi::bluetooth_characteristic_free_int_array(descriptors);
        }
        Ok(v)
    }
}

impl Drop for Characteristic {
    fn drop(&mut self) {
        println!("{} Drop Characteristic {:?}", time::precise_time_ns(), self.characteristic());
        /*unsafe {
            ffi::bluetooth_characteristic_free_characteristic(self.characteristic());
        }*/
    }
}
