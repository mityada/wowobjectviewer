#ifndef RIBBON_EMITTER_H
#define RIBBON_EMITTER_H

#include <QGLShaderProgram>
#include <QGLBuffer>

#include "m2structures.h"
#include "animatedvalue.h"
#include "mvp.h"
#include "particleemitter.h"

struct Ribbon
{
    QVector3D position;
    QVector3D up;
};

class RibbonEmitter
{
public:
    RibbonEmitter(const M2RibbonEmitter &emitter, const quint32 *sequences, const QByteArray &data);

    void update(quint32 animation, quint32 time, QMatrix4x4 boneMatrix);
    void render(QGLShaderProgram *program, MVP viewProjection);

    qint32 getBoneId() const;
    qint32 getTextureId() const;

private:
    void initialize();

    qint32 m_bone;
    QVector3D m_position;
    QList<qint32> m_textures;
    QList<qint32> m_blending;
    AnimatedValue<QVector3D> m_color;
    AnimatedValue<qint16> m_opacity;
    AnimatedValue<float> m_above;
    AnimatedValue<float> m_below;
    float m_segments;
    float m_length;
    float m_angle;

    QList<Ribbon> m_ribbons;
    ParticleVertex *m_vertices;
    QGLBuffer *m_vertexBuffer;

    bool m_initialized;
};

#endif
