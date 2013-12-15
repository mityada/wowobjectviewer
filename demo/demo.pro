TEMPLATE = app
TARGET = demo
QT += quick

SOURCES += main.cpp

win32:INCLUDEPATH += ../StormLib
win32:LIBS += -L$$PWD/../StormLib/ -lStormLib_dll

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lwov
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lwov
else:unix: LIBS += -L$$OUT_PWD/../src/ -lwov

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

RESOURCES += demo.qrc
