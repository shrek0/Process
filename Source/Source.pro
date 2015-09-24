TEMPLATE = lib
TARGET = Source
INCLUDEPATH += .

CONFIG += console
CONFIG += c++11
CONFIG -= qt

# Input
HEADERS += directory.h process.h processes.h \
    console.h
SOURCES += directory.cpp process.cpp processes.cpp \
    processconsole.cpp \
    console.cpp
