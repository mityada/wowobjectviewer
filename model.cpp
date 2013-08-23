#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QMatrix4x4>

#include "model.h"
#include "mvp.h"
#include "dbc.h"

Model::Model() : m_model(0), m_modelChanged(false)
{
    m_rotationX = 0;
    m_rotationY = 0;
    m_distance = 20.0;

    connect(this, SIGNAL(windowChanged(QQuickWindow *)), this, SLOT(handleWindowChanged(QQuickWindow *)));
}

void Model::loadCreatureModel(quint32 displayId)
{
    CreatureDisplayInfoDBC::entry displayInfo = CreatureDisplayInfoDBC::getEntry(displayId);
    CreatureModelDataDBC::entry modelData = CreatureModelDataDBC::getEntry(displayInfo.model);

    m_modelFileName = QString(modelData.model).replace("\\", "/").replace(".mdx", ".m2");

    QString modelPath = m_modelFileName.section('/', 0, -2);

    setTexture(11, modelPath + "/" + QString(displayInfo.skin1) + ".blp");
    setTexture(12, modelPath + "/" + QString(displayInfo.skin2) + ".blp");
    setTexture(13, modelPath + "/" + QString(displayInfo.skin3) + ".blp");

    m_modelChanged = true;
}

void Model::setModel(QString model)
{
    if (m_modelFileName != model) {
        m_modelFileName = model.remove("file://");
        m_modelChanged = true;
    }
}

void Model::setTexture(quint32 type, QString texture)
{
    m_textureFileNames[type] = texture.remove("file://");

    if (m_model)
        m_model->setTexture(type, m_textureFileNames[type]);
}

void Model::setRotationX(float rotation)
{
    m_rotationX = rotation;
}

void Model::setRotationY(float rotation)
{
    m_rotationY = rotation;
}

void Model::setDistance(float distance)
{
    m_distance = qMax(0.0f, distance);
}

void Model::setAnimation(quint32 animation)
{
    if (m_model)
        m_model->setAnimation(animation);
}

void Model::setAnimating(bool animating)
{
    if (m_model)
        m_model->setAnimating(animating);

    emit animatingChanged();
}

QString Model::model() const
{
    return m_modelFileName;
}

float Model::rotationX() const
{
    return m_rotationX;
}

float Model::rotationY() const
{
    return m_rotationY;
}

float Model::distance() const
{
    return m_distance;
}

quint32 Model::animation() const
{
    return m_model ? m_model->animation() : 0;
}

bool Model::animating() const
{
    return m_model ? m_model->animating() : false;
}

void Model::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(afterRendering()), this, SLOT(paint()), Qt::DirectConnection);

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()), Qt::DirectConnection);
        timer->start(0);

        m_time.start();
    }
}

void Model::paint()
{
    if (m_modelChanged) {
        if (m_model)
            delete m_model;

        m_model = new M2(m_modelFileName);

        QMap<quint32, QString>::const_iterator i = m_textureFileNames.constBegin();
        while (i != m_textureFileNames.constEnd()) {
            m_model->setTexture(i.key(), i.value());
            i++;
        }

        m_modelChanged = false;
    }

    int timeDelta = m_time.restart();

    if (!m_program) {
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vs");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.fs");
        m_program->link();

        m_particleProgram = new QOpenGLShaderProgram();
        m_particleProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/particle.vs");
        m_particleProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/particle.fs");
        m_particleProgram->link();

        const GLubyte white[3] = {255, 255, 255};

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white); 
    }

    QPointF position = mapToScene(QPointF(0, height()));
    glViewport(position.x(), window()->height() - position.y(), width(), height());

    glEnable(GL_SCISSOR_TEST);
    glScissor(position.x(), window()->height() - position.y(), width(), height());

    glDepthMask(GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MVP mvp;
    mvp.projection.perspective(32.25, float(width()) / float(height()), 1.0, 100.0);
    mvp.view.lookAt(QVector3D(m_distance, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));
    mvp.model.rotate(rotationY(), 0.0, 0.0, 1.0);
    mvp.model.rotate(rotationX(), 0.0, 1.0, 0.0);
    mvp.model.rotate(-90, 1.0, 0.0, 0.0);

    if (m_model) {
        m_model->update(timeDelta);

        m_program->bind();
        m_program->setUniformValue("light.position", QVector3D(0.0, 0.0, 1.0));
        m_program->setUniformValue("light.ambient", QVector4D(0.5, 0.5, 0.5, 1.0));
        m_program->setUniformValue("light.diffuse", QVector4D(1.0, 1.0, 1.0, 1.0));
        m_model->render(m_program, mvp);
        m_program->release();

        m_particleProgram->bind();
        m_model->renderParticles(m_particleProgram, mvp);
        m_particleProgram->release();
    }

    glDepthMask(GL_FALSE);
}

void Model::update()
{
    window()->update();
}
