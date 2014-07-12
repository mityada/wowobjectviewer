TEMPLATE = lib
TARGET = wov
QT += quick

DEFINES += WOV_LIBRARY

HEADERS = \
    animatedvalue.h \
    bone.h \
    camerashake.h \
    character.h \
    combinedtexture.h \
    creature.h \
    dbc.h \
    global.h \
    m2.h \
    m2structures.h \
    model.h \
    modelscene.h \
    mpq.h \
    mvp.h \
    particleemitter.h \
    ribbonemitter.h \
    spellvisual.h \
    spellvisualkit.h \
    textureanimation.h \
    texture.h

SOURCES = \
    bone.cpp \
    camerashake.cpp \
    character.cpp \
    combinedtexture.cpp \
    creature.cpp \
    dbc.cpp \
    m2.cpp \
    model.cpp \
    modelscene.cpp \
    mpq.cpp \
    mvp.cpp \
    particleemitter.cpp \
    ribbonemitter.cpp \
    spellvisual.cpp \
    spellvisualkit.cpp \
    textureanimation.cpp \
    texture.cpp

unix:LIBS += -lstorm

win32:INCLUDEPATH += ../StormLib
win32:LIBS += -L$$PWD/../StormLib/ -lStormLib_dll

RESOURCES += src.qrc
