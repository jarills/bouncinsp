#pragma once

#include "PatternEvent.h"

#include <vector>
#include <iostream>
#include <set>

class Pattern
{
public:

    Pattern();

    void reset();

    bool read_pattern( std::istream & in );

    std::set< unsigned > pads_playing() const;

    unsigned pattern_length() const;

    const std::vector< PatternEvent > & events() const;

private:

    unsigned char pattern_length_;
    std::vector< PatternEvent > events_;
    float running_quarter_;
};
