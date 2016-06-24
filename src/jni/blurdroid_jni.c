#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>

static const char* kTAG = "blurdroid";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

typedef int bool;
#define true 1
#define false 0

typedef struct {
    // jvm
    JavaVM *javaVM;
    // main activity
    jclass main_activity_cls;
    jobject main_activity_obj;
    // class loader
    jclass class_loader_cls;
    jclass class_cls;
    jobject class_loader_obj;
    jmethodID find_class;
    jmethodID get_class_loader;
    // app
    jclass app_global_cls;
    jobject app_global_obj;
    jmethodID get_initial_application;
    jmethodID get_application_context;
    // context
    jclass context_cls;
    jobject context_obj;
    // adapter
    jclass adapter_cls;
    jobject adapter_obj;
    jmethodID adapter_get_adapter;
    jmethodID adapter_get_address;
    jmethodID adapter_get_name;
    jmethodID adapter_get_devices;
    jmethodID adapter_get_remote_device;
    jmethodID adapter_is_enabled;
    jmethodID adapter_enable;
    jmethodID adapter_disable;
    // device
    jclass device_cls;
    jobject device_obj;
    jmethodID device_get_address;
    jmethodID device_get_name;
    jmethodID device_connect_gatt;
    // gatt
    jclass gatt_cls;
    jobject gatt_obj;
    jmethodID gatt_discover_services;
    jmethodID gatt_get_services;
    // gatt service
    jclass service_cls;
    jobject service_obj;
    jmethodID service_get_uuid;
    // gatt callback
    //jclass gatt_callback_cls;
    //jobject gatt_callback_obj;
} BTContext;

BTContext g_ctx;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }

    //BluetoothAdapter_init(env);
    Helper_setClassLoader(env);

    return JNI_VERSION_1_6;
}

//TODO fix this
int Helper_arraySize(const char** array) {
    LOGI("Helper_arraySize");
    if (array == NULL || array[0] == NULL || (int) array[0] == 0) {
        return 0;
    }
    LOGI("array size: %d", (int)( sizeof(array) / sizeof(array[0])));
    return (int)( sizeof(array) / sizeof(array[0]) );
}

JNIEnv* Helper_getEnv() {
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

bool Helper_setRemoteDevice(const char* address) {
    LOGI("Helper_setRemoteDevice");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }

    if (g_ctx.device_obj != NULL) {
        LOGI("Helper_setRemoteDevice remove current device");
        (*env)->DeleteGlobalRef(env, g_ctx.device_obj);
        g_ctx.device_obj = NULL;
    }

    LOGI("Helper_setRemoteDevice set device address: %s", address);
    jstring strAddress = (*env)->NewStringUTF(env, address);
    jobject objBtd =
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_remote_device, strAddress);
    g_ctx.device_obj = (*env)->NewGlobalRef(env, objBtd);
    (*env)->DeleteLocalRef(env, strAddress);
    LOGI("Helper_setRemoteDevice result: %s", g_ctx.device_obj != NULL ? "success" : "failure");
    return g_ctx.device_obj != NULL;
}

void BluetoothAdapter_init() {
    LOGI("BluetoothAdapterInit init");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return;
    }
    // Adapter class
    LOGI("BluetoothAdapterInit adapter class");
    jclass classBta = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothAdapterAndroid");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // Adapter getAdapter
    LOGI("BluetoothAdapterInit adapter getAdapter");
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getAdapter",
                                  "()Lhu/uszeged/bluetooth/BluetoothAdapterAndroid;");

    // Adapter object
    LOGI("BluetoothAdapterInit adapter object");
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);
    g_ctx.adapter_obj = (*env)->NewGlobalRef(env, objBta);

    // Adapter getAddress
    LOGI("BluetoothAdapterInit adapter getAddress");
    g_ctx.adapter_get_address =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Adapter getName
    LOGI("BluetoothAdapterInit adapter getName");
    g_ctx.adapter_get_name =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Adapter getBondedDevices
    LOGI("BluetoothAdapterInit adapter getDevices");
    g_ctx.adapter_get_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getDevices",
                            "()Ljava/util/Set;");

    // Adapter getRemoteDevice
    LOGI("BluetoothAdapterInit adapter getRemoteDevice");
    g_ctx.adapter_get_remote_device =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getRemoteDevice",
                            "(Ljava/lang/String;)Lhu/uszeged/bluetooth/BluetoothDeviceAndroid;");

    // Adapter isEnabled
    LOGI("BluetoothAdapterInit adapter isEnabled");
    g_ctx.adapter_is_enabled =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "isEnabled",
                            "()Z");

    // Adapter enable
    LOGI("BluetoothAdapterInit adapter enable");
    g_ctx.adapter_enable =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "enable",
                            "()Z");

    // Adapter disable
    LOGI("BluetoothAdapterInit adapter disable");
    g_ctx.adapter_disable =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "disable",
                            "()Z");

    // Device class
    LOGI("BluetoothAdapterInit device class");
    jclass classBtd = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothDeviceAndroid");
    g_ctx.device_cls = (*env)->NewGlobalRef(env, classBtd);

    // Device getAddress
    LOGI("BluetoothAdapterInit device getAddress");
    g_ctx.device_get_address =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // Device getName
    LOGI("BluetoothAdapterInit device getName");
    g_ctx.device_get_name =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // Device connectGatt
    LOGI("BluetoothAdapterInit device connectGatt");
    g_ctx.device_connect_gatt =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "connectGatt",
                            "(Landroid/content/Context;Z)"
                            "Lhu/uszeged/bluetooth/BluetoothGattAndroid;");

    // Gatt class
    LOGI("BluetoothAdapterInit gatt class");
    jclass classBtg = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattAndroid");
    g_ctx.gatt_cls = (*env)->NewGlobalRef(env, classBtg);

    // Gatt discoverServices
    LOGI("BluetoothAdapterInit gatt discoverServices");
    g_ctx.gatt_discover_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "discoverServices",
                            "()Z");

    // Gatt getServices
    LOGI("BluetoothAdapterInit gatt getServices");
    g_ctx.gatt_get_services =
        (*env)->GetMethodID(env, g_ctx.gatt_cls,
                            "getServices",
                            "()Ljava/util/Set;");

    // GattService class
    LOGI("BluetoothAdapterInit service class");
    jclass classBtgs = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattServiceAndroid");
    g_ctx.service_cls = (*env)->NewGlobalRef(env, classBtgs);

    // Gatt getUuid
    LOGI("BluetoothAdapterInit gatt getUuid");
    g_ctx.service_get_uuid =
        (*env)->GetMethodID(env, g_ctx.service_cls,
                            "getUuid",
                            "()Ljava/lang/String;");

    // GattCallback class
    //LOGI("BluetoothAdapterInit gatt_callback class");
    //jclass classBtgc = Helper_findClass(env, "hu/uszeged/bluetooth/BluetoothGattCallbackAndroid");
    //g_ctx.gatt_callback_cls = (*env)->NewGlobalRef(env, classBtgc);

    LOGI("BluetoothAdapterInit end");
}

bool BluetoothAdapter_enable() {
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.adapter_obj, g_ctx.adapter_enable);
}

bool BluetoothAdapter_disable() {
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return false;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.adapter_obj, g_ctx.adapter_disable);
}

bool BluetoothAdapter_isEnabled() {
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

    const char* devices[deviceSize];
    size_t i = 0;
    // Iterate over the device Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        jobject device = (*env)->CallObjectMethod(env, iter, next);
        jstring strAddress = (jstring)
            (*env)->CallObjectMethod(env, device, g_ctx.device_get_address);
        const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
        LOGI("#### Device Address: %s", address);
        devices[i++] = address;
        //(*env)->ReleaseStringUTFChars(env, strAddress, address);
        (*env)->DeleteLocalRef(env, device);
        (*env)->DeleteLocalRef(env, strAddress);
    }
    //return NULL;
    return devices;
}

const char* BluetoothDevice_getAddress() {
    LOGI("BluetoothDevice_getAddress");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    if (g_ctx.device_obj == NULL) {
        LOGI("BluetoothDevice_getAddress no device");
        return NULL;
    }

    jstring strAddress = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.device_obj, g_ctx.device_get_address);
    const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
    LOGI("#### Device Address: %s", address);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strAddress);
    return address;
}

const char* BluetoothDevice_getName() {
    LOGI("BluetoothDevice_getName");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.device_obj, g_ctx.device_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    LOGI("#### Device Name: %s", name);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

void BluetoothDevice_connectGatt() {
    LOGI("BluetoothDevice_connectGatt");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return;
    }
    jclass theClass = (*env)->FindClass(env,"android/app/ActivityThread");
    if(!theClass){LOGE("#### failed to get android/app/ActivityThread ####");}
    else{LOGI("#### got android/app/ActivityThread = %p ####", theClass);}

    jmethodID method = (*env)->GetStaticMethodID(env,theClass,"currentApplication","()Landroid/app/Application;");
    if(!method){LOGE("#### failed to get currentApplication ####");}
    else{LOGI("#### got currentApplication = %ld ####",(long) method);}

    jobject context = (*env)->CallStaticObjectMethod(env,theClass,method);
    if(!context){LOGE("#### failed to get context ####");}
    else{LOGI("#### got context ####");}
    g_ctx.context_obj = (*env)->NewGlobalRef(env, context);

    /*LOGI("BluetoothDevice_connectGatt GetMethodID ctor");
    jmethodID ctor = (*env)->GetMethodID(env, g_ctx.gatt_callback_cls, "<init>", "()V");
    if(!ctor) {LOGE("#### failed to get ctor");}
    LOGI("BluetoothDevice_connectGatt NewObject callback");
    jobject callback = (*env)->NewObject(env, g_ctx.gatt_callback_cls, ctor);
    if(!callback) {LOGE("#### failed to get callback");}
    LOGI("BluetoothDevice_connectGatt NewGlobalRef callback");
    g_ctx.gatt_callback_obj = (*env)->NewGlobalRef(env, callback);*/
    /*LOGI("BluetoothDevice_connectGatt CallObjectMethod connectGatt");
    jobject gatt = (jobject)
        (*env)->CallObjectMethod(env, g_ctx.device_obj,
                                 g_ctx.device_connect_gatt,
                                 g_ctx.context_obj,
                                 JNI_FALSE,
                                 g_ctx.gatt_callback_obj);
    if(!gatt) {LOGE("#### failed to get gatt");}
    g_ctx.gatt_obj = (*env)->NewGlobalRef(env, gatt);
    LOGI("BluetoothDevice_connectGatt done");*/
    LOGI("BluetoothDevice_connectGatt CallObjectMethod connectGatt");
    jobject gatt = (jobject)
        (*env)->CallObjectMethod(env, g_ctx.device_obj,
                                 g_ctx.device_connect_gatt,
                                 g_ctx.context_obj,
                                 JNI_FALSE);
    if(!gatt) {LOGE("#### failed to get gatt");}
    g_ctx.gatt_obj = (*env)->NewGlobalRef(env, gatt);
    LOGI("BluetoothDevice_connectGatt done");
}

bool BluetoothGatt_discoverServices() {
    LOGI("BluetoothGatt_discoverServices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    return (*env)->CallBooleanMethod(env, g_ctx.gatt_obj, g_ctx.gatt_discover_services);
}

const char** BluetoothGatt_getServices() {
    LOGI("BluetoothGatt_getServices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, g_ctx.gatt_obj, g_ctx.gatt_get_services);
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

    const char* services[serviceSize];
    size_t i = 0;
    // Iterate over the service Set
    while ((*env)->CallBooleanMethod(env, iter, hasNext)) {
        LOGI("#### Get Service ####");
        jobject service = (*env)->CallObjectMethod(env, iter, next);
        LOGI("#### Get UUID ####");
        jstring strUuid = (jstring)
            (*env)->CallObjectMethod(env, service, g_ctx.service_get_uuid);
        const char* uuid = (*env)->GetStringUTFChars(env, strUuid, NULL);
        LOGI("#### Service UUID: %s", uuid);
        services[i++] = uuid;
        //(*env)->ReleaseStringUTFChars(env, strUuid, address);
        (*env)->DeleteLocalRef(env, service);
        (*env)->DeleteLocalRef(env, strUuid);
    }
    //return NULL;
    return services;
}

/*JNIEXPORT void JNICALL
Java_android_bluetooth_BluetoothGattCallback_onConnectionStateChange
    (JNIEnv *env, jobject thisObj, jobject gatt, jint status, jint new_state) {
        LOGI("#!#!#onConnectionStateChange#!#!#");
        LOGI("#### staus: %d newState: %d", status, new_state);
}*/
