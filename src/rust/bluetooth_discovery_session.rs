use bluetooth_adapter::Adapter;
use ffi;
use std::error::Error;
use std::sync::Arc;

#[derive(Debug)]
pub struct DiscoverySession {
    adapter: Arc<Adapter>,
}

impl DiscoverySession {
    fn new(adapter: Arc<Adapter>) -> DiscoverySession {
        DiscoverySession {
            adapter: adapter,
        }
    }

    pub fn create_session(adapter: Arc<Adapter>) -> Result<DiscoverySession, Box<Error>> {
        Ok(DiscoverySession::new(adapter.clone()))
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
