#include <QApplication>
#include <memory>

#include "Qt/QtTileGrid.h"

namespace Prototyping::UI::Qt {

    namespace Application {
        namespace _ {
            int          _appArgc = 0;
            QApplication qtApp{_appArgc, nullptr};
        }
        QApplication& Get() { return _::qtApp; }
    }

    std::unique_ptr<UITileGrid> CreateTileGrid(const UITileGrid::Config& config) {
        return std::make_unique<QtTileGrid>(config);
    }

    int Run() { return Application::Get().exec(); }
}
