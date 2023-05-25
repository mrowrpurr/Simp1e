#pragma once

#ifndef __SpecsHack_UniqueCppFileValue
    #ifdef spec_name
        #define __SpecsHack_UniqueCppFileValue __SpecsHack_Variable1_##spec_name
    #else
        #define __SpecsHack_UniqueCppFileValue __SpecsHack_Variable1
    #endif
#endif

#ifndef __SpecsHack_UniqueCppFileValue2
    #ifdef spec_name
        #define __SpecsHack_UniqueCppFileValue2 __SpecsHack_Variable2_##spec_name
    #else
        #define __SpecsHack_UniqueCppFileValue2 __SpecsHack_Variable2
    #endif
#endif

#ifndef ENABLE_LOGS
    #define _Log_(...)
#endif

#include "SpecHelperInternal.h"
