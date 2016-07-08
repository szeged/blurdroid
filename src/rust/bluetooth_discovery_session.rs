use std::error::Error;
use ffi;
use bluetooth_adapter::Adapter;

#[derive(Debug)]
pub struct DiscoverySession {
    adapter: Adapter,
}

impl DiscoverySession {
    fn new(adapter: Adapter) -> DiscoverySession {
        DiscoverySession {
            adapter: adapter,
        }
    }

    pub fn create_session(adapter: Adapter) -> Result<DiscoverySession, Box<Error>> {
        Ok(DiscoverySession::new(adapter))
    }

    pub fn start_discovery(&self) -> Result<(), Box<Error>> {
        unsafe {
            ffi::bluetooth_adapter_start_discovery(self.adapter.adapter());
        }
        Ok(())
    }

    pub fn stop_discovery(&self) -> Result<(), Box<Error>> {
        unsafe {
            ffi::bluetooth_adapter_stop_discovery(self.adapter.adapter());
        }
        Ok(())
    }
}
