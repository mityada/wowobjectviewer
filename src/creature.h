#ifndef CREATURE_H
#define CREATURE_H

#include "model.h"

enum ItemSlot
{
    ITEM_HELM     = 0,
    ITEM_SHOULDER = 1,
    ITEM_SHIRT    = 2,
    ITEM_CHEST    = 3,
    ITEM_BELT     = 4,
    ITEM_LEGS     = 5,
    ITEM_BOOTS    = 6,
    ITEM_WRIST    = 7,
    ITEM_GLOVES   = 8,
    ITEM_TABARD   = 9
};

struct Appearance
{
    quint8 race;
    quint8 gender;
    quint8 skinColor;
    quint8 faceType;
    quint8 hairType;
    quint8 hairStyle;
    quint8 beardStyle;

    quint32 equipment[10];
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
    void setAppearance(const Appearance& appearance, bool body = false);

private:
    quint32 m_displayId;
};

#endif
