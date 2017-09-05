#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LC2000-nHD
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
    PacketHandler.cpp \
    bmpparser.cpp \
    MBMCReadmeTxtParser.cpp

HEADERS  += mainwindow.h \
    PacketHandler.h \
    Helper.h \
    Version.h \
    bmpparser.h \
    MBMCReadmeTxtParser.h \
    Temperature.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -fpermissive

CONFIG += STATIC

#-------------------------------------------------
# Icons
macx{
ICON = Keynote.ico
}

win32{
RC_FILE = LightCrafterGUI.rc
}

linux-g++{
#QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
}

OTHER_FILES += \
    Release_Notes.txt \
    LightCrafterGUI.rc \
    Keynote.ico
