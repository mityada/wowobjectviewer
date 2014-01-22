#include "creature.h"
#include "dbc.h"
#include "m2.h"

Creature::Creature()
    : m_displayId(0)
{
}

quint32 Creature::displayId() const
{
    return m_displayId;
}

void Creature::setDisplayId(quint32 displayId)
{
    m_displayId = displayId;

    CreatureDisplayInfoDBC::entry displayInfo = CreatureDisplayInfoDBC::getEntry(m_displayId);
    CreatureModelDataDBC::entry modelData = CreatureModelDataDBC::getEntry(displayInfo.model);

    if (displayInfo.id != m_displayId) {
        qDebug("Creature display ID %u does not exist", m_displayId);
        return;
    }

    setFileName(QString(modelData.model).replace("\\", "/").replace(".mdx", ".m2"));

    if (!m_model)
        return;

    QString modelPath = fileName().section('/', 0, -2);

    m_model->setTexture(TEXTURE_SKIN1, modelPath + "/" + QString(displayInfo.skin1) + ".blp");
    m_model->setTexture(TEXTURE_SKIN2, modelPath + "/" + QString(displayInfo.skin2) + ".blp");
    m_model->setTexture(TEXTURE_SKIN3, modelPath + "/" + QString(displayInfo.skin3) + ".blp");

    if (displayInfo.extra != 0) {
        CreatureDisplayInfoExtraDBC::entry extraInfo = CreatureDisplayInfoExtraDBC::getEntry(displayInfo.extra);

        Appearance appearance;
        appearance.race = extraInfo.race;
        appearance.gender = extraInfo.gender;
        appearance.skinColor = extraInfo.skinColor;
        appearance.faceType = extraInfo.faceType;
        appearance.hairType = extraInfo.hairType;
        appearance.hairStyle = extraInfo.hairStyle;
        appearance.beardStyle = extraInfo.beardStyle;
    
        setAppearance(appearance);

        m_model->setTexture(TEXTURE_BODY, "textures/BakedNpcTextures/" + QString(extraInfo.texture));
    }
}

void Creature::setAppearance(const Appearance& appearance)
{
    if (!m_model)
        return;

    CharSectionsDBC::entry base = CharSectionsDBC::getSection(appearance.race, appearance.gender, 0, 0, appearance.skinColor);
    CharSectionsDBC::entry hair = CharSectionsDBC::getSection(appearance.race, appearance.gender, 3, appearance.hairType, appearance.hairStyle);

    m_model->setTexture(TEXTURE_BODY, QString(base.texture1));
    m_model->setTexture(TEXTURE_HAIR, QString(hair.texture1));

    CharHairGeosetsDBC::entry hairGeoset = CharHairGeosetsDBC::getHairGeoset(appearance.race, appearance.gender, appearance.hairType);
    CharacterFacialHairStylesDBC::entry facialHairStyle = CharacterFacialHairStylesDBC::getEntry(appearance.race, appearance.gender, appearance.beardStyle);

    m_model->setGeoset(GEOSET_HAIR, hairGeoset.geoset);
    m_model->setGeoset(GEOSET_FACE1, facialHairStyle.geoset[0]);
    m_model->setGeoset(GEOSET_FACE2, facialHairStyle.geoset[1]);
    m_model->setGeoset(GEOSET_FACE3, facialHairStyle.geoset[2]);
}
