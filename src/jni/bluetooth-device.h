//TODO copyright
#ifndef BLUETOOTH_DEVICE_H_
#define BLUETOOTH_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "bluetooth-adapter.h"

typedef struct BluetoothDevice BluetoothDevice;

struct BluetoothDevice
{
    jobject device;
};

BluetoothDevice* bluetooth_device_create_device (BluetoothAdapter*, const char*);

const char* bluetooth_device_get_address (BluetoothDevice*);
const char* bluetooth_device_get_name (BluetoothDevice*);

void bluetooth_device_free_device (BluetoothDevice*);
void bluetooth_device_free_string (const char*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_DEVICE_H_
