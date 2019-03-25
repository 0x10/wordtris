CXX=g++
ANDROID_DIR=org.libsdl.wordtris/
SRC_DIR=./src
BUILD_DIR=.
SDL_DIR=./SDL2-2.0.9

TARGET_SDL=$(BUILD_DIR)/wordtris_sdl

all: clean sdl

sdl:
	$(CXX) -Os -Wall -I. $(SRC_DIR)/wordtris.cpp -L$(SDL_DIR)/build/.libs -lSDL2 -o $(TARGET_SDL)

sdl-system:
	$(CXX) -Os -Wall -DUSE_SYSTEM_SDL -I. $(SRC_DIR)/wordtris.cpp -lSDL2 -o $(TARGET_SDL)

run-sdl: sdl
	LD_LIBRARY_PATH=$(SDL_DIR)/build/.libs ./$(TARGET_SDL)

ncurses: prepare
	$(CXX) -g -Wall -DRENDER_NCURSES $(SRC_DIR)/wordtris.cpp -lncurses -o $(BUILD_DIR)/wordtris_ncurses

android-dbg:
	cd $(ANDROID_DIR) && ./gradlew assembleDebug
	cd -

clean:
	rm -f $(TARGET_SDL)
	cd $(ANDROID_DIR) && ./gradlew clean
	cd -
