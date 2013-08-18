#include "bone.h"

Bone::Bone(const M2Bone &bone, const quint32 *sequences, const QByteArray &data)
    : parent(0),
      m_translation(bone.translation, sequences, data),
      m_rotation(bone.rotation, sequences, data),
      m_scaling(bone.scaling, sequences, data),
      m_pivot(bone.pivot)
{
}

QMatrix4x4 Bone::getMatrix(quint32 animation, quint32 time)
{
    QMatrix4x4 matrix;
    matrix.translate(m_pivot);

    if (m_scaling.isValid())
        matrix.scale(m_scaling.getValue(animation, time));

    if (m_rotation.isValid())
        matrix.rotate(m_rotation.getValue(animation, time));

    if (m_translation.isValid())
        matrix.translate(m_translation.getValue(animation, time));

    matrix.translate(-1.0 * m_pivot);

    if (parent)
        matrix = parent->getMatrix(animation, time) * matrix;

    return matrix;
}

const QVector3D & Bone::getPivot()
{
    return m_pivot;
}
