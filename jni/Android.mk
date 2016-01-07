LOCAL_PATH := $(call my-dir)
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

include $(CLEAR_VARS)
LOCAL_MODULE    := ceres
LOCAL_SRC_FILES := libceres.a
include $(PREBUILT_STATIC_LIBRARY)
  
include $(CLEAR_VARS)

#LOCAL_C_INCLUDES += C:/android-ndk-r10e/sources/cxx-stl/stlport/stlport
LOCAL_C_INCLUDES += D:/lib/eigen-3.2.7
LOCAL_C_INCLUDES += D:/lib/ceres-solver-1.11.0/include
LOCAL_C_INCLUDES += D:/lib/ceres-solver-1.11.0/internal/ceres/miniglog
  
#opencv
#OPENCVROOT:= /home/robotbase/Android/OpenCV-2.4.10-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include D:/Eclipse/4.4.x/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES := ATAMNative.cpp
LOCAL_SRC_FILES += PointDetector.cpp
LOCAL_SRC_FILES += Calibration.cpp
LOCAL_SRC_FILES += ATAMData.cpp
LOCAL_SRC_FILES += ATAM.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_STATIC_LIBRARIES  =  ceres
LOCAL_LDLIBS := -llog -ldl

LOCAL_MODULE := ATAMNative

include $(BUILD_SHARED_LIBRARY)
