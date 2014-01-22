#include "character.h"

Character::Character() : m_race(0), m_gender(0), m_skinColor(0), m_faceType(0), m_hairType(0), m_hairStyle(0), m_beardStyle(0)
{
}

quint32 Character::race() const
{
    return m_race;
}

quint32 Character::gender() const
{
    return m_gender;
}

quint32 Character::skinColor() const
{
    return m_skinColor;
}

quint32 Character::faceType() const
{
    return m_faceType;
}

quint32 Character::hairType() const
{
    return m_hairType;
}

quint32 Character::hairStyle() const
{
    return m_hairStyle;
}

quint32 Character::beardStyle() const
{
    return m_beardStyle;
}

void Character::setRace(quint32 race)
{
    m_race = race;
    updateModel();
}

void Character::setGender(quint32 gender)
{
    m_gender = gender;
    updateModel();
}

void Character::setSkinColor(quint32 skinColor)
{
    m_skinColor = skinColor;
    updateAppearance();
}

void Character::setFaceType(quint32 faceType)
{
    m_faceType = faceType;
    updateAppearance();
}

void Character::setHairType(quint32 hairType)
{
    m_hairType = hairType;
    updateAppearance();
}

void Character::setHairStyle(quint32 hairStyle)
{
    m_hairStyle = hairStyle;
    updateAppearance();
}

void Character::setBeardStyle(quint32 beardStyle)
{
    m_beardStyle = beardStyle;
    updateAppearance();
}

void Character::updateModel()
{
    ChrRacesDBC::entry race = ChrRacesDBC::getEntry(m_race);

    if (race.id != m_race)
        return;

    if (m_gender == 0)
        setDisplayId(race.maleDisplayId);
    else
        setDisplayId(race.femaleDisplayId);

    updateAppearance();
}

void Character::updateAppearance()
{
    Appearance appearance;
    appearance.race = m_race;
    appearance.gender = m_gender;
    appearance.skinColor = m_skinColor;
    appearance.faceType = m_faceType;
    appearance.hairType = m_hairType;
    appearance.hairStyle = m_hairStyle;
    appearance.beardStyle = m_beardStyle;

    setAppearance(appearance);
}
