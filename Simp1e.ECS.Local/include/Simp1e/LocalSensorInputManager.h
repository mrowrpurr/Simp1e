#pragma once

#include <Simp1e/ISensorInputManager.h>
#include <function_pointer.h>

namespace Simp1e {

    class LocalSensorInputManager : public ISensorInputManager {
        IFunctionPointer<Vec3<sreal>()>* _readAccelerometerFunction;

    public:
        void SetAccelerometerReadFunction(IFunctionPointer<Vec3<sreal>()>* readAccelerometerFunction) {
            _readAccelerometerFunction = readAccelerometerFunction;
        }

        Vec3<sreal> ReadAccelerometer() const override {
            if (!_readAccelerometerFunction) return {};
            return _readAccelerometerFunction->invoke();
        }
    };
}
