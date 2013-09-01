TEMPLATE = app
TARGET = wowobjectviewer
QT += quick

HEADERS += m2structures.h\
           m2.h\
           model.h\
           texture.h\
           animatedvalue.h\
           bone.h\
           textureanimation.h\
           particleemitter.h\
           mvp.h\
           dbc.h\
           spellvisualkit.h\
           spellvisual.h\
           modelscene.h\
           mpq.h

SOURCES += main.cpp\
           m2.cpp\
           model.cpp\
           texture.cpp\
           bone.cpp\
           textureanimation.cpp\
           particleemitter.cpp\
           mvp.cpp\
           dbc.cpp\
           spellvisualkit.cpp\
           spellvisual.cpp\
           modelscene.cpp\
           mpq.cpp

unix:LIBS += -lstorm

win32:INCLUDEPATH += StormLib
win32:LIBS += -L$$PWD/StormLib/ -lStormLib_dll

RESOURCES += wowobjectviewer.qrc
