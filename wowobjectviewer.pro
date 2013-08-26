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
           particleemitter.h\
           mvp.h\
           dbc.h\
           util.h\
           spellvisualkit.h\
           spellvisual.h\
           modelscene.h

SOURCES += main.cpp\
           m2.cpp\
           model.cpp\
           texture.cpp\
           bone.cpp\
           textureanimation.cpp\
           particleemitter.cpp\
           mvp.cpp\
           dbc.cpp\
           util.cpp\
           spellvisualkit.cpp\
           spellvisual.cpp\
           modelscene.cpp

RESOURCES += wowobjectviewer.qrc
