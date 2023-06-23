#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <functional>
#include <optional>

namespace Simp1e {

    class QTGraphicsItem : public QGraphicsItem {
        QRectF                     _boundingRect{0, 0, 0, 0};
        std::optional<std::string> _text;
        std::optional<QColor>      _borderColor;

        std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> _painter = [](auto, auto, auto) {};

    public:
        QTGraphicsItem(std::function<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)> painter)
            : _painter(painter) {}

        void SetBoundingRect(const QRectF& boundingRect) {
            if (_boundingRect == boundingRect) return;
            _boundingRect = boundingRect;
            prepareGeometryChange();
            update();
        }
        QRectF GetBoundingRect() const { return _boundingRect; }

        void SetText(const std::string& text) {
            _text = text;
            update();
        }
        std::optional<std::string> GetText() const { return _text; }

        void SetSize(const QSizeF& size) {
            _boundingRect.setSize(size);
            update();
        }
        QSizeF GetSize() const { return _boundingRect.size(); }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
            _painter(painter, option, widget);
        }
        QRectF boundingRect() const override { return _boundingRect; }
    };
}
