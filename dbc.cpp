#include "dbc.h"

DBC::DBC(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical("File '%s' cannot be opened!", qPrintable(fileName));
        return;
    }

    m_data = file.readAll();
    file.close();

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

AnimationDataDBC::entry AnimationDataDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/AnimationData.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.name = dbc.getStringBlock() + quint32(e.name);

    return e;
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

CreatureModelDataDBC::entry CreatureModelDataDBC::getEntry(quint32 id)
{
    static DBC dbc("DBFilesClient/CreatureModelData.dbc");

    entry e = dbc.getEntry<entry>(id);
    e.model = dbc.getStringBlock() + quint32(e.model);

    return e;
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


