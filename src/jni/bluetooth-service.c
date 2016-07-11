//TODO copyright
#include "bluetooth-service.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothService *
bluetooth_service_create_service (BluetoothDevice *device, int id)
{
    LOGI("bluetooth_service_create_service\n");
    BluetoothService *service = jni_calloc (sizeof (*device));

    service->service = jni_device_create_service(device->device, id);

    LOGI("s->s: %p\n", service->service);
    return service;
}

const char*
bluetooth_service_get_uuid (BluetoothService *service)
{
    LOGI("bluetooth_service_get_uuid\n");
    LOGI("s->s: %p\n", service->service);
    return jni_service_get_uuid (service->service);
}

int
bluetooth_service_is_primary (BluetoothService *service)
{
    LOGI("bluetooth_service_is_primary\n");
    LOGI("s->s: %p\n", service->service);
    return jni_service_is_primary (service->service);
}

const int*
bluetooth_service_get_gatt_characteristics (BluetoothService *service)
{
    LOGI("bluetooth_service_get_gatt_characteristics\n");
    LOGI("s->s: %p\n", service->service);
    return jni_service_get_gatt_characteristics (service->service);
}

int
bluetooth_service_get_gatt_characteristics_size (BluetoothService *service)
{
    LOGI("bluetooth_service_get_gatt_characteristics_size\n");
    LOGI("s->s: %p\n", service->service);
    return jni_service_get_gatt_characteristics_size (service->service);
}

void
bluetooth_service_free_service (BluetoothService *service) {
    LOGI("bluetooth_service_free_service\n");
    LOGI("s->s: %p\n", service->service);
    jni_free (service);
}

void
bluetooth_service_free_string(const char* string) {
    LOGI("bluetooth_service_free_string %s\n", string);
    jni_free ((char*)string);
}

void
bluetooth_service_free_int_array (int* array)
{
    LOGI("bluetooth_service_free_int_array\n");
    jni_free (array);
}
