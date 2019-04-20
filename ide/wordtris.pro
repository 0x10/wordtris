QT      -= core
QT      -= gui
QT      -= qml
QT      -= quick

##QMAKE_CFLAGS += -std=c99
#CONFIG      += c++11

CONFIG      += console
CONFIG      -= app_bundle

CODECFORSRC = UTF-8
CODECFORTR   = UTF-8

TEMPLATE = app
TARGET = wordtris_sdl

windows {
    message(Windows Desktop)
    DESTDIR = $$PWD/builddir_windows
    LIBS += -lSDL2

    QMAKE_CXXFLAGS += -Wno-format-zero-length
    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

linux-g++ {
    message(Linux Desktop)
    DESTDIR = $$PWD/builddir_desktop
    LIBS += -lSDL2

    QMAKE_CXXFLAGS += -Wno-format-zero-length
    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

linux-buildroot-g++ {
    message(cross-compile-buildroot)
    DESTDIR = $$PWD/builddir_
    LIBS += -lSDL2

    QMAKE_CXXFLAGS += -Wno-format-zero-length
    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

android-clang {
    message(cross-compile-android-clang)
    DESTDIR = $$PWD/builddir_android
    #LIBS += -lSDL2

    QT += androidextras

    QMAKE_CXXFLAGS += -Wno-format-zero-length
    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

android-g++ {
    message(cross-compile-android-g++)
    DESTDIR = $$PWD/builddir_android
    #LIBS += -lSDL2

    QMAKE_CXXFLAGS += -Wno-format-zero-length
    QMAKE_CXXFLAGS += -Wno-tautological-pointer-compare
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field
}

OBJECTS_DIR = $$DESTDIR/.obj_
MOC_DIR = $$DESTDIR/.moc_
RCC_DIR = $$DESTDIR/.rcc_
UI_DIR = $$DESTDIR/.ui_

INCLUDEPATH += ../pc_include
INCLUDEPATH += ../src
INCLUDEPATH += ../src/game_modes
INCLUDEPATH += ../src/menus
INCLUDEPATH += ../src/sdl
INCLUDEPATH += ../src/widgets

HEADERS += \
    ../pc_include/SDL.h \
    ../pc_include/wt_ncurses.h \
    ../src/game_modes/wt_game_mode_abc.h \
    ../src/game_modes/wt_game_mode_guessing.h \
    ../src/game_modes/wt_game_mode_tetris.h \
    ../src/game_modes/wt_game_mode_wordtris.h \
    ../src/menus/wt_menu_help.h \
    ../src/menus/wt_menu_highscores.h \
    ../src/menus/wt_menu_pause.h \
    ../src/menus/wt_menu_settings.h \
    ../src/sdl/wt_sdl.h \
    ../src/sdl/wt_sdl_config.h \
    ../src/sdl/wt_sdl_drawing.h \
    ../src/sdl/wt_sdl_font.h \
    ../src/sdl/wt_sdl_input.h \
    ../src/sdl/wt_sdl_utils.h \
    ../src/widgets/wt_button.h \
    ../src/widgets/wt_checkbox_button.h \
    ../src/widgets/wt_grid_touch_overlay.h \
    ../src/widgets/wt_horizontal_carousel.h \
    ../src/widgets/wt_tristate_button.h \
    ../src/dea.h \
    ../src/global.h \
    ../src/wt_active_letter.h \
    ../src/wt_animations.h \
    ../src/wt_backend_select.h \
    ../src/wt_basic_types.h \
    ../src/wt_board.h \
    ../src/wt_clickable_if.h \
    ../src/wt_drawing.h \
    ../src/wt_game_ctr.h \
    ../src/wt_game_mode_ctr.h \
    ../src/wt_game_mode_if.h \
    ../src/wt_input.h \
    ../src/wt_input_types.h \
    ../src/wt_l10n.h \
    ../src/wt_l10n_translations.h \
    ../src/wt_menu_ctr.h \
    ../src/wt_menu_if.h \
    ../src/wt_player.h \
    ../src/wt_settings_observer_if.h \
    ../src/wt_storable.h \
    ../src/wt_storage.h \
    ../src/wt_storage_types.h \
    ../src/wt_string_utils.h \
    ../src/wt_utils.h \
    ../src/wt_wordlist.h

SOURCES += \
    ../src/wordtris.cpp \
    ../src/wordtris.cpp
