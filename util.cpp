#include "util.h"

#include <QDir>
#include <QStringList>

QString getLocalFileName(const QString &mpqFileName)
{
    QStringList path = mpqFileName.split("\\");

    QDir dir;

    for (int i = 0; i < path.size(); i++) {
        QStringList entryList = dir.entryList();

        for (int j = 0; j < entryList.size(); j++) {
            if (path[i].compare(entryList[j], Qt::CaseInsensitive) == 0) {
                if (i == path.size() - 1) {
                    if (dir.exists(entryList[j]))
                        return dir.filePath(entryList[j]);
                    else
                        return QString();
                } else {
                    if (!dir.cd(entryList[j]))
                        return QString();
                }

                break;
            }

            if (j == entryList.size() - 1)
                return QString();
        }
    }

    return QString();
}

