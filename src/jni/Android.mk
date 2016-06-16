LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := blurdroid
LOCAL_SRC_FILES := blurdroid_jni.c
LOCAL_CFLAGS := -g

include $(BUILD_STATIC_LIBRARY)
