#pragma once

#include <QWidget>
#include <unordered_map>

#include "IQtGraphicsItem.h"
#include "QtResizeRotateHandle.h"

namespace Simp1e::UI::Qt {
    class QtResizeRotateHandleCollection {
        IQtGraphicsItem* _parent;
        uint32_t         _padding = 20;
        std::unordered_map<QtResizeRotateHandle::HandlePosition, QtResizeRotateHandle*> _handles;

        void OnResize(QRectF boundingBox, qreal rotation) {
            _parent->SetBoundingBox(boundingBox);
            _parent->UpdateBorder();
            _parent->setRotation(rotation);
            RepositionHandles(boundingBox);
        }

        void CreateHandle(
            QtResizeRotateHandle::HandlePosition       position,
            QtResizeRotateHandle::HandleResponsibility responsibility =
                QtResizeRotateHandle::HandleResponsibility::Resize
        ) {
            auto* handle = new QtResizeRotateHandle(_parent, position, responsibility);
            if (responsibility == QtResizeRotateHandle::HandleResponsibility::Rotate)
                handle->SetColor(::Qt::red);
            handle->setEnabled(true);
            _handles[position] = handle;
            handle->OnResize([this](QRectF boundingBox, qreal rotation) {
                OnResize(boundingBox, rotation);
            });
        }

        void CreateResizerHandles() {
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::TopLeft))
                CreateHandle(QtResizeRotateHandle::HandlePosition::TopLeft);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::Top))
                CreateHandle(QtResizeRotateHandle::HandlePosition::Top);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::TopRight))
                CreateHandle(QtResizeRotateHandle::HandlePosition::TopRight);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::Right))
                CreateHandle(QtResizeRotateHandle::HandlePosition::Right);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::BottomRight))
                CreateHandle(QtResizeRotateHandle::HandlePosition::BottomRight);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::Bottom))
                CreateHandle(QtResizeRotateHandle::HandlePosition::Bottom);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::BottomLeft))
                CreateHandle(QtResizeRotateHandle::HandlePosition::BottomLeft);
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::Left))
                CreateHandle(QtResizeRotateHandle::HandlePosition::Left);
        }

        void CreateRotationHandle() {
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::TopRotation))
                CreateHandle(
                    QtResizeRotateHandle::HandlePosition::TopRotation,
                    QtResizeRotateHandle::HandleResponsibility::Rotate
                );
            if (!_handles.contains(QtResizeRotateHandle::HandlePosition::BottomRotation))
                CreateHandle(
                    QtResizeRotateHandle::HandlePosition::BottomRotation,
                    QtResizeRotateHandle::HandleResponsibility::Rotate
                );
        }

        void DestroyAllHandles() {
            for (auto& [position, handle] : _handles) delete handle;
            _handles.clear();
        }

    public:
        QtResizeRotateHandleCollection(IQtGraphicsItem* parent) : _parent(parent) {}
        ~QtResizeRotateHandleCollection() { DestroyAllHandles(); }

        void     SetPadding(uint32_t padding) { _padding = padding; }
        uint32_t GetPadding() const { return _padding; }

        void ActivateResizing() {
            CreateResizerHandles();
            RepositionHandles();
        }

        void ActivateRotation() {
            CreateRotationHandle();
            RepositionHandles();
        }

        void Deactivate() { DestroyAllHandles(); }

        void RepositionHandles(QRectF itemBoundingBox) {
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::Top))
                _handles[QtResizeRotateHandle::HandlePosition::Top]->setPos(
                    itemBoundingBox.left() + itemBoundingBox.width() / 2,
                    itemBoundingBox.top() - _padding
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::Bottom))
                _handles[QtResizeRotateHandle::HandlePosition::Bottom]->setPos(
                    itemBoundingBox.left() + itemBoundingBox.width() / 2,
                    itemBoundingBox.bottom() + _padding
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::Left))
                _handles[QtResizeRotateHandle::HandlePosition::Left]->setPos(
                    itemBoundingBox.left() - _padding,
                    itemBoundingBox.top() + itemBoundingBox.height() / 2
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::Right))
                _handles[QtResizeRotateHandle::HandlePosition::Right]->setPos(
                    itemBoundingBox.right() + _padding,
                    itemBoundingBox.top() + itemBoundingBox.height() / 2
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::TopLeft))
                _handles[QtResizeRotateHandle::HandlePosition::TopLeft]->setPos(
                    itemBoundingBox.left() - _padding, itemBoundingBox.top() - _padding
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::TopRight))
                _handles[QtResizeRotateHandle::HandlePosition::TopRight]->setPos(
                    itemBoundingBox.right() + _padding, itemBoundingBox.top() - _padding
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::BottomLeft))
                _handles[QtResizeRotateHandle::HandlePosition::BottomLeft]->setPos(
                    itemBoundingBox.left() - _padding, itemBoundingBox.bottom() + _padding
                );
            if (_handles.contains(QtResizeRotateHandle::HandlePosition::BottomRight))
                _handles[QtResizeRotateHandle::HandlePosition::BottomRight]->setPos(
                    itemBoundingBox.right() + _padding, itemBoundingBox.bottom() + _padding
                );
            if (_handles.find(QtResizeRotateHandle::HandlePosition::TopRotation) != _handles.end())
                _handles[QtResizeRotateHandle::HandlePosition::TopRotation]->setPos(
                    itemBoundingBox.left() + itemBoundingBox.width() / 2,
                    itemBoundingBox.top() - _padding * 3
                );
            if (_handles.find(QtResizeRotateHandle::HandlePosition::BottomRotation) !=
                _handles.end())
                _handles[QtResizeRotateHandle::HandlePosition::BottomRotation]->setPos(
                    itemBoundingBox.left() + itemBoundingBox.width() / 2,
                    itemBoundingBox.bottom() + _padding * 3
                );
        }

        void RepositionHandles() {
            if (_parent == nullptr) return;
            RepositionHandles(_parent->GetBoundingBox());
        }
    };
}
