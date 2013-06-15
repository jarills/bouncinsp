#pragma once

#include <vector>
#include <iostream>

class PadInfo
{
public:

    PadInfo( const char * block, unsigned pad_idx );

    unsigned original_sample_start() const;
    unsigned original_sample_end() const;
    unsigned user_sample_start() const;
    unsigned user_sample_end() const;

    unsigned char volume() const;
    bool is_lofi() const;
    bool is_loop() const;
    bool is_gate() const;
    bool is_reverse() const;
    unsigned char num_channels() const;
    unsigned char tempo_mode() const; // 0 = off, 1 = pattern, 2 = user

    unsigned original_tempo() const;
    unsigned user_tempo() const;

    unsigned idx() const;

    void print() const;

    static bool read_padinfo( std::istream & in, std::vector< PadInfo > &out );

private:

    unsigned sample_convert(unsigned char offset) const;

    char data_[32]; // original data from the binary file
    unsigned idx_;
};
