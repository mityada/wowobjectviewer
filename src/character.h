#ifndef CHARACTER_H
#define CHARACTER_H

#include "creature.h"
#include "dbc.h"

class Character : public Creature 
{
    Q_OBJECT
    Q_PROPERTY(quint32 race READ race WRITE setRace)
    Q_PROPERTY(quint32 gender READ gender WRITE setGender)
    Q_PROPERTY(quint32 skinColor READ skinColor WRITE setSkinColor)
    Q_PROPERTY(quint32 faceType READ faceType WRITE setFaceType)
    Q_PROPERTY(quint32 hairType READ hairType WRITE setHairType)
    Q_PROPERTY(quint32 hairStyle READ hairStyle WRITE setHairStyle)
    Q_PROPERTY(quint32 beardStyle READ beardStyle WRITE setBeardStyle)

public:
    Character();

    quint32 race() const;
    quint32 gender() const;
    quint32 skinColor() const;
    quint32 faceType() const;
    quint32 hairType() const;
    quint32 hairStyle() const;
    quint32 beardStyle() const;

    void setRace(quint32 race);
    void setGender(quint32 gender);
    void setSkinColor(quint32 skinColor);
    void setFaceType(quint32 faceType);
    void setHairType(quint32 hairType);
    void setHairStyle(quint32 hairStyle);
    void setBeardStyle(quint32 beardStyle);

private:
    void updateModel();
    void updateAppearance();

    quint8 m_race;
    quint8 m_gender;
    quint8 m_skinColor;
    quint8 m_faceType;
    quint8 m_hairType;
    quint8 m_hairStyle;
    quint8 m_beardStyle;

};

#endif
