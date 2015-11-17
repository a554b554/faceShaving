TARGET = facedetect
SOURCES += main.cpp \
    preprocessor.cpp \
    detectobject.cpp


LIBS += "/usr/local/lib/*.so"

HEADERS += \
    preprocessor.h \
    detectobject.h
