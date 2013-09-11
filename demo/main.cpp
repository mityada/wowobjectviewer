#include <QApplication>

#include "window.h"
#include "mpq.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringList args = app.arguments();

    if (args.size() > 1)
        MPQ::gameDir() = args.at(1);
    else
        MPQ::gameDir() = app.applicationDirPath() + "/";

    Window window;
    window.show();

    return app.exec();
}
