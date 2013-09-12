TEMPLATE = lib
TARGET = wov
QT += opengl

DEFINES += WOV_LIBRARY

HEADERS = m2structures.h\
          m2.h\
          model.h\
          texture.h\
          animatedvalue.h\
          bone.h\
          textureanimation.h\
          particleemitter.h\
          ribbonemitter.h\
          mvp.h\
          dbc.h\
          spellvisualkit.h\
          spellvisual.h\
          modelscene.h\
          mpq.h\
          global.h

SOURCES = m2.cpp\
          model.cpp\
          texture.cpp\
          bone.cpp\
          textureanimation.cpp\
          particleemitter.cpp\
          ribbonemitter.cpp\
          mvp.cpp\
          dbc.cpp\
          spellvisualkit.cpp\
          spellvisual.cpp\
          modelscene.cpp\
          mpq.cpp

unix:LIBS += -lstorm

win32:INCLUDEPATH += ../StormLib
win32:LIBS += -L$$PWD/../StormLib/ -lStormLib_dll

RESOURCES += src.qrc
