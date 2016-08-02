#include "bluetooth-characteristic.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothCharacteristic *
bluetooth_characteristic_create_characteristic (BluetoothService *service, int id)
{
    LOGI("bluetooth_characteristic_create_characteristic\n");
    BluetoothCharacteristic *characteristic = jni_calloc (sizeof (*characteristic));

    characteristic->characteristic = jni_service_create_characteristic(service->service, id);
    characteristic->count = 1;
    LOGI("create_characteristic c->c: %p\n", characteristic->characteristic);

    return characteristic;
}

const char*
bluetooth_characteristic_get_uuid (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_get_uuid c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_get_uuid (characteristic->characteristic);
}

const int*
bluetooth_characteristic_get_gatt_descriptors (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_get_gatt_descriptors c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_get_gatt_descriptors (characteristic->characteristic);
}

int
bluetooth_characteristic_get_gatt_descriptors_size (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_get_gatt_descriptors_size c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_get_gatt_descriptors_size (characteristic->characteristic);
}

const int*
bluetooth_characteristic_get_value (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_get_value c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_get_value (characteristic->characteristic);
}

const int
bluetooth_characteristic_get_value_size (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_get_value_size c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_get_value_size (characteristic->characteristic);
}

const int*
bluetooth_characteristic_read_value (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_read_value c->c: %p\n", characteristic->characteristic);
    return jni_characteristic_read_value (characteristic->characteristic);
}

// TODO bool
void
bluetooth_characteristic_write_value (BluetoothCharacteristic *characteristic, const int* values, int length)
{
    LOGI("bluetooth_characteristic_write_value c->c: %p\n", characteristic->characteristic);
    jni_characteristic_write_value (characteristic->characteristic, values, length);
}

void
bluetooth_characteristic_inc_refcount (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_inc_refcount c->c: %p\n", characteristic->characteristic);
    characteristic->count = characteristic->count + 1;
    LOGI("bluetooth_characteristic_inc_refcount c->rc: %d\n", characteristic->count);
}

void
bluetooth_characteristic_dec_refcount (BluetoothCharacteristic *characteristic)
{
    LOGI("bluetooth_characteristic_dec_refcount c->c: %p\n", characteristic->characteristic);
    characteristic->count = characteristic->count - 1;
    LOGI("bluetooth_characteristic_dec_refcount c->rc: %d\n", characteristic->count);
}

void
bluetooth_characteristic_free_characteristic (BluetoothCharacteristic *characteristic) {
    LOGI("bluetooth_characteristic_free_characteristic c->c: %p\n", characteristic->characteristic);
    LOGI("bluetooth_characteristic_free_characteristic c->rc: %d\n", characteristic->count);
    if (characteristic->count <= 0) {
        jni_free (characteristic);
        LOGI("characteristic free!");
    }
}

void
bluetooth_characteristic_free_string(const char* string) {
    LOGI("bluetooth_characteristic_free_string %s\n", string);
    jni_free ((char*)string);
}

void
bluetooth_characteristic_free_int_array (int* array)
{
    LOGI("bluetooth_characteristic_free_int_array\n");
    jni_free (array);
}
