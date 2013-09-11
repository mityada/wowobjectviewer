TEMPLATE = app
TARGET = demo
QT += opengl

HEADERS = window.h

SOURCES = window.cpp\
           main.cpp

INCLUDEPATH += ../src

LIBS += -L../src -lwov
