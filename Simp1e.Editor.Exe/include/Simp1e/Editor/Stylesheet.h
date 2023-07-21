#pragma once

#include <QFile>

namespace Simp1e::Editor {

    QString GetStylesheet() {
        QFile file(":/Editor.qss");
        if (file.open(QFile::ReadOnly)) return QLatin1String(file.readAll()).toString();
        return {};
    }
}
