#include <QTimer>
#include <QRegExp>
#include <QVector2D>
#include <QtCore/qmath.h>

#include "spellvisual.h"
#include "modelscene.h"

SpellVisual::SpellVisual() : m_scene(0), m_caster(0), m_target(0), m_missile(0)
{
    m_visual.id = 0;

    m_castingTime = 2000;
    m_duration = 5000;
    m_channeled = false;
}

ModelScene * SpellVisual::scene() const
{
    return m_scene;
}

quint32 SpellVisual::visual() const
{
    return m_visual.id;
}

quint32 SpellVisual::castingTime() const
{
    return m_castingTime;
}

quint32 SpellVisual::duration() const
{
    return m_duration;
}

bool SpellVisual::channeled() const
{
    return m_channeled;
}

Model * SpellVisual::caster()
{
    return m_caster;
}

Model * SpellVisual::target()
{
    return m_target;
}

void SpellVisual::setScene(ModelScene *scene)
{
    if (m_scene)
        m_scene->removeVisual(this);

    m_scene = scene;

    if (m_scene)
        m_scene->addVisual(this);
}

void SpellVisual::setVisual(quint32 visual)
{
    m_visual = SpellVisualDBC::getEntry(visual);

    if (m_visual.id != visual)
        m_visual.id = 0;
}

void SpellVisual::setCastingTime(quint32 castingTime)
{
    m_castingTime = castingTime;
}

void SpellVisual::setDuration(quint32 duration)
{
    m_duration = duration;
}

void SpellVisual::setChanneled(bool channeled)
{
    m_channeled = channeled;
}

void SpellVisual::setCaster(Model *caster)
{
    m_caster = caster;
}

void SpellVisual::setTarget(Model *target)
{
    m_target = target;
}

void SpellVisual::update(int timeDelta)
{
    if (!m_missile || !m_target)
        return;

    QVector2D speed;
    speed.setX(m_target->x() - m_missile->x());
    speed.setY(m_target->y() - m_missile->y());

    if (speed.length() < 0.1f) {
        impact();

        m_scene->removeModel(m_missile);
        delete m_missile;
        m_missile = 0;

        return;
    }

    speed.normalize();
    speed *= 20.0f;

    float time = timeDelta / 1000.0f;

    m_missile->setX(m_missile->x() + speed.x() * time);
    m_missile->setY(m_missile->y() + speed.y() * time);
    m_missile->setOrientation(180 * qAtan2(speed.y(), speed.x()) / M_PI);
}

void SpellVisual::impact()
{
    if (target()) {
        target()->addSpellVisualKit(m_visual.impact, true);

        if (duration())
            target()->addSpellVisualKit(m_visual.state);
    }

    if (duration())
        QTimer::singleShot(duration(), this, SLOT(cancel()));
}

void SpellVisual::start()
{
    if (!visual())
        return;

    if (caster())
        caster()->addSpellVisualKit(m_visual.precast);

    QTimer::singleShot(castingTime(), this, SLOT(cast()));
}

void SpellVisual::cast()
{
    if (!visual())
        return;

    if (caster()) {
        caster()->removeSpellVisualKit(m_visual.precast);
        caster()->addSpellVisualKit(m_visual.cast, true);

        if (duration() && channeled())
            caster()->addSpellVisualKit(m_visual.channel);

        if (target()) {
            if (m_scene && m_visual.hasMissile) {
                SpellVisualEffectNameDBC::entry missile = SpellVisualEffectNameDBC::getEntry(m_visual.missile);
                QString missileFile(QString(missile.model).replace(QRegExp(".md[x]", Qt::CaseInsensitive), ".m2"));

                m_missile = new Model();
                m_missile->setFileName(missileFile);
                m_missile->setX(caster()->x());
                m_missile->setY(caster()->y());
                m_missile->setOrientation(caster()->orientation());

                m_scene->addModel(m_missile);
            } else {
                impact();
            }
        }
    }
}

void SpellVisual::cancel()
{
    if (!visual())
        return;

    if (caster()) {
        caster()->removeSpellVisualKit(m_visual.precast);
        caster()->removeSpellVisualKit(m_visual.channel);
    }

    if (target())
        target()->removeSpellVisualKit(m_visual.state);
}
