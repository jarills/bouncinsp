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

#include "Pattern.h"

#include "BouncinSP.h"

Pattern::Pattern( unsigned idx )
    :
      pattern_length_(0),
      running_quarter_(0.0f),
      idx_(idx),
      pattern_name_("Pattern " + BouncinSP::idx_to_pad_name(idx_))
{
}

std::string Pattern::pattern_name() const
{
    return pattern_name_;
}

void Pattern::set_pattern_name(const std::string &str)
{
    pattern_name_ = str;
}

void Pattern::reset()
{
    running_quarter_ = 0.0f;
    events_.clear();
}

unsigned Pattern::index() const
{
    return idx_;
}

bool Pattern::read_pattern( std::istream & in )
{
    reset();

    char block[8];

    bool note_mode = true;
    bool read_footer = false;

    while ( !read_footer && in.read(block, 8) )
    {
        if ( note_mode )
        {
            PatternEvent pe(block, running_quarter_);

            if ( pe.note_number() == 0x8C )
            {
                note_mode = false;
                continue;
            }

            events_.push_back(pe);
            pe.print();
            running_quarter_ += pe.quarters_offset_to_next();
        }
        else
        {
            pattern_length_ = block[1];
            read_footer = true;
            std::cout << "footer found. pattern length: " << (int) pattern_length_ << std::endl;
        }
    }

    return read_footer;
}

std::set< unsigned > Pattern::pads_playing() const
{
    std::set< unsigned > out;

    for ( const auto & e : events_ )
    {
        if ( e.is_sample() )
        {
            out.insert(e.pad_index());
        }
    }

    return out;
}

unsigned Pattern::pattern_length() const
{
    return pattern_length_;
}

const std::vector<PatternEvent> &Pattern::events() const
{
    return events_;
}
