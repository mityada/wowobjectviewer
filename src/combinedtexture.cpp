#include "combinedtexture.h"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>

#include "texture.h"

#include <QDebug>

CombinedTexture::CombinedTexture(int width, int height)
    : m_width(width), m_height(height),
      m_created(false), m_framebuffer(0)
{
}

bool CombinedTexture::add(const QString &fileName, int x, int y, int width, int height)
{
    m_created = false;

    Texture *texture = new Texture();
    if (!texture->load(fileName))
        return false;

    float *position = new float[4];
    position[0] = 2.0f * float(x) / float(m_width) - 1.0f;
    position[1] = 2.0f * float(y) / float(m_height) - 1.0f;
    position[2] = 2.0f * float(x + width) / float(m_width) - 1.0f;
    position[3] = 2.0f * float(y + height) / float(m_height) - 1.0f;

    m_textures << qMakePair(texture, position);

    return true;
}

bool CombinedTexture::addItem(const QString &fileName, quint8 region, quint8 gender)
{
    if (fileName.isEmpty())
        return false;

    QString path = "Item/TextureComponents/" + PATHS[region] + "/" + fileName;

    if (!add(path + (gender ? "_F.blp" : "_M.blp"), REGIONS[region][0], REGIONS[region][1], REGIONS[region][2], REGIONS[region][3]))
        return add(path + "_U.blp", REGIONS[region][0], REGIONS[region][1], REGIONS[region][2], REGIONS[region][3]);

    return true;
}

void CombinedTexture::bind()
{
    if (!m_created)
        create();

    if (!m_framebuffer)
        return;

    glBindTexture(GL_TEXTURE_2D, m_framebuffer->texture());
}

void CombinedTexture::create()
{
    if (m_framebuffer)
        delete m_framebuffer;

    QOpenGLFramebufferObjectFormat format;
    format.setMipmap(true);

    m_framebuffer = new QOpenGLFramebufferObject(m_width, m_height, format);
    m_framebuffer->bind();

    glViewport(0, 0, m_width, m_height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);

    for (int i = 0; i < m_textures.size(); i++) {
        Texture *texture = m_textures.at(i).first;
        float *position = m_textures.at(i).second;

        texture->bind();

        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(position[0], position[1], 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(position[2], position[1], 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(position[2], position[3], 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(position[0], position[3], 0.0f);

        glEnd();
    }

    m_framebuffer->release();

    glBindTexture(GL_TEXTURE_2D, m_framebuffer->texture());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    QOpenGLFunctions gl(QOpenGLContext::currentContext());
    gl.initializeOpenGLFunctions();
    gl.glGenerateMipmap(GL_TEXTURE_2D);

    m_created = true;
}
