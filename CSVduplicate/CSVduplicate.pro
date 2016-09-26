QT += core
QT -= gui

TARGET = CSVduplicate
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Line.cpp

HEADERS += \
    Line.h


QMAKE_CXXFLAGS += -std=c++11
