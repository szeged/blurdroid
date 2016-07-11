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

    LOGI("d->d: %d\n", device->device);
    return device;
}

const char*
bluetooth_device_get_address (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_address\n");
    LOGI("d->d: %d\n", device->device);
    return jni_device_get_address (device->device);
}

const char*
bluetooth_device_get_name (BluetoothDevice *device)
{
    LOGI("bluetooth_device_get_name\n");
    LOGI("d->d: %d\n", device->device);
    return jni_device_get_name (device->device);
}

void
bluetooth_device_connect_gatt (BluetoothDevice *device)
{
    LOGI("bluetooth_device_connect_gatt\n");
    LOGI("d->d: %d\n", device->device);
    jni_device_connect_gatt (device->device);
}

int
bluetooth_device_is_connected (BluetoothDevice *device)
{
    LOGI("bluetooth_device_is_connected\n");
    LOGI("d->d: %d\n", device->device);
    return jni_device_is_connected (device->device);
}


void
bluetooth_device_free_device (BluetoothDevice *device) {
    LOGI("bluetooth_device_free_device\n");
    LOGI("d->d: %d\n", device->device);
    jni_free (device);
}

void
bluetooth_device_free_string(const char* string) {
    LOGI("bluetooth_device_free_string %s\n", string);
    jni_free ((char*)string);
}
