QT += core
QT -= gui

TARGET = facedetect
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


LIBS += "/usr/local/lib/*.so"
