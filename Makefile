CXX=g++
ANDROID_DIR=org.libsdl.wordtris/
SRC_DIR=./src
BUILD_DIR=./build

all: clean sdl

prepare:
	mkdir -p $(BUILD_DIR)

sdl: prepare
	$(CXX) -Os -Wall -I. $(SRC_DIR)/wordtris.cpp -lSDL2 -o $(BUILD_DIR)/wordtris_sdl

ncurses: prepare
	$(CXX) -g -Wall -DRENDER_NCURSES $(SRC_DIR)/wordtris.cpp -lncurses -o $(BUILD_DIR)/wordtris_ncurses

android-dbg:
	cd $(ANDROID_DIR) && ./gradlew assembleDebug
	cd -

clean:
	rm -f $(BUILD_DIR)
	cd $(ANDROID_DIR) && ./gradlew clean
	cd -
