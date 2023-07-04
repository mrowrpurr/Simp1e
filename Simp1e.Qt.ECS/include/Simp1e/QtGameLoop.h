#pragma once

#include <Simp1e/IEngine.h>

#include <QTimer>

namespace Simp1e {

    class QtGameLoop {
        IEngine* _engine;
        QTimer   _timer;

    public:
        QtGameLoop(IEngine* engine, double interval = 1000.0 / 60) : _engine(engine) {
            _timer.setInterval(interval);
            QObject::connect(&_timer, &QTimer::timeout, [this]() { RunMainLoopCycle(); });
        }

        void Run() { _timer.start(); }
        void Stop() { _timer.stop(); }

        void RunMainLoopCycle() { _engine->RunMainLoopCycle(); }
    };
}
