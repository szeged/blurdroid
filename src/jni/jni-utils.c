#include "jni-utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>

JNIEnv*
jni_get_env ()
{
    JNIEnv *env;
    JavaVM *javaVM = g_ctx.javaVM;
    jint res = (*javaVM)->GetEnv (javaVM, (void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK)
    {
        res = (*javaVM)->AttachCurrentThread (javaVM, &env, NULL);
        if (res != JNI_OK)
        {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }

    return env;
}

void
jni_init (JNIEnv *env)
{
    jclass classBta =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothAdapterWrapper");
    g_ctx.adapter_cls =
        (*env)->NewGlobalRef (env, classBta);

    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID (env, g_ctx.adapter_cls,
                                   "getAdapter",
                                   "()Lhu/uszeged/bluetooth/BluetoothAdapterWrapper;");

    g_ctx.adapter_get_address =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "getAddress",
                             "()Ljava/lang/String;");

    g_ctx.adapter_get_name =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "getName",
                             "()Ljava/lang/String;");

    g_ctx.adapter_get_devices =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "getDevices",
                             "()Ljava/util/Set;");

    g_ctx.adapter_get_devices_size =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "getDevicesSize",
                             "()I");

    g_ctx.adapter_get_remote_device =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "getRemoteDevice",
                             "(Ljava/lang/String;)Lhu/uszeged/bluetooth/BluetoothDeviceWrapper;");

    g_ctx.adapter_start_le_scan =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "startLeScan",
                             "()V");

    g_ctx.adapter_stop_le_scan =
        (*env)->GetMethodID (env, g_ctx.adapter_cls,
                             "stopLeScan",
                             "()V");

    jclass classBtd =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothDeviceWrapper");
    g_ctx.device_cls =
        (*env)->NewGlobalRef (env, classBtd);

    g_ctx.device_get_address =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getAddress",
                             "()Ljava/lang/String;");

    g_ctx.device_get_name =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getName",
                             "()Ljava/lang/String;");

    g_ctx.device_get_uuids =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getUuids",
                             "()Ljava/util/Set;");

    g_ctx.device_get_uuids_size =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getUuidsSize",
                             "()I");

    g_ctx.device_get_rssi =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getRssi",
                             "()I");

    g_ctx.device_get_tx_power =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getTxPower",
                             "()I");

    g_ctx.device_is_connected =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "isConnected",
                             "()Z");

    g_ctx.device_connect_gatt =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "connectGatt",
                             "()Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    g_ctx.device_disconnect =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "disconnect",
                             "()V");

    g_ctx.device_get_gatt =
        (*env)->GetMethodID (env, g_ctx.device_cls,
                             "getGatt",
                             "()Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    jclass classBtg =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothGattWrapper");
    g_ctx.gatt_cls =
        (*env)->NewGlobalRef (env, classBtg);

    g_ctx.gatt_get_gatt_services =
        (*env)->GetMethodID (env, g_ctx.gatt_cls,
                             "getServices",
                             "()Ljava/util/Set;");

    g_ctx.gatt_get_gatt_service =
        (*env)->GetMethodID (env, g_ctx.gatt_cls,
                             "getService",
                             "(I)Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");

    g_ctx.gatt_get_gatt_services_size =
        (*env)->GetMethodID (env, g_ctx.gatt_cls,
                             "getServicesSize",
                             "()I");

    jclass classBtgs =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothGattServiceWrapper");
    g_ctx.service_cls =
        (*env)->NewGlobalRef (env, classBtgs);

    g_ctx.service_get_id =
        (*env)->GetMethodID (env, g_ctx.service_cls,
                             "getId",
                             "()I");

    g_ctx.service_get_uuid =
        (*env)->GetMethodID (env, g_ctx.service_cls,
                             "getUuid",
                             "()Ljava/lang/String;");

    g_ctx.service_is_primary =
        (*env)->GetMethodID (env, g_ctx.service_cls,
                             "isPrimary",
                             "()Z");

    g_ctx.service_get_gatt_characteristic =
        (*env)->GetMethodID (env, g_ctx.service_cls,
                             "getCharacteristic",
                             "(I)Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");

    g_ctx.service_get_gatt_characteristics =
        (*env)->GetMethodID (env, g_ctx.service_cls,
                             "getCharacteristics",
                             "()Ljava/util/Set;");

    g_ctx.service_get_gatt_characteristics_size =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristicsSize",
                            "()I");

    jclass classBtc =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper");
    g_ctx.characteristic_cls =
        (*env)->NewGlobalRef (env, classBtc);

    g_ctx.characteristic_get_id =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getId",
                             "()I");

    g_ctx.characteristic_get_uuid =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getUuid",
                             "()Ljava/lang/String;");

    g_ctx.characteristic_get_flags =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getFlags",
                             "()Ljava/util/Set;");

    g_ctx.characteristic_get_flags_size =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getFlagsSize",
                             "()I");

    g_ctx.characteristic_get_gatt_descriptor =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getDescriptor",
                             "(Ljava/lang/String;)"
                             "Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;");

    g_ctx.characteristic_get_gatt_descriptors =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getDescriptors",
                             "()Ljava/util/Set;");

    g_ctx.characteristic_get_gatt_descriptors_size =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getDescriptorsSize",
                             "()I");

    g_ctx.characteristic_get_value =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getValue",
                             "()[I");

    g_ctx.characteristic_get_value_size =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "getValueSize",
                             "()I");

    g_ctx.characteristic_read_value =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "readValue",
                             "()Z");

    g_ctx.characteristic_write_value =
        (*env)->GetMethodID (env, g_ctx.characteristic_cls,
                             "writeValue",
                             "([I)Z");

    jclass classBtdsc =
        (*env)->FindClass (env, "hu/uszeged/bluetooth/BluetoothGattDescriptorWrapper");
    g_ctx.descriptor_cls =
        (*env)->NewGlobalRef (env, classBtdsc);

    g_ctx.descriptor_get_id =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "getId",
                             "()I");


    g_ctx.descriptor_get_uuid =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "getUuid",
                             "()Ljava/lang/String;");

    g_ctx.descriptor_get_value =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "getValue",
                             "()[I");

    g_ctx.descriptor_get_value_size =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "getValueSize",
                             "()I");

    g_ctx.descriptor_read_value =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "readValue",
                             "()Z");

    g_ctx.descriptor_write_value =
        (*env)->GetMethodID (env, g_ctx.descriptor_cls,
                             "writeValue",
                             "([I)Z");

    jclass classSet =
        (*env)->FindClass (env, "java/util/Set");
    g_ctx.set_cls =
        (*env)->NewGlobalRef (env, classSet);

    g_ctx.set_iterator =
        (*env)->GetMethodID (env, g_ctx.set_cls,
                             "iterator",
                             "()Ljava/util/Iterator;");

    g_ctx.set_size =
        (*env)->GetMethodID (env, g_ctx.set_cls,
                             "size",
                             "()I");

    jclass iteratorClass =
        (*env)->FindClass (env, "java/util/Iterator");
    g_ctx.iterator_cls =
        (*env)->NewGlobalRef (env, iteratorClass);

    g_ctx.iterator_has_next =
        (*env)->GetMethodID (env, g_ctx.iterator_cls,
                             "hasNext",
                             "()Z");

    g_ctx.iterator_next =
        (*env)->GetMethodID (env, g_ctx.iterator_cls,
                             "next",
                             "()Ljava/lang/Object;");

    g_ctx.null = NULL;
}

jint
JNI_OnLoad (JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    memset (&g_ctx, 0, sizeof (g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv (vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGE ("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }

    jni_init (env);

    return JNI_VERSION_1_6;
}

jobject
jni_create_static_object (jclass cls, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;

    jobject obj = (jobject)
        (*env)->CallStaticObjectMethod (env, cls, mid);

    return (*env)->NewGlobalRef (env, obj);
}

jobject
jni_create_object (jobject jobj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;

    jobject obj = (jobject)
        (*env)->CallObjectMethod (env, jobj, mid);

    return (*env)->NewGlobalRef (env, obj);
}

jobject
jni_create_object_int (jobject jobj, jmethodID mid, int i)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;

    jobject obj = (jobject)
        (*env)->CallObjectMethod (env, jobj, mid, (jint)i);

    return (*env)->NewGlobalRef (env, obj);
}

jobject
jni_create_object_str (jobject jobj, jmethodID mid, const char* str, int length)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;
    char str2[length];
    memcpy(str2, str, length);
    str2[length] = '\0';
    jobject obj = (jobject)
        (*env)->CallObjectMethod (env, jobj, mid,
                                  (*env)->NewStringUTF (env, str2));
    return (*env)->NewGlobalRef (env, obj);
}

void
jni_delete_object (jobject jobj)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;
    (*env)->DeleteGlobalRef (env, jobj);
}

const char*
jni_call_str (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return NULL;

    jstring jstr = (jstring)
        (*env)->CallObjectMethod (env, obj, mid);
    const char* str = (*env)->GetStringUTFChars (env, jstr, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef (env, jstr);

    return str;
}

void
jni_call_void (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;

    (*env)->CallVoidMethod (env, obj, mid);
}

int
jni_call_int (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return 0;

    return (*env)->CallIntMethod (env, obj, mid);
}

int
jni_call_bool (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return 0;

    return (*env)->CallBooleanMethod (env, obj, mid);
}

jobject
jni_call_object (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return NULL;

    return (*env)->CallObjectMethod (env, obj, mid);
}

const int*
jni_get_value (jobject obj, jmethodID mid)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return NULL;

    jintArray jarr = (jintArray) (*env)->CallObjectMethod (env, obj, mid);
    if (jarr == NULL)
        return NULL;

    jsize len = (*env)->GetArrayLength (env, jarr);
    if (len <= 0)
        return 0;

    int *rarr = calloc ((size_t)len, sizeof *rarr);

    jboolean iscopy;
    jint* values = (*env)->GetIntArrayElements (env, jarr, &iscopy);

    int i;
    for (i = 0; i < len ; i++)
    {
        rarr[i] = (int)values[i];
    }

    if (iscopy == JNI_TRUE)
        (*env)->ReleaseIntArrayElements (env, jarr, values, JNI_ABORT);
    
    (*env)->DeleteLocalRef (env, jarr);

    return rarr;
}

int
jni_set_value (jobject obj, jmethodID mid, const int* values, int length)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return;

    jintArray arr = (*env)->NewIntArray (env, length);
    (*env)->SetIntArrayRegion (env, arr, 0, length, values);
    return (*env)->CallBooleanMethod (env, obj, mid, arr);
}

const char**
jni_call_str_array (jobject jobj, jmethodID mid1, jmethodID mid2)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return NULL;
    jobject set = (jobject)
        (*env)->CallObjectMethod (env, jobj, mid1);
    jobject iter = (*env)->CallObjectMethod (env, set, g_ctx.set_iterator);
    jint setSize = (size_t)(*env)->CallIntMethod (env, set, g_ctx.set_size);
    if (setSize <= 0) {
        (*env)->DeleteLocalRef (env, iter);
        (*env)->DeleteLocalRef (env, set);
        return NULL;
    }

    const char **arr = (const char**) calloc (setSize, sizeof (char*));
    size_t i = 0;
    /*for ( i = 0; i < setSize; i++ )
    {
        arr[i] = (char*) calloc (40, sizeof (char));
    }*/

    i = 0;
    while ((*env)->CallBooleanMethod (env, iter, g_ctx.iterator_has_next))
    {
        jobject obj = (*env)->CallObjectMethod (env, iter, g_ctx.iterator_next);
        jstring jstr;
        if (mid2) {
            jstr = (jstring)
                (*env)->CallObjectMethod (env, obj, mid2);
        } else {
            jstr = (jstring) obj;
        }
        const char* str = (*env)->GetStringUTFChars (env, jstr, NULL);
        arr[i++] = strdup(str);
        LOGE("### jni_call_str_array ### %s %p", arr[i-1], arr[i-1]);
        //(*env)->ReleaseStringUTFChars(env, jstr, str);
        (*env)->DeleteLocalRef (env, obj);
        if (mid2)
            (*env)->DeleteLocalRef (env, jstr);
    }
    (*env)->DeleteLocalRef (env, iter);
    (*env)->DeleteLocalRef (env, set);
    return arr;
}

const int*
jni_call_int_array (jobject jobj, jmethodID mid1, jmethodID mid2)
{
    JNIEnv *env = jni_get_env ();
    if (env == NULL)
        return NULL;

    jobject set = (jobject)
        (*env)->CallObjectMethod (env, jobj, mid1);
    jobject iter = (*env)->CallObjectMethod (env, set, g_ctx.set_iterator);
    size_t setSize = (size_t) (*env)->CallIntMethod (env, set, g_ctx.set_size);
    if (setSize <= 0) {
        (*env)->DeleteLocalRef (env, iter);
        (*env)->DeleteLocalRef (env, set);
        return NULL;
    }

    int *arr = calloc (setSize, sizeof *arr);

    size_t i = 0;
    while ((*env)->CallBooleanMethod (env, iter, g_ctx.iterator_has_next))
    {
        jobject obj = (*env)->CallObjectMethod (env, iter, g_ctx.iterator_next);
        arr[i++] = (int) (*env)->CallIntMethod (env, obj, mid2);
        (*env)->DeleteLocalRef (env, obj);
    }
    (*env)->DeleteLocalRef (env, iter);
    (*env)->DeleteLocalRef (env, set);
    return arr;
}
