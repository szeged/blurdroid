#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <jni.h>
#include <android/log.h>

static const char* kTAG = "### blurdroid ###";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

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
    //jobject adapter_obj;
    jmethodID adapter_get_adapter;
    jmethodID adapter_get_address;
    jmethodID adapter_get_name;
    //jmethodID adapter_get_bonded_devices;
    jmethodID adapter_get_devices;
    jmethodID adapter_get_devices_size;
    jmethodID adapter_get_remote_device;
    //jmethodID adapter_is_enabled;
    //jmethodID adapter_enable;
    //jmethodID adapter_disable;
    jmethodID adapter_start_le_scan;
    jmethodID adapter_stop_le_scan;
    // device
    jclass device_cls;
    //jobject device_obj;
    jmethodID device_get_address;
    jmethodID device_get_name;
    /*jmethodID device_connect_gatt;
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
    jmethodID descriptor_set_value;*/
} BTContext;

BTContext g_ctx;

//void jni_adapter_create_adapter(jobject*);
jobject jni_adapter_create_adapter();
const char* jni_adapter_get_address(jobject);
const char* jni_adapter_get_name(jobject);
void jni_adapter_start_discovery(jobject);
void jni_adapter_stop_discovery(jobject);
const char** jni_adapter_get_devices(jobject);
int jni_adapter_get_devices_size(jobject);
jobject jni_adapter_create_device();
const char* jni_device_get_address(jobject);
const char* jni_device_get_name(jobject);

#endif /* JNI_UTILS_H */
