#include <string_format.h>

#include <QDebug>
#define _Log_(...) qDebug() << string_format(__VA_ARGS__)
#include <_Log_.h>

/*
    TODO

  [ ]  0(a). Show names of .dll to load [Windows]
  [ ]  0(b). Show names of .so to load [Linux]
  [ ]  0(c). Show names of .dylib to load [MacOS

  [ ]  1(a). Load .dll with certain name pattern [Windows]
  [ ]  1(b). Load .so with certain name pattern [Linux]
  [ ]  1(c). Load .dylib with certain name pattern [MacOS]

  [ ]  2(a). Show list of registered System types provided by the .dll (simple registration)
  [ ]  2(b). Can actually call a function on one of the Systems (#update) from main .exe

  [ ]  3(a). Show list of registered Components types provided by the .dll (simple registration)
  [ ]  3(b). Can actually instantiate a Component of this type using a factory
             and the component lives in the definining DLL space (or anywhere that works)
  [ ]  3(b). Can populate field(s) of the Component

  [ ]  4(a). Can instantiate a Component defined in a .dll from a JSON definition!

  [ ]  5(a). Can setup system groups using System defined in .dll

  [ ]  6(a). A system can search for components of given types AND it can access
             those components' fields (read/write) and call their methods

*/

#include <Simp1e/GetLibraryLoader.h>

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
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

void LoadAnotherLibrary(const std::string& fileBasename = "DLL-LoadMe-SystemAndComponent") {
    _Log_("Getting library loader...");
    auto* libraryLoader = Simp1e::GetLibraryLoader();
    _Log_("Loading library...");
    libraryLoader->Load(fileBasename.c_str());
    _Log_("Loaded library?");
    // auto* fn = libraryLoader->GetFunctionPointer<bool(void*)>(fileBasename.c_str(), "Simp1e_ECS_Load");
    auto* fnPtr = libraryLoader->GetFunctionVoidPointer(fileBasename.c_str(), "Simp1e_ECS_Load");
    if (fnPtr) {
        _Log_("Calling function...");
        auto* fn     = reinterpret_cast<bool (*)(void*)>(fnPtr);
        auto  result = fn(nullptr);
        _Log_("Result from fn: {}", result);
    } else {
        _Log_("Failed to get function pointer");
    }
}

int main(int argc, char** argv) {
    _Log_("Simp1e Application Start");
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

    QLabel lbl_Title("Simp1e");
    lbl_Title.setObjectName("WindowTitle");
    lbl_Title.setProperty("class", "title");
    layout->addWidget(&lbl_Title, 0, Qt::AlignCenter);

    QLabel lbl_Version("v0.0.1");
    lbl_Version.setObjectName("WindowSubtitle");
    lbl_Version.setProperty("class", "subtitle");
    layout->addWidget(&lbl_Version, 0, Qt::AlignCenter);

    QPushButton btn_DoThings("Do Things");
    layout->addWidget(&btn_DoThings, 0, Qt::AlignCenter);
    QObject::connect(&btn_DoThings, &QPushButton::clicked, [&]() { LoadAnotherLibrary(); });

    LoadFont(app, ":/Fonts/fredericka-the-great.regular.ttf");
    LoadFont(app, ":/Fonts/yoster-island.regular.ttf");

    QPixmap logo{":/logo.png"};

    ReloadStylesheet(app);

    window.resize(640, 480);
    window.show();
    _Log_("Simp1e Application Exit");
    return app.exec();
}
