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

typedef struct {
    JavaVM *javaVM;
    jclass adapter_cls;
    jobject adapter_obj;
    jmethodID adapter_get_adapter;
    jmethodID adapter_get_address;
    jmethodID adapter_get_name;
    jmethodID adapter_get_bonded_devices;
    jclass device_cls;
    jmethodID device_get_address;
} BTContext;

BTContext g_ctx;

int Helper_arraySize(char** array) {
    if (array[0] == NULL) {
        return 0;
    }
    return (int)( sizeof(array) / sizeof(array[0]) );
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }
    return JNI_VERSION_1_6;
}

void BluetoothAdapter_init() {
    LOGI("BluetoothAdapterInit");
    JNIEnv *env;
    JavaVM *javaVM = g_ctx.javaVM;
    jint res = (*javaVM)->GetEnv(javaVM, (void**)&env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (res != JNI_OK) {
            LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
            return;
        }
    }
    // Adapter class
    LOGI("BluetoothAdapterInit Adapter class");
    jclass classBta = (*env)->FindClass(env, "android/bluetooth/BluetoothAdapter");
    g_ctx.adapter_cls = (*env)->NewGlobalRef(env, classBta);

    // getAdapter
    LOGI("BluetoothAdapterInit getAdapter");
    g_ctx.adapter_get_adapter =
        (*env)->GetStaticMethodID(env, g_ctx.adapter_cls,
                                  "getDefaultAdapter",
                                  "()Landroid/bluetooth/BluetoothAdapter;");

    // Adapter object
    LOGI("BluetoothAdapterInit object");
    jobject objBta = (jobject)
        (*env)->CallStaticObjectMethod(env, g_ctx.adapter_cls, g_ctx.adapter_get_adapter);// miGetAdapter);
    g_ctx.adapter_obj = (*env)->NewGlobalRef(env, objBta);

    // getAddress
    LOGI("BluetoothAdapterInit getAddress");
    g_ctx.adapter_get_address =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getAddress",
                            "()Ljava/lang/String;");

    // getName
    LOGI("BluetoothAdapterInit getName");
    g_ctx.adapter_get_name =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getName",
                            "()Ljava/lang/String;");

    // getBondedDevices
    LOGI("BluetoothAdapterInit getBondedDevices");
    g_ctx.adapter_get_bonded_devices =
        (*env)->GetMethodID(env, g_ctx.adapter_cls,
                            "getBondedDevices",
                            "()Ljava/util/Set;");

    // Device class
    jclass classBtd = (*env)->FindClass(env, "android/bluetooth/BluetoothDevice");
    g_ctx.device_cls = (*env)->NewGlobalRef(env, classBtd);

    // getAddress
    g_ctx.device_get_address =
        (*env)->GetMethodID(env, g_ctx.device_cls,
                            "getAddress",
                            "()Ljava/lang/String;");
}

const char* BluetoothAdapter_getAddress() {
    LOGI("BluetoothAdapter_getAddress");
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
    jstring strAddress = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_address);
    const char* address = (*env)->GetStringUTFChars(env, strAddress, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strAddress);
    return address;
}

const char* BluetoothAdapter_getName() {
    LOGI("BluetoothAdapter_getName");
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
    jstring strName = (jstring)
        (*env)->CallObjectMethod(env, g_ctx.adapter_obj, g_ctx.adapter_get_name);
    const char* name = (*env)->GetStringUTFChars(env, strName, NULL);
    //(*env)->ReleaseStringUTFChars(env, strAddress, address);
    (*env)->DeleteLocalRef(env, strName);
    return name;
}

char** BluetoothAdapter_getBondedDevices() {
    LOGI("BluetoothAdapter_getBondedDevices");
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

    char* devices[deviceSize];
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
