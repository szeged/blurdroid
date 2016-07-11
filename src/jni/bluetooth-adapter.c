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

    LOGI("d->d: %p\n", adapter->adapter);
    return adapter;
}

const char*
bluetooth_adapter_get_address (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_address\n");
    LOGI("a->a: %p\n", adapter->adapter);
    return jni_adapter_get_address (adapter->adapter);
}

const char*
bluetooth_adapter_get_name (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_name\n");
    LOGI("a->a: %p\n", adapter->adapter);
    return jni_adapter_get_name (adapter->adapter);
}

void
bluetooth_adapter_start_discovery (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_start_discovery\n");
    LOGI("a->a: %p\n", adapter->adapter);
    jni_adapter_start_discovery (adapter->adapter);
}

void
bluetooth_adapter_stop_discovery (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_stop_discovery\n");
    LOGI("a->a: %p\n", adapter->adapter);
    jni_adapter_stop_discovery (adapter->adapter);
}

const char **
bluetooth_adapter_get_devices(BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_devices\n");
    LOGI("a->a: %p\n", adapter->adapter);
    return jni_adapter_get_devices (adapter->adapter);
}

int
bluetooth_adapter_get_devices_size(BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_get_devices_size\n");
    LOGI("a->a: %p\n", adapter->adapter);
    return jni_adapter_get_devices_size (adapter->adapter);
}

void
bluetooth_adapter_free_adapter (BluetoothAdapter *adapter)
{
    LOGI("bluetooth_adapter_free_adapter\n");
    LOGI("a->a: %p\n", adapter->adapter);
    jni_free (adapter);
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
