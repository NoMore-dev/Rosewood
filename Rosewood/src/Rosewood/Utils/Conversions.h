#pragma once

#include <stdint.h>
#include "Rosewood/Core.h"


namespace Rosewood
{
    namespace Utils
    {
        static uint32_t SizeToUint32(size_t size)
        {
            RW_CORE_ASSERT(size <= UINT32_MAX, " Size_t too large for uint32_t resulting in information loss! ");
            return static_cast<uint32_t>(size);
        }
    }
}