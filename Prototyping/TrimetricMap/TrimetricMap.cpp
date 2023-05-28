#include <QApplication>
#include <QColor>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include <map>

// This is actually Isometric ;)
class TrimetricMap {
public:
    TrimetricMap(int rows, int cols)
        : scene(std::make_shared<QGraphicsScene>()), rows(rows), cols(cols) {
        createGrid();
    }

    std::shared_ptr<QGraphicsScene> getScene() const { return scene; }

private:
    std::shared_ptr<QGraphicsScene> scene;
    int                             rows, cols;
    int                             tileWidth  = 64;
    int                             tileHeight = 32;

    void createGrid() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                QPolygonF polygon;

                int offsetX = (j - i) * tileWidth / 2;
                int offsetY = (i + j) * tileHeight / 2;

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
};

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    TrimetricMap map{10, 10};

    QGraphicsView view(map.getScene().get());
    view.show();

    return app.exec();
}
