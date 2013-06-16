#-------------------------------------------------
#
# Project created by QtCreator 2013-06-16T11:12:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BouncinSP
TEMPLATE = app

FORMS    += MainWindow.ui

SOURCES += main.cpp \
        MainWindow.cpp \
        ../src/PatternEvent.cpp \
        ../src/Pattern.cpp \
        ../src/PadInfo.cpp \
        ../src/Pad.cpp \
        ../src/SimpleAudioWave.cpp \
        ../src/Endian.cpp \
        ../src/Song.cpp \
    ../src/BouncinSP.cpp

HEADERS += \
     MainWindow.h \
    ../src/PatternEvent.h \
    ../src/Pattern.h \
    ../src/PadInfo.h \
    ../src/Pad.h \
    ../src/SimpleAudioWave.h \
    ../src/AudioException.h \
    ../src/Endian.h \
    ../src/Song.h \
    ../src/BouncinSP.h

INCLUDEPATH += ../src
DEPENDPATH += ../src

macx: QMAKE_CFLAGS_RELEASE += -fvisibility=hidden
macx: QMAKE_CXXFLAGS_RELEASE += -fvisibility=hidden -fvisibility-inlines-hidden

win32: LIBS += -lole32 -luser32

unix: LIBS += -lsndfile -lboost_system -lboost_filesystem
unix:!macx: QMAKE_CXXFLAGS += -std=c++0x

OTHER_FILES += \
    ../README.md
