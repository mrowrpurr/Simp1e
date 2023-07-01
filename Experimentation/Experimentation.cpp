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

template <typename T>
void RegisterCallback(T* ptr, int (T::*func)(int)) {
    RegisterCallback(new MemberFunctionPointer<T, int, int>(func, ptr));
}

void RegisterCallback(int (*func)(int)) { RegisterCallback(new FunctionPointer<int, int>(func)); }

int ThisIsAFuntion(int value) {
    _Log_("ThisIsAFuntion called with value = {}", value);
    return value * 2;
}

struct ThisIsAStruct {
    int ThisIsAMemberFunction(int value) {
        _Log_("ThisIsAMemberFunction called with value = {}", value);
        return value * 3;
    }
};

void RunCallbacks() {
    for (auto& callback : _callbacks) {
        IValueWrapper* args[1];
        args[0]     = new ValueWrapper<int>(69);
        auto result = static_cast<ValueWrapper<int>*>(callback->Invoke(args))->GetValue();
        _Log_("result = {}", result);
    }
}

int main() {
    _Log_("hi");
    RegisterCallback(ThisIsAFuntion);
    ThisIsAStruct thisIsAStruct;
    RegisterCallback(&thisIsAStruct, &ThisIsAStruct::ThisIsAMemberFunction);
    RunCallbacks();
}
