#include <QApplication>
#include <QColor>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <map>

struct RGBColor {
    int red, green, blue;
};

class IsometricMap {
public:
    IsometricMap(int rows, int cols)
        : scene(std::make_shared<QGraphicsScene>()), rows(rows), cols(cols) {
        createGrid();
    }

    void setTileLines(bool drawLines) {
        this->drawLines = drawLines;
        updateGrid();
    }

    void setTileColor(int row, int col, RGBColor color) {
        auto   key = std::make_pair(row, col);
        QColor qtColor(color.red, color.green, color.blue);
        tileColors[key] = qtColor;
        updateGrid();
    }

    std::shared_ptr<QGraphicsScene> getScene() const { return scene; }

private:
    std::shared_ptr<QGraphicsScene>       scene;
    int                                   rows, cols;
    bool                                  drawLines = false;
    std::map<std::pair<int, int>, QColor> tileColors;
    int                                   tileWidth  = 100;
    int                                   tileHeight = 50;

    void createGrid() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                QPolygonF polygon;
                int       offsetX = j * tileWidth;
                int       offsetY = i * tileHeight / 2;

                // Correct for the staggering of the diamonds
                if (i % 2 == 1) {
                    offsetX += tileWidth / 2;
                }

                polygon << QPointF(offsetX, offsetY)
                        << QPointF(offsetX + tileWidth / 2, offsetY + tileHeight / 2)
                        << QPointF(offsetX, offsetY + tileHeight)
                        << QPointF(offsetX - tileWidth / 2, offsetY + tileHeight / 2);

                QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
                item->setPen(QPen(Qt::black, 1));
                item->setBrush(Qt::white);

                scene->addItem(item);

                // Create the QGraphicsTextItem
                QGraphicsTextItem* textItem =
                    new QGraphicsTextItem(QString("(%1, %2)").arg(i).arg(j));
                textItem->setDefaultTextColor(Qt::black);

                // Set the position of the QGraphicsTextItem. This will center the text in the tile.
                textItem->setPos(offsetX - textItem->boundingRect().width() / 2, offsetY);

                scene->addItem(textItem);
            }
        }
    }

    void updateGrid() {
        for (QGraphicsItem* item : scene->items()) {
            scene->removeItem(item);
            delete item;
        }
        createGrid();
    }
};

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    IsometricMap map{20, 10};
    map.setTileLines(true);
    map.setTileColor(1, 2, {0, 255, 0});
    map.setTileColor(3, 4, {255, 0, 0});

    // Connect the QGraphicsScene from the map to your QGraphicsView
    QGraphicsView view(map.getScene().get());
    view.show();

    return app.exec();
}
