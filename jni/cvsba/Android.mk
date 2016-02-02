LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
#OPENCV_LIB_TYPE:=SHARED
include D:/Eclipse/4.4.x/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES := $(LOCAL_PATH)/cvsba.cpp
#					$(LOCAL_PATH)/sba_chkjac.c \
#					$(LOCAL_PATH)/sba_crsm.c \
#					$(LOCAL_PATH)/sba_lapack.c \
#					$(LOCAL_PATH)/sba_levmar_wrap.c \
#					$(LOCAL_PATH)/sba_levmar.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE := cvsba

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)
