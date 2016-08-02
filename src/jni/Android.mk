LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := blurdroid
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_CFLAGS := -g

include $(BUILD_STATIC_LIBRARY)
