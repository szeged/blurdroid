//TODO copyright
#include "bluetooth-adapter.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothAdapter *
bluetooth_adapter_get_adapter ()
{
    LOGI("bluetooth_adapter_get_adapter\n");
    BluetoothAdapter *adapter = jni_calloc (sizeof (*adapter));

    adapter->adapter = jni_adapter_create_adapter();
    adapter->count = 1;
    LOGI("get_adapter d->d: %p\n", adapter->adapter);
    return adapter;
}

const char*
bluetooth_adapter_get_address (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_address a->a: %p\n", adapter->adapter);
    return jni_adapter_get_address (adapter->adapter);
}

const char*
bluetooth_adapter_get_name (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_name a->a: %p\n", adapter->adapter);
    return jni_adapter_get_name (adapter->adapter);
}

void
bluetooth_adapter_start_discovery (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_start_discovery a->a: %p\n", adapter->adapter);
    jni_adapter_start_discovery (adapter->adapter);
}

void
bluetooth_adapter_stop_discovery (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_stop_discovery a->a: %p\n", adapter->adapter);
    jni_adapter_stop_discovery (adapter->adapter);
}

const char **
bluetooth_adapter_get_devices(BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_devices a->a: %p\n", adapter->adapter);
    return jni_adapter_get_devices (adapter->adapter);
}

int
bluetooth_adapter_get_devices_size(BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_devices_size a->a: %p\n", adapter->adapter);
    return jni_adapter_get_devices_size (adapter->adapter);
}

void
bluetooth_adapter_inc_refcount (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_devices_size a->a: %p\n", adapter->adapter);
    adapter->count = adapter->count + 1;
    LOGI("bluetooth_adapter_inc_refcount a->rc: %d\n", adapter->count);
}

void
bluetooth_adapter_dec_refcount (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_dec_refcount a->a: %p\n", adapter->adapter);
    adapter->count = adapter->count - 1;
    LOGI("bluetooth_adapter_dec_refcount a->rc: %d\n", adapter->count);
}

void
bluetooth_adapter_free_adapter (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_free_adapter a->a: %p\n", adapter->adapter);
    LOGI("bluetooth_adapter_free_adapter a->rc: %d\n", adapter->count);
    if (adapter->count <= 0) {
        jni_free (adapter);
        LOGI("adapter free!");
    }
}

void
bluetooth_adapter_free_string (const char* string)
{
    LOGI("bluetooth_adapter_free_string %s\n", string);
    jni_free ((char*)string);
}

void
bluetooth_adapter_free_string_array (const char** array)
{
    LOGI("bluetooth_adapter_free_string_array\n");
    jni_free_string_array ((char**)array);
}
