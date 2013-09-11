#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include "mvp.h"
#include "spellvisualkit.h"

class QOpenGLShaderProgram;

class M2;

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 displayId MEMBER m_displayId WRITE setDisplayId)
    Q_PROPERTY(float x MEMBER m_x)
    Q_PROPERTY(float y MEMBER m_y)
    Q_PROPERTY(float orientation MEMBER m_orientation)

public:
    Model();

    Q_INVOKABLE void addSpellVisualKit(quint32 id, bool oneshot = false);
    Q_INVOKABLE void removeSpellVisualKit(quint32 id);

    void setDisplayId(quint32 displayId);

    void update(int timeDelta);
    void render(QOpenGLShaderProgram *program, MVP mvp);
    void renderParticles(QOpenGLShaderProgram *program, MVP mvp);

private:
    void updateVisualKits();

    quint32 m_displayId;
    float m_x, m_y, m_orientation;

    M2 *m_model;
    bool m_modelChanged;
    QString m_modelFileName;
    QHash<quint32, QString> m_textureFileNames;

    QHash<quint32, SpellVisualKit> m_visualKits;
};

#endif
