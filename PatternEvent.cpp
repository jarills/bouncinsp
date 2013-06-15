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
    return (unsigned char)data_[1];
}

unsigned char PatternEvent::velocity() const
{
    return (unsigned char)data_[4];
}

float PatternEvent::quarters_absolute_position() const
{
    return quarters_absolute_position_;
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
    cout << "note number: " << (int)note_number() << "\t";
    cout << "velocity: " << (int)velocity() << "\t";
    cout << "held for: " << quarters_held() << endl;
}
