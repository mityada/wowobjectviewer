#include <QApplication>

#include "window.h"
#include "mpq.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MPQ::gameDir() = app.applicationDirPath() + "/";

    Window window;
    window.show();

    return app.exec();
}
