#include <QtQuick/QQuickWindow>
#include <QMatrix4x4>

#include "modelscene.h"
#include "mvp.h"

ModelScene::ModelScene()
    : m_rotationX(0.0f),
      m_rotationY(0.0f),
      m_distance(20.0f),
      m_program(0),
      m_particleProgram(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow *)), this, SLOT(handleWindowChanged(QQuickWindow *)));
}

void ModelScene::setRotationX(float rotation)
{
    m_rotationX = rotation;
}

void ModelScene::setRotationY(float rotation)
{
    m_rotationY = rotation;
}

void ModelScene::setDistance(float distance)
{
    m_distance = qMax(0.0f, distance);
}

QQmlListProperty<Model> ModelScene::models()
{
    return QQmlListProperty<Model>(this, m_models);
}

float ModelScene::rotationX() const
{
    return m_rotationX;
}

float ModelScene::rotationY() const
{
    return m_rotationY;
}

float ModelScene::distance() const
{
    return m_distance;
}

void ModelScene::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(afterRendering()), this, SLOT(paint()), Qt::DirectConnection);

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()), Qt::DirectConnection);
        timer->start(0);

        m_time.start();
    }
}

void ModelScene::paint()
{
    int timeDelta = m_time.restart();

    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->update(timeDelta);

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

    m_program->bind();

    m_program->setUniformValue("light.position", QVector3D(0.0, 0.0, 1.0));
    m_program->setUniformValue("light.ambient", QVector4D(0.5, 0.5, 0.5, 1.0));
    m_program->setUniformValue("light.diffuse", QVector4D(1.0, 1.0, 1.0, 1.0));

    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->render(m_program, mvp);

    m_program->release();

    m_particleProgram->bind();

    for (int i = 0; i < m_models.size(); i++)
        m_models[i]->renderParticles(m_particleProgram, mvp);

    m_particleProgram->release();

    glDepthMask(GL_FALSE);
}

void ModelScene::update()
{
    window()->update();
}
