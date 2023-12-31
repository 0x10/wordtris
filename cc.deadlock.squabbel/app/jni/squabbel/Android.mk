LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

SDL_TTF_PATH := ../ttf
SDL_MIXER_PATH := ../mixer

LOCAL_C_INCLUDES := \
					$(LOCAL_PATH)/$(SDL_PATH)/include \
					$(LOCAL_PATH)/$(SDL_TTF_PATH) \
					$(LOCAL_PATH)/src

# Add your application source files here...
LOCAL_SRC_FILES :=  \
					$(wildcard $(LOCAL_PATH)/src/*.cpp) \

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
