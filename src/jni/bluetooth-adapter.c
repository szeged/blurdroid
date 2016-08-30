#include "bluetooth-adapter.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothAdapter *
bluetooth_adapter_get_adapter ()
{
    BluetoothAdapter *adapter = jni_calloc (sizeof (*adapter));

    adapter->adapter = jni_create_static_object (g_ctx.adapter_cls, g_ctx.adapter_get_adapter);
    adapter->count = 1;
    return adapter;
}

const char*
bluetooth_adapter_get_address (BluetoothAdapter *adapter)
{
    return jni_call_str (adapter->adapter, g_ctx.adapter_get_address);
}

const char*
bluetooth_adapter_get_name (BluetoothAdapter *adapter)
{
    return jni_call_str (adapter->adapter, g_ctx.adapter_get_name);
}

void
bluetooth_adapter_start_discovery (BluetoothAdapter *adapter)
{
    jni_call_void (adapter->adapter, g_ctx.adapter_start_le_scan);
}

void
bluetooth_adapter_stop_discovery (BluetoothAdapter *adapter)
{
    jni_call_void (adapter->adapter, g_ctx.adapter_stop_le_scan);
}

const char **
bluetooth_adapter_get_devices (BluetoothAdapter *adapter)
{
    return jni_call_str_array (adapter->adapter, g_ctx.adapter_get_devices, g_ctx.device_get_address);
}

int
bluetooth_adapter_get_devices_size (BluetoothAdapter *adapter)
{
    return jni_call_int (adapter->adapter, g_ctx.adapter_get_devices_size);
}

void
bluetooth_adapter_inc_refcount (BluetoothAdapter *adapter)
{
    adapter->count = adapter->count + 1;
}

void
bluetooth_adapter_dec_refcount (BluetoothAdapter *adapter)
{
    adapter->count = adapter->count - 1;
}

void
bluetooth_adapter_free_adapter (BluetoothAdapter *adapter)
{
    if (adapter->count <= 0)
    {
        jni_delete_object(adapter->adapter);
        jni_free (adapter);
    }
}
