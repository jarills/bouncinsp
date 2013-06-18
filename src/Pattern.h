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

#pragma once

#include "PatternEvent.h"

#include <vector>
#include <iostream>
#include <set>

class Pattern
{
public:

    Pattern( unsigned idx );

    std::string pattern_name() const;
    void set_pattern_name( const std::string & str );

    void reset();

    unsigned index() const;

    bool read_pattern( std::istream & in );

    std::set< unsigned > pads_playing() const;

    unsigned pattern_length() const;

    const std::vector< PatternEvent > & events() const;

private:

    unsigned char pattern_length_;
    std::vector< PatternEvent > events_;
    float running_quarter_;
    unsigned idx_;

    std::string pattern_name_;
};
