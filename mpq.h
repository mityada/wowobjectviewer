#ifndef MPQ_H
#define MPQ_H

#include <QByteArray>
#include <QString>

#include "StormLib.h"

namespace MPQ
{
    QString & gameDir();
    QByteArray readFile(const QString &fileName);
};

#endif
