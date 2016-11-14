#include "bluetooth-descriptor.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothDescriptor *
bluetooth_descriptor_create_descriptor (BluetoothCharacteristic *characteristic, int id)
{
    BluetoothDescriptor *descriptor = jni_calloc (sizeof (*descriptor));

    descriptor->descriptor = jni_create_object_int (characteristic->characteristic, g_ctx.characteristic_get_gatt_descriptor, id);
    descriptor->count = 1;

    return descriptor;
}

const char*
bluetooth_descriptor_get_uuid (BluetoothDescriptor *descriptor)
{
    return jni_call_str (descriptor->descriptor, g_ctx.descriptor_get_uuid);
}

const int*
bluetooth_descriptor_get_value (BluetoothDescriptor *descriptor)
{
    return jni_get_value (descriptor->descriptor, g_ctx.descriptor_get_value, -1);
}

const int
bluetooth_descriptor_get_value_size (BluetoothDescriptor *descriptor)
{
    return jni_call_int (descriptor->descriptor, g_ctx.descriptor_get_value_size);
}

int
bluetooth_descriptor_read_value (BluetoothDescriptor *descriptor)
{
    return jni_call_bool (descriptor->descriptor, g_ctx.descriptor_read_value);
}

int
bluetooth_descriptor_write_value (BluetoothDescriptor *descriptor, const int* values, int length)
{
    return jni_set_value (descriptor->descriptor, g_ctx.descriptor_write_value, values, length);
}

void
bluetooth_descriptor_inc_refcount (BluetoothDescriptor *descriptor)
{
    descriptor->count = descriptor->count + 1;
}

void
bluetooth_descriptor_dec_refcount (BluetoothDescriptor *descriptor)
{
    descriptor->count = descriptor->count - 1;
}

void
bluetooth_descriptor_free_descriptor (BluetoothDescriptor *descriptor)
{
    if (descriptor->count <= 0)
    {
        jni_free (descriptor);
    }
}
