CXX=g++
CXX_OPTIMIZATION=-Os
CXX_WARNINGS=-Wpedantic -pedantic-errors -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Weffc++ 
CXX_DEBUG=-g
CXX_FLAGS=-std=c++11 $(CXX_OPTIMIZATION) $(CXX_WARNINGS)

ANDROID_DIR=org.libsdl.wordtris
SRC_DIR=./src
BUILD_DIR=.
SDL_DIR=./SDL2-2.0.9

TARGET_SDL=$(BUILD_DIR)/wordtris_sdl

APK_PATH=./$(ANDROID_DIR)/app/build/outputs/apk/debug
APK_TRG_NAME=wordtris.apk

all: clean sdl

sdl:
	$(CXX) $(CXX_FLAGS) -I./pc_include -I. $(SRC_DIR)/wordtris.cpp -L$(SDL_DIR)/build/.libs -lSDL2 -o $(TARGET_SDL)

sdl-system:
	$(CXX) $(CXX_FLAGS) -DUSE_SYSTEM_SDL -I./pc_include $(SRC_DIR)/wordtris.cpp -lSDL2 -o $(TARGET_SDL)

run-sdl: sdl
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs ./$(TARGET_SDL)

debug-sdl:
	$(CXX) $(CXX_DEBUG) $(CXX_FLAGS) -I./pc_include -I. $(SRC_DIR)/wordtris.cpp -L$(SDL_DIR)/build/.libs -lSDL2 -o $(TARGET_SDL)
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs gdb ./$(TARGET_SDL)

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
