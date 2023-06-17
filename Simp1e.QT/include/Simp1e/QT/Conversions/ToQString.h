#pragma once

#include <QString>
#include <string>


namespace Simp1e {
    QString ToQString(const std::string& text) { return QString::fromStdString(text); }
}
