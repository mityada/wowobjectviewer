#ifndef MPQ_H
#define MPQ_H

#include <QByteArray>
#include <QString>

#include "global.h"
#include "StormLib.h"

namespace MPQ
{
    WOV_EXPORT QString & gameDir();
    QByteArray readFile(const QString &fileName);
};

#endif
