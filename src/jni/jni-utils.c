#include "jni-utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>

JNIEnv* jni_get_env () {
    JNIEnv *env;
    JavaVM *javaVM = g_ctx.javaVM;
    jint res = (*javaVM)->GetEnv(javaVM, (void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (res != JNI_OK) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }
    return env;
}

jint JNI_OnLoad (JavaVM* vm, void* reserved) {
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }

    jni_init(env);

    return JNI_VERSION_1_6;
}

void jni_init(JNIEnv *env) {
    // Adapter class
    jclass classBta = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothAdapterWrapper");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // Adapter getAdapter
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getAdapter",
                                  "()Lhu/uszeged/bluetooth/BluetoothAdapterWrapper;");

    // Adapter getAddress
    g_ctx.adapter_get_address =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Adapter getName
    g_ctx.adapter_get_name =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Adapter getDevices
    g_ctx.adapter_get_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getDevices",
                            "()Ljava/util/Set;");

    // Adapter getDevicesSize
    g_ctx.adapter_get_devices_size =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getDevicesSize",
                            "()I");

    // Adapter getRemoteDevice
    g_ctx.adapter_get_remote_device =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getRemoteDevice",
                            "(Ljava/lang/String;)Lhu/uszeged/bluetooth/BluetoothDeviceWrapper;");

    // Adapter startLeScan
    g_ctx.adapter_start_le_scan =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "startLeScan",
                            "()V");

    // Adapter stopLeScan
    g_ctx.adapter_stop_le_scan =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "stopLeScan",
                            "()V");

    // Device class
    jclass classBtd = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothDeviceWrapper");
    g_ctx.device_cls = (*env)->NewGlobalRef(env, classBtd);

    // Device getAddress
    g_ctx.device_get_address =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Device getName
    g_ctx.device_get_name =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Device isConnected
    g_ctx.device_is_connected =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "isConnected",
                            "()Z");

    // Device connectGatt
    g_ctx.device_connect_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "connectGatt",
                            "(Landroid/content/Context;Z)"
                            "Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Device disconnect
    g_ctx.device_disconnect =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "disconnect",
                            "()V");

    // Device getGatt
    g_ctx.device_get_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getGatt",
                            "()Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Gatt class
    jclass classBtg = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothGattWrapper");
    g_ctx.gatt_cls = (*env)->NewGlobalRef(env, classBtg);

    // Gatt getServices
    g_ctx.gatt_get_gatt_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServices",
                            "()Ljava/util/Set;");

    // Gatt getService
    g_ctx.gatt_get_gatt_service =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getService",
                            "(I)Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");

    // Gatt getServicesSize
    g_ctx.gatt_get_gatt_services_size =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServicesSize",
                            "()I");

    // GattService class
    jclass classBtgs = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothGattServiceWrapper");
    g_ctx.service_cls = (*env)->NewGlobalRef(env, classBtgs);

    // GattService getId
    g_ctx.service_get_id =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getId",
                            "()I");

    // GattService getUuid
    g_ctx.service_get_uuid =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattService isPrimary
    g_ctx.service_is_primary =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "isPrimary",
                            "()Z");

    // GattService getCharacteristic
    g_ctx.service_get_gatt_characteristic =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristic",
                            "(I)Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");

    // GattService getCharacteristics
    g_ctx.service_get_gatt_characteristics =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristics",
                            "()Ljava/util/Set;");

    // GattService getCharacteristicsSize
    g_ctx.service_get_gatt_characteristics_size =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristicsSize",
                            "()I");

    // GattCharacteristic class
    jclass classBtc = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper");
    g_ctx.characteristic_cls = (*env)->NewGlobalRef(env, classBtc);

    // GattCharacteristic getId
    g_ctx.characteristic_get_id =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getId",
                            "()I");

   // GattCharacteristic getUuid
    g_ctx.characteristic_get_uuid =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattCharacteristic getDescriptor
    g_ctx.characteristic_get_gatt_descriptor =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptor",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;");

    // GattCharacteristic getDescriptors
    g_ctx.characteristic_get_gatt_descriptors =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptors",
                            "()Ljava/util/Set;");

    // GattCharacteristic getDescriptorsSize
    g_ctx.characteristic_get_gatt_descriptors_size =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptorsSize",
                            "()I");

    // GattCharacteristic getValue
    g_ctx.characteristic_get_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getValue",
                            "()[I");

    // GattCharacteristic getValueSize
    g_ctx.characteristic_get_value_size =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getValueSize",
                            "()I");

    // GattCharacteristic readValue
    g_ctx.characteristic_read_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "readValue",
                            "()[I");

    // GattCharacteristic writeValue
    g_ctx.characteristic_write_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "writeValue",
                            "([I)V");

    // GattDescriptor class
    jclass classBtdsc = (*env)->FindClass(env, "hu/uszeged/bluetooth/BluetoothGattDescriptorWrapper");
    g_ctx.descriptor_cls = (*env)->NewGlobalRef(env, classBtdsc);

    // GattDescriptor getId
    g_ctx.descriptor_get_id =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getId",
                            "()I");


    // GattDescriptor getUuid
    g_ctx.descriptor_get_uuid =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattDescriptor getValue
    g_ctx.descriptor_get_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getValue",
                            "()[I");

    // GattDescriptor getValueSize
    g_ctx.descriptor_get_value_size =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getValueSize",
                            "()I");

    // GattDescriptor readValue
    g_ctx.descriptor_read_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "readValue",
                            "()[I");

    // GattDescriptor writeValue
    g_ctx.descriptor_write_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "writeValue",
                            "([I)V");

    // Set class
    jclass classSet = (*env)->FindClass(env, "java/util/Set");
    g_ctx.set_cls = (*env)->NewGlobalRef(env, classSet);

    // Set iterator
    g_ctx.set_iterator =
        (*env)->GetMethodID(env, g_ctx.set_cls,
                            "iterator",
                            "()Ljava/util/Iterator;");

    // Set size
    g_ctx.set_size =
        (*env)->GetMethodID(env, g_ctx.set_cls,
                            "size",
                            "()I");

    // Iterator class
    jclass iteratorClass = (*env)->FindClass(env, "java/util/Iterator");
    g_ctx.iterator_cls = (*env)->NewGlobalRef(env, iteratorClass);

    // Iterator hasNext
    g_ctx.iterator_has_next =
        (*env)->GetMethodID(env, g_ctx.iterator_cls,
                            "hasNext",
                            "()Z");

    // Iterator next
    g_ctx.iterator_next =
        (*env)->GetMethodID(env, g_ctx.iterator_cls,
                            "next",
                            "()Ljava/lang/Object;");

    // ActivityThread class
    jclass classAT = (*env)->FindClass(env,"android/app/ActivityThread");
    g_ctx.activity_thread_cls = (*env)->NewGlobalRef(env, classAT);

    // ActivityThread currentApplication
    g_ctx.activity_thread_current_application =
        (*env)->GetStaticMethodID(env, g_ctx.activity_thread_cls,
                                  "currentApplication",
                                  "()Landroid/app/Application;");
}

jobject jni_adapter_create_adapter () {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);
    return (*env)->NewGlobalRef(env, objBta);
}

const char* jni_adapter_get_address(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strAddress = (jstring)
        (*env)->CallObjectMethod(env, adapter, g_ctx.adapter_get_address);
    const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strAddress);
    return address;
}

const char* jni_adapter_get_name(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, adapter, g_ctx.adapter_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    //(*env)->ReleaseStringUTFChars(env, strName, name);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

void jni_adapter_start_discovery(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env, adapter, g_ctx.adapter_start_le_scan);
}

void jni_adapter_stop_discovery(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env, adapter, g_ctx.adapter_stop_le_scan);
}

const char** jni_adapter_get_devices(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, adapter, g_ctx.adapter_get_devices);
    jobject iter = (*env)->CallObjectMethod(env, set, g_ctx.set_iterator);
    jint setSize = (*env)->CallIntMethod(env, set, g_ctx.set_size);
    size_t deviceSize = (size_t) setSize;
    if (deviceSize <= 0) {
        return NULL;
    }

    const char **arr = (const char**) calloc(deviceSize, sizeof(char*));
    size_t i = 0;
    for ( i = 0; i < deviceSize; i++ )
    {
        arr[i] = (char*) calloc(20, sizeof(char));
    }

    i = 0;
    while ((*env)->CallBooleanMethod(env, iter, g_ctx.iterator_has_next)) {
        jobject device = (*env)->CallObjectMethod(env, iter, g_ctx.iterator_next);
        jstring strAddress = (jstring)
            (*env)->CallObjectMethod(env, device, g_ctx.device_get_address);
        const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
        arr[i++] = address;
        //(*env)->ReleaseStringUTFChars(env, strAddress, address);
        (*env)->DeleteLocalRef(env, device);
        (*env)->DeleteLocalRef(env, strAddress);
    }
    return arr;
}

int jni_adapter_get_devices_size(jobject adapter) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, adapter, g_ctx.adapter_get_devices_size);
}

jobject jni_adapter_create_device(jobject adapter, const char* address) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject objBtd = (jobject)
        (*env)->CallObjectMethod(env, adapter,
                                 g_ctx.adapter_get_remote_device,
                                 (*env)->NewStringUTF(env, address));
    return (*env)->NewGlobalRef(env, objBtd);
}

const char* jni_device_get_address(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strAddress = (jstring)
        (*env)->CallObjectMethod(env, device, g_ctx.device_get_address);
    const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strAddress);
    return address;
}

const char* jni_device_get_name(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, device, g_ctx.device_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    //(*env)->ReleaseStringUTFChars(env, strName, name);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

void jni_device_connect_gatt(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    //TODO move the context to the java side
    jobject context = (*env)->CallStaticObjectMethod(env, g_ctx.activity_thread_cls,
                                                     g_ctx.activity_thread_current_application);
    (*env)->CallObjectMethod(env, device,
                             g_ctx.device_connect_gatt,
                             context,
                             JNI_FALSE);
}

void jni_device_disconnect(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env, device, g_ctx.device_disconnect);
}

int jni_device_is_connected(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallBooleanMethod(env, device, g_ctx.device_is_connected);
}

jobject jni_device_get_gatt(JNIEnv *env, jobject device) {
    if (device == NULL) {
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device, g_ctx.device_get_gatt);
}


const int* jni_device_get_gatt_services(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject gatt = jni_device_get_gatt(env, device);
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, gatt, g_ctx.gatt_get_gatt_services);
    jobject iter = (*env)->CallObjectMethod(env, set, g_ctx.set_iterator);
    jint setSize = (*env)->CallIntMethod(env, set, g_ctx.set_size);
    size_t serviceSize = (size_t) setSize;
    if (serviceSize <= 0) {
        return NULL;
    }

    int *arr = calloc(serviceSize, sizeof *arr);
    size_t i = 0;
    while ((*env)->CallBooleanMethod(env, iter, g_ctx.iterator_has_next)) {
        jobject service = (*env)->CallObjectMethod(env, iter, g_ctx.iterator_next);
        jint id = (*env)->CallIntMethod(env, service, g_ctx.service_get_id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, service);
    }
    return arr;
}

int jni_device_get_gatt_services_size(jobject device) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, jni_device_get_gatt(env, device), g_ctx.gatt_get_gatt_services_size);
}

jobject jni_device_create_service(jobject device, int id) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject objBtd = (jobject)
        (*env)->CallObjectMethod(env, jni_device_get_gatt(env, device),
                                 g_ctx.gatt_get_gatt_service,
                                 (jint) id);
    return (*env)->NewGlobalRef(env, objBtd);
}

const char* jni_service_get_uuid(jobject service) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strUuid = (jstring)
        (*env)->CallObjectMethod(env, service, g_ctx.service_get_uuid);
    const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
    //(*env)->ReleaseStringUTFChars(env, strUuid, uuid);
    (*env)->DeleteLocalRef(env, strUuid);
    return uuid;
}

int jni_service_is_primary(jobject service) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallBooleanMethod(env, service, g_ctx.service_is_primary);
}

const int* jni_service_get_gatt_characteristics(jobject service) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, service, g_ctx.service_get_gatt_characteristics);
    // Obtain an iterator over the Set
    jobject iter = (*env)->CallObjectMethod(env, set, g_ctx.set_iterator);
    jint setSize = (*env)->CallIntMethod(env, set, g_ctx.set_size);
    size_t characteristicSize = (size_t) setSize;
    if (characteristicSize <= 0) {
        return NULL;
    }

    int *arr = calloc(characteristicSize, sizeof *arr);
    size_t i = 0;
    while ((*env)->CallBooleanMethod(env, iter, g_ctx.iterator_has_next)) {
        jobject characteristic = (*env)->CallObjectMethod(env, iter, g_ctx.iterator_next);
        jint id = (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, characteristic);
    }
    return arr;
}

int jni_service_get_gatt_characteristics_size(jobject service) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, service, g_ctx.service_get_gatt_characteristics_size);
}

jobject jni_service_create_characteristic(jobject service, int id) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject objBtd = (jobject)
        (*env)->CallObjectMethod(env, service,
                                 g_ctx.service_get_gatt_characteristic,
                                 (jint) id);
    return (*env)->NewGlobalRef(env, objBtd);
}

const char* jni_characteristic_get_uuid(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strUuid = (jstring)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_uuid);
    const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
    //(*env)->ReleaseStringUTFChars(env, strUuid, uuid);
    (*env)->DeleteLocalRef(env, strUuid);
    return uuid;
}

const int* jni_characteristic_get_gatt_descriptors(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_gatt_descriptors);
    jobject iter = (*env)->CallObjectMethod(env, set, g_ctx.set_iterator);
    jint setSize = (*env)->CallIntMethod(env, set, g_ctx.set_size);
    size_t descriptorSize = (size_t) setSize;
    if (descriptorSize <= 0) {
        return NULL;
    }

    int *arr = calloc(descriptorSize, sizeof *arr);
    size_t i = 0;
    while ((*env)->CallBooleanMethod(env, iter, g_ctx.iterator_has_next)) {
        jobject descriptor = (*env)->CallObjectMethod(env, iter, g_ctx.iterator_next);
        jint id = (*env)->CallIntMethod(env, descriptor, g_ctx.descriptor_get_id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, descriptor);
    }
    return arr;
}

int jni_characteristic_get_gatt_descriptors_size(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_gatt_descriptors_size);
}

const int* jni_characteristic_get_value(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jintArray jarr = (jintArray) (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_value);
    if (jarr == NULL) {
        return NULL;
    }
    jsize len = (*env)->GetArrayLength(env, jarr);
    if (len <= 0) {
        return 0;
    }
    int *rarr = calloc((size_t)len, sizeof *rarr);

    jboolean iscopy;
    jint* values = (*env)->GetIntArrayElements(env, jarr, &iscopy);
    int i;
    for (i = 0; i < len ; i++) {
        rarr[i] = (int)values[i];
    }
    if (iscopy == JNI_TRUE) {
        (*env)->ReleaseIntArrayElements(env, jarr, values, JNI_ABORT);
    }
    return rarr;
}

const int jni_characteristic_get_value_size(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_value_size);
}

const int* jni_characteristic_read_value(jobject characteristic) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jintArray jarr = (jintArray) (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_read_value);
    if (jarr == NULL) {
        return NULL;
    }
    jsize len = (*env)->GetArrayLength(env, jarr);
    if (len <= 0) {
        return NULL;
    }
    int *rarr = calloc((size_t)len, sizeof *rarr);

    jboolean iscopy;
    jint* values = (*env)->GetIntArrayElements(env, jarr, &iscopy);
    int i;
    for (i = 0; i < len ; i++) {
        rarr[i] = (int)values[i];
    }
    if (iscopy == JNI_TRUE) {
        (*env)->ReleaseIntArrayElements(env, jarr, values, JNI_ABORT);
    }
    return rarr;
}

void jni_characteristic_write_value(jobject characteristic, const int* values, int length) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    jintArray arr = (*env)->NewIntArray(env, length);
    (*env)->SetIntArrayRegion(env, arr, 0, length, values);
    (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_write_value, arr);
}

jobject jni_characteristic_create_descriptor(jobject characteristic, int id) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject objBtd = (jobject)
        (*env)->CallObjectMethod(env, characteristic,
                                 g_ctx.characteristic_get_gatt_descriptor,
                                 (jint) id);
    return (*env)->NewGlobalRef(env, objBtd);
}

const char* jni_descriptor_get_uuid(jobject descriptor) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jstring strUuid = (jstring)
        (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_get_uuid);
    const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
    //(*env)->ReleaseStringUTFChars(env, strUuid, uuid);
    (*env)->DeleteLocalRef(env, strUuid);
    return uuid;
}

const int* jni_descriptor_get_value(jobject descriptor) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jintArray jarr = (jintArray) (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_get_value);
    if (jarr == NULL) {
        return NULL;
    }
    jsize len = (*env)->GetArrayLength(env, jarr);
    if (len <= 0) {
        return 0;
    }
    int *rarr = calloc((size_t)len, sizeof *rarr);

    jboolean iscopy;
    jint* values = (*env)->GetIntArrayElements(env, jarr, &iscopy);
    int i;
    for (i = 0; i < len ; i++) {
        rarr[i] = (int)values[i];
    }
    if (iscopy == JNI_TRUE) {
        (*env)->ReleaseIntArrayElements(env, jarr, values, JNI_ABORT);
    }
    return rarr;
}

const int jni_descriptor_get_value_size(jobject descriptor) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, descriptor, g_ctx.descriptor_get_value_size);
}

const int* jni_descriptor_read_value(jobject descriptor) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jintArray jarr = (jintArray) (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_read_value);
    if (jarr == NULL) {
        return NULL;
    }
    jsize len = (*env)->GetArrayLength(env, jarr);
    if (len <= 0) {
        return 0;
    }
    int *rarr = calloc((size_t)len, sizeof *rarr);

    jboolean iscopy;
    jint* values = (*env)->GetIntArrayElements(env, jarr, &iscopy);
    int i;
    for (i = 0; i < len ; i++) {
        rarr[i] = (int)values[i];
    }
    if (iscopy == JNI_TRUE) {
        (*env)->ReleaseIntArrayElements(env, jarr, values, JNI_ABORT);
    }
    return rarr;
}

void jni_descriptor_write_value(jobject descriptor, const int* values, int length) {
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    jintArray arr = (*env)->NewIntArray(env, length);
    (*env)->SetIntArrayRegion(env, arr, 0, length, values);
    (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_write_value, arr);
}
