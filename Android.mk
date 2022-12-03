LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := AS4AML
LOCAL_SRC_FILES := main.cpp as4aml.cpp mod/logger.cpp mod/config.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/angelscript/source/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/angelscript/source/*.S)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scriptarray/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scriptbuilder/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scriptstdstring/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scriptmath/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scripthelper/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/scripthandle/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/add_on/datetime/*.cpp)
# Finalize wildcards
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++14
LOCAL_C_INCLUDES += $(LOCAL_PATH)/sdk/angelscript/include \
                    $(LOCAL_PATH)/sdk/add_on
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)
