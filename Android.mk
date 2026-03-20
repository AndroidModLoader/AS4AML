LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cpp .cc
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_MODULE := AS4AML
else
	LOCAL_MODULE := AS4AML.64
endif
LOCAL_SRC_FILES := main.cpp as4aml.cpp amlscriptbuilder.cpp mod/logger.cpp mod/config.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/angelscript/source/*.cpp)
#FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/angelscript/source/*.S)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scriptarray/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scriptbuilder/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scriptstdstring/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scriptmath/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scripthelper/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/scripthandle/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/angelscript/sdk/add_on/datetime/*.cpp)
# Finalize wildcards
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_SRC_FILES += $(LOCAL_PATH)/angelscript/sdk/angelscript/source/as_callfunc_arm_gcc.S
else
	LOCAL_SRC_FILES += $(LOCAL_PATH)/angelscript/sdk/angelscript/source/as_callfunc_arm64_gcc.S
endif
LOCAL_CFLAGS += -O2 -mfloat-abi=softfp -DNDEBUG -std=c++14 -fexceptions -DAS_NO_THREADS -DAS4AML_NO_LINE_CUES
LOCAL_C_INCLUDES += $(LOCAL_PATH)/angelscript/sdk/angelscript/include \
                    $(LOCAL_PATH)/angelscript/sdk/add_on $(LOCAL_PATH)/angelscript
LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS += -llog

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
	LOCAL_CFLAGS += -mfpu=neon
endif

include $(BUILD_SHARED_LIBRARY)
