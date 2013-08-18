#include <QFile>
#include <QUrl>

#include "m2.h"

M2::M2(const QString &fileName) : m_loaded(false), m_initialized(false), m_animating(false)
{
    QFile file(fileName);
    if (!file.exists()) {
        qCritical("File '%s' does not exist!", qPrintable(fileName));
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qCritical("File '%s' cannot be opened!", qPrintable(fileName));
        return;
    }

    m_data = file.readAll();

    file.close();

    m_header = reinterpret_cast<M2Header *>(m_data.data());

    if (strncmp(m_header->magic, M2_MAGIC, 4) != 0) {
        qCritical("File '%s' is not a valid M2 file!", qPrintable(fileName));
        return;
    }

    m_sequences = reinterpret_cast<quint32 *>(m_data.data() + m_header->sequencesOffset);

    m_vertices = reinterpret_cast<M2Vertex *>(m_data.data() + m_header->verticesOffset);

    m_views = reinterpret_cast<M2View *>(m_data.data() + m_header->viewsOffset);

    quint16 *indices = reinterpret_cast<quint16 *>(m_data.data() + m_views[0].indicesOffset);
    quint16 *triangles = reinterpret_cast<quint16 *>(m_data.data() + m_views[0].trianglesOffset);

    m_indicesCount = m_views[0].trianglesCount;
    m_indices = new quint16[m_indicesCount];

    for (quint32 i = 0; i < m_indicesCount; i++)
        m_indices[i] = indices[triangles[i]];

    m_submeshes = reinterpret_cast<M2Submesh *>(m_data.data() + m_views[0].submeshOffset);
    m_textureUnits = reinterpret_cast<M2TextureUnit *>(m_data.data() + m_views[0].textureUnitsOffset);

    m_renderFlags = reinterpret_cast<M2RenderFlags *>(m_data.data() + m_header->renderFlagsOffset);

    m_textureLookup = reinterpret_cast<quint16 *>(m_data.data() + m_header->textureLookupOffset);

    M2Texture *textures = reinterpret_cast<M2Texture *>(m_data.data() + m_header->texturesOffset);

    for (quint32 i = 0; i < m_header->texturesCount; i++) {
        QString fileName(m_data.data() + textures[i].fileNameOffset);

        m_textures << Texture();

        if (textures[i].type == 0)
            m_textures[i].load(fileName);
    }

    m_textureAnimationLookup = reinterpret_cast<qint16 *>(m_data.data() + m_header->textureAnimationLookupOffset);

    M2TextureAnimation *textureAnimations = reinterpret_cast<M2TextureAnimation *>(m_data.data() + m_header->textureAnimationsOffset);

    for (quint32 i = 0; i < m_header->textureAnimationsCount; i++)
        m_textureAnimations << TextureAnimation(textureAnimations[i], m_sequences, m_data);

    m_replaceableTextures = reinterpret_cast<qint16 *>(m_data.data() + m_header->textureReplaceOffset);

    M2Bone *bones = reinterpret_cast<M2Bone *>(m_data.data() + m_header->bonesOffset);

    for (quint32 i = 0; i < m_header->bonesCount; i++)
        m_bones << Bone(bones[i], m_sequences, m_data);

    for (quint32 i = 0; i < m_header->bonesCount; i++) {
        if (bones[i].parent != -1)
            m_bones[i].parent = &m_bones[bones[i].parent];
    }

    m_boneMatrices = new QMatrix4x4[m_bones.size()];

    m_animations = reinterpret_cast<M2Animation *>(m_data.data() + m_header->animationsOffset);

    m_transparencyLookup = reinterpret_cast<qint16 *>(m_data.data() + m_header->transparencyLookupOffset);

    M2AnimationBlock *transparencies = reinterpret_cast<M2AnimationBlock *>(m_data.data() + m_header->transparencyOffset);

    for (quint32 i = 0; i < m_header->transparencyCount; i++)
        m_transparencies << AnimatedValue<quint16>(transparencies[i], m_sequences, m_data);

    m_loaded = true;
}

void M2::initialize(QOpenGLShaderProgram *program)
{
    m_vao = new QOpenGLVertexArrayObject(this);
    m_vao->create();
    m_vao->bind();

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBuffer->create();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_vertices, m_header->verticesCount * sizeof(M2Vertex));

    program->enableAttributeArray("position");
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, sizeof(M2Vertex));

    program->enableAttributeArray("texcoord");
    program->setAttributeBuffer("texcoord", GL_FLOAT, 6 * sizeof(float) + 8 * sizeof(quint8), 2, sizeof(M2Vertex));

    program->enableAttributeArray("boneweights");
    program->setAttributeBuffer("boneweights", GL_UNSIGNED_BYTE, 3 * sizeof(float), 4, sizeof(M2Vertex));

    program->enableAttributeArray("boneindices");
    program->setAttributeBuffer("boneindices", GL_UNSIGNED_BYTE, 3 * sizeof(float) + 4 * sizeof(quint8), 4, sizeof(M2Vertex));


    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_indexBuffer->create();
    m_indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices, m_indicesCount * sizeof(GLushort));

    m_vao->release();

    m_vertexBuffer->release();
    m_indexBuffer->release();

    setAnimation(0);

    m_initialized = true;
}

void M2::render(QOpenGLShaderProgram *program)
{
    if (!m_loaded)
        return;

    if (!m_initialized)
        initialize(program);

    QMatrix4x4 model;
    model.rotate(-90, 1.0, 0.0, 0.0);

    program->setUniformValue("model", model);
    program->setUniformValueArray("bones", m_boneMatrices, m_bones.size());

    m_vao->bind();

    glAlphaFunc(GL_GREATER, 0.3f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (quint32 i = 0; i < m_views[0].textureUnitsCount; i++) {
        M2RenderFlags flags = m_renderFlags[m_textureUnits[i].renderFlagIndex];

        switch (flags.blending) {
            case BM_OPAQUE:
                glDisable(GL_BLEND);
                glDisable(GL_ALPHA_TEST);
                break;
            case BM_TRANSPARENT:
                glDisable(GL_BLEND);
                glEnable(GL_ALPHA_TEST);
                break;
            case BM_ALPHA_BLEND:
                glEnable(GL_BLEND);
                glDisable(GL_ALPHA_TEST);
                break;
            case BM_ADDITIVE:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_COLOR, GL_ONE);
                glDisable(GL_ALPHA_TEST);
                break;
            case BM_ADDITIVE_ALPHA:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                glDisable(GL_ALPHA_TEST);
                break;
            case BM_MODULATE:
            default:
                glEnable(GL_BLEND);
                glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
                glDisable(GL_ALPHA_TEST);
                break;
        }

        if (flags.flags & RENDER_FLAG_TWO_SIDED)
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);

        if (flags.flags & RENDER_FLAG_NO_Z_WRITE)
            glDepthMask(GL_FALSE);
        else
            glDepthMask(GL_TRUE);

        m_textures[m_textureLookup[m_textureUnits[i].textureIndex]].bind();

        QMatrix4x4 textureMatrix;

        qint16 textureAnimation = m_textureAnimationLookup[m_textureUnits[i].textureAnimationId];

        if (textureAnimation != -1 && animating())
            textureMatrix = m_textureAnimations[textureAnimation].getMatrix(m_animation, m_time);

        program->setUniformValue("textureMatrix", textureMatrix);

        qint16 transparency = m_transparencyLookup[m_textureUnits[i].transparencyIndex];

        float alpha = 1.0f;

        if (transparency != -1) {
            alpha = m_transparencies[transparency].getValue(m_animation, m_time) / 32767.0f;
        }

        program->setUniformValue("alpha", alpha);

        quint32 submesh = m_textureUnits[i].submeshIndex;

        glDrawElements(GL_TRIANGLES, m_submeshes[submesh].trianglesCount, GL_UNSIGNED_SHORT, (const GLvoid *)(m_submeshes[submesh].startingTriangle * sizeof(GLushort)));
    }

    glAlphaFunc(GL_ALWAYS, 0.0f);
    glBlendFunc(GL_ONE, GL_ZERO);

    m_vao->release();
}

void M2::update(int timeDelta)
{
    if (!m_initialized)
        return;

    if (animating()) {
        m_time += timeDelta;

        if (m_time >= m_animations[m_animation].endTime)
            m_time = m_animations[m_animation].startTime;

        updateBones();
    }
}

void M2::updateBones()
{
    for (qint32 i = 0; i < m_bones.size(); i++)
       m_boneMatrices[i] = m_bones[i].getMatrix(m_animation, m_time);
}

void M2::setAnimation(quint32 animation)
{
    if (!m_loaded)
        return;

    if (animation >= m_header->animationsCount)
        return;

    m_animation = animation;
    m_time = m_animations[m_animation].startTime;

    updateBones();
}

quint32 M2::animation() const
{
    return m_animation;
}

void M2::setAnimating(bool animating)
{
    m_animating = animating;
}

bool M2::animating() const
{
    return m_animating;
}

void M2::setTexture(quint32 type, QString fileName)
{
    if (!m_loaded)
        return;

    if (type >= m_header->textureReplaceCount || m_replaceableTextures[type] == -1)
        return;

    m_textures[m_replaceableTextures[type]].load(fileName);
}
