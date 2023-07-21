#include "Simp1e/Editor/Logging.h"
//

#include "Simp1e/Editor/App.h"
#include "Simp1e/Editor/LogoWindow.h"

using namespace Simp1e::Editor;

int main() {
    App        app;
    LogoWindow logoWindow;
    logoWindow.show();
    return app.exec();
}
