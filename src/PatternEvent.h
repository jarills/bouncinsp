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

class PatternEvent
{
public:
    PatternEvent( const char * block, float absolute_position_in_quarters );

    unsigned char offset_to_next() const;
    float quarters_offset_to_next() const;
    unsigned char note_number() const;
    int pad_index() const; // includes bank switch. -1 for non-pads
    bool bank_switch() const;
    unsigned char velocity() const;
    float quarters_held() const;
    float quarters_absolute_position() const;

    bool is_sample() const; // true if midi number is one of the 120 pads

    const char *data() const;

    void print() const;

private:

    char data_[8]; // each event has 8 bytes
    float quarters_absolute_position_;
};

