#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <jni.h>
#include <android/log.h>

static const char* kTAG = "blurdroid";
#define LOGI(...) \
  ((void)__android_log_print (ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print (ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print (ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

typedef struct {
    // jvm
    JavaVM *javaVM;
    // adapter
    jclass adapter_cls;
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
    jmethodID device_get_uuids;
    jmethodID device_get_uuids_size;
    jmethodID device_get_rssi;
    jmethodID device_get_tx_power;
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
    jmethodID characteristic_get_flags;
    jmethodID characteristic_get_flags_size;
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
    // utils
    jclass set_cls;
    jmethodID set_iterator;
    jmethodID set_size;
    jclass iterator_cls;
    jmethodID iterator_has_next;
    jmethodID iterator_next;
    jmethodID null;
} BTContext;

BTContext g_ctx;

jobject jni_create_static_object (jclass, jmethodID);
jobject jni_create_object (jobject, jmethodID);
jobject jni_create_object_int (jobject, jmethodID, int);
jobject jni_create_object_str (jobject, jmethodID, const char*);
jobject jni_call_object (jobject, jmethodID);
void jni_call_void (jobject, jmethodID);
int jni_call_bool (jobject, jmethodID);
int jni_call_int (jobject, jmethodID);
void jni_set_value(jobject, jmethodID, const int*, int);
const int* jni_get_value(jobject, jmethodID);
const int* jni_call_int_array (jobject, jmethodID, jmethodID);
const char* jni_call_str (jobject, jmethodID);
const char** jni_call_str_array (jobject, jmethodID, jmethodID);

#endif /* JNI_UTILS_H */
