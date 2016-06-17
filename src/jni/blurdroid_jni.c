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
    JavaVM *javaVM;
    jclass adapter_cls;
    jobject adapter_obj;
    jmethodID adapter_get_adapter;
    jmethodID adapter_get_address;
    jmethodID adapter_get_name;
    jmethodID adapter_get_bonded_devices;
    jmethodID adapter_get_remote_device;
    jmethodID adapter_is_enabled;
    jmethodID adapter_enable;
    jmethodID adapter_disable;
    jclass device_cls;
    jobject device_obj;
    jmethodID device_get_address;
    jmethodID device_get_name;
    jmethodID device_connect_gatt;
} BTContext;

BTContext g_ctx;

int Helper_arraySize(char** array) {
    LOGI("Helper_arraySize");
    if (array == NULL || array[0] == NULL || (int) array[0] == 0) {
        return 0;
    }
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

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI version not supported.");
        return JNI_ERR; // JNI version not supported.
    }
    return JNI_VERSION_1_6;
}

void BluetoothAdapter_init() {
    LOGI("BluetoothAdapterInit init");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return;
    }
    // Adapter class
    LOGI("BluetoothAdapterInit adapter class");
    jclass classBta = (*env)->FindClass(env, "android/bluetooth/BluetoothAdapter");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // Adapter getAdapter
    LOGI("BluetoothAdapterInit adapter getAdapter");
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getDefaultAdapter",
                                  "()Landroid/bluetooth/BluetoothAdapter;");

    // Adapter object
    LOGI("BluetoothAdapterInit adapter object");
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);// miGetAdapter);
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
    LOGI("BluetoothAdapterInit adapter getBondedDevices");
    g_ctx.adapter_get_bonded_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getBondedDevices",
                            "()Ljava/util/Set;");

    // Adapter getRemoteDevice
    LOGI("BluetoothAdapterInit adapter getRemoteDevice");
    g_ctx.adapter_get_remote_device =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getRemoteDevice",
                            "(Ljava/lang/String;)Landroid/bluetooth/BluetoothDevice;");

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
    jclass classBtd = (*env)->FindClass(env, "android/bluetooth/BluetoothDevice");
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
                            "(Landroid/content/Context;"
                            "ZLandroid/bluetooth/BluetoothGattCallback;"
                            ")Landroid/bluetooth/BluetoothGatt;");
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

const char** BluetoothAdapter_getBondedDevices() {
    LOGI("BluetoothAdapter_getBondedDevices");
    JNIEnv *env = Helper_getEnv();
    if (env == NULL) {
        return NULL;
    }
    jobject set = (jobject)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_bonded_devices);
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
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}
