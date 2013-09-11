TEMPLATE = app
TARGET = demo
QT += quick

SOURCES = main.cpp

INCLUDEPATH += ../src

LIBS += -L../src -lwov

RESOURCES += demo.qrc
