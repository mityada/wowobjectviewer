#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Model;
class ModelScene;
class SpellVisual;
class QLineEdit;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void addModel();
    void selectCaster();
    void selectTarget();
    void cast();

    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void wheelRotated(QWheelEvent *event);

private:
    ModelScene *m_scene;

    quint8 m_selection;
    Model *m_drag;

    int m_mouseX, m_mouseY;
    float m_worldX, m_worldY, m_worldZ;

    SpellVisual *m_visual;

    QLineEdit *m_displayEdit;
    QLineEdit *m_visualEdit;
};

#endif
