#include "bluetooth-characteristic.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothCharacteristic *
bluetooth_characteristic_create_characteristic (BluetoothService *service, int id)
{
    BluetoothCharacteristic *characteristic = jni_calloc (sizeof (*characteristic));

    characteristic->characteristic = jni_create_object_int (service->service, g_ctx.service_get_gatt_characteristic, id);
    characteristic->count = 1;

    return characteristic;
}

const char*
bluetooth_characteristic_get_uuid (BluetoothCharacteristic *characteristic)
{
    return jni_call_str (characteristic->characteristic, g_ctx.characteristic_get_uuid);
}

const int*
bluetooth_characteristic_get_gatt_descriptors (BluetoothCharacteristic *characteristic)
{
    return jni_call_int_array (characteristic->characteristic, g_ctx.characteristic_get_gatt_descriptors, g_ctx.descriptor_get_id);
}

int
bluetooth_characteristic_get_gatt_descriptors_size (BluetoothCharacteristic *characteristic)
{
    return jni_call_int (characteristic->characteristic, g_ctx.characteristic_get_gatt_descriptors_size);
}

const int*
bluetooth_characteristic_get_value (BluetoothCharacteristic *characteristic)
{
    return jni_get_value (characteristic->characteristic, g_ctx.characteristic_get_value);
}

const int
bluetooth_characteristic_get_value_size (BluetoothCharacteristic *characteristic)
{
    return jni_call_int (characteristic->characteristic, g_ctx.characteristic_get_value_size);
}

const int*
bluetooth_characteristic_read_value (BluetoothCharacteristic *characteristic)
{
    return jni_get_value (characteristic->characteristic, g_ctx.characteristic_read_value);
}

void
bluetooth_characteristic_write_value (BluetoothCharacteristic *characteristic, const int* values, int length)
{
    jni_set_value (characteristic->characteristic, g_ctx.characteristic_write_value, values, length);
}

void
bluetooth_characteristic_inc_refcount (BluetoothCharacteristic *characteristic)
{
    characteristic->count = characteristic->count + 1;
}

void
bluetooth_characteristic_dec_refcount (BluetoothCharacteristic *characteristic)
{
    characteristic->count = characteristic->count - 1;
}

void
bluetooth_characteristic_free_characteristic (BluetoothCharacteristic *characteristic) {
    if (characteristic->count <= 0)
    {
        jni_free (characteristic);
    }
}
