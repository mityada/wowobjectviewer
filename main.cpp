#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "modelscene.h"
#include "model.h"
#include "spellvisual.h"
#include "mpq.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MPQ::gameDir() = app.applicationDirPath() + "/";

    qmlRegisterType<ModelScene>("WoWObjectViewer", 1, 0, "ModelScene");
    qmlRegisterType<Model>("WoWObjectViewer", 1, 0, "Model");
    qmlRegisterType<SpellVisual>("WoWObjectViewer", 1, 0, "SpellVisual");

    QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));

    return app.exec();
}
