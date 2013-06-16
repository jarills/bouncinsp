TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main-console.cpp \
    ../src/PatternEvent.cpp \
    ../src/Pattern.cpp \
    ../src/PadInfo.cpp \
    ../src/Pad.cpp \
    ../src/SimpleAudioWave.cpp \
    ../src/Endian.cpp \
    ../src/Song.cpp

INCLUDEPATH += ../src
DEPENDPATH += ../src

macx: QMAKE_CFLAGS_RELEASE += -fvisibility=hidden
macx: QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden -fvisibility-inlines-hidden

win32: LIBS += -lole32 -luser32

unix: LIBS += -lsndfile -lboost_system -lboost_filesystem
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    ../src/PatternEvent.h \
    ../src/Pattern.h \
    ../src/PadInfo.h \
    ../src/Pad.h \
    ../src/SimpleAudioWave.h \
    ../src/AudioException.h \
    ../src/Endian.h \
    ../src/Song.h \
    ../src/BouncinSP.h
