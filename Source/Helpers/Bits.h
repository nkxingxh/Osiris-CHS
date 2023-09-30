#pragma once

#include <cassert>

#include <Platform/Macros/IsPlatform.h>

#if IS_WIN64()
#include <intrin.h>
#endif

namespace bits
{

[[nodiscard]] inline int countrZero(unsigned int x) noexcept
{
    assert(x != 0 && "x must be non-zero");
#if IS_WIN64()
    unsigned long result;
    _BitScanForward(&result, x);
    return static_cast<int>(result);
#elif IS_LINUX()
    return __builtin_ctz(x);
#endif
}

}
