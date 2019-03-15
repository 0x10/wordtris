CXX=g++
ANDROID_DIR=org.libsdl.wordtris/

all: clean sdl

sdl:
	$(CXX) -g -Wall wordtris.cpp -lSDL2 -o wordtris_sdl

ncurses:
	$(CXX) -g -Wall -DRENDER_NCURSES wordtris.cpp -lncurses -o wordtris_ncurses

android-dbg:
	cd $(ANDROID_DIR) && ./gradlew bundleDebug
	cd -

clean:
	rm -f wordtris_sdl wordtris_ncurses
	cd $(ANDROID_DIR) && ./gradlew clean
	cd -
