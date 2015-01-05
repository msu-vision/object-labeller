QT += core gui
CONFIG += debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = object-labeller
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    application.cpp \
    imagearea.cpp \
    frame.cpp

HEADERS += application.h \
    imagearea.h \
    frame.h

FORMS +=
