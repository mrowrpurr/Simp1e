#include <_Log_.h>

#include <string>
#include <unordered_map>

struct Item {
    std::string _name;
};

struct IStandardStuffStorageInterface {
    virtual void* GetItemPointer(const std::string& name) = 0;

    template <typename T>
    T* GetItem(const std::string& name) {
        return static_cast<T*>(GetItemPointer(name));
    }

    virtual void AddItemPointer(const std::string& name, void* pointer) = 0;

    template <typename T, typename... Args>
    void AddItem(const std::string& name, Args&&... args) {
        auto* item = new T(std::forward<Args>(args)...);
        // ???
    }
};

class PointersToStuff : public IStandardStuffStorageInterface {
    std::unordered_map<std::string, void*> _pointersToStuff;

public:
    // TODO implement IStandardStuffStorageInterface
};

class PointersToStuffLocalProxy : public IStandardStuffStorageInterface {
    PointersToStuff*                      _pointersToStuff;
    std::unordered_map<std::string, Item> _stuffItems;

public:
    // TODO implement IStandardStuffStorageInterface
};

int main() {
    _Log_("HELLO!");

    // Let's
}