#include "model.h"
#include "m2.h"
#include "dbc.h"

Model::Model()
    : m_displayId(0),
      m_x(0.0f), m_y(0.0f), m_orientation(0.0f),
      m_model(0), m_modelChanged(false)
{
}

void Model::addSpellVisualKit(quint32 id, bool oneshot)
{
    if (m_visualKits.contains(id))
        return;

    m_visualKits.insert(id, SpellVisualKit(id, oneshot));
}

void Model::removeSpellVisualKit(quint32 id)
{
    QHash<quint32, SpellVisualKit>::iterator it = m_visualKits.find(id);

    if (it != m_visualKits.end())
        it.value().detach();
}

void Model::setDisplayId(quint32 displayId)
{
    m_displayId = displayId;

    CreatureDisplayInfoDBC::entry displayInfo = CreatureDisplayInfoDBC::getEntry(m_displayId);
    CreatureModelDataDBC::entry modelData = CreatureModelDataDBC::getEntry(displayInfo.model);

    if (displayInfo.id != m_displayId) {
        qDebug("Creature display ID %u does not exist", m_displayId);
        return;
    }

    m_modelFileName = QString(modelData.model).replace("\\", "/").replace(".mdx", ".m2");

    QString modelPath = m_modelFileName.section('/', 0, -2);

    m_textureFileNames[11] = modelPath + "/" + QString(displayInfo.skin1) + ".blp";
    m_textureFileNames[12] = modelPath + "/" + QString(displayInfo.skin2) + ".blp";
    m_textureFileNames[13] = modelPath + "/" + QString(displayInfo.skin3) + ".blp";

    m_modelChanged = true;
}

void Model::update(int timeDelta)
{
    if (m_modelChanged) {
        if (m_model)
            delete m_model;

        QTime time;
        time.start();

        m_model = new M2(m_modelFileName);

        m_model->setAnimation(0);

        QHash<quint32, QString>::const_iterator it = m_textureFileNames.constBegin();
        while (it != m_textureFileNames.constEnd()) {
            m_model->setTexture(it.key(), it.value());
            it++;
        }

        m_modelChanged = false;
    }

    if (m_model)
        m_model->update(timeDelta);

    updateVisualKits();
}

void Model::render(QOpenGLShaderProgram *program, MVP mvp)
{
    mvp.model.translate(m_x, m_y, 0.0f);
    mvp.model.rotate(m_orientation, 0.0f, 0.0f, 1.0f);

    if (m_model)
        m_model->render(program, mvp);
}

void Model::renderParticles(QOpenGLShaderProgram *program, MVP mvp)
{
    mvp.model.translate(m_x, m_y, 0.0f);
    mvp.model.rotate(m_orientation, 0.0f, 0.0f, 1.0f);

    if (m_model)
        m_model->renderParticles(program, mvp);
}

void Model::updateVisualKits()
{
    QHash<quint32, SpellVisualKit>::iterator it = m_visualKits.begin();
    while (it != m_visualKits.end()) {
        if (!it.value().update(m_model))
            it = m_visualKits.erase(it);
        else
            it++;
    }
}
