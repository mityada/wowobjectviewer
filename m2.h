#ifndef M2_H
#define M2_H

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QTimer>

#include "m2structures.h"
#include "texture.h"
#include "textureanimation.h"
#include "bone.h"

class M2 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 animation READ animation WRITE setAnimation)
    Q_PROPERTY(bool animating READ animating WRITE setAnimating)

public:
    explicit M2(const QString &fileName);

    void initialize(QOpenGLShaderProgram *program);
    void render(QOpenGLShaderProgram *program);
    void update(int timeDelta);

    void setAnimation(quint32 animation);
    quint32 animation() const;

    void setAnimating(bool animating);
    bool animating() const;

    void setTexture(quint32 type, QString fileName);

    template <typename T>
    T * getArray(quint32 offset)
    {
        return reinterpret_cast<T *>(m_data.data() + offset);
    }

private:
    void updateBones();

    QByteArray m_data;

    M2Header *m_header;

    M2Vertex *m_vertices;
    quint16 *m_indices;
    quint32 m_indicesCount;

    M2View *m_views;
    M2Submesh *m_submeshes;
    M2TextureUnit *m_textureUnits;
    M2RenderFlags *m_renderFlags;

    QList<Bone> m_bones;
    QMatrix4x4 *m_boneMatrices;

    QList<Texture> m_textures;
    quint16 *m_textureLookup;
    qint16 *m_replaceableTextures;

    qint16 *m_textureAnimationLookup;
    QList<TextureAnimation> m_textureAnimations;

    M2Animation *m_animations;

    QList<AnimatedValue<QVector3D> > m_colors;
    QList<AnimatedValue<quint16> > m_opacities;

    qint16 *m_transparencyLookup;
    QList<AnimatedValue<quint16> > m_transparencies;

    quint32 *m_sequences;

    bool m_loaded;
    bool m_initialized;

    bool m_animating;
    quint32 m_animation;
    quint32 m_time;

    QOpenGLVertexArrayObject *m_vao;
    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer *m_indexBuffer;
};

#endif
