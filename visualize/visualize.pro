#-------------------------------------------------
#
# Project created by QtCreator 2010-03-29T00:47:03
#
#-------------------------------------------------

QT       += testlib

TARGET = visualize
TEMPLATE = app

LIBS += -L/home/vden/work/PacketParser/bin/ -lanalyzer
INCLUDEPATH += /home/vden/work/PacketParser/bin/

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
