#pragma once

namespace Simp1e {

#ifdef SIMP1E_DOUBLE_PRECISION
    using sreal = double;
#else
    using sreal = float;
#endif
}
