#pragma once

#include <QFontDatabase>

namespace Simp1e::Editor {

    void LoadFont(const std::string& fontName) { QFontDatabase::addApplicationFont(QString::fromStdString(fontName)); }

    void LoadFonts() {
        LoadFont(":/Fonts/fredericka-the-great.regular.ttf");
        LoadFont(":/Fonts/yoster-island.regular.ttf");
        LoadFont(":/Fonts/dpcomic.regular.ttf");
    }
}
