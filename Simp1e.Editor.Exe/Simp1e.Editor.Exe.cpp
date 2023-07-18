#include <string_format.h>
#define _Log_(...) qDebug() << string_format(__VA_ARGS__)
#include <_Log_.h>

/*
    TODO

  [ ]  1(a). Load .dll with certain name pattern [Windows]
  [ ]  1(b). Load .so with certain name pattern [Linux]
  [ ]  1(c). Load .dylib with certain name pattern [MacOS]

  [ ]  2(a). Show list of registered Service types provided by the .dll (simple registration)
  [ ]  2(b). Can actually call a function on one of the Services (#update) from main .exe

  [ ]  3(a). Show list of registered Components types provided by the .dll (simple registration)
  [ ]  3(b). Can actually instantiate a Component of this type using a factory
             and the component lives in the definining DLL space (or anywhere that works)
  [ ]  3(b). Can populate field(s) of the Component

  [ ]  4(a). Can instantiate a Component defined in a .dll from a JSON definition!

  [ ]  5(a). Can setup service groups using Service defined in .dll

  [ ]  6(a). A service can search for components of given types AND it can access
             those components' fields (read/write) and call their methods

*/

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

    QLabel lbl_Description{"< some loaded output goes here >"};
    layout->addWidget(&lbl_Description);

    LoadFont(app, ":/Fonts/fredericka-the-great.regular.ttf");
    LoadFont(app, ":/Fonts/yoster-island.regular.ttf");

    QPixmap logo{":/logo.png"};

    ReloadStylesheet(app);

    window.resize(640, 480);
    window.show();
    return app.exec();
}
