//TODO copyright
#ifndef BLUETOOTH_CHARACTERISTIC_H_
#define BLUETOOTH_CHARACTERISTIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "bluetooth-service.h"

typedef struct BluetoothCharacteristic BluetoothCharacteristic;

struct BluetoothCharacteristic
{
    jobject characteristic;
};

BluetoothCharacteristic* bluetooth_characteristic_create_characteristic (BluetoothService*, int);

const char* bluetooth_characteristic_get_uuid (BluetoothCharacteristic*);
const int* bluetooth_characteristic_get_gatt_descriptors (BluetoothCharacteristic*);
int bluetooth_characteristic_get_gatt_descriptors_size (BluetoothCharacteristic*);
const int* bluetooth_characteristic_get_value(BluetoothCharacteristic*);
const int bluetooth_characteristic_get_value_size(BluetoothCharacteristic*);
const int* bluetooth_characteristic_read_value(BluetoothCharacteristic*);
void bluetooth_characteristic_write_value(BluetoothCharacteristic*, const int*, int length);
void bluetooth_characteristic_free_characteristic (BluetoothCharacteristic*);
void bluetooth_characteristic_free_string (const char*);
void bluetooth_characteristic_free_int_array (int*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_CHARACTERISTIC_H_
