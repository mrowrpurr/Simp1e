#include "Simp1e/Editor/Logging.h"
//

#include "Simp1e/Editor/App.h"
#include "Simp1e/Editor/AppWindow.h"
#include "Simp1e/Editor/ComponentListWindow.h"
#include "Simp1e/Editor/EntityListWindow.h"
#include "Simp1e/Editor/LogoWindow.h"
#include "Simp1e/Editor/PrefabListWindow.h"
#include "Simp1e/Editor/SystemListWindow.h"

using namespace Simp1e::Editor;

int main() {
    App                 app;
    AppWindow           appWindow;
    LogoWindow          logoWindow;
    ComponentListWindow componentListWindow;
    EntityListWindow    entityListWindow;
    PrefabListWindow    prefabListWindow;
    SystemListWindow    systemListWindow;
    appWindow.setCentralWidget(new QWidget);
    appWindow.DockWidget(&logoWindow, Qt::TopDockWidgetArea);
    appWindow.DockWidget(&entityListWindow, Qt::LeftDockWidgetArea);
    appWindow.DockWidget(&prefabListWindow, Qt::LeftDockWidgetArea);
    appWindow.DockWidget(&componentListWindow, Qt::RightDockWidgetArea);
    appWindow.DockWidget(&systemListWindow, Qt::RightDockWidgetArea);
    appWindow.show();
    return app.exec();
}
