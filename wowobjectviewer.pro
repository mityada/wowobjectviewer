TEMPLATE = app
TARGET = wowobjectviewer
QT += quick

CONFIG += debug

HEADERS += m2structures.h\
           m2.h\
           model.h\
           texture.h\
           animatedvalue.h\
           bone.h\
           textureanimation.h\
           particleemitter.h

SOURCES += main.cpp\
           m2.cpp\
           model.cpp\
           texture.cpp\
           bone.cpp\
           textureanimation.cpp\
           particleemitter.cpp

RESOURCES += wowobjectviewer.qrc
