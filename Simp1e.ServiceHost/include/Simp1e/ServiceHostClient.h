#pragma once

#include "Export.h"
#include "IServiceHost.h"

#define OnSimp1eInit extern "C" EXPORT void Simp1eInit(Simp1e::IServiceHost* Simp1eServices)
#define OnSimp1eLoad extern "C" EXPORT void Simp1eLoad(Simp1e::IServiceHost* Simp1eServices)
#define OnSimp1eUnload extern "C" EXPORT void Simp1eUnload(Simp1e::IServiceHost* Simp1eServices)
#define OnSimp1eStart extern "C" EXPORT void Simp1eStart(Simp1e::IServiceHost* Simp1eServices)
