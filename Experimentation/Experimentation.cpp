#include <_Log_.h>

// TODO: IValueWrapper and ValueWrapper<T>.
// TODO: We'll pass multiple arguments as a IValueWrapper**

#include <Simp1e/FunctionPointer.h>
#include <Simp1e/MemberFunctionPointer.h>

#include <unordered_map>
#include <vector>

using namespace Simp1e;

std::vector<std::unique_ptr<IFunctionPointer>> _callbacks;

void RegisterCallback(IFunctionPointer* ptr) { _callbacks.push_back(std::unique_ptr<IFunctionPointer>(ptr)); }

// template <typename T>
// void RegisterCallback(T* ptr, int (T::*func)(int)) {
//     RegisterCallback(new MemberFunctionPointer<T, void, int>(ptr, func));
// }

void RegisterCallback(int (*func)(int)) { RegisterCallback(new FunctionPointer<int, int>(func)); }

int ThisIsAFuntion(int value) {
    _Log_("ThisIsAFuntion called with value = {}", value);
    return value * 2;
}

void RunCallbacks() {
    // for (auto& callback : _callbacks) callback->Invoke(42);
    for (auto& callback : _callbacks) {
        if (auto* functionPointer = dynamic_cast<FunctionPointer<int, int>*>(callback.get())) {
            IValueWrapper* args[1];
            args[0]     = new ValueWrapper<int>(69);
            auto result = static_cast<ValueWrapper<int>*>(functionPointer->Invoke(args))->GetValue();
            _Log_("result = {}", result);
        }
        // } else if (auto* memberFunctionPointer = dynamic_cast<MemberFunctionPointer<Environment, void,
        // int>*>(callback.get())) {
        //     memberFunctionPointer->Invoke(nullptr, 42);
        // }
    }
}

int main() {
    _Log_("hi");
    RegisterCallback(ThisIsAFuntion);
    RunCallbacks();
}
