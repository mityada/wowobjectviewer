TEMPLATE = app
TARGET = wowobjectviewer
QT += opengl

HEADERS += m2structures.h\
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
           window.h

SOURCES += main.cpp\
           m2.cpp\
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
           mpq.cpp\
           window.cpp

unix:LIBS += -lstorm

win32:INCLUDEPATH += StormLib
win32:LIBS += -L$$PWD/StormLib/ -lStormLib_dll

RESOURCES += wowobjectviewer.qrc
