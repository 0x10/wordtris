CXX=g++
CXX_OPTIMIZATION=-Os
CXX_WARNINGS=-Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Weffc++ -Wuseless-cast -Wnull-dereference -Wduplicated-branches -Wduplicated-cond -Wstack-protector
#-pedantic-errors 
CXX_DEBUG=-g
CXX_FLAGS=-std=c++11 -fPIE -fstack-protector-all -pipe $(CXX_WARNINGS)#-D_FORTIFY_SOURCE=2
LD_FLAGS=-Wl,-z,defs -Wl,-z,now -Wl,-z,relro -Wl,-z,noexecstack

LIBS=-L$(SDL_DIR)/build/.libs -lSDL2 -L$(SDL_TTF_DIR)/build/.libs -lSDL2_ttf -L$(SDL_MIXER_DIR)/build/.libs -lSDL2_mixer
INCLUDES=-I./pc_include -I$(BUILD_DIR) -I$(SRC_DIR) -I$(SDL_DIR)/include -I$(SDL_TTF_DIR) -I$(SDL_MIXER_DIR)

ANDROID_DIR=cc.deadlock.squabbel
SRC_DIR=./src
BUILD_DIR=.
SDL_DIR=./SDL2-2.0.9
SDL_TTF_DIR=./SDL2_ttf-2.0.15
SDL_MIXER_DIR=./SDL2_mixer-2.0.4

TARGET_SDL=$(BUILD_DIR)/squabbel

APK_PATH=./$(ANDROID_DIR)/app/build/outputs/apk
APK_TRG_NAME=squabbel

all: clean sdl

sdl:
	$(CXX) $(CXX_OPTIMIZATION) $(CXX_FLAGS) $(LD_FLAGS) $(INCLUDES) $(SRC_DIR)/wordtris.cpp $(LIBS) -o $(TARGET_SDL)_lite
	$(CXX) $(CXX_OPTIMIZATION) $(CXX_FLAGS) $(LD_FLAGS) -DWT_PRO_MODE $(INCLUDES) $(SRC_DIR)/wordtris.cpp $(LIBS) -o $(TARGET_SDL)

sdl-system:
	$(CXX) -g $(CXX_FLAGS) $(LD_FLAGS) -DUSE_SYSTEM_SDL -I./pc_include -I$(SRC_DIR) $(SRC_DIR)/wordtris.cpp $(LIBS) -o $(TARGET_SDL)

run-sdl: sdl
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs:$(SDL_TTF_DIR)/build/.libs:$(SDL_MIXER_DIR)/build/.libs ./$(TARGET_SDL)

system-run-sdl: sdl-system
	./$(TARGET_SDL)

debug-sdl:
	$(CXX) $(CXX_DEBUG) $(CXX_FLAGS) $(LD_FLAGS) -DWT_PRO_MODE $(INCLUDES) $(SRC_DIR)/wordtris.cpp $(LIBS) -o $(TARGET_SDL)
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs:$(SDL_TTF_DIR)/build/.libs:$(SDL_MIXER_DIR)/build/.libs gdb ./$(TARGET_SDL)

ncurses:
	$(CXX) -g -Wall -I./pc_include -DRENDER_NCURSES $(SRC_DIR)/wordtris.cpp -lncurses -o $(BUILD_DIR)/wordtris_ncurses

android-dbg:
	cd $(ANDROID_DIR) && ./gradlew assembleLiteDebug assembleProDebug
	cd -

android-release:
	cd $(ANDROID_DIR) && ./gradlew assembleLiteRelease assembleProRelease
	cd -

publish-dbg:
	@cp $(APK_PATH)/lite/debug/app-lite-debug.apk $(APK_PATH)/lite/debug/$(APK_TRG_NAME)_lite.apk
	@cp $(APK_PATH)/pro/debug/app-pro-debug.apk $(APK_PATH)/pro/debug/$(APK_TRG_NAME)_pro.apk
	@curl -X POST -F 'file=@$(APK_PATH)/lite/debug/$(APK_TRG_NAME)_lite.apk' -F 'pruefe=SonneMondUndSterne' https://deadlock.cc/apk/upload.php
	@curl -X POST -F 'file=@$(APK_PATH)/pro/debug/$(APK_TRG_NAME)_pro.apk' -F 'pruefe=SonneMondUndSterne' https://deadlock.cc/apk/upload.php

clean:
	rm -f $(TARGET_SDL)
	cd $(ANDROID_DIR) && ./gradlew clean
	cd -
