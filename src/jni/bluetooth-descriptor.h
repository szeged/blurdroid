//TODO copyright
#ifndef BLUETOOTH_DESCRIPTOR_H_
#define BLUETOOTH_DESCRIPTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "bluetooth-characteristic.h"

typedef struct BluetoothDescriptor BluetoothDescriptor;

struct BluetoothDescriptor
{
    jobject descriptor;
};

BluetoothDescriptor* bluetooth_descriptor_create_descriptor (BluetoothCharacteristic*, int);

const char* bluetooth_descriptor_get_uuid (BluetoothDescriptor*);
void bluetooth_descriptor_free_descriptor (BluetoothDescriptor*);
void bluetooth_descriptor_free_string (const char*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_DESCRIPTOR_H_
