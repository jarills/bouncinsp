#include "Endian.h"

#include <boost/detail/endian.hpp>

namespace Endian
{
    unsigned big_endian_to_native_32(unsigned big)
    {
#if defined(BOOST_BIG_ENDIAN)
        return big;
#else
        return __builtin_bswap32(big);
#endif
    }

    unsigned short big_endian_to_native_16(unsigned short big)
    {
#if defined(BOOST_BIG_ENDIAN)
        return big;
#else
        return (big<<8)|(big>>8);
#endif
    }

    unsigned data_32bit_to_native_endian(const char *data)
    {
        return big_endian_to_native_32( *(reinterpret_cast<const unsigned*>(data)) );
    }

    unsigned short data_16bit_to_native_endian(const char *data)
    {
        return big_endian_to_native_16( *(reinterpret_cast<const unsigned short*>(data)) );
    }
};

