#include "Simp1e/Editor/Logging.h"
//

#include "Simp1e/Editor/App.h"
#include "Simp1e/Editor/AppWindow.h"
#include "Simp1e/Editor/AvailableComponentsListWindow.h"
#include "Simp1e/Editor/AvailableSystemsListWindow.h"
#include "Simp1e/Editor/DataFileLoaderTestWindow.h"
#include "Simp1e/Editor/EntityListWindow.h"
#include "Simp1e/Editor/LogoWindow.h"
#include "Simp1e/Editor/PrefabListWindow.h"

using namespace Simp1e::Editor;

void DoStuff() { _Log_("Do stuff!"); }

void OnDataFileLoad(std::string path) {
    _Log_("Data file loaded: {}", path);
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly)) {
        _Log_("Failed to open data file: {}", path);
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    _Log_("Data file content: {}", data.toStdString());
}

int main() {
    App              app;
    AppWindow        appWindow;
    LogoWindow       logoWindow;
    EntityListWindow entityListWindow;
    PrefabListWindow prefabListWindow;

    AvailableComponentsListWindow AvailableComponentsListWindow;
    AvailableSystemsListWindow    AvailableSystemsListWindow;

    DataFileLoaderTestWindow dataFileLoaderTestWindow;

    logoWindow.OnDoStuff(DoStuff);
    dataFileLoaderTestWindow.OnDataFileLoad(OnDataFileLoad);

    appWindow.setCentralWidget(&dataFileLoaderTestWindow);
    appWindow.DockWidget(&logoWindow, Qt::TopDockWidgetArea);
    appWindow.DockWidget(&entityListWindow, Qt::LeftDockWidgetArea);
    appWindow.DockWidget(&prefabListWindow, Qt::LeftDockWidgetArea);
    appWindow.DockWidget(&AvailableComponentsListWindow, Qt::RightDockWidgetArea);
    appWindow.DockWidget(&AvailableSystemsListWindow, Qt::RightDockWidgetArea);

    appWindow.show();
    return app.exec();
}
