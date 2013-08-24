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
#include "particleemitter.h"
#include "mvp.h"

class M2 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 animation READ animation WRITE setAnimation)
    Q_PROPERTY(bool animating READ animating WRITE setAnimating)

public:
    explicit M2(const QString &fileName);

    void initialize(QOpenGLShaderProgram *program);
    void render(QOpenGLShaderProgram *program, MVP mvp);
    void renderParticles(QOpenGLShaderProgram *program, MVP mvp);
    void update(int timeDelta);

    void setAnimation(quint32 animation);
    qint32 animation() const;

    void setAnimating(bool animating);
    bool animating() const;

    void setTexture(quint32 type, QString fileName);

    bool attachModel(quint32 attachmentId, M2 *model);
    bool detachModel(quint32 attachmentId, M2 *model);

    template <typename T>
    T * getArray(quint32 offset)
    {
        return reinterpret_cast<T *>(m_data.data() + offset);
    }

private:
    void switchAnimation();

    void updateParticleEmitters(int timeDelta);
    void updateAttachments(int timeDelta);

    void renderAttachments(QOpenGLShaderProgram *program, MVP mvp);
    void renderAttachmentsParticles(QOpenGLShaderProgram *program, MVP mvp);

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

    QList<Texture> m_textures;
    quint16 *m_textureLookup;
    qint16 *m_replaceableTextures;

    qint16 *m_textureAnimationLookup;
    QList<TextureAnimation> m_textureAnimations;

    M2Animation *m_animations;
    qint16 *m_animationsLookup;

    QList<AnimatedValue<QVector3D> > m_colors;
    QList<AnimatedValue<quint16> > m_opacities;

    qint16 *m_transparencyLookup;
    QList<AnimatedValue<quint16> > m_transparencies;

    QHash<quint32, M2Attachment *> m_attachments;
    QMultiHash<quint32, M2 *> m_attachedModels;

    QList<ParticleEmitter> m_particleEmitters;

    quint32 *m_sequences;

    bool m_loaded;
    bool m_initialized;

    bool m_animating;
    quint32 m_animation;
    quint32 m_time;
    qint32 m_animationState;
    qint32 m_animationOneshot;

    QOpenGLVertexArrayObject *m_vao;
    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer *m_indexBuffer;
};

#endif
