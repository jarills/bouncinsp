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

#include "BouncinSP.h"

#include <boost/format.hpp>

namespace BouncinSP
{
    std::string idx_to_pad_name( unsigned idx )
    {
        return (boost::format("%1%%2%") % (char)('A'+(idx/12)) % ((idx % 12)+1)).str();
    }
};
