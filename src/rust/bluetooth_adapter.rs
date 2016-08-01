use ffi;
use std::cell::Cell;
use std::error::Error;
use std::ptr;
use utils;
use time;
//use std::collections::HashMap;

const NOT_SUPPORTED_ERROR: &'static str = "Error! Not supported platform!";

#[derive(Clone, Debug)]
struct IAdapter {
    adapter: Cell<*mut ffi::BluetoothAdapter>,
}

#[derive(Debug)]
pub struct Adapter {
    i: Box<IAdapter>,
    //devices: Option<HashMap<Device>>,
}

unsafe impl Send for Adapter {}
unsafe impl Sync for Adapter { }

impl Adapter {
    #[inline(always)]
    pub fn adapter(&self) -> *mut ffi::BluetoothAdapter {
        self.i.adapter.get()
    }

    pub fn get_adapter() -> Result<Adapter, Box<Error>> {
        println!("{} Adapter get_adapter", time::precise_time_ns());
        let adapter = unsafe { ffi::bluetooth_adapter_get_adapter() };
        if adapter == ptr::null_mut() {
            return Err(Box::from("No adapter found!"));
        }
        println!("{} Adapter get_adapter {:?}", time::precise_time_ns(), adapter);
        let a = Adapter { i: Box::new(IAdapter {
            adapter: Cell::new(adapter),
            //devices: None,
        })};
        Ok(a)
    }

    pub fn get_id(&self) -> String {
        "Adapter".to_string()
    }

    pub fn get_address(&self) -> Result<String, Box<Error>> {
        println!("{} Adapter get_address {:?}", time::precise_time_ns(), self.adapter());
        let address = unsafe { ffi::bluetooth_adapter_get_address(self.adapter()) };
        let res = match utils::c_str_to_slice(&address) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No address!")),
        };
        println!("{} Adapter get_address {:?} {:?}", time::precise_time_ns(), address, res);
        unsafe { ffi::bluetooth_adapter_free_string(address) };
        res
    }

    pub fn get_name(&self) -> Result<String, Box<Error>> {
        println!("{} Adapter get_name {:?}", time::precise_time_ns(), self.adapter());
        let name = unsafe { ffi::bluetooth_adapter_get_name(self.adapter()) };
        let res = match utils::c_str_to_slice(&name) {
            Some(a) => Ok(a.to_string()),
            None => Err(Box::from("No name!")),
        };
        println!("{} Adapter get_name {:?} {:?}", time::precise_time_ns(), name, res);
        unsafe { ffi::bluetooth_adapter_free_string(name) };
        res
    }

    pub fn get_device_list(&self) -> Result<Vec<String>, Box<Error>> {
        let mut v = vec!();
        unsafe {
            println!("####GET_DEVICES####");
            let devices = ffi::bluetooth_adapter_get_devices(self.adapter());
            println!("#### DEVICES PTR {:?} ####", devices);
            let max = ffi::bluetooth_adapter_get_devices_size(self.adapter()) as isize;
            //(0..max).map(|i| {
            //CStr::from_ptr(*devices.offset(i)).to_bytes().to_vec()
            //}).collect()
            println!("#### DEVICES LENGTH {:?} ####", max);
            for i in 0..max {
                println!("#### dev #{} ####", i);
                let d_ptr = *devices.offset(i);
                println!("#### dev offset {:?} ####", d_ptr);
                let d = match utils::c_str_to_slice(&d_ptr) {
                    None => continue,
                    Some(dev) => dev.to_owned(),
                };
                println!("#### DEVICE #{}: {:?} ####", i, d);
                v.push(d.clone());
            }
            ffi::bluetooth_adapter_free_string_array(devices);
        }
        Ok(v)
    }

    pub fn get_alias(&self) -> Result<String, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_alias(&self, _value: String) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_class(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_powered(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_powered(&self, _value: bool) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_discoverable(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_discoverable(&self, _value: bool) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_pairable(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_pairable(&self, _value: bool) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_pairable_timeout(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_pairable_timeout(&self, _value: u32) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_discoverable_timeout(&self) -> Result<u32, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn set_discoverable_timeout(&self, _value: u32) -> Result<(), Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn is_discovering(&self) -> Result<bool, Box<Error>> {
        Err(Box::from(NOT_SUPPORTED_ERROR))
    }

    pub fn get_uuids(&self) -> Result<Vec<String>, Box<Error>> {
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
}

impl Clone for Adapter {
    fn clone(&self) -> Adapter {
        println!("{} Clone Adapter {:?}", time::precise_time_ns(), self.adapter());
        unsafe { ffi::bluetooth_adapter_inc_refcount(self.adapter()) };
        Adapter {
            i: self.i.clone(),
        }
    }
}

impl Drop for Adapter {
    fn drop(&mut self) {
        println!("{} Drop Adapter {:?}", time::precise_time_ns(), self.adapter());
        unsafe {
            ffi::bluetooth_adapter_dec_refcount(self.adapter());
            ffi::bluetooth_adapter_free_adapter(self.adapter());
        }
    }
}
