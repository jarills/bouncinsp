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
