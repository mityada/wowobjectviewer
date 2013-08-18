#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "m2.h"
#include "model.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Model>("WoWObjectViewer", 1, 0, "Model");

    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));

    return app.exec();
}
