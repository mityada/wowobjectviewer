#include "creature.h"
#include "dbc.h"
#include "m2.h"
#include "combinedtexture.h"

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

        for (int i = 0; i < 10; i++)
            appearance.equipment[i] = extraInfo.equipment[i];
    
        setAppearance(appearance);

        m_model->setTexture(TEXTURE_BODY, "textures/BakedNpcTextures/" + QString(extraInfo.texture));
    }
}

void Creature::setAppearance(const Appearance& appearance, bool body)
{
    if (!m_model)
        return;

    CharSectionsDBC::entry base = CharSectionsDBC::getSection(appearance.race, appearance.gender, 0, 0, appearance.skinColor);
    CharSectionsDBC::entry face = CharSectionsDBC::getSection(appearance.race, appearance.gender, 1, appearance.faceType, appearance.skinColor);
    CharSectionsDBC::entry facialHair = CharSectionsDBC::getSection(appearance.race, appearance.gender, 2, appearance.beardStyle, appearance.hairStyle);
    CharSectionsDBC::entry hair = CharSectionsDBC::getSection(appearance.race, appearance.gender, 3, appearance.hairType, appearance.hairStyle);
    CharSectionsDBC::entry underwear = CharSectionsDBC::getSection(appearance.race, appearance.gender, 4, 0, appearance.skinColor);

    CombinedTexture *texture = 0;

    if (body) {
        texture = new CombinedTexture(256, 256);
        texture->add(QString(base.texture1), 0, 0, 256, 256);
        texture->add(QString(face.texture1), 0, 192, 128, 64);
        texture->add(QString(face.texture2), 0, 160, 128, 32);
        texture->add(QString(facialHair.texture1), 0, 192, 128, 64);
        texture->add(QString(facialHair.texture2), 0, 160, 128, 32);
        texture->add(QString(underwear.texture1), 128, 96, 128, 64);
        texture->add(QString(underwear.texture2), 128, 0, 128, 64);
    }

    for (int i = 0; i < 10; i++) {
        if (appearance.equipment[i] == 0)
            continue;

        ItemDisplayInfoDBC::entry item = ItemDisplayInfoDBC::getEntry(appearance.equipment[i]);

        if (item.id != appearance.equipment[i])
            continue;

        if (body) {
            texture->addItem(QString(item.upperArmTexture), TEXTURE_UPPER_ARM, appearance.gender);
            texture->addItem(QString(item.lowerArmTexture), TEXTURE_LOWER_ARM, appearance.gender);
            texture->addItem(QString(item.handsTexture), TEXTURE_HAND, appearance.gender);
            texture->addItem(QString(item.upperTorsoTexture), TEXTURE_UPPER_TORSO, appearance.gender);
            texture->addItem(QString(item.lowerTorsoTexture), TEXTURE_LOWER_TORSO, appearance.gender);
            texture->addItem(QString(item.upperLegTexture), TEXTURE_UPPER_PELVIS, appearance.gender);
            texture->addItem(QString(item.lowerLegTexture), TEXTURE_LOWER_PELVIS, appearance.gender);
            texture->addItem(QString(item.footTexture), TEXTURE_FOOT, appearance.gender);
        }

        if (i == ITEM_SHIRT || i == ITEM_CHEST)
            m_model->setGeoset(GEOSET_SLEEVES, item.geoset[0] + 1);

        if (i == ITEM_LEGS)
            m_model->setGeoset(GEOSET_PANTS, item.geoset[1] + 1);

        if (i == ITEM_GLOVES)
            m_model->setGeoset(GEOSET_WRISTS, item.geoset[0] + 1);

        if (i == ITEM_BOOTS)
            m_model->setGeoset(GEOSET_FEET, item.geoset[0] + 1);

        if (i == ITEM_TABARD)
            m_model->setGeoset(GEOSET_TABARD, 2);

        if (i == ITEM_CHEST)
            m_model->setGeoset(GEOSET_LEGS, item.geoset[2] + 1);

        if (i == ITEM_SHOULDER) {
            M2 *left = new M2("Item/ObjectComponents/Shoulder/" + QString(item.leftModel).replace(".mdx", ".m2"));
            M2 *right = new M2("Item/ObjectComponents/Shoulder/" + QString(item.rightModel).replace(".mdx", ".m2"));

            left->setTexture(TEXTURE_CAPE, "Item/ObjectComponents/Shoulder/" + QString(item.leftTexture) + ".blp");
            right->setTexture(TEXTURE_CAPE, "Item/ObjectComponents/Shoulder/" + QString(item.rightTexture) + ".blp");

            m_model->attachModel(ATTACHMENT_LEFT_SHOULDER, left);
            m_model->attachModel(ATTACHMENT_RIGHT_SHOULDER, right);
        }

        if (i == ITEM_HELM) {
            ChrRacesDBC::entry race = ChrRacesDBC::getEntry(appearance.race);

            M2 *helm = new M2("Item/ObjectComponents/Head/" + QString(item.leftModel).replace(".mdx", "_" + QString(race.prefix) + (appearance.gender ? "F" : "M") + ".m2"));

            helm->setTexture(TEXTURE_CAPE, "Item/ObjectComponents/Head/" + QString(item.leftTexture) + ".blp");

            m_model->attachModel(ATTACHMENT_HELM, helm);
        }
    }

    if (body)
        m_model->setBodyTexture(texture);

    m_model->setTexture(TEXTURE_HAIR, QString(hair.texture1));

    CharHairGeosetsDBC::entry hairGeoset = CharHairGeosetsDBC::getHairGeoset(appearance.race, appearance.gender, appearance.hairType);
    CharacterFacialHairStylesDBC::entry facialHairStyle = CharacterFacialHairStylesDBC::getEntry(appearance.race, appearance.gender, appearance.beardStyle);

    m_model->setGeoset(GEOSET_HAIR, hairGeoset.geoset);
    m_model->setGeoset(GEOSET_FACE1, facialHairStyle.geoset[0]);
    m_model->setGeoset(GEOSET_FACE2, facialHairStyle.geoset[1]);
    m_model->setGeoset(GEOSET_FACE3, facialHairStyle.geoset[2]);
}
