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

class CreatureDisplayInfoDBC
{
public:
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

    static entry getEntry(quint32 id)
    {
        entry e = dbc.getEntry<entry>(id);
        e.skin1 = dbc.getStringBlock() + quint32(e.skin1);
        e.skin2 = dbc.getStringBlock() + quint32(e.skin2);
        e.skin3 = dbc.getStringBlock() + quint32(e.skin3);

        return e;
    }
private:
    static const DBC dbc;
};

class CreatureModelDataDBC
{
public:
    struct entry
    {
        quint32 id;
        quint32 flags;
        const char *model;
    };

    static entry getEntry(quint32 id)
    {
        entry e = dbc.getEntry<entry>(id);
        e.model = dbc.getStringBlock() + quint32(e.model);

        return e;
    }

private:
    static const DBC dbc;
};

#endif
