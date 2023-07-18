#include <_Log_.h>

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

void ReloadStylesheet(QApplication& app) {
    QFile file(":/Editor.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
}

void LoadFont(QApplication& app, const std::string& fontName) {
    int fontId = QFontDatabase::addApplicationFont(QString::fromStdString(fontName));
    if (fontId == -1) {
        throw std::runtime_error("Failed to load font: " + fontName);
    }
    auto  family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font{family};
    app.setFont(font);
}

int main(int argc, char** argv) {
    QApplication app{argc, argv};
    app.setStyle("Fusion");

    QWidget window;
    window.setObjectName("MainWindow");

    QVBoxLayout* layout = new QVBoxLayout(&window);

    QLabel lbl_Logo;
    lbl_Logo.setObjectName("WindowTitleImage");
    lbl_Logo.setProperty("class", "image");
    lbl_Logo.setPixmap(QPixmap(":/logo.png"));
    lbl_Logo.setScaledContents(true);
    layout->addWidget(&lbl_Logo, 0, Qt::AlignCenter);

    QLabel lbl_Title{"Simp1e"};
    lbl_Title.setObjectName("WindowTitle");
    lbl_Title.setProperty("class", "title");
    layout->addWidget(&lbl_Title, 0, Qt::AlignCenter);

    QLabel lbl_Version{"v0.0.1"};
    lbl_Version.setObjectName("WindowSubtitle");
    lbl_Version.setProperty("class", "subtitle");
    layout->addWidget(&lbl_Version, 0, Qt::AlignCenter);

    LoadFont(app, ":/Fonts/fredericka-the-great.regular.ttf");
    LoadFont(app, ":/Fonts/yoster-island.regular.ttf");

    QPixmap logo{":/logo.png"};

    ReloadStylesheet(app);

    window.resize(640, 480);
    window.show();
    return app.exec();
}
