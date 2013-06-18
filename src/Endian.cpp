/*

    Bouncin' SP copyright jarills 2013

    This file is part of Bouncin' SP.

    Bouncin' SP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bouncin' SP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bouncin' SP.  If not, see <http://www.gnu.org/licenses/>.

*/

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

