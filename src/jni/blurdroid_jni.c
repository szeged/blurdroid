#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>

static const char* kTAG = "### blurdroid ###";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

typedef int bool;
#define true 1
#define false 0

typedef unsigned char byte;

typedef struct {
    // jvm
    JavaVM *javaVM;
    // main activity
    jclass main_activity_cls;
    //jobject main_activity_obj;
    // class loader
    jclass class_loader_cls;
    jclass class_cls;
    jobject class_loader_obj;
    jmethodID find_class;
    jmethodID get_class_loader;
    // app
    //jclass app_global_cls;
    //jobject app_global_obj;
    //jmethodID get_initial_application;
    //jmethodID get_application_context;
    // context
    //jclass context_cls;
    //jobject context_obj;
    // adapter
    jclass adapter_cls;
    jobject adapter_obj;
    jmethodID adapter_get_adapter;
    jmethodID adapter_get_address;
    jmethodID adapter_get_name;
    jmethodID adapter_get_bonded_devices;
    jmethodID adapter_get_devices;
    jmethodID adapter_get_devices_size;
    jmethodID adapter_get_remote_device;
    jmethodID adapter_is_enabled;
    jmethodID adapter_enable;
    jmethodID adapter_disable;
    jmethodID adapter_start_le_scan;
    jmethodID adapter_stop_le_scan;
    // device
    jclass device_cls;
    //jobject device_obj;
    jmethodID device_get_address;
    jmethodID device_get_name;
    jmethodID device_connect_gatt;
    jmethodID device_get_gatt;
    jmethodID device_get_service;
    jmethodID device_get_characteristic;
    jmethodID device_get_descriptor;
    jmethodID device_get_services_size;
    jmethodID device_get_characteristics_size;
    jmethodID device_get_descriptors_size;
    // gatt
    jclass gatt_cls;
    //jobject gatt_obj;
    jmethodID gatt_connect;
    jmethodID gatt_disconnect;
    jmethodID gatt_discover_services;
    jmethodID gatt_get_service;
    jmethodID gatt_get_service_id;
    jmethodID gatt_get_services;
    jmethodID gatt_read_characteristic;
    jmethodID gatt_read_descriptor;
    jmethodID gatt_write_characteristic;
    jmethodID gatt_write_descriptor;
    // gatt service
    jclass service_cls;
    //jobject service_obj;
    jmethodID service_get_uuid;
    jmethodID service_get_instance_id;
    jmethodID service_get_type;
    jmethodID service_is_primary;
    jmethodID service_get_characteristic;
    jmethodID service_get_characteristic_id;
    jmethodID service_get_characteristics;
    jmethodID service_get_included_services;
    // gatt characteristic
    jclass characteristic_cls;
    //jobject characteristic_obj;
    jmethodID characteristic_get_uuid;
    jmethodID characteristic_get_instance_id;
    jmethodID characteristic_get_descriptor;
    jmethodID characteristic_get_descriptors;
    jmethodID characteristic_get_value;
    jmethodID characteristic_set_value;
    // gatt descriptor
    jclass descriptor_cls;
    //jobject descriptor_obj;
    jmethodID descriptor_get_uuid;
    jmethodID descriptor_get_value;
    jmethodID descriptor_set_value;
} BTContext;

BTContext g_ctx;

JNIEnv* Helper_getEnv() {
    LOGI("Helper_getEnv");
    JNIEnv *env;
    JavaVM *javaVM = g_ctx.javaVM;
    jint res = (*javaVM)->GetEnv(javaVM, (void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        LOGI("Helper_AttachCurrentThread");
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (res != JNI_OK) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return NULL;
        }
    }
    return env;
}

void Helper_setClassLoader(JNIEnv *env) {
    LOGI("Helper_setClassLoader");

    LOGI("Helper_setClassLoader find main activity class");
    g_ctx.main_activity_cls = (*env)->FindClass(env, "com/mozilla/servo/MainActivity");
    if(!g_ctx.main_activity_cls) {LOGE("#### failed to get g_ctx.main_activity_cls");}

    LOGI("Helper_setClassLoader get object class");
    g_ctx.class_cls = (*env)->GetObjectClass(env, g_ctx.main_activity_cls);
    if(!g_ctx.class_cls) {LOGE("#### failed to get g_ctx.class_cls");}

    LOGI("Helper_setClassLoader find class loader class");
    g_ctx.class_loader_cls = (*env)->FindClass(env, "java/lang/ClassLoader");
    if(!g_ctx.class_loader_cls) {LOGE("#### failed to get g_ctx.class_loader_cls");}

    LOGI("Helper_setClassLoader get getClassLoader method");
    g_ctx.get_class_loader = (*env)->GetMethodID(env, g_ctx.class_cls, "getClassLoader",
                                                 "()Ljava/lang/ClassLoader;");
    if(!g_ctx.get_class_loader) {LOGE("#### failed to get g_ctx.get_class_loader");}

    LOGI("Helper_setClassLoader call getClassLoader on MainActivity");
    jobject classLoaderObj = (*env)->CallObjectMethod(env,
                                                      g_ctx.main_activity_cls,
                                                      g_ctx.get_class_loader);
    if(!classLoaderObj) {LOGE("#### failed to get classLoaderObj");}
    g_ctx.class_loader_obj = (*env)->NewGlobalRef(env, classLoaderObj);
    if(!g_ctx.class_loader_obj) {LOGE("#### failed to get g_ctx.class_loader_obj");}

    LOGI("Helper_setClassLoader get findClass method");
    g_ctx.find_class = (*env)->GetMethodID(env, g_ctx.class_loader_cls,
                                           "findClass",
                                           "(Ljava/lang/String;)Ljava/lang/Class;");
    if(!g_ctx.find_class) {LOGE("#### failed to get g_ctx.find_class");}
}

jclass Helper_findClass(JNIEnv *env, const char* name) {
    LOGI("Helper_findClass");
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

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }

    Helper_setClassLoader(env);

    return JNI_VERSION_1_6;
}

void BluetoothAdapter_init() {
    LOGI("Init init");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return;
    }
    // Adapter class
    LOGI("Init adapter class");
    jclass classBta = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothAdapterWrapper");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // Adapter getAdapter
    LOGI("Init adapter getAdapter");
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getAdapter",
                                  "()Lhu/uszeged/bluetooth/BluetoothAdapterWrapper;");

    // Adapter object
    LOGI("Init adapter object");
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);
    g_ctx.adapter_obj = (*env)->NewGlobalRef(env, objBta);

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

    // Adapter getBondedDevices
    LOGI("Init adapter getBondedDevices");
    g_ctx.adapter_get_bonded_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getBondedDevices",
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

    // Adapter isEnabled
    LOGI("Init adapter isEnabled");
    g_ctx.adapter_is_enabled =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "isEnabled",
                            "()Z");

    // Adapter enable
    LOGI("Init adapter enable");
    g_ctx.adapter_enable =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "enable",
                            "()Z");

    // Adapter disable
    LOGI("Init adapter disable");
    g_ctx.adapter_disable =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "disable",
                            "()Z");

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
    jclass classBtd = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothDeviceWrapper");
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

    // Device connectGatt
    LOGI("Init device connectGatt");
    g_ctx.device_connect_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "connectGatt",
                            "(Landroid/content/Context;Z)"
                            "Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Device getGatt
    LOGI("Init device getGatt");
    g_ctx.device_get_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getGatt",
                            "()Lhu/uszeged/bluetooth/BluetoothGattWrapper;");

    // Device getService
    LOGI("Init device getService");
    g_ctx.device_get_service =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getService",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");

    // Device getCharacteristic
    LOGI("Init device getCharacteristic");
    g_ctx.device_get_characteristic =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getCharacteristic",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");

    // Device getDescriptor
    LOGI("Init device getDescriptor");
    g_ctx.device_get_descriptor =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getDescriptor",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;");

    // Device getServicesSize
    LOGI("Init device getServicesSize");
    g_ctx.device_get_services_size =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getServicesSize",
                            "()"
                            "I");

    // Device getCharacteristicsSize
    LOGI("Init device getCharacteristicsSize");
    g_ctx.device_get_characteristics_size =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getCharacteristicsSize",
                            "()"
                            "I");

    // Device getDescriptorsSize
    LOGI("Init device getDescriptorsSize");
    g_ctx.device_get_descriptors_size =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getDescriptorsSize",
                            "()"
                            "I");

    // Gatt class
    LOGI("Init gatt class");
    jclass classBtg = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattWrapper");
    g_ctx.gatt_cls = (*env)->NewGlobalRef(env, classBtg);

    // Gatt connect
    LOGI("Init gatt connect");
    g_ctx.gatt_connect =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "connect",
                            "()Z");

    // Gatt disconnect
    LOGI("Init gatt disconnect");
    g_ctx.gatt_disconnect =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "disconnect",
                            "()V");

    // Gatt discoverServices
    LOGI("Init gatt discoverServices");
    g_ctx.gatt_discover_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "discoverServices",
                            "()Z");

    // Gatt getService
    LOGI("Init gatt getService");
    g_ctx.gatt_get_service =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getService",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");

    // Gatt getService
    LOGI("Init gatt getService id");
    g_ctx.gatt_get_service_id =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getService",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattServiceWrapper;");


    // Gatt getServices
    LOGI("Init gatt getServices");
    g_ctx.gatt_get_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServices",
                            "()Ljava/util/Set;");

    // Gatt readCharacteristic
    LOGI("Init gatt readCharacteristic");
    g_ctx.gatt_read_characteristic =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "readCharacteristic",
                            "(Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;)"
                            "Z");

    // Gatt writeCharacteristic
    LOGI("Init gatt writeCharacteristic");
    g_ctx.gatt_write_characteristic =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "writeCharacteristic",
                            "(Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;)"
                            "Z");

    // Gatt readDescriptor
    LOGI("Init gatt readDescriptor");
    g_ctx.gatt_read_descriptor =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "readDescriptor",
                            "(Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;)"
                            "Z");

    // Gatt writeDescriptor
    LOGI("Init gatt writeDescriptor");
    g_ctx.gatt_write_descriptor =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "writeDescriptor",
                            "(Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;)"
                            "Z");

    // GattService class
    LOGI("Init service class");
    jclass classBtgs = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattServiceWrapper");
    g_ctx.service_cls = (*env)->NewGlobalRef(env, classBtgs);

    // GattService getUuid
    LOGI("Init service getUuid");
    g_ctx.service_get_uuid =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattService getInstanceId
    LOGI("Init service getInstanceId");
    g_ctx.service_get_instance_id =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getInstanceId",
                            "()I");

    // GattService getType
    LOGI("Init service getType");
    g_ctx.service_get_type =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getType",
                            "()I");

    // GattService isPrimary
    LOGI("Init service isPrimary");
    g_ctx.service_is_primary =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "isPrimary",
                            "()Z");

    // GattService getCharacteristic
    LOGI("Init service getCharacteristic");
    g_ctx.service_get_characteristic =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristic",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");

    // GattService getCharacteristic
    LOGI("Init service getCharacteristic id");
    g_ctx.service_get_characteristic_id =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristic",
                            "(I)"
                            "Lhu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper;");


    // GattService getCharacteristics
    LOGI("Init service getCharacteristics");
    g_ctx.service_get_characteristics =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getCharacteristics",
                            "()Ljava/util/Set;");

    // GattService getServices
    LOGI("Init service getIncludedServices");
    g_ctx.service_get_included_services =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getIncludedServices",
                            "()Ljava/util/Set;");

    // GattCharacteristic class
    LOGI("Init characteristic class");
    jclass classBtc = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattCharacteristicWrapper");
    g_ctx.characteristic_cls = (*env)->NewGlobalRef(env, classBtc);

    // GattCharacteristic getUuid
    LOGI("Init characteristic getUuid");
    g_ctx.characteristic_get_uuid =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattCharacteristic getInstanceId
    LOGI("Init characteristic getInstanceId");
    g_ctx.characteristic_get_instance_id =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getInstanceId",
                            "()I");

    // GattCharacteristic getDescriptor
    LOGI("Init characteristic getDescriptor");
    g_ctx.characteristic_get_descriptor =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptor",
                            "(Ljava/lang/String;)"
                            "Lhu/uszeged/bluetooth/BluetoothGattDescriptorWrapper;");

    // GattCharacteristic getDescriptors
    LOGI("Init characteristic getDescriptors");
    g_ctx.characteristic_get_descriptors =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getDescriptors",
                            "()Ljava/util/Set;");

    // GattCharacteristic getValue
    LOGI("Init characteristic getValue");
    g_ctx.characteristic_get_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "getValue",
                            "()[B");

    // GattCharacteristic setValue
    LOGI("Init characteristic setValue");
    g_ctx.characteristic_set_value =
        (*env)->GetMethodID(env, g_ctx.characteristic_cls,
                            "setValue",
                            "([B)Z");

    // GattDescriptor class
    LOGI("Init descriptor class");
    jclass classBtdsc = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattDescriptorWrapper");
    g_ctx.descriptor_cls = (*env)->NewGlobalRef(env, classBtdsc);

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
                            "()[B");

    // GattDescriptor setValue
    LOGI("Init descriptor setValue");
    g_ctx.descriptor_set_value =
        (*env)->GetMethodID(env, g_ctx.descriptor_cls,
                            "setValue",
                            "([B)Z");

    LOGI("Init end");
}

bool BluetoothAdapter_enable() {
    LOGI("BluetoothAdapter_enable");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.adapter_obj, g_ctx.adapter_enable);
}

bool BluetoothAdapter_disable() {
    LOGI("BluetoothAdapter_disable");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.adapter_obj, g_ctx.adapter_disable);
}

bool BluetoothAdapter_isEnabled() {
    LOGI("BluetoothAdapter_isEnabled");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.adapter_obj, g_ctx.adapter_is_enabled);
}

const char* BluetoothAdapter_getAddress() {
    LOGI("BluetoothAdapter_getAddress");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jstring strAddress = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_address);
    const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strAddress);
    return address;
}

const char* BluetoothAdapter_getName() {
    LOGI("BluetoothAdapter_getName");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

const char** BluetoothAdapter_getDevices() {
    LOGI("BluetoothAdapter_getDevices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_devices);
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

int BluetoothAdapter_getDevicesSize() {
    LOGI("BluetoothAdapter_getDevicesSize");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_devices_size);
}

void BluetoothAdapter_startDiscovery() {
    LOGI("BluetoothAdapter_startDiscovery");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    (*env)->CallVoidMethod(env, g_ctx.adapter_obj, g_ctx.adapter_start_le_scan);
}

void BluetoothAdapter_stopDiscovery() {
    LOGI("BluetoothAdapter_stopDiscovery");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    (*env)->CallVoidMethod(env, g_ctx.adapter_obj, g_ctx.adapter_stop_le_scan);
}

void Helper_freeCharArray(char** arr, size_t num_elements) {
    LOGI("Helper_freeCharArray");
    size_t i;
    for (i = 0; i < num_elements; i++ ) {
        free(arr[i]);
    }
    free(arr);
}

void Helper_freeUChar(unsigned char* arr) {
    LOGI("Helper_freeUChar");
    free(arr);
}

void Helper_freeIntArray(int* arr) {
    LOGI("Helper_freeIntArray");
    free(arr);
}

jobject BluetoothAdapter_getRemoteDevice(JNIEnv *env, const char* address) {
    LOGI("BluetoothAdapter_getRemoteDevice %s", address);
    /*JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }*/
    return
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj,
                                 g_ctx.adapter_get_remote_device,
                                 (*env)->NewStringUTF(env, address));
}

const char* BluetoothDevice_getName(const char* address) {
    LOGI("BluetoothDevice_getName");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, device, g_ctx.device_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    LOGI("#### Device Name: %s", name);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

void BluetoothDevice_connectGatt(const char* address) {
    LOGI("BluetoothDevice_connectGatt");
    JNIEnv *env = Helper_getEnv();
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

    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return;
    }

    LOGI("BluetoothDevice_connectGatt CallObjectMethod connectGatt");
    //jobject gatt = (jobject)
        (*env)->CallObjectMethod(env, device,
                                 g_ctx.device_connect_gatt,
                                 context,
                                 JNI_FALSE);
    //if(!gatt) {LOGE("#### failed to get gatt");}
    //g_ctx.gatt_obj = (*env)->NewGlobalRef(env, gatt);
    LOGI("BluetoothDevice_connectGatt done");
}

// This will be called on the java side
/*bool BluetoothGatt_discoverServices() {
    LOGI("BluetoothGatt_discoverServices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.gatt_obj, g_ctx.gatt_discover_services);
}*/

jobject BluetoothDevice_getGatt(JNIEnv *env, const char* address) {
    LOGI("BluetoothDevice_getGatt");
    /*JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }*/
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device, g_ctx.device_get_gatt);
}

jobject BluetoothDevice_getService(JNIEnv *env, const char* address, int id) {
    LOGI("BluetoothDevice_getService");
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device,
                                    g_ctx.device_get_service,
                                    (*env)->NewStringUTF(env, id));
}

jobject BluetoothDevice_getCharacteristic(JNIEnv *env, const char* address, int id) {
    LOGI("BluetoothDevice_getCharacteristic");
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device,
                                    g_ctx.device_get_characteristic,
                                    (*env)->NewStringUTF(env, id));
}

jobject BluetoothDevice_getDescriptor(JNIEnv *env, const char* address, const char* uuid) {
    LOGI("BluetoothDevice_getDescriptor");
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }
    return (*env)->CallObjectMethod(env, device,
                                    g_ctx.device_get_descriptor,
                                    (*env)->NewStringUTF(env, uuid));
}

int BluetoothDevice_getServicesSize(const char* address) {
    LOGI("BluetoothDevice_getServicesSize");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return 0;
    }
    return (*env)->CallIntMethod(env, device, g_ctx.device_get_services_size);
}

int BluetoothDevice_getCharacteristicsSize(const char* address) {
    LOGI("BluetoothDevice_getCharacteristicsSize");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return 0;
    }
    return (*env)->CallIntMethod(env, device, g_ctx.device_get_characteristics_size);
}

int BluetoothDevice_getDescriptorsSize(const char* address) {
    LOGI("BluetoothDevice_getDescriptorsSize");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    jobject device = BluetoothAdapter_getRemoteDevice(env, address);
    if (device == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return 0;
    }
    return (*env)->CallIntMethod(env, device, g_ctx.device_get_descriptors_size);
}

const int* BluetoothGatt_getServices(const char* address) {
    LOGI("BluetoothGatt_getServices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject gatt = BluetoothDevice_getGatt(env, address);
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, gatt, g_ctx.gatt_get_services);
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

    //const char* services[serviceSize];
    //int services[serviceSize];
    int *arr = calloc(serviceSize, sizeof *arr);
    size_t i = 0;
    // Iterate over the service Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Service ####");
        jobject service = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get InstanceId ####");
        //jstring strUuid = (jstring)
        //    (*env)->CallObjectMethod(env, service, g_ctx.service_get_uuid);
        //const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
        //LOGI("#### Service UUID: %s", uuid);
        //services[i++] = uuid;
        //(*env)->ReleaseStringUTFChars(env, strUuid, address);
        //(*env)->DeleteLocalRef(env, service);
        //(*env)->DeleteLocalRef(env, strUuid);
        jint instanceId = (*env)->CallIntMethod(env, service, g_ctx.service_get_instance_id);
        LOGI("InstanceId: %d", instanceId);
        //services[i++] = (int) instanceId;
        arr[i++] = (int) instanceId;
        (*env)->DeleteLocalRef(env, service);
    }
    //return NULL;
    //return services;
    return arr;
}

// TODO (dis)connect, read/write

const char* BluetoothGattService_getUuid(const char* address, const int id) {
    LOGI("BluetoothGattService_getUuid");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject service = BluetoothDevice_getService(env, address, id);
    if (service == NULL) {
        return NULL;
    }
    jstring strUuid = (jstring)
        (*env)->CallObjectMethod(env, service, g_ctx.service_get_uuid);
    return (*env)->GetStringUTFChars(env, strUuid, NULL);
}

int BluetoothGattService_getType(const char* address, const int id) {
    LOGI("BluetoothGattService_getType");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return 0;
    }
    jobject service = BluetoothDevice_getService(env, address, id);
    if (service == NULL) {
        return 0;
    }
    return (*env)->CallIntMethod(env, service, g_ctx.service_get_type);
}

bool BluetoothGattService_isPrimary(const char* address, const int id) {
    LOGI("BluetoothGattService_isPrimary");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    jobject service = BluetoothDevice_getService(env, address, id);
    if (service == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, service, g_ctx.service_is_primary);
}

const int* BluetoothGattService_getCharacteristics(const char* address, const int id) {
    LOGI("BluetoothGattService_getCharacteristics");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject service = BluetoothDevice_getService(env, address, id);
    if (service == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, service, g_ctx.service_get_characteristics);
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

    //int characteristics[characteristicSize];
    int *arr = calloc(characteristicSize, sizeof *arr);
    size_t i = 0;
    // Iterate over the characteristic Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Characteristic ####");
        jobject characteristic = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get InstanceId ####");
        jint instanceId = (*env)->CallIntMethod(env, characteristic, g_ctx.characteristic_get_instance_id);
        LOGI("InstanceId: %d", instanceId);
        //characteristics[i++] = (int) instanceId;
        arr[i++] = (int) instanceId;
        (*env)->DeleteLocalRef(env, characteristic);
    }
    //return characteristics;
    return arr;
}

const char* BluetoothGattCharacteristic_getUuid(const char* address, const int id) {
    LOGI("BluetoothGattCharacteristic_getUuid");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject characteristic = BluetoothDevice_getCharacteristic(env, address, id);
    if (characteristic == NULL) {
        return NULL;
    }
    jstring strUuid = (jstring)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_uuid);
    return (*env)->GetStringUTFChars(env, strUuid, NULL);
}

/*const unsigned char* BluetoothGattCharacteristic_getValue(const char* address, const int id) {
    LOGI("BluetoothGattCharacteristic_getValue");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject characteristic = BluetoothDevice_getCharacteristic(env, address, id);
    if (characteristic == NULL) {
        return NULL;
    }
    jbyteArray array = (jbyteArray)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_value);

    int len = (*env)->GetArrayLength (env, array);
    jboolean isCopy;
    unsigned char* buf = (const unsigned char*) (*env)->GetByteArrayElements(env, array, &isCopy);
    //if (content_array == NULL)
    //return 0;
    unsigned char* result = calloc(len, sizeof *result);
    memcpy(result, buf, strlen(buf)+1);
    if (isCopy) {
        (*env)->ReleaseByteArrayElements(env, array, buf, JNI_ABORT);
    }
    return result;
}*/

/*void BluetoothGattCharacteristic_setValue(const char* address, const int id, const unsigned char* values) {
}*/

const char** BluetoothGattCharacteristic_getDescriptors(const char* address, const int id) {
    LOGI("BluetoothGattCharacteristic_getDescriptors");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject characteristic = BluetoothDevice_getCharacteristic(env, address, id);
    if (characteristic == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, characteristic, g_ctx.characteristic_get_descriptors);
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

    //const char* descriptors[descriptorSize];
    const char **arr = (const char**) calloc(descriptorSize, sizeof(char*));
    size_t i = 0;
    for ( i = 0; i < descriptorSize; i++ )
    {
        arr[i] = (char*) calloc(40, sizeof(char));
    }

    i = 0;
    // Iterate over the characteristic Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Descriptor ####");
        jobject descriptor = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get UUID ####");
        jstring strUuid = (jstring)
            (*env)->CallObjectMethod(env, descriptor, g_ctx.descriptor_get_uuid);
        const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
        LOGI("UUID: %s", uuid);
        //descriptors[i++] = uuid;
        arr[i++] = uuid;
        (*env)->ReleaseStringUTFChars(env, strUuid, uuid);
        (*env)->DeleteLocalRef(env, descriptor);
        (*env)->DeleteLocalRef(env, strUuid);
    }
    //return descriptors;
    return arr;
}
