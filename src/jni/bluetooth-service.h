#ifndef BLUETOOTH_SERVICE_H_
#define BLUETOOTH_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>
#include "bluetooth-device.h"

typedef struct BluetoothService BluetoothService;

struct BluetoothService
{
    jobject service;
    int count;
};

BluetoothService* bluetooth_service_create_service (BluetoothDevice*, int);

const char* bluetooth_service_get_uuid (BluetoothService*);
int bluetooth_service_is_primary (BluetoothService*);
const int* bluetooth_service_get_gatt_characteristics (BluetoothService*);
int bluetooth_service_get_gatt_characteristics_size (BluetoothService*);
void bluetooth_service_inc_refcount (BluetoothService*);
void bluetooth_service_dec_refcount (BluetoothService*);
void bluetooth_service_free_service (BluetoothService*);
void bluetooth_service_free_string (const char*);
void bluetooth_service_free_int_array (int*);

#ifdef __cplusplus
}; /* extern "C" */
#endif

#endif  // BLUETOOTH_SERVICE_H_
