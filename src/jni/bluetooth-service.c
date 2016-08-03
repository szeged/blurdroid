#include "bluetooth-service.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothService *
bluetooth_service_create_service (BluetoothDevice *device, int id)
{
    BluetoothService *service = jni_calloc (sizeof (*device));

    jobject gatt = jni_call_object (device->device, g_ctx.device_get_gatt);
    service->service = jni_create_object_int(gatt, g_ctx.gatt_get_gatt_service, id);
    service->count = 1;

    return service;
}

const char*
bluetooth_service_get_uuid (BluetoothService *service)
{
    return jni_call_str (service->service, g_ctx.service_get_uuid);
}

int
bluetooth_service_is_primary (BluetoothService *service)
{
    return jni_call_bool (service->service, g_ctx.service_is_primary);
}

const int*
bluetooth_service_get_gatt_characteristics (BluetoothService *service)
{
    return jni_call_int_array (service->service, g_ctx.service_get_gatt_characteristics, g_ctx.characteristic_get_id);
}

int
bluetooth_service_get_gatt_characteristics_size (BluetoothService *service)
{
    return jni_call_int (service->service, g_ctx.service_get_gatt_characteristics_size);
}

void
bluetooth_service_inc_refcount (BluetoothService *service)
{
    service->count = service->count + 1;
}

void
bluetooth_service_dec_refcount (BluetoothService *service)
{
    service->count = service->count - 1;
}

void
bluetooth_service_free_service (BluetoothService *service) {
    if (service->count <= 0) {
        jni_free (service);
    }
}
