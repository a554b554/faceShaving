TARGET = facedetect
SOURCES += main.cpp \
    preprocessor.cpp \
    detectobject.cpp \
    helpfunc.cpp
CONFIG += c++11

LIBS += "/usr/local/lib/*.so"

HEADERS += \
    preprocessor.h \
    detectobject.h \
    helpfunc.h
