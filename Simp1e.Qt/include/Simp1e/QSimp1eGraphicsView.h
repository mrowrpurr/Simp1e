#pragma once

#include <QGraphicsView>

namespace Simp1e {

    class QSimp1eGraphicsView : public QGraphicsView {
    public:
        QSimp1eGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {
            // Enable scrollbars
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
    };
}
