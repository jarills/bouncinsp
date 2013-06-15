TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    PatternEvent.cpp \
    Pattern.cpp \
    PadInfo.cpp \
    Pad.cpp \
    SimpleAudioWave.cpp \
    Endian.cpp \
    Song.cpp

macx: QMAKE_CFLAGS_RELEASE += -fvisibility=hidden
macx: QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden -fvisibility-inlines-hidden

win32: LIBS += -lole32 -luser32

unix: LIBS += -lsndfile -lboost_system -lboost_filesystem
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    PatternEvent.h \
    Pattern.h \
    PadInfo.h \
    Pad.h \
    SimpleAudioWave.h \
    AudioException.h \
    Endian.h \
    Song.h \
    BouncinSP.h
