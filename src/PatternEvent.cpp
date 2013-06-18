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

#include "PatternEvent.h"

#include <algorithm>
#include <iostream>

#include "Endian.h"

#include "BouncinSP.h"

PatternEvent::PatternEvent( const char * block, float absolute_position_in_quarters )
    :
      quarters_absolute_position_(absolute_position_in_quarters)
{
    std::copy(block, block + 8, data_);
}

unsigned char PatternEvent::offset_to_next() const
{
    return (unsigned char)data_[0];
}

float PatternEvent::quarters_offset_to_next() const
{
    return (float)offset_to_next()/BouncinSP::SP404SX::PPQ;
}

unsigned char PatternEvent::note_number() const
{
    return (unsigned char) data_[1];
}

int PatternEvent::pad_index() const
{
    if ( !is_sample() ) return -1;

    return (note_number() - BouncinSP::SP404SX::PadA1MidiOffset) + (bank_switch() ? 60 : 0);
}

bool PatternEvent::bank_switch() const
{
    return data_[2] == 1;
}

unsigned char PatternEvent::velocity() const
{
    return (unsigned char)data_[4];
}

float PatternEvent::quarters_absolute_position() const
{
    return quarters_absolute_position_;
}

bool PatternEvent::is_sample() const
{
    return note_number() >= BouncinSP::SP404SX::PadA1MidiOffset && note_number() < 0x80;
}

const char *PatternEvent::data() const
{
    return data_;
}

float PatternEvent::quarters_held() const
{
    return ((float)Endian::data_16bit_to_native_endian(&data_[6]))/BouncinSP::SP404SX::PPQ;
}

void PatternEvent::print() const
{
    using namespace std;
    cout << "pattern event:" << "\t";
    cout << "position: " << quarters_absolute_position() << "\t";
    cout << "pad index: " << (int)pad_index() << "\t";
    cout << "velocity: " << (int)velocity() << "\t";
    cout << "held for: " << quarters_held() << endl;
}
