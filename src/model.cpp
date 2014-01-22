#include "model.h"
#include "m2.h"
#include "spellvisualkit.h"
#include "camerashake.h"

Model::Model()
    : m_model(0),
      m_x(0.0f), m_y(0.0f), m_orientation(0.0f)
{
}

void Model::addSpellVisualKit(quint32 id, bool oneshot)
{
    if (m_visualKits.contains(id))
        return;

    m_visualKits[id] = new SpellVisualKit(id, oneshot);
    m_visualKits[id]->addCameraShakes(this);
}

void Model::removeSpellVisualKit(quint32 id)
{
    QHash<quint32, SpellVisualKit *>::iterator it = m_visualKits.find(id);

    if (it != m_visualKits.end())
        it.value()->detach();
}

void Model::addCameraShake(quint32 id)
{
    m_shakes << new CameraShake(id);
};

QVector3D Model::getShake() const
{
    return m_shake;
}

const QString & Model::fileName() const
{
    return m_fileName;
}

void Model::setFileName(const QString &fileName)
{
    m_fileName = fileName;

    if (m_model)
        delete m_model;

    m_model = new M2(fileName);
    m_model->setAnimation(0);
}

float Model::x() const
{
    return m_x;
}

float Model::y() const
{
    return m_y;
}

float Model::orientation() const
{
    return m_orientation;
}

void Model::setX(float x)
{
    m_x = x;
}

void Model::setY(float y)
{
    m_y = y;
}

void Model::setOrientation(float orientation)
{
    m_orientation = orientation;
}

void Model::update(int timeDelta)
{
    if (m_model) {
        QMatrix4x4 matrix;
        matrix.translate(m_x, m_y, 0.0f);
        matrix.rotate(m_orientation, 0.0f, 0.0f, 1.0f);

        m_model->update(timeDelta, matrix);
    }

    m_shake = QVector3D();

    for (int i = 0; i < m_shakes.size();) {
        if (m_shakes[i]->update(timeDelta)) {
            m_shake += m_shakes[i++]->getShake();
        } else {
            delete m_shakes[i];
            m_shakes.removeAt(i);
        }
    }

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
    if (m_model)
        m_model->renderParticles(program, mvp);
}

void Model::updateVisualKits()
{
    QHash<quint32, SpellVisualKit *>::iterator it = m_visualKits.begin();
    while (it != m_visualKits.end()) {
        if (!it.value()->update(m_model))
            it = m_visualKits.erase(it);
        else
            it++;
    }
}
