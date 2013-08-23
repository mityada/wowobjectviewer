#ifndef MODEL_H
#define MODEL_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QTime>

#include "m2.h"

class Model : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString model READ model WRITE setModel)
    Q_PROPERTY(float rotationX READ rotationX WRITE setRotationX)
    Q_PROPERTY(float rotationY READ rotationY WRITE setRotationY)
    Q_PROPERTY(float distance READ distance WRITE setDistance)
    Q_PROPERTY(quint32 animation READ animation WRITE setAnimation)
    Q_PROPERTY(bool animating READ animating WRITE setAnimating NOTIFY animatingChanged)

public:
    Model();

    Q_INVOKABLE void loadCreatureModel(quint32 displayId);

    void setModel(QString model);
    Q_INVOKABLE void setTexture(quint32 type, QString texture);
    void setRotationX(float rotation);
    void setRotationY(float rotation);
    void setDistance(float distance);
    void setAnimation(quint32 animation);
    void setAnimating(bool animating);

    QString model() const;
    float rotationX() const;
    float rotationY() const;
    float distance() const;
    quint32 animation() const;
    bool animating() const;

public slots:
    void paint();
    void update();

signals:
    void animatingChanged();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    M2 *m_model;
    bool m_modelChanged;
    QString m_modelFileName;
    QMap<quint32, QString> m_textureFileNames;

    QTime m_time;
    float m_rotationX;
    float m_rotationY;
    float m_distance;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_particleProgram;
};

#endif
