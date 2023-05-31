#pragma once

#include <QApplication>

namespace Simp1e::UI::Qt {

    namespace Application {
        namespace _ {
            int          _appArgc = 0;
            QApplication qtApp{_appArgc, nullptr};
        }
        QApplication& Get() { return _::qtApp; }
    }

    int Run() {
        auto& app = Application::Get();
#ifdef _WIN32
        app.setStyle("fusion");
#endif
        return app.exec();
    }
}
