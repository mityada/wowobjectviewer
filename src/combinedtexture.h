#ifndef COMBINED_TEXTURE_H
#define COMBINED_TEXTURE_H

#include <QList>
#include <QPair>
#include <QString>

class Texture;
class QOpenGLFramebufferObject;

enum TextureRegion
{
    TEXTURE_UPPER_ARM    = 0,
    TEXTURE_LOWER_ARM    = 1 ,
    TEXTURE_HAND         = 2,
    TEXTURE_UPPER_FACE   = 3,
    TEXTURE_LOWER_FACE   = 4,
    TEXTURE_UPPER_TORSO  = 5,
    TEXTURE_LOWER_TORSO  = 6,
    TEXTURE_UPPER_PELVIS = 7,
    TEXTURE_LOWER_PELVIS = 8,
    TEXTURE_FOOT         = 9
};

static const int REGIONS[10][4] = {
    {  0,   0, 128, 64},
    {  0,  64, 128, 64},
    {  0, 128, 128, 32},
    {  0, 160, 128, 32},
    {  0, 192, 128, 64}, 
    {128,   0, 128, 64},
    {128,  64, 128, 32},
    {128,  96, 128, 64},
    {128, 160, 128, 64},
    {128, 224, 128, 32}
};

static const QString PATHS[10] = {
    "ArmUpperTexture",
    "ArmLowerTexture",
    "HandTexture",
    "",
    "",
    "TorsoUpperTexture",
    "TorsoLowerTexture",
    "LegUpperTexture",
    "LegLowerTexture",
    "FootTexture"
};

class CombinedTexture
{
public:
    CombinedTexture(int width, int height);

    bool add(const QString &fileName, int x, int y, int width, int height);
    bool addItem(const QString &fileName, quint8 region, quint8 gender);
    void bind();

private:
    void create();

    int m_width;
    int m_height;

    QList<QPair<Texture *, float *> > m_textures;

    bool m_created;

    QOpenGLFramebufferObject *m_framebuffer;
};

#endif
