#include "bluetooth-device.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothDevice *
bluetooth_device_create_device (BluetoothAdapter *adapter, const char* address)
{
    BluetoothDevice *device = jni_calloc (sizeof (*device));

    device->device = jni_create_object_str(adapter->adapter, g_ctx.adapter_get_remote_device, address);
    device->count = 1;

    return device;
}

const char*
bluetooth_device_get_address (BluetoothDevice *device)
{
    return jni_call_str (device->device, g_ctx.device_get_address);
}

const char*
bluetooth_device_get_name (BluetoothDevice *device)
{
    return jni_call_str (device->device, g_ctx.device_get_name);
}

const char**
bluetooth_device_get_uuids (BluetoothDevice *device)
{
    return jni_call_str_array (device->device, g_ctx.device_get_uuids, g_ctx.null);
}

int
bluetooth_device_get_uuids_size (BluetoothDevice *device)
{
    return jni_call_int (device->device, g_ctx.device_get_uuids_size);
}

int
bluetooth_device_get_rssi (BluetoothDevice *device)
{
    return jni_call_int (device->device, g_ctx.device_get_rssi);
}

int
bluetooth_device_get_tx_power (BluetoothDevice *device)
{
    return jni_call_int (device->device, g_ctx.device_get_tx_power);
}

void
bluetooth_device_connect_gatt (BluetoothDevice *device)
{
    jni_call_void (device->device, g_ctx.device_connect_gatt);
}

void
bluetooth_device_disconnect (BluetoothDevice *device)
{
    jni_call_void (device->device, g_ctx.device_disconnect);
}

int
bluetooth_device_is_connected (BluetoothDevice *device)
{
    return jni_call_bool (device->device, g_ctx.device_is_connected);
}

jobject get_gatt (BluetoothDevice *device) {
    return jni_call_object (device->device, g_ctx.device_get_gatt);
}

const int*
bluetooth_device_get_gatt_services (BluetoothDevice *device)
{
    return jni_call_int_array (get_gatt (device), g_ctx.gatt_get_gatt_services, g_ctx.service_get_id);
}

int
bluetooth_device_get_gatt_services_size (BluetoothDevice *device)
{
    return jni_call_int (get_gatt (device), g_ctx.gatt_get_gatt_services_size);
}

void
bluetooth_device_inc_refcount (BluetoothDevice *device)
{
    device->count = device->count + 1;
}

void
bluetooth_device_dec_refcount (BluetoothDevice *device)
{
    device->count = device->count - 1;
}

void
bluetooth_device_free_device (BluetoothDevice *device)
{
    if (device->count <= 0)
    {
        jni_free (device);
    }
}
