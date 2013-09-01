#ifndef MODEL_SCENE_H
#define MODEL_SCENE_H

#include <QtQuick/QQuickItem>
#include <QOpenGLShaderProgram>
#include <QTime>

#include "model.h"

class ModelScene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float rotationX READ rotationX WRITE setRotationX)
    Q_PROPERTY(float rotationY READ rotationY WRITE setRotationY)
    Q_PROPERTY(float distance READ distance WRITE setDistance)
    Q_PROPERTY(int mouseX MEMBER m_mouseX)
    Q_PROPERTY(int mouseY MEMBER m_mouseY)
    Q_PROPERTY(Model * selectedModel READ selectedModel)
    Q_PROPERTY(float worldX MEMBER m_worldX)
    Q_PROPERTY(float worldY MEMBER m_worldY)
    Q_PROPERTY(float worldZ MEMBER m_worldZ)

public:
    ModelScene();

    void setRotationX(float rotation);
    void setRotationY(float rotation);
    void setDistance(float distance);

    float rotationX() const;
    float rotationY() const;
    float distance() const;
    Model * selectedModel();

    Q_INVOKABLE void addModel(Model *model);
    Q_INVOKABLE void removeModel(Model *model);

public slots:
    void paint();
    void update();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void renderGrid(int size, float step, MVP mvp);
    void updateMouseCoordinates(MVP mvp);

    QList<Model *> m_models;

    QTime m_time;
    float m_rotationX;
    float m_rotationY;
    float m_distance;

    int m_mouseX, m_mouseY;
    int m_selection;

    float m_worldX, m_worldY, m_worldZ;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_particleProgram;

    QOpenGLBuffer *m_gridBuffer;
};

#endif
