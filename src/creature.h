#ifndef CREATURE_H
#define CREATURE_H

#include "model.h"

struct Appearance
{
    quint8 race;
    quint8 gender;
    quint8 skinColor;
    quint8 faceType;
    quint8 hairType;
    quint8 hairStyle;
    quint8 beardStyle;
};

class WOV_EXPORT Creature : public Model
{
    Q_OBJECT
    Q_PROPERTY(quint32 displayId READ displayId WRITE setDisplayId)

public:
    Creature();

    quint32 displayId() const;
    void setDisplayId(quint32 displayId);

protected:
    void setAppearance(const Appearance& appearance);

private:
    quint32 m_displayId;
};

#endif
