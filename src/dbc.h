#ifndef DBC_H
#define DBC_H

#include <QString>

#define DBC_MAGIC "WDBC"

struct DBCHeader
{
    char magic[4];
    quint32 recordCount;
    quint32 fieldCount;
    quint32 recordSize;
    quint32 stringBlockSize;
};

class DBC
{
public:
    explicit DBC(const QString &fileName);

    template <typename T>
    T getEntry(quint32 id) const
    {
        if (!m_header)
            return T();

        for (quint32 i = 0; i < m_header->recordCount; i++) {
            T entry = getRecord<T>(i);

            if (entry.id == id)
                return entry;
        }

        return T();
    }

    template <typename T>
    T getRecord(quint32 id) const
    {
        return *reinterpret_cast<const T *>(m_records + m_header->recordSize * id);
    }

    const char * getStringBlock() const;
    quint32 getRecordCount() const;

private:
    QByteArray m_data;
    const DBCHeader *m_header;
    const char *m_records;
    const char *m_strings;
};

namespace AnimationDataDBC
{
    struct entry
    {
        quint32 id;
        const char *name;
    };

    entry getEntry(quint32 id);
}

namespace CameraShakesDBC
{
    struct entry
    {
        quint32 id;
        quint32 type;
        quint32 axis;
        float amplitude;
        float frequency;
        float duration;
        float phase;
        float coefficient;
    };

    entry getEntry(quint32 id);
}

namespace CharacterFacialHairStylesDBC
{
    struct entry
    {
        quint32 race;
        quint32 gender;
        quint32 type;
        quint32 unknown[3];
        quint32 geoset[3];
    };

    entry getEntry(quint32 race, quint32 gender, quint32 type);
}

namespace CharHairGeosetsDBC
{
    struct entry
    {
        quint32 id;
        quint32 race;
        quint32 gender;
        quint32 hairType;
        quint32 geoset;
        quint32 bald;
    };

    entry getHairGeoset(quint32 race, quint32 gender, quint32 hairType);
}

namespace CharSectionsDBC
{
    struct entry
    {
        quint32 id;
        quint32 race;
        quint32 gender;
        quint32 sectionType;
        quint32 type;
        quint32 color;
        const char *texture1;
        const char *texture2;
        const char *texture3;
    };

    entry getSection(quint32 race, quint32 gender, quint32 sectionType, quint32 type, quint32 color);
}

namespace ChrRacesDBC
{
    struct entry
    {
        quint32 id;
        quint32 flags;
        quint32 faction;
        quint32 explorationSound;
        quint32 maleDisplayId;
        quint32 femaleDisplayId;
        const char *prefix;
    };

    entry getEntry(quint32 id);
}

namespace CreatureDisplayInfoDBC
{
    struct entry
    {
        quint32 id;
        quint32 model;
        quint32 sound;
        quint32 extra;
        float scale;
        quint32 opacity;
        const char *skin1;
        const char *skin2;
        const char *skin3;
        quint32 unknown1;
        quint32 unknown2;
        quint32 npcSound;
    };

    entry getEntry(quint32 id);
    entry getRecord(quint32 record);
}

namespace CreatureDisplayInfoExtraDBC
{
    struct entry
    {
        quint32 id;
        quint32 race;
        quint32 gender;
        quint32 skinColor;
        quint32 faceType;
        quint32 hairType;
        quint32 hairStyle;
        quint32 beardStyle;
        quint32 equipment[10];
        const char *texture;
    };

    entry getEntry(quint32 id);
}

namespace CreatureModelDataDBC
{
    struct entry
    {
        quint32 id;
        quint32 flags;
        const char *model;
    };

    entry getEntry(quint32 id);
}

namespace ItemDisplayInfoDBC
{
    struct entry
    {
        quint32 id;
        const char *leftModel;
        const char *rightModel;
        const char *leftTexture;
        const char *rightTexture;
        const char *icon1;
        quint32 geoset[4];
        quint32 spellVisual;
        quint32 groupSound;
        quint32 helmetGeosetVis[2];
        const char *upperArmTexture;
        const char *lowerArmTexture;
        const char *handsTexture;
        const char *upperTorsoTexture;
        const char *lowerTorsoTexture;
        const char *upperLegTexture;
        const char *lowerLegTexture;
        const char *footTexture;
    };

    entry getEntry(quint32 id);
}

namespace SpellEffectCameraShakesDBC
{
    struct entry
    {
        quint32 id;
        quint32 shakes[3];
    };

    entry getEntry(quint32 id);
}

namespace SpellDBC
{
    struct entry
    {
        quint32 id;
        quint32 resistances;
        quint32 category;
        quint32 unknown1;
        quint32 dispelType;
        quint32 mechanic;
        quint32 attributes;
        quint32 attributesEx;
        quint32 targets;
        quint32 unknown2[5];;
    };
}

namespace SpellVisualDBC
{
    struct entry
    {
        quint32 id;
        quint32 precast;
        quint32 cast;
        quint32 impact;
        quint32 state;
        quint32 channel;
        quint32 hasMissile;
        quint32 missile;
    };

    entry getEntry(quint32 id);
}

namespace SpellVisualKitDBC
{
    struct entry
    {
        quint32 id;
        qint32 startAnimation;
        qint32 animation;
        qint32 head;
        qint32 chest;
        qint32 base;
        qint32 leftHand;
        qint32 rightHand;
        qint32 breath1;
        qint32 breath2;
        qint32 unknown1;
        qint32 unknown2;
        qint32 base2;
        quint32 sound;
        quint32 shakes;
    };

    entry getEntry(quint32 id);
}

namespace SpellVisualEffectNameDBC
{
    struct entry
    {
        quint32 id;
        const char *name;
        const char *model;
    };

    entry getEntry(quint32 id);
}

#endif
