#pragma once

#include <QCursor>
#include <QEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <functional>
#include <vector>

namespace Simp1e::UI::Qt {
    class QtResizeRotateHandle : public QGraphicsItem {
    public:
        enum class HandlePosition {
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight,
            Top,
            Bottom,
            Left,
            Right,
            TopRotation,
            BottomRotation,
        };

        enum class HandleResponsibility {
            None,
            Resize,
            Rotate,
        };

    private:
        QColor                                                  _color{::Qt::black};
        QRectF                                                  _boundingBox{0, 0, 10, 10};
        HandleResponsibility                                    _responsibility;
        HandlePosition                                          _position;
        std::vector<std::function<void(QtResizeRotateHandle*)>> _onMouseEnterCallbacks;
        std::vector<std::function<void(QtResizeRotateHandle*)>> _onMouseLeaveCallbacks;

        ::Qt::CursorShape GetCursorShape() const {
            switch (_position) {
                case HandlePosition::TopLeft:
                    return ::Qt::CursorShape::SizeFDiagCursor;
                case HandlePosition::TopRight:
                    return ::Qt::CursorShape::SizeBDiagCursor;
                case HandlePosition::BottomLeft:
                    return ::Qt::CursorShape::SizeBDiagCursor;
                case HandlePosition::BottomRight:
                    return ::Qt::CursorShape::SizeFDiagCursor;
                case HandlePosition::Top:
                    return ::Qt::CursorShape::SizeVerCursor;
                case HandlePosition::Bottom:
                    return ::Qt::CursorShape::SizeVerCursor;
                case HandlePosition::Left:
                    return ::Qt::CursorShape::SizeHorCursor;
                case HandlePosition::Right:
                    return ::Qt::CursorShape::SizeHorCursor;
                case HandlePosition::TopRotation:
                case HandlePosition::BottomRotation:
                    return ::Qt::CursorShape::CrossCursor;
                default:
                    return ::Qt::CursorShape::ArrowCursor;
            }
        }

    public:
        QtResizeRotateHandle(
            HandlePosition       position,
            HandleResponsibility responsibility = HandleResponsibility::Resize,
            QGraphicsItem*       parent         = nullptr
        )
            : _position(position), _responsibility(responsibility), QGraphicsItem(parent) {
            setAcceptHoverEvents(true);
        }

        HandleResponsibility GetResponsibility() const { return _responsibility; }
        HandlePosition       GetPosition() const { return _position; }

        void SetColor(const QColor& color) { _color = color; }
        void SetSize(const QSizeF& size) { _boundingBox.setSize(size); }

        void OnMouseEnter(std::function<void(QtResizeRotateHandle*)> callback) {
            _onMouseEnterCallbacks.push_back(callback);
        }
        void OnMouseLeave(std::function<void(QtResizeRotateHandle*)> callback) {
            _onMouseLeaveCallbacks.push_back(callback);
        }

    protected:
        QRectF boundingRect() const override { return _boundingBox; }
        void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(_color);
            painter->drawEllipse(_boundingBox);
        }

        void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
            setCursor(QCursor(GetCursorShape()));
        }
        void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override { unsetCursor(); }
    };
}
