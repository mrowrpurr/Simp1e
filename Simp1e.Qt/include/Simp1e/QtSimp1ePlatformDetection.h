#pragma once

#include <QtSystemDetection>

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    #define SIMP1E_MOBILE
#elif defined(Q_OS_WASM)
    #define SIMP1E_WEB
#else
    #define SIMP1E_DESKTOP
#endif
