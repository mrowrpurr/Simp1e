#pragma once

#include <QApplication>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

class GameBoard : public QObject {
    Q_OBJECT
public:
    GameBoard(
        int diamondRows, int diamondCols, float diamondWidth, float diamondHeight, float hexWidth,
        float hexHeight, float hexXOffset = 0.0, float hexYOffset = 0.0
    )
        : m_diamondRows(diamondRows),
          m_diamondCols(diamondCols),
          m_diamondWidth(diamondWidth),
          m_diamondHeight(diamondHeight),
          m_hexWidth(hexWidth),
          m_hexHeight(hexHeight),
          m_hexXOffset(hexXOffset),
          m_hexYOffset(hexYOffset) {
        m_scene = new QGraphicsScene();

        drawDiamonds();
        drawHexes();
    }

    QGraphicsScene* getScene() { return m_scene; }

private:
    int             m_diamondRows, m_diamondCols;
    float           m_diamondWidth, m_diamondHeight, m_hexWidth, m_hexHeight;
    float           m_hexXOffset, m_hexYOffset;
    QGraphicsScene* m_scene;

    void drawDiamonds() {
        for (int i = 0; i < m_diamondRows; ++i) {
            for (int j = 0; j < m_diamondCols; ++j) {
                QPointF topLeft(j * m_diamondWidth, i * m_diamondHeight);
                QPointF topRight((j + 0.5) * m_diamondWidth, i * m_diamondHeight);
                QPointF middleRight((j + 1) * m_diamondWidth, (i + 0.5) * m_diamondHeight);
                QPointF bottomRight((j + 0.5) * m_diamondWidth, (i + 1) * m_diamondHeight);
                QPointF bottomLeft(j * m_diamondWidth, (i + 1) * m_diamondHeight);
                QPointF middleLeft(j * m_diamondWidth, (i + 0.5) * m_diamondHeight);

                QPolygonF diamondPolygon;
                diamondPolygon << topLeft << topRight << middleRight << bottomRight << bottomLeft
                               << middleLeft;

                QGraphicsPolygonItem* diamondItem = new QGraphicsPolygonItem(diamondPolygon);
                diamondItem->setBrush(i % 2 == j % 2 ? Qt::gray : Qt::lightGray);
                diamondItem->setPen(Qt::NoPen);
                m_scene->addItem(diamondItem);
            }
        }
    }

    void drawHexes() {
        for (int i = 0; i < (m_diamondHeight * m_diamondRows) / m_hexHeight; ++i) {
            for (int j = 0; j < (m_diamondWidth * m_diamondCols) / m_hexWidth; ++j) {
                QPointF topLeft(j * m_hexWidth + m_hexXOffset, i * m_hexHeight + m_hexYOffset);
                QPointF topRight(
                    (j + 0.5) * m_hexWidth + m_hexXOffset, i * m_hexHeight + m_hexYOffset
                );
                QPointF middleRight(
                    (j + 1) * m_hexWidth + m_hexXOffset, (i + 0.5) * m_hexHeight + m_hexYOffset
                );
                QPointF bottomRight(
                    (j + 0.5) * m_hexWidth + m_hexXOffset, (i + 1) * m_hexHeight + m_hexYOffset
                );
                QPointF bottomLeft(
                    j * m_hexWidth + m_hexXOffset, (i + 1) * m_hexHeight + m_hexYOffset
                );
                QPointF middleLeft(
                    j * m_hexWidth + m_hexXOffset, (i + 0.5) * m_hexHeight + m_hexYOffset
                );

                QPolygonF hexagonPolygon;
                hexagonPolygon << topLeft << topRight << middleRight << bottomRight << bottomLeft
                               << middleLeft;

                QGraphicsPolygonItem* hexagonItem = new QGraphicsPolygonItem(hexagonPolygon);
                hexagonItem->setPen(QPen(Qt::black, 2));
                hexagonItem->setBrush(Qt::NoBrush);
                m_scene->addItem(hexagonItem);
            }
        }
    }
};
