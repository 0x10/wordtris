CXX=g++
ANDROID_DIR=org.libsdl.wordtris
SRC_DIR=./src
BUILD_DIR=.
SDL_DIR=./SDL2-2.0.9

TARGET_SDL=$(BUILD_DIR)/wordtris_sdl

APK_PATH=./$(ANDROID_DIR)/app/build/outputs/apk/debug
APK_TRG_NAME=wordtris.apk

all: clean sdl

sdl:
	$(CXX) -g -Wpedantic -pedantic-errors -Wextra -Wall -I./pc_include -I. $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp -L$(SDL_DIR)/build/.libs -lSDL2 -o $(TARGET_SDL)

sdl-system:
	$(CXX) -Os -Wall -DUSE_SYSTEM_SDL -I./pc_include $(SRC_DIR)/wordtris.cpp -lSDL2 -o $(TARGET_SDL)

run-sdl: sdl
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs ./$(TARGET_SDL)

ncurses:
	$(CXX) -g -Wall -I./pc_include -DRENDER_NCURSES $(SRC_DIR)/wordtris.cpp -lncurses -o $(BUILD_DIR)/wordtris_ncurses

android-dbg:
	cd $(ANDROID_DIR) && ./gradlew assembleDebug
	cd -

publish-dbg:
	@cp $(APK_PATH)/app-debug.apk $(APK_PATH)/$(APK_TRG_NAME)
	@curl -X POST -F 'file=@$(APK_PATH)/$(APK_TRG_NAME)' -F 'pruefe=SonneMondUndSterne' https://deadlock.cc/apk/upload.php

clean:
	rm -f $(TARGET_SDL)
	cd $(ANDROID_DIR) && ./gradlew clean
	cd -
