LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_MODULE    := AS4AML
LOCAL_SRC_FILES := main.cpp mod/logger.cpp mod/config.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/angelscript/source/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/sdk/angelscript/source/*.S)
# Finalize wildcards
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++14
LOCAL_C_INCLUDES += $(LOCAL_PATH)/sdk/angelscript/include
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)
