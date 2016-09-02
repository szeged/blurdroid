#ifndef BLUETOOTH_ADAPTER_H_
#define BLUETOOTH_ADAPTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

typedef struct BluetoothAdapter BluetoothAdapter;

struct BluetoothAdapter
{
    jobject adapter;
    int count;
};

BluetoothAdapter* bluetooth_adapter_get_adapter ();

const char* bluetooth_adapter_get_address (BluetoothAdapter*);
const char* bluetooth_adapter_get_name (BluetoothAdapter*);
int bluetooth_adapter_start_discovery (BluetoothAdapter *adapter);
int bluetooth_adapter_stop_discovery (BluetoothAdapter *adapter);
const char ** bluetooth_adapter_get_devices (BluetoothAdapter*);
int bluetooth_adapter_get_devices_size (BluetoothAdapter*);
void bluetooth_adapter_inc_refcount (BluetoothAdapter*);
void bluetooth_adapter_dec_refcount (BluetoothAdapter*);
void bluetooth_adapter_free_adapter (BluetoothAdapter*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_ADAPTER_H_
