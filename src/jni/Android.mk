LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := blurdroid
#LOCAL_SRC_FILES := blurdroid_jni.c
#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*/*.c)
LOCAL_SRC_FILES := bluetooth-adapter.c bluetooth-device.c jni-memory.c jni-utils.c
LOCAL_CFLAGS := -g

include $(BUILD_STATIC_LIBRARY)
