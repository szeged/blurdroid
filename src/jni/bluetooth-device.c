//TODO copyright
#include "bluetooth-device.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothDevice *
bluetooth_device_create_device (BluetoothAdapter *adapter, const char* address)
{
    LOGI("bluetooth_device_create_device\n");
    BluetoothDevice *device = jni_calloc (sizeof (*device));

    device->device = jni_adapter_create_device(adapter->adapter, address);
    device->count = 1;
    LOGI("create_device d->d: %p\n", device->device);
    return device;
}

const char*
bluetooth_device_get_address (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_address d->d: %p\n", device->device);
    return jni_device_get_address (device->device);
}

const char*
bluetooth_device_get_name (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_name d->d: %p\n", device->device);
    return jni_device_get_name (device->device);
}

void
bluetooth_device_connect_gatt (BluetoothDevice *device)
{
    LOGI("bluetooth_device_connect_gatt d->d: %p\n", device->device);
    jni_device_connect_gatt (device->device);
}

void
bluetooth_device_disconnect (BluetoothDevice *device)
{
    LOGI("bluetooth_device_disconnect d->d: %p\n", device->device);
    jni_device_disconnect (device->device);
}

int
bluetooth_device_is_connected (BluetoothDevice *device)
{
    LOGI("bluetooth_device_is_connected d->d: %p\n", device->device);
    return jni_device_is_connected (device->device);
}

const int*
bluetooth_device_get_gatt_services (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_gatt_services d->d: %p\n", device->device);
    return jni_device_get_gatt_services (device->device);
}

int
bluetooth_device_get_gatt_services_size (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_gatt_services_size d->d: %p\n", device->device);
    return jni_device_get_gatt_services_size (device->device);
}

void
bluetooth_device_inc_refcount (BluetoothDevice *device)
{
    LOGI("bluetooth_device_inc_refcount d->d: %p\n", device->device);
    device->count = device->count + 1;
    LOGI("bluetooth_device_inc_refcount d->rc: %d\n", device->count);
}

void
bluetooth_device_dec_refcount (BluetoothDevice *device)
{
    LOGI("bluetooth_device_dec_refcount d->d: %p\n", device->device);
    device->count = device->count - 1;
    LOGI("bluetooth_device_dec_refcount d->rc: %d\n", device->count);
}

void
bluetooth_device_free_device (BluetoothDevice *device)
{
    LOGI("bluetooth_device_free_device d->d: %p\n", device->device);
    LOGI("bluetooth_device_free_device d->rc: %d\n", device->count);
    if (device->count <= 0) {
        jni_free (device);
        LOGI("device free!");
    }
}

void
bluetooth_device_free_string(const char* string)
{
    LOGI("bluetooth_device_free_string %s\n", string);
    jni_free ((char*)string);
}

void
bluetooth_device_free_int_array (int* array)
{
    LOGI("bluetooth_adapter_free_int_array\n");
    jni_free (array);
}
