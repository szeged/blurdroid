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
