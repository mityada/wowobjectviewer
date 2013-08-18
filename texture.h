#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions>

#define BLP_MAGIC "BLP2"

struct BLPHeader
{
    char magic[4];
    quint32 type;
    quint8 compression;
    quint8 alphaDepth;
    quint8 alphaType;
    quint8 hasMips;
    quint32 width;
    quint32 height;
    quint32 mipmapOffset[16];
    quint32 mipmapLength[16];
};

class Texture
{
public:
    Texture();

    bool load(const QString &fileName);
    void bind();

private:
    void create();

    QString getLocalFileName(const QString &mpqFileName);

    QByteArray m_data;
    BLPHeader *m_header;

    bool m_dirty;

    GLuint m_texture;
};

#endif
