
##QMAKE_CFLAGS += -std=c99
#CONFIG      += c++11

CONFIG      += console
CONFIG      -= app_bundle

CODECFORSRC = UTF-8
CODECFORTR   = UTF-8

TEMPLATE = app
TARGET = wordtris_sdl

INCLUDEPATH += ../pc_include
INCLUDEPATH += ../src
INCLUDEPATH += ..

windows {
    message(Windows Desktop)
    DESTDIR = $$PWD/builddir_windows
    LIBS += -lSDL2
}

linux-g++ {
    message(Linux Desktop)
    DESTDIR = $$PWD/builddir_desktop
    LIBS += -lSDL2
}

linux-buildroot-g++ {
    message(cross-compile-buildroot)
    DESTDIR = $$PWD/builddir_
    LIBS += -lSDL2
}

android-clang {
    message(cross-compile-android-clang)
    DESTDIR = $$PWD/builddir_android
    #LIBS += -lSDL2

    QT += androidextras

    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

android-g++ {
    message(cross-compile-android-g++)
    DESTDIR = $$PWD/builddir_android
    #LIBS += -lSDL2

    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

OBJECTS_DIR = $$DESTDIR/.obj_
MOC_DIR = $$DESTDIR/.moc_
RCC_DIR = $$DESTDIR/.rcc_
UI_DIR = $$DESTDIR/.ui_

HEADERS += \
    ../pc_include/SDL.h \
    ../pc_include/wt_ncurses.h \
    ../src/dea.h \
    ../src/global.h \
    ../src/wt_active_letter.h \
    ../src/wt_animations.h \
    ../src/wt_backend_policy_if.h \
    ../src/wt_backend_select.h \
    ../src/wt_board.h \
    ../src/wt_button.h \
    ../src/wt_drawing.h \
    ../src/wt_game_ctr.h \
    ../src/wt_game_mode_abc.h \
    ../src/wt_game_mode_ctr.h \
    ../src/wt_game_mode_guessing.h \
    ../src/wt_game_mode_if.h \
    ../src/wt_game_mode_tetris.h \
    ../src/wt_game_mode_wordtris.h \
    ../src/wt_input.h \
    ../src/wt_l10n.h \
    ../src/wt_l10n_translations.h \
    ../src/wt_menu_ctr.h \
    ../src/wt_menu_game_mode_select.h \
    ../src/wt_menu_help.h \
    ../src/wt_menu_highscores.h \
    ../src/wt_menu_if.h \
    ../src/wt_menu_pause.h \
    ../src/wt_menu_settings.h \
    ../src/wt_player.h \
    ../src/wt_random.h \
    ../src/wt_sdl.h \
    ../src/wt_sdl_config.h \
    ../src/wt_sdl_drawing.h \
    ../src/wt_sdl_font.h \
    ../src/wt_sdl_input.h \
    ../src/wt_sdl_utils.h \
    ../src/wt_settings_observer_if.h \
    ../src/wt_storage.h \
    ../src/wt_string_utils.h \
    ../src/wt_types.h \
    ../src/wt_wordlist.h

SOURCES += \
    ../src/wordtris.cpp
