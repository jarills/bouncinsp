#include "BouncinSP.h"

#include <boost/format.hpp>

namespace BouncinSP
{
    std::string idx_to_pad_name( unsigned idx )
    {
        return (boost::format("%1%%2%") % (char)('A'+(idx/12)) % ((idx % 12)+1)).str();
    }
};
