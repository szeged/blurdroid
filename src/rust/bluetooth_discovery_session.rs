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
        let discovery = unsafe { ffi::bluetooth_adapter_start_discovery(self.adapter.adapter()) };
        if !discovery.is_positive() {
            return Err(Box::from("Discovery error!"));
        }
        Ok(())
    }

    pub fn stop_discovery(&self) -> Result<(), Box<Error>> {
        let discovery = unsafe { ffi::bluetooth_adapter_stop_discovery(self.adapter.adapter()) };
        if !discovery.is_positive() {
            return Err(Box::from("Discovery error!"));
        }
        Ok(())
    }
}
