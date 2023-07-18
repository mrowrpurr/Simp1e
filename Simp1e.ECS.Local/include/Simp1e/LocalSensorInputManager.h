#pragma once

#include <Simp1e/ISensorInputManager.h>
#include <Simp1e/Position.h>
#include <function_pointer.h>

namespace Simp1e {

    class LocalSensorInputManager : public ISensorInputManager {
        IFunctionPointer<PositionF()>* _readAccelerometerFunction;

    public:
        void SetAccelerometerReadFunction(IFunctionPointer<PositionF()>* readAccelerometerFunction) {
            _readAccelerometerFunction = readAccelerometerFunction;
        }

        PositionF ReadAccelerometer() const override {
            if (!_readAccelerometerFunction) return {};
            return _readAccelerometerFunction->invoke();
        }
    };
}
