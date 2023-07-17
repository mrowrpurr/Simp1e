#pragma once

namespace Simp1e {

#ifdef SIMP1E_SREAL_DOUBLE
    using sreal = double;
#elif SIMP1E_SREAL_FLOAT
    using sreal = float;
#else
    using sreal = double;
#endif
}
