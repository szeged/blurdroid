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
} BTContext;

BTContext g_ctx;

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
