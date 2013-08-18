#include "texture.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStringList>

Texture::Texture() : m_dirty(false), m_texture(0)
{
}

bool Texture::load(const QString &fileName)
{
    QFile file(fileName);

    if (!file.exists())
        file.setFileName(getLocalFileName(fileName));

    if (!file.exists()) {
        qCritical("File '%s' does not exist!", qPrintable(fileName));
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical("File '%s' cannot be opened!", qPrintable(fileName));
        return false;
    }

    m_data = file.readAll();

    file.close();

    m_header = reinterpret_cast<BLPHeader *>(m_data.data());

    if (qstrncmp(m_header->magic, BLP_MAGIC, 4) != 0) {
        qCritical("File '%s' is not a valid BLP file!", qPrintable(fileName));
        return false;
    }

    m_dirty = true;

    return true;
}

QString Texture::getLocalFileName(const QString &mpqFileName)
{
    QStringList path = mpqFileName.split("\\");

    QDir dir;

    for (int i = 0; i < path.size(); i++) {
        QStringList entryList = dir.entryList();

        for (int j = 0; j < entryList.size(); j++) {
            if (path[i].compare(entryList[j], Qt::CaseInsensitive) == 0) {
                if (i == path.size() - 1) {
                    if (dir.exists(entryList[j]))
                        return dir.filePath(entryList[j]);
                    else
                        return QString();
                } else {
                    if (!dir.cd(entryList[j]))
                        return QString();
                }

                break;
            }

            if (j == entryList.size() - 1)
                return QString();
        }
    }

    return QString();
}

void Texture::create()
{
    if (!m_texture)
        glGenTextures(1, &m_texture);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    quint32 width = m_header->width;
    quint32 height = m_header->height;

    GLenum internalFormat;

    switch (m_header->alphaDepth) {
        case 0:
            internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            break;
        case 1:
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        default:
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
    }

    int i;

    for (i = 0; i < 16; i++) {
        if (!m_header->mipmapLength[i])
            break;

        glCompressedTexImage2D(GL_TEXTURE_2D, i, internalFormat, width, height, 0, m_header->mipmapLength[i], m_data.data() + m_header->mipmapOffset[i]);

        if (glGetError() == GL_INVALID_VALUE)
            break;

        width >>= 1;

        if (width == 0)
            width = 1;

        height >>= 1;

        if (height == 0)
            height = 1;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i - 1);

    m_dirty = false;
}

void Texture::bind()
{
    if (m_dirty)
        create();

    glBindTexture(GL_TEXTURE_2D, m_texture);
}
