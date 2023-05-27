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

    std::unique_ptr<UITileGrid> CreateTileGrid(
        TileGrid*                  tileGrid,
        UITileGrid::RenderingStyle renderingStyle = UITileGrid::RenderingStyle::Grid
    ) {
        return std::make_unique<QtTileGrid>(tileGrid, renderingStyle);
    }

    int Run() { return Application::Get().exec(); }
}
