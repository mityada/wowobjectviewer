#include "dbc.h"
#include "mpq.h"

DBC::DBC(const QString &fileName) : m_header(0), m_records(0), m_strings(0)
{
    m_data = MPQ::readFile(fileName);

    if (m_data.size() == 0) {
        qCritical("Cannot load DBC '%s'", qPrintable(fileName));
        return;
    }

    m_header = reinterpret_cast<const DBCHeader *>(m_data.constData());

    if (qstrncmp(m_header->magic, DBC_MAGIC, 4) != 0) {
        qCritical("File '%s' is not a valid DBC file!", qPrintable(fileName));
        return;
    }

    m_records = m_data.constData() + sizeof(DBCHeader);
    m_strings = m_records + m_header->recordCount * m_header->recordSize;
}

const char * DBC::getStringBlock() const
{
    return m_strings;
}

quint32 DBC::getRecordCount() const
{
    return m_header->recordCount;
}

AnimationDataDBC::entry AnimationDataDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/AnimationData.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.name = dbc.getStringBlock() + quint32(e.name);

    return e;
}

CameraShakesDBC::entry CameraShakesDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/CameraShakes.dbc");

    return dbc.getEntry<entry>(id);
}

CharacterFacialHairStylesDBC::entry CharacterFacialHairStylesDBC::getEntry(quint32 race, quint32 gender, quint32 type)
{
    static DBC dbc("DBFilesClient/CharacterFacialHairStyles.dbc");

    quint32 count = dbc.getRecordCount();

    for (quint32 i = 0; i < count; i++) {
        entry e = dbc.getRecord<entry>(i);

        if (e.race != race || e.gender != gender || e.type != type)
            continue;

        return e;
    }

    return entry();
}

CharHairGeosetsDBC::entry CharHairGeosetsDBC::getHairGeoset(quint32 race, quint32 gender, quint32 hairType)
{
    static DBC dbc("DBFilesClient/CharHairGeosets.dbc");

    quint32 count = dbc.getRecordCount();

    for (quint32 i = 0; i < count; i++) {
        entry e = dbc.getRecord<entry>(i);

        if (e.race != race || e.gender != gender || e.hairType != hairType)
            continue;

        return e;
    }

    return entry();
}

CharSectionsDBC::entry CharSectionsDBC::getSection(quint32 race, quint32 gender, quint32 sectionType, quint32 type, quint32 color)
{
    static DBC dbc("DBFilesClient/CharSections.dbc");

    quint32 count = dbc.getRecordCount();

    for (quint32 i = 0; i < count; i++) {
        entry e = dbc.getRecord<entry>(i);

        if (e.race != race || e.gender != gender)
            continue;

        if (e.sectionType != sectionType)
            continue;

        if (e.type != type || e.color != color)
            continue;

        e.texture1 = dbc.getStringBlock() + quint32(e.texture1);
        e.texture2 = dbc.getStringBlock() + quint32(e.texture2);
        e.texture3 = dbc.getStringBlock() + quint32(e.texture3);

        return e;
    }

    return entry();
}

ChrRacesDBC::entry ChrRacesDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/ChrRaces.dbc");

    return dbc.getEntry<entry>(id);
}

CreatureDisplayInfoDBC::entry CreatureDisplayInfoDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/CreatureDisplayInfo.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.skin1 = dbc.getStringBlock() + quint32(e.skin1);
    e.skin2 = dbc.getStringBlock() + quint32(e.skin2);
    e.skin3 = dbc.getStringBlock() + quint32(e.skin3);

    return e;
}

CreatureDisplayInfoDBC::entry CreatureDisplayInfoDBC::getRecord(quint32 record)
{
    static DBC dbc("DBFilesClient/CreatureDisplayInfo.dbc");

    if (record >= dbc.getRecordCount())
        return entry();

    entry e = dbc.getRecord<entry>(record);
    e.skin1 = dbc.getStringBlock() + quint32(e.skin1);
    e.skin2 = dbc.getStringBlock() + quint32(e.skin2);
    e.skin3 = dbc.getStringBlock() + quint32(e.skin3);

    return e;
}

CreatureDisplayInfoExtraDBC::entry CreatureDisplayInfoExtraDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/CreatureDisplayInfoExtra.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.texture = dbc.getStringBlock() + quint32(e.texture);

    return e;
}

CreatureModelDataDBC::entry CreatureModelDataDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/CreatureModelData.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.model = dbc.getStringBlock() + quint32(e.model);

    return e;
}

SpellEffectCameraShakesDBC::entry SpellEffectCameraShakesDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/SpellEffectCameraShakes.dbc");

    return dbc.getEntry<entry>(id);
}

SpellVisualDBC::entry SpellVisualDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/SpellVisual.dbc");

    return dbc.getEntry<entry>(id);
}

SpellVisualKitDBC::entry SpellVisualKitDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/SpellVisualKit.dbc");

    return dbc.getEntry<entry>(id);
}

SpellVisualEffectNameDBC::entry SpellVisualEffectNameDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/SpellVisualEffectName.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.name = dbc.getStringBlock() + quint32(e.name);
    e.model = dbc.getStringBlock() + quint32(e.model);

    return e;
}


