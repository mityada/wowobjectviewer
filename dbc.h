#ifndef DBC_H
#define DBC_H

#include <QString>
#include <QFile>

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
        for (quint32 i = 0; i < m_header->recordCount; i++) {
            T entry = getRecord<T>(i);

            if (entry.id == id)
                return entry;
        }

        return T();
    }

    const char * getStringBlock() const;

private:
    template <typename T>
    T getRecord(quint32 id) const
    {
        return *reinterpret_cast<const T *>(m_records + m_header->recordSize * id);
    }

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
