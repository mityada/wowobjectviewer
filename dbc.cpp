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

const DBC AnimationDataDBC::dbc("DBFilesClient/AnimationData.dbc");
const DBC CreatureDisplayInfoDBC::dbc("DBFilesClient/CreatureDisplayInfo.dbc");
const DBC CreatureModelDataDBC::dbc("DBFilesClient/CreatureModelData.dbc");
const DBC SpellVisualDBC::dbc("DBFilesClient/SpellVisual.dbc");
const DBC SpellVisualKitDBC::dbc("DBFilesClient/SpellVisualKit.dbc");
const DBC SpellVisualEffectNameDBC::dbc("DBFilesClient/SpellVisualEffectName.dbc");
