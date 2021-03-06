LOCAL_PATH := $(call my-dir)

3RD_INC_DIR=$(LOCAL_PATH)/3rd/include
3RD_LIB_DIR=$(LOCAL_PATH)/3rd/libs

#Prebuild 3rdLib
include $(CLEAR_VARS)
LOCAL_MODULE	 := PVRScopeDeveloper

LOCAL_SRC_FILES  := $(3RD_LIB_DIR)/$(TARGET_ARCH_ABI)/Static/libPVRScopeDeveloper.a
#LOCAL_SRC_FILES  := $(3RD_LIB_DIR)/$(TARGET_ARCH_ABI)/Dynamic/libPVRScopeDeveloper.so

include $(PREBUILT_STATIC_LIBRARY)
#include $(PREBUILT_SHARED_LIBRARY)

#Build Target
include $(CLEAR_VARS)
LOCAL_MODULE     := PVRTest
LOCAL_MODULE_TAGS:= optional
LOCAL_SRC_FILES  := $(LOCAL_PATH)/src/sotest.cpp 
LOCAL_C_INCLUDES := $(3RD_INC_DIR) $(LOCAL_PATH)/src 


LOCAL_CFLAGS :=	-DANDROID
LOCAL_LDLIBS := -ldl -llog 

LOCAL_STATIC_LIBRARIES := PVRScopeDeveloper 
#LOCAL_SHARED_LIBRARIES := PVRScopeDeveloper 

LOCAL_LDFLAGS += -Wl,--no-undefined

include $(BUILD_SHARED_LIBRARY)
