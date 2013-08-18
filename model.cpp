#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QMatrix4x4>

#include "model.h"

Model::Model() : m_model(0), m_modelChanged(false)
{
    m_rotationX = 0;
    m_rotationY = 0;
    m_distance = 20.0;

    connect(this, SIGNAL(windowChanged(QQuickWindow *)), this, SLOT(handleWindowChanged(QQuickWindow *)));
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

        const GLubyte white[3] = {255, 255, 255};

        glBindTexture(GL_TEXTURE_2D, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white); 
    }

    QPointF position = mapToScene(QPointF(0, height()));
    glViewport(position.x(), window()->height() - position.y(), width(), height());

    glDepthMask(GL_TRUE);

    glClear(GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 projection;
    projection.perspective(32.25, float(width()) / float(height()), 1.0, 100.0);

    QMatrix4x4 view;
    view.lookAt(QVector3D(m_distance, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));

    QMatrix4x4 rotate;
    rotate.rotate(rotationY(), 0.0, 0.0, 1.0);
    rotate.rotate(rotationX(), 0.0, 1.0, 0.0);

    m_program->setUniformValue("viewProjection", projection * view * rotate);

    if (m_model) {
        m_model->update(timeDelta);
        m_model->render(m_program);
    }

    m_program->release();

    glDepthMask(GL_FALSE);
}

void Model::update()
{
    window()->update();
}
