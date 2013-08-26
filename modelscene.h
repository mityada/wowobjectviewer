#ifndef MODEL_SCENE_H
#define MODEL_SCENE_H

#include <QtQuick/QQuickItem>
#include <QOpenGLShaderProgram>
#include <QTime>

#include "model.h"

class ModelScene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Model> models READ models)
    Q_PROPERTY(float rotationX READ rotationX WRITE setRotationX)
    Q_PROPERTY(float rotationY READ rotationY WRITE setRotationY)
    Q_PROPERTY(float distance READ distance WRITE setDistance)
    Q_CLASSINFO("DefaultProperty", "models")

public:
    ModelScene();

    void setRotationX(float rotation);
    void setRotationY(float rotation);
    void setDistance(float distance);

    QQmlListProperty<Model> models();
    float rotationX() const;
    float rotationY() const;
    float distance() const;

public slots:
    void paint();
    void update();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QList<Model *> m_models;

    QTime m_time;
    float m_rotationX;
    float m_rotationY;
    float m_distance;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_particleProgram;
};

#endif
