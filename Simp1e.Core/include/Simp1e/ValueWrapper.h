#pragma once

#include "IValueWrapper.h"

namespace Simp1e {

    template <typename T>
    class ValueWrapper : public IValueWrapper {
        T _value;

    public:
        ValueWrapper(const T& value) : _value(value) {}

        T    GetValue() const { return _value; }
        void SetValue(T value) { _value = value; }
    };
}
