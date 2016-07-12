//TODO copyright
#include "bluetooth-descriptor.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "jni-memory.h"
#include "jni-utils.h"

BluetoothDescriptor *
bluetooth_descriptor_create_descriptor (BluetoothCharacteristic *characteristic, int id)
{
    LOGI("bluetooth_descriptor_create_descriptor\n");
    BluetoothDescriptor *descriptor = jni_calloc (sizeof (*descriptor));

    descriptor->descriptor = jni_characteristic_create_descriptor(characteristic->characteristic, id);

    LOGI("d->d: %p\n", descriptor->descriptor);
    return descriptor;
}

const char*
bluetooth_descriptor_get_uuid (BluetoothDescriptor *descriptor)
{
    LOGI("bluetooth_descriptor_get_uuid\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    return jni_descriptor_get_uuid (descriptor->descriptor);
}

const int*
bluetooth_descriptor_get_value (BluetoothDescriptor *descriptor)
{
    LOGI("bluetooth_descriptor_get_value\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    return jni_descriptor_get_value (descriptor->descriptor);
}

const int
bluetooth_descriptor_get_value_size (BluetoothDescriptor *descriptor)
{
    LOGI("bluetooth_descriptor_get_value_size\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    return jni_descriptor_get_value_size (descriptor->descriptor);
}

const int*
bluetooth_descriptor_read_value (BluetoothDescriptor *descriptor)
{
    LOGI("bluetooth_descriptor_read_value\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    return jni_descriptor_read_value (descriptor->descriptor);
}

// TODO bool
void
bluetooth_descriptor_write_value (BluetoothDescriptor *descriptor, const int* values, int length)
{
    LOGI("bluetooth_descriptor_write_value\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    return jni_descriptor_write_value (descriptor->descriptor, values, length);
}

void
bluetooth_descriptor_free_descriptor (BluetoothDescriptor *descriptor) {
    LOGI("bluetooth_descriptor_free_descriptor\n");
    LOGI("d->d: %p\n", descriptor->descriptor);
    jni_free (descriptor);
}

void
bluetooth_descriptor_free_string(const char* string) {
    LOGI("bluetooth_descriptor_free_string %s\n", string);
    jni_free ((char*)string);
}

void
bluetooth_descriptor_free_int_array (int* array)
{
    LOGI("bluetooth_descriptor_free_int_array\n");
    jni_free (array);
}
