#include <QApplication>
#include <memory>

#include "Qt/QtMutlipleTileGrids.h"
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

    std::unique_ptr<UITileGrid> CreateMultiTileGrid(
        const UITileGrid::Config&               config,
        std::vector<UITileGrid::RenderingStyle> renderingStyles =
            {UITileGrid::RenderingStyle::IsometricWithHexagons, UITileGrid::RenderingStyle::Grid,
             UITileGrid::RenderingStyle::Hexagons, UITileGrid::RenderingStyle::Isometric}
    ) {
        return std::make_unique<QtMutlipleTileGrids>(config, renderingStyles);
    }

    int Run() { return Application::Get().exec(); }
}
