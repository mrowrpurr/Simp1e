#pragma once

struct CoolStruct {
    int CoolNumber;
};

struct AwesomeStruct {
    const char* AwesomeText;
};

// Virtual won't work for Rust
struct AppStruct {
    virtual CoolStruct*    GetCoolStruct()    = 0;
    virtual AwesomeStruct* GetAwesomeStruct() = 0;
};
