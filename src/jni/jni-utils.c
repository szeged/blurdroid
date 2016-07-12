#include "jni-utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>

JNIEnv* jni_get_env () {
    LOGI("jni_get_env");
    JNIEnv *env;
    JavaVM *javaVM = g_ctx.javaVM;
    jint res = (*javaVM)->GetEnv(javaVM, (void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        LOGI("jni_get_env_AttachCurrentThread");
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (res != JNI_OK) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }
    return env;
}

void jni_set_class_loader (JNIEnv *env) {
    LOGI("jni_set_class_loader");

    LOGI("jni_set_class_loader find main activity class");
    g_ctx.main_activity_cls = (*env)->FindClass(env, "com/mozilla/servo/MainActivity");
    if(!g_ctx.main_activity_cls) {LOGE("#### failed to get g_ctx.main_activity_cls");}

    LOGI("jni_set_class_loader get object class");
    g_ctx.class_cls = (*env)->GetObjectClass(env, g_ctx.main_activity_cls);
    if(!g_ctx.class_cls) {LOGE("#### failed to get g_ctx.class_cls");}

    LOGI("jni_set_class_loader find class loader class");
    g_ctx.class_loader_cls = (*env)->FindClass(env, "java/lang/ClassLoader");
    if(!g_ctx.class_loader_cls) {LOGE("#### failed to get g_ctx.class_loader_cls");}

    LOGI("jni_set_class_loader get getClassLoader method");
    g_ctx.get_class_loader = (*env)->GetMethodID(env, g_ctx.class_cls, "getClassLoader",
                                                 "()Ljava/lang/ClassLoader;");
    if(!g_ctx.get_class_loader) {LOGE("#### failed to get g_ctx.get_class_loader");}

    LOGI("jni_set_class_loader call getClassLoader on MainActivity");
    jobject classLoaderObj = (*env)->CallObjectMethod(env,
                                                      g_ctx.main_activity_cls,
                                                      g_ctx.get_class_loader);
    if(!classLoaderObj) {LOGE("#### failed to get classLoaderObj");}
    g_ctx.class_loader_obj = (*env)->NewGlobalRef(env, classLoaderObj);
    if(!g_ctx.class_loader_obj) {LOGE("#### failed to get g_ctx.class_loader_obj");}

    LOGI("jni_set_class_loader get findClass method");
    g_ctx.find_class = (*env)->GetMethodID(env, g_ctx.class_loader_cls,
                                           "findClass",
                                           "(Ljava/lang/String;)Ljava/lang/Class;");
    if(!g_ctx.find_class) {LOGE("#### failed to get g_ctx.find_class");}
}

jclass jni_find_class (JNIEnv *env, const char* name) {
    LOGI("jni_find_class");
    if(!env) {LOGE("#### failed to get env");}
    if(!name) {LOGE("#### failed to get name");}
    if(!g_ctx.class_loader_obj) {LOGE("#### failed to get g_ctx.class_loader_obj");}
    if(!g_ctx.find_class) {LOGE("#### failed to get g_ctx.find_class");}
    jclass resultClass =
        (jclass) ((*env)->CallObjectMethod(env,
                                           g_ctx.class_loader_obj,
                                           g_ctx.find_class,
                                           (*env)->NewStringUTF(env, name)));
    if(!resultClass) {LOGE("#### failed to get %s class", name);}
    else {LOGI("#### %s!!",name);}
    jthrowable exception = (*env)->ExceptionOccurred(env);
    if (exception) { (*env)->ExceptionDescribe(env);}
    return resultClass;
}

jint JNI_OnLoad (JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }

    jni_set_class_loader(env);

    return JNI_VERSION_1_6;
}

//void jni_adapter_create_adapter (jobject *adapter) {
jobject jni_adapter_create_adapter () {
    LOGI("Init init");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    // Adapter class
    LOGI("Init adapter class");
    jclass classBta = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothAdapterWrapper");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // Adapter getAdapter
    LOGI("Init adapter getAdapter");
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getAdapter",
                                  "()Lhu/uszeged/bluetooth/BluetoothAdapterWrapper;");

    // Adapter getAddress
    LOGI("Init adapter getAddress");
    g_ctx.adapter_get_address =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Adapter getName
    LOGI("Init adapter getName");
    g_ctx.adapter_get_name =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Adapter getDevices
    LOGI("Init adapter getDevices");
    g_ctx.adapter_get_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getDevices",
                            "()Ljava/util/Set;");

    // Adapter getDevicesSize
    LOGI("Init adapter getDevicesSize");
    g_ctx.adapter_get_devices_size =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getDevicesSize",
                            "()I");

    // Adapter getRemoteDevice
    LOGI("Init adapter getRemoteDevice");
    g_ctx.adapter_get_remote_device =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getRemoteDevice",
                            "(Ljava/lang/String;)Lhu/uszeged/bluetooth/BluetoothDeviceWrapper;");

    // Adapter startLeScan
    LOGI("Init adapter startLeScan");
    g_ctx.adapter_start_le_scan =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "startLeScan",
                            "()V");

    // Adapter stopLeScan
    LOGI("Init adapter stopLeScan");
    g_ctx.adapter_stop_le_scan =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "stopLeScan",
                            "()V");

    // Device class
    LOGI("Init device class");
    jclass classBtd = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothDeviceWrapper");
    g_ctx.device_cls = (*env)->NewGlobalRef(env, classBtd);

    // Device getAddress
    LOGI("Init device getAddress");
    g_ctx.device_get_address =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Device getName
    LOGI("Init device getName");
    g_ctx.device_get_name =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Device isConnected
    LOGI("Init device isConnected");
    g_ctx.device_is_connected =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "isConnected",
                            "()Z");

    // Device connectGatt
    LOGI("Init device connectGatt");
    g_ctx.device_connect_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "connectGatt",
                            "(Landroid/content/Context;Z)"
                            "Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Device disconnect
    LOGI("Init device disconnect");
    g_ctx.device_disconnect =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "disconnect",
                            "()V");

    // Device getGatt
    LOGI("Init device getGatt");
    g_ctx.device_get_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getGatt",
                            "()Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Gatt class
    LOGI("Init gatt class");
    jclass classBtg = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothGattWrapper");
    g_ctx.gatt_cls = (*env)->NewGlobalRef(env, classBtg);

    // Gatt getServices
    LOGI("Init gatt getServices");
    g_ctx.gatt_get_gatt_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServices",
                            "()Ljava/util/Set;");

    // Gatt getService
    LOGI("Init gatt getService");
    g_ctx.gatt_get_gatt_service =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getService",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");

    // Gatt getServicesSize
    LOGI("Init gatt getServicesSize");
    g_ctx.gatt_get_gatt_services_size =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServicesSize",
                            "()"
                            "I");

    // GattService class
    LOGI("Init service class");
    jclass classBtgs = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothGattServiceWrapper");
    g_ctx.service_cls = (*env)->NewGlobalRef(env, classBtgs);

    // GattService getId
    LOGI("Init service getId");
    g_ctx.service_get_id =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getId",
                            "()I");

    // GattService getUuid
    LOGI("Init service getUuid");
    g_ctx.service_get_uuid =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattService isPrimary
    LOGI("Init service isPrimary");
    g_ctx.service_is_primary =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "isPrimary",
                            "()Z");

    // GattService getCharacteristic
    LOGI("Init service getCharacteristic");
    g_ctx.service_get_gatt_characteristic =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristic",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");

    // GattService getCharacteristics
    LOGI("Init service getCharacteristics");
    g_ctx.service_get_gatt_characteristics =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristics",
                            "()Ljava/util/Set;");

    // GattService getCharacteristicsSize
    LOGI("Init service getCharacteristicsSize");
    g_ctx.service_get_gatt_characteristics_size =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristicsSize",
                            "()"
                            "I");

    // GattCharacteristic class
    LOGI("Init characteristic class");
    jclass classBtc = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper");
    g_ctx.characteristic_cls = (*env)->NewGlobalRef(env, classBtc);

    // GattCharacteristic getId
    LOGI("Init characteristic getId");
    g_ctx.characteristic_get_id =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getId",
                            "()I");

   // GattCharacteristic getUuid
    LOGI("Init characteristic getUuid");
    g_ctx.characteristic_get_uuid =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattCharacteristic getDescriptor
    LOGI("Init characteristic getDescriptor");
    g_ctx.characteristic_get_gatt_descriptor =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptor",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;");

    // GattCharacteristic getDescriptors
    LOGI("Init characteristic getDescriptors");
    g_ctx.characteristic_get_gatt_descriptors =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptors",
                            "()Ljava/util/Set;");

    // GattCharacteristic getDescriptorsSize
    LOGI("Init characteristic getDescriptorsSize");
    g_ctx.characteristic_get_gatt_descriptors_size =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptorsSize",
                            "()"
                            "I");

    // GattCharacteristic getValue
    LOGI("Init characteristic getValue");
    g_ctx.characteristic_get_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getValue",
                            "()[I");

    // GattCharacteristic getValueSize
    LOGI("Init characteristic getValueSize");
    g_ctx.characteristic_get_value_size =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getValueSize",
                            "()"
                            "I");

    // GattCharacteristic readValue
    LOGI("Init characteristic readValue");
    g_ctx.characteristic_read_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "readValue",
                            "()[I");

    // GattCharacteristic writeValue
    LOGI("Init characteristic writeValue");
    g_ctx.characteristic_write_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "writeValue",
                            "([I)V");

    // GattDescriptor class
    LOGI("Init descriptor class");
    jclass classBtdsc = jni_find_class(env, "hu/uszeged/bluetooth/BluetoothGattDescriptorWrapper");
    g_ctx.descriptor_cls = (*env)->NewGlobalRef(env, classBtdsc);

    // GattDescriptor getId
    LOGI("Init descriptor getId");
    g_ctx.descriptor_get_id =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getId",
                            "()I");


    // GattDescriptor getUuid
    LOGI("Init descriptor getUuid");
    g_ctx.descriptor_get_uuid =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattDescriptor getValue
    LOGI("Init descriptor getValue");
    g_ctx.descriptor_get_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getValue",
                            "()[I");

    // GattDescriptor getValueSize
    LOGI("Init descriptor getValueSize");
    g_ctx.descriptor_get_value_size =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "getValueSize",
                            "()"
                            "I");

    // GattDescriptor readValue
    LOGI("Init descriptor readValue");
    g_ctx.descriptor_read_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "readValue",
                            "()[I");

    // GattDescriptor writeValue
    LOGI("Init descriptor writeValue");
    g_ctx.descriptor_write_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "writeValue",
                            "([I)V");

    // Adapter object
    LOGI("Init adapter object");
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);
    //adapter = (*env)->NewGlobalRef(env, objBta);
    //LOGI("Init %d",adapter);
    return (*env)->NewGlobalRef(env, objBta);
}

const char* jni_adapter_get_address(jobject adapter) {
    LOGI("jni_adapter_get_address");
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
    LOGI("jni_adapter_get_name");
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
    LOGI("jni_adapter_start_discovery");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env, adapter, g_ctx.adapter_start_le_scan);
}

void jni_adapter_stop_discovery(jobject adapter) {
    LOGI("jni_adapter_stop_discovery");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    (*env)->CallVoidMethod(env, adapter, g_ctx.adapter_stop_le_scan);
}

const char** jni_adapter_get_devices(jobject adapter) {
    LOGI("jni_adapter_get_devices");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, adapter, g_ctx.adapter_get_devices);
    // Obtain an iterator over the Set
    jclass setClass = (*env)->FindClass(env, "java/util/Set");
    jmethodID iterator =
            (*env)->GetMethodID(env, setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = (*env)->CallObjectMethod(env, set, iterator);
    // Get the Set size
    jmethodID size =
            (*env)->GetMethodID(env, setClass, "size", "()I");
    jint setSize = (*env)->CallIntMethod(env, set, size);
    size_t deviceSize = (size_t) setSize;
    LOGI("#### %d device(s) found!####", deviceSize);
    if (deviceSize <= 0) {
        return NULL;
    }

    // Get the Iterator method IDs
    jclass iteratorClass = (*env)->FindClass(env, "java/util/Iterator");
    jmethodID hasNext = (*env)->GetMethodID(env, iteratorClass, "hasNext", "()Z");
    jmethodID next =
        (*env)->GetMethodID(env, iteratorClass, "next", "()Ljava/lang/Object;");

    //const char* devices[deviceSize];
    const char **arr = (const char**) calloc(deviceSize, sizeof(char*));
    size_t i = 0;
    for ( i = 0; i < deviceSize; i++ )
    {
        arr[i] = (char*) calloc(20, sizeof(char));
    }

    i = 0;
    // Iterate over the device Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        jobject device = (*env)->CallObjectMethod(env, iter, next);
        jstring strAddress = (jstring)
            (*env)->CallObjectMethod(env, device, g_ctx.device_get_address);
        const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
        LOGI("#### Device Address: %s", address);
        //devices[i++] = address;
        arr[i++] = address;
        //(*env)->ReleaseStringUTFChars(env, strAddress, address);
        (*env)->DeleteLocalRef(env, device);
        (*env)->DeleteLocalRef(env, strAddress);
    }
    //return devices;
    return arr;
}

int jni_adapter_get_devices_size(jobject adapter) {
    LOGI("jni_adapter_get_devices_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, adapter, g_ctx.adapter_get_devices_size);
}

jobject jni_adapter_create_device(jobject adapter, const char* address) {
    LOGI("jni_adapter_create_device %s", address);
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
    LOGI("jni_device_get_address");
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
    LOGI("jni_device_get_name");
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
    LOGI("jni_device_connect_gatt");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    //TODO move the context to the java side
    jclass theClass = (*env)->FindClass(env,"android/app/ActivityThread");
    if(!theClass){LOGE("#### failed to get android/app/ActivityThread ####");}
    else{LOGI("#### got android/app/ActivityThread = %p ####", theClass);}

    jmethodID method = (*env)->GetStaticMethodID(env,theClass,"currentApplication","()Landroid/app/Application;");
    if(!method){LOGE("#### failed to get currentApplication ####");}
    else{LOGI("#### got currentApplication = %ld ####",(long) method);}

    jobject context = (*env)->CallStaticObjectMethod(env,theClass,method);
    if(!context){LOGE("#### failed to get context ####");}
    else{LOGI("#### got context ####");}
    //g_ctx.context_obj = (*env)->NewGlobalRef(env, context);

    LOGI("BluetoothDevice_connectGatt CallObjectMethod connectGatt");
    (*env)->CallObjectMethod(env, device,
                             g_ctx.device_connect_gatt,
                             context,
                             JNI_FALSE);
    LOGI("BluetoothDevice_connectGatt done");
}

void jni_device_disconnect(jobject device) {
    LOGI("jni_device_disconnect");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    (*env)->CallVoidMethod(env, device, g_ctx.device_disconnect);
}

int jni_device_is_connected(jobject device) {
    LOGI("jni_device_is_connected");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallBooleanMethod(env, device, g_ctx.device_is_connected);
}

jobject jni_device_get_gatt(JNIEnv *env, jobject device) {
    LOGI("jni_device_get_gatt");
    if (device == NULL) {
        LOGI("jni_device_get_gatt no device");
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device, g_ctx.device_get_gatt);
}


const int* jni_device_get_gatt_services(jobject device) {
    LOGI("jni_device_get_gatt_services");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject gatt = jni_device_get_gatt(env, device);
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, gatt, g_ctx.gatt_get_gatt_services);
    // Obtain an iterator over the Set
    jclass setClass = (*env)->FindClass(env, "java/util/Set");
    jmethodID iterator =
            (*env)->GetMethodID(env, setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = (*env)->CallObjectMethod(env, set, iterator);
    // Get the Set size
    jmethodID size =
            (*env)->GetMethodID(env, setClass, "size", "()I");
    jint setSize = (*env)->CallIntMethod(env, set, size);
    size_t serviceSize = (size_t) setSize;
    LOGI("#### %d service(s) found!####", serviceSize);
    if (serviceSize <= 0) {
        return NULL;
    }

    // Get the Iterator method IDs
    jclass iteratorClass = (*env)->FindClass(env, "java/util/Iterator");
    jmethodID hasNext = (*env)->GetMethodID(env, iteratorClass, "hasNext", "()Z");
    jmethodID next =
        (*env)->GetMethodID(env, iteratorClass, "next", "()Ljava/lang/Object;");

    int *arr = calloc(serviceSize, sizeof *arr);
    size_t i = 0;
    // Iterate over the service Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Service ####");
        jobject service = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get InstanceId ####");
        jint id = (*env)->CallIntMethod(env, service, g_ctx.service_get_id);
        LOGI("id: %d", id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, service);
    }
    return arr;
}

int jni_device_get_gatt_services_size(jobject device) {
    LOGI("jni_device_get_gatt_services_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, jni_device_get_gatt(env, device), g_ctx.gatt_get_gatt_services_size);
}

jobject jni_device_create_service(jobject device, int id) {
    LOGI("jni_device_create_service");
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
    LOGI("jni_service_get_uuid");
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
    LOGI("jni_service_is_primary");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallBooleanMethod(env, service, g_ctx.service_is_primary);
}

const int* jni_service_get_gatt_characteristics(jobject service) {
    LOGI("jni_service_get_gatt_characteristics");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, service, g_ctx.service_get_gatt_characteristics);
    // Obtain an iterator over the Set
    jclass setClass = (*env)->FindClass(env, "java/util/Set");
    jmethodID iterator =
            (*env)->GetMethodID(env, setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = (*env)->CallObjectMethod(env, set, iterator);
    // Get the Set size
    jmethodID size =
            (*env)->GetMethodID(env, setClass, "size", "()I");
    jint setSize = (*env)->CallIntMethod(env, set, size);
    size_t characteristicSize = (size_t) setSize;
    LOGI("#### %d characteristic(s) found!####", characteristicSize);
    if (characteristicSize <= 0) {
        return NULL;
    }

    // Get the Iterator method IDs
    jclass iteratorClass = (*env)->FindClass(env, "java/util/Iterator");
    jmethodID hasNext = (*env)->GetMethodID(env, iteratorClass, "hasNext", "()Z");
    jmethodID next =
        (*env)->GetMethodID(env, iteratorClass, "next", "()Ljava/lang/Object;");

    int *arr = calloc(characteristicSize, sizeof *arr);
    size_t i = 0;
    // Iterate over the characteristic Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Characteristic ####");
        jobject characteristic = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get InstanceId ####");
        jint id = (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_id);
        LOGI("id: %d", id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, characteristic);
    }
    return arr;
}

int jni_service_get_gatt_characteristics_size(jobject service) {
    LOGI("jni_service_get_gatt_characteristics_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, service, g_ctx.service_get_gatt_characteristics_size);
}

jobject jni_service_create_characteristic(jobject service, int id) {
    LOGI("jni_service_create_characteristic");
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
    LOGI("jni_characteristic_get_uuid");
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
    LOGI("jni_characteristic_get_gatt_descriptors");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_gatt_descriptors);
    // Obtain an iterator over the Set
    jclass setClass = (*env)->FindClass(env, "java/util/Set");
    jmethodID iterator =
            (*env)->GetMethodID(env, setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iter = (*env)->CallObjectMethod(env, set, iterator);
    // Get the Set size
    jmethodID size =
            (*env)->GetMethodID(env, setClass, "size", "()I");
    jint setSize = (*env)->CallIntMethod(env, set, size);
    size_t descriptorSize = (size_t) setSize;
    LOGI("#### %d descriptor(s) found!####", descriptorSize);
    if (descriptorSize <= 0) {
        return NULL;
    }

    // Get the Iterator method IDs
    jclass iteratorClass = (*env)->FindClass(env, "java/util/Iterator");
    jmethodID hasNext = (*env)->GetMethodID(env, iteratorClass, "hasNext", "()Z");
    jmethodID next =
        (*env)->GetMethodID(env, iteratorClass, "next", "()Ljava/lang/Object;");

    int *arr = calloc(descriptorSize, sizeof *arr);
    size_t i = 0;
    // Iterate over the descriptor Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Descriptor ####");
        jobject descriptor = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get InstanceId ####");
        jint id = (*env)->CallIntMethod(env, descriptor, g_ctx.descriptor_get_id);
        LOGI("id: %d", id);
        arr[i++] = (int) id;
        (*env)->DeleteLocalRef(env, descriptor);
    }
    return arr;
}

int jni_characteristic_get_gatt_descriptors_size(jobject characteristic) {
    LOGI("jni_characteristic_get_gatt_descriptors_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_gatt_descriptors_size);
}

const int* jni_characteristic_get_value(jobject characteristic) {
    LOGI("jni_characteristic_get_value");
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
    LOGI("jni_characteristic_get_value_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_value_size);
}

const int* jni_characteristic_read_value(jobject characteristic) {
    LOGI("jni_characteristic_read_value");
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
    LOGI("jni_characteristic_write_value");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    jintArray arr = (*env)->NewIntArray(env, length);
    (*env)->SetIntArrayRegion(env, arr, 0, length, values);
    (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_write_value, arr);
}

jobject jni_characteristic_create_descriptor(jobject characteristic, int id) {
    LOGI("jni_characteristic_create_descriptor");
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
    LOGI("jni_descriptor_get_uuid");
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
    LOGI("jni_descriptor_get_value");
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
    LOGI("jni_descriptor_get_value_size");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, descriptor, g_ctx.descriptor_get_value_size);
}

const int* jni_descriptor_read_value(jobject descriptor) {
    LOGI("jni_descriptor_read_value");
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
    LOGI("jni_descriptor_write_value");
    JNIEnv *env = jni_get_env();
    if (env == NULL) {
        return;
    }
    jintArray arr = (*env)->NewIntArray(env, length);
    (*env)->SetIntArrayRegion(env, arr, 0, length, values);
    (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_write_value, arr);
}
