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
    int count;
};

BluetoothDescriptor* bluetooth_descriptor_create_descriptor (BluetoothCharacteristic*, int);

const char* bluetooth_descriptor_get_uuid (BluetoothDescriptor*);
const int* bluetooth_descriptor_get_value (BluetoothDescriptor*);
const int bluetooth_descriptor_get_value_size (BluetoothDescriptor*);
const int* bluetooth_descriptor_read_value (BluetoothDescriptor*);
void bluetooth_descriptor_write_value (BluetoothDescriptor*, const int*, int);
void bluetooth_descriptor_inc_refcount (BluetoothDescriptor*);
void bluetooth_descriptor_dec_refcount (BluetoothDescriptor*);
void bluetooth_descriptor_free_descriptor (BluetoothDescriptor*);
void bluetooth_descriptor_free_string (const char*);
void bluetooth_descriptor_free_int_array (int*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_DESCRIPTOR_H_
