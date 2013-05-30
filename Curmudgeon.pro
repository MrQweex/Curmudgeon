#-------------------------------------------------
#
# Project created by QtCreator 2013-05-27T16:51:38
#
#-------------------------------------------------

VERSION = 1.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Curmudgeon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SoundButton.cpp \
    SoundBoard.cpp \
    VersionDialog.cpp \
    AudioPlayer/AudioPlayerWin.cpp

HEADERS  += mainwindow.h \
    SoundButton.h \
    Options.h \
    SoundBoard.h \
    AudioPlayer/AudioPlayerCallback.h \
    VersionDialog.h \
    AudioPlayer/AudioPlayer.h \
    AudioPlayer/AudioPlayerFactory.h

FORMS    += mainwindow.ui


#mpg123
#LIBS += /usr/local/lib/libmpg123.dylib

#ffmpeg
#INCLUDEPATH += include
#LIBS += -Llib
#LIBS += -lavcodec
#LIBS += -lavfilter
#LIBS += -lavformat
#LIBS += -lavutil
#LIBS += -lswscale

#ao
#INCLUDEPATH += libao/include


#SFML
#LIBS += -framework sfml-audio
#LIBS += -framework sfml-system

RESOURCES += resources.qrc

macx {
    ICON = images/logo.icns
    SOURCES += AudioPlayer/AudioPlayerOsx.cpp
    HEADERS += AudioPlayer/AudioPlayerOsx.h
    LIBS += -framework CoreFoundation
    LIBS += -framework AudioToolbox
}
#win32 {
    ICON = images/logo.ico
    #SOURCES += AudioPlayer/AudioPlayerWin.cpp
    HEADERS += AudioPlayer/AudioPlayerWin.h
    LIBS += -lwinmm
    RC_FILE = winicon.rc
#}

#QMAKE_CXX = g++
