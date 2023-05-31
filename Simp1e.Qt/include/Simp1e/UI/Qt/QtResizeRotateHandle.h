#pragma once

#include <QCursor>
#include <QEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
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
        QColor                                   _color{::Qt::black};
        QRectF                                   _boundingBox{0, 0, 10, 10};
        HandleResponsibility                     _responsibility;
        HandlePosition                           _position;
        QPointF                                  _resizeInitialMousePos;
        QRectF                                   _resizeInitialBoundingBox;
        qreal                                    _resizeInitialRotation = 0;
        std::vector<std::function<void(QRectF)>> _onResizeCallbacks;

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
            QGraphicsItem* parent, HandlePosition position,
            HandleResponsibility responsibility = HandleResponsibility::Resize
        )
            : _position(position), _responsibility(responsibility), QGraphicsItem(parent) {
            setAcceptHoverEvents(true);
        }

        HandleResponsibility GetResponsibility() const { return _responsibility; }
        HandlePosition       GetPosition() const { return _position; }

        void SetColor(const QColor& color) { _color = color; }
        void SetSize(const QSizeF& size) { _boundingBox.setSize(size); }
        void OnResize(std::function<void(QRectF)> callback) {
            _onResizeCallbacks.push_back(callback);
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
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->buttons() != ::Qt::MouseButton::LeftButton)
                return QGraphicsItem::mousePressEvent(event);

            _resizeInitialMousePos    = event->scenePos();
            _resizeInitialBoundingBox = parentItem()->boundingRect();
            event->accept();
        }

        int i = 0;

        void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->buttons() != ::Qt::MouseButton::LeftButton)
                return QGraphicsItem::mouseMoveEvent(event);

            i++;
            if (i % 2 != 0) {
                _resizeInitialMousePos    = event->scenePos();
                _resizeInitialBoundingBox = parentItem()->boundingRect();
            }

            QPointF dragPos = event->scenePos();
            qreal   dx      = dragPos.x() - _resizeInitialMousePos.x();
            qreal   dy      = dragPos.y() - _resizeInitialMousePos.y();
            QRectF  newRect = _resizeInitialBoundingBox;

            switch (_position) {
                case HandlePosition::TopLeft:
                    newRect.setTopLeft(newRect.topLeft() + QPointF(dx, dy));
                    break;
                case HandlePosition::TopRight:
                    newRect.setTopRight(newRect.topRight() + QPointF(dx, dy));
                    break;
                case HandlePosition::BottomLeft:
                    newRect.setBottomLeft(newRect.bottomLeft() + QPointF(dx, dy));
                    break;
                case HandlePosition::BottomRight:
                    newRect.setBottomRight(newRect.bottomRight() + QPointF(dx, dy));
                    break;
                case HandlePosition::Top:
                    newRect.setTop(newRect.top() + dy);
                    break;
                case HandlePosition::Bottom:
                    newRect.setBottom(newRect.bottom() + dy);
                    break;
                case HandlePosition::Left:
                    newRect.setLeft(newRect.left() + dx);
                    break;
                case HandlePosition::Right:
                    newRect.setRight(newRect.right() + dx);
                    break;

                case HandlePosition::TopRotation:
                case HandlePosition::BottomRotation:
                    // TODO: Rotation
                    break;

                default:
                    break;
            }

            // TODO: maintain aspect ratio

            // Prevent inverting rectangle when dragging past the opposite edge
            if (newRect.width() < 10.0) newRect.setWidth(10.0);
            if (newRect.height() < 10.0) newRect.setHeight(10.0);

            for (auto& callback : _onResizeCallbacks) callback(newRect);

            event->accept();
        }
    };
}
