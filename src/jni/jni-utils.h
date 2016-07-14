#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <jni.h>
#include <android/log.h>

static const char* kTAG = "blurdroid";
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
    jmethodID device_get_address;
    jmethodID device_get_name;
    jmethodID device_connect_gatt;
    jmethodID device_disconnect;
    jmethodID device_is_connected;
    jmethodID device_get_gatt;
    // gatt
    jclass gatt_cls;
    //jmethodID gatt_connect;
    //jmethodID gatt_disconnect;
    //jmethodID gatt_discover_services;
    jmethodID gatt_get_gatt_services;
    jmethodID gatt_get_gatt_service;
    jmethodID gatt_get_gatt_services_size;
    // gatt service
    jclass service_cls;
    jmethodID service_get_id;
    jmethodID service_get_uuid;
    //jmethodID service_get_instance_id;
    //jmethodID service_get_type;
    jmethodID service_is_primary;
    jmethodID service_get_gatt_characteristics;
    jmethodID service_get_gatt_characteristic;
    jmethodID service_get_gatt_characteristics_size;
    //jmethodID service_get_included_services;
    // gatt characteristic
    jclass characteristic_cls;
    jmethodID characteristic_get_id;
    jmethodID characteristic_get_uuid;
    //jmethodID characteristic_get_instance_id;
    jmethodID characteristic_get_gatt_descriptors;
    jmethodID characteristic_get_gatt_descriptor;
    jmethodID characteristic_get_gatt_descriptors_size;
    jmethodID characteristic_get_value;
    jmethodID characteristic_get_value_size;
    jmethodID characteristic_read_value;
    jmethodID characteristic_write_value;
    // gatt descriptor
    jclass descriptor_cls;
    jmethodID descriptor_get_id;
    jmethodID descriptor_get_uuid;
    jmethodID descriptor_get_value;
    jmethodID descriptor_get_value_size;
    jmethodID descriptor_read_value;
    jmethodID descriptor_write_value;
} BTContext;

BTContext g_ctx;

// adapter
jobject jni_adapter_create_adapter ();
const char* jni_adapter_get_address (jobject);
const char* jni_adapter_get_name (jobject);
void jni_adapter_start_discovery (jobject);
void jni_adapter_stop_discovery (jobject);
const char** jni_adapter_get_devices (jobject);
int jni_adapter_get_devices_size (jobject);

// device
jobject jni_adapter_create_device (jobject, const char*);
const char* jni_device_get_address (jobject);
const char* jni_device_get_name (jobject);
void jni_device_connect_gatt (jobject);
void jni_device_disconnect (jobject);
int jni_device_is_connected (jobject);
const int* jni_device_get_gatt_services (jobject);
int jni_device_get_gatt_services_size (jobject);

// service
jobject jni_device_create_service(jobject, int);
const char* jni_service_get_uuid(jobject);
int jni_service_is_primary(jobject);
const int* jni_service_get_gatt_characteristics (jobject);
int jni_service_get_gatt_characteristics_size (jobject);

// characteristic
jobject jni_service_create_characteristic(jobject, int);
const char* jni_characteristic_get_uuid(jobject);
int jni_characteristic_is_primary(jobject);
const int* jni_characteristic_get_gatt_descriptors (jobject);
int jni_characteristic_get_gatt_descriptors_size (jobject);
const int* jni_characteristic_get_value(jobject);
const int jni_characteristic_get_value_size(jobject);
const int* jni_characteristic_read_value(jobject);
void jni_characteristic_write_value(jobject, const int*, int);

// descriptor
jobject jni_characteristic_create_descriptor(jobject, int);
const char* jni_descriptor_get_uuid(jobject);
const int* jni_descriptor_get_value(jobject);
const int jni_descriptor_get_value_size(jobject);
const int* jni_descriptor_read_value(jobject);
void jni_descriptor_write_value(jobject, const int*, int);

#endif /* JNI_UTILS_H */
