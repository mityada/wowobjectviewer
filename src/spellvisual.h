#ifndef SPELL_VISUAL_H
#define SPELL_VISUAL_H

#include <QObject>

#include "global.h"
#include "dbc.h"
#include "model.h"

class ModelScene;

class WOV_EXPORT SpellVisual : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ModelScene * scene READ scene WRITE setScene)
    Q_PROPERTY(quint32 visual READ visual WRITE setVisual)
    Q_PROPERTY(quint32 castingTime READ castingTime WRITE setCastingTime)
    Q_PROPERTY(quint32 duration READ duration WRITE setDuration)
    Q_PROPERTY(bool channeled READ channeled WRITE setChanneled)
    Q_PROPERTY(Model * caster READ caster WRITE setCaster)
    Q_PROPERTY(Model * target READ target WRITE setTarget)

public:
    SpellVisual();

    ModelScene * scene() const;
    quint32 visual() const;
    quint32 castingTime() const;
    quint32 duration() const;
    bool channeled() const;
    Model * caster();
    Model * target();

    void setScene(ModelScene *scene);
    void setVisual(quint32 visual);
    void setCastingTime(quint32 castingTime);
    void setDuration(quint32 duration);
    void setChanneled(bool channeled);
    void setCaster(Model *caster);
    void setTarget(Model *target);

    void update(int timeDelta);

    void impact();

public slots:
    void start();
    void cast();
    void cancel();

private:
    ModelScene *m_scene;

    SpellVisualDBC::entry m_visual;

    quint32 m_castingTime;
    quint32 m_duration;
    bool m_channeled;

    Model *m_caster;
    Model *m_target;
    Model *m_missile;
};

#endif
