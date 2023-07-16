#pragma once

#include <Simp1e/EventCast.h>
#include <Simp1e/EventResult.h>
#include <function_pointer.h>

#include <QApplication>
#include <QKeyEvent>
#include <memory>
#include <vector>

namespace Simp1e {

    class QSimp1eApp : public QApplication {
        int _mockArgcForQApplication = 0;

        std::vector<std::unique_ptr<IFunctionPointer<EventResult::Value(QKeyEvent*)>>> _keyboardEventListeners;

    public:
        QSimp1eApp(int& argc, char** argv) : QApplication(argc, argv) { setStyle("Fusion"); }
        QSimp1eApp() : QSimp1eApp(_mockArgcForQApplication, nullptr) {}

        void OnKeyEvent(IFunctionPointer<EventResult::Value(QKeyEvent*)>* callback) {
            _keyboardEventListeners.emplace_back(callback);
        }

        void OnKeyEvent(FunctionPointer<EventResult::Value(QKeyEvent*)> callback) {
            callback.do_not_destroy_function_pointer();
            _keyboardEventListeners.emplace_back(callback.inner_function_pointer());
        }

    protected:
        bool notify(QObject* receiver, QEvent* event) override {
            if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
                for (auto& listener : _keyboardEventListeners)
                    if (listener->invoke(static_cast<QKeyEvent*>(event))) return true;
            return QApplication::notify(receiver, event);
        }
    };
}
