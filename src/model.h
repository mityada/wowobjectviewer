#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include "global.h"
#include "mvp.h"

class QOpenGLShaderProgram;

class M2;
class ModelScene;
class SpellVisualKit;
class CameraShake;

class WOV_EXPORT Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(float x READ x WRITE setX)
    Q_PROPERTY(float y READ y WRITE setY)
    Q_PROPERTY(float orientation READ orientation WRITE setOrientation)

public:
    Model();

    Q_INVOKABLE void addSpellVisualKit(quint32 id, bool oneshot = false);
    Q_INVOKABLE void removeSpellVisualKit(quint32 id);

    void addCameraShake(quint32 id);
    QVector3D getShake() const;

    const QString & fileName() const;
    void setFileName(const QString &fileName);

    float x() const;
    float y() const;
    float orientation() const;

    void setX(float x);
    void setY(float y);
    void setOrientation(float orientation);

    void update(int timeDelta);
    void render(QOpenGLShaderProgram *program, MVP mvp);
    void renderParticles(QOpenGLShaderProgram *program, MVP mvp);

protected:
    M2 *m_model;

private:
    void updateVisualKits();

    QString m_fileName;

    float m_x, m_y, m_orientation;

    QHash<quint32, SpellVisualKit *> m_visualKits;

    QList<CameraShake *> m_shakes;
    QVector3D m_shake;
};

#endif
