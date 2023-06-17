#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <optional>

namespace Simp1e {

    class QTGraphicsItem : public QGraphicsItem {
        QRectF                     _boundingRect{0, 0, 0, 0};
        std::optional<std::string> _text;
        std::optional<QColor>      _borderColor;

    public:
        QTGraphicsItem() = default;
        QTGraphicsItem(QGraphicsScene* scene) { scene->addItem(this); }

        void SetText(const std::string& text) { _text = text; }
        void SetSize(const QSizeF& size) { _boundingRect.setSize(size); }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            // Draw white rectangle border
            // if (_borderColor) {
            painter->setPen(QPen(Qt::white, 1));
            painter->drawRect(boundingRect());
            // }

            if (_text) {
                painter->setPen(Qt::white);
                painter->drawText(boundingRect(), Qt::AlignCenter, QString::fromStdString(*_text));
            }
        }
        QRectF boundingRect() const override { return _boundingRect; }
    };
}
