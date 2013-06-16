#include "PadInfo.h"

#include <algorithm>
#include <iostream>
#include "Endian.h"

namespace PadDataOffsets
{
    static const unsigned original_sample_start = 0;
    static const unsigned original_sample_end = 4;
    static const unsigned user_sample_start = 8;
    static const unsigned user_sample_end = 12;

    static const unsigned volume = 16;
    static const unsigned lofi = 17;
    static const unsigned loop = 18;
    static const unsigned gate = 19;
    static const unsigned reverse = 20;

    static const unsigned unknown = 21;
    static const unsigned channels = 22;
    static const unsigned tempo_mode = 23;

    static const unsigned original_tempo = 24;
    static const unsigned user_tempo = 28;
};

PadInfo::PadInfo(const char *block, unsigned pad_idx)
    :
      idx_(pad_idx)
{
    std::copy(block, block + 32, data_);
}

unsigned PadInfo::original_sample_start() const
{
    return sample_convert(PadDataOffsets::original_sample_start);
}

unsigned PadInfo::original_sample_end() const
{
    return sample_convert(PadDataOffsets::original_sample_end);
}

unsigned PadInfo::user_sample_start() const
{
    return sample_convert(PadDataOffsets::user_sample_start);
}

unsigned PadInfo::user_sample_end() const
{
    return sample_convert(PadDataOffsets::user_sample_end);
}

unsigned char PadInfo::volume() const
{
    return data_[PadDataOffsets::volume];
}

bool PadInfo::is_lofi() const
{
    return data_[PadDataOffsets::lofi] == 1;
}

bool PadInfo::is_loop() const
{
    return data_[PadDataOffsets::loop] == 1;
}

bool PadInfo::is_gate() const
{
    return data_[PadDataOffsets::gate] == 1;
}

bool PadInfo::is_reverse() const
{
    return data_[PadDataOffsets::reverse] == 1;
}

unsigned char PadInfo::num_channels() const
{
    return data_[PadDataOffsets::channels];
}

unsigned char PadInfo::tempo_mode() const
{
    return data_[PadDataOffsets::tempo_mode];
}

unsigned PadInfo::original_tempo() const
{
    return Endian::data_32bit_to_native_endian(&data_[PadDataOffsets::original_tempo]);
}

unsigned PadInfo::user_tempo() const
{
    return Endian::data_32bit_to_native_endian(&data_[PadDataOffsets::user_tempo]);
}

unsigned PadInfo::idx() const
{
    return idx_;
}

bool PadInfo::read_padinfo( std::istream & in, std::vector< PadInfo > & out )
{
    const unsigned num_pads = 120;

    char buffer[32];

    unsigned idx = 0;

    while ( in.read(buffer, 32) )
    {
        PadInfo info( buffer, idx );
        out.push_back( info );
        idx++;
    }

    const bool success = out.size() == num_pads;
    return success;
}

unsigned PadInfo::sample_convert(unsigned char offset) const
{
    return (Endian::data_32bit_to_native_endian(&data_[offset])-512)
            / (2 * num_channels());
}

void PadInfo::print() const
{
    using namespace std;
    cout << "pad info:" << endl;
    cout << "orig sample start: " << original_sample_start() << endl;
    cout << "orig sample end: " << original_sample_end() << endl;
    cout << "user sample start: " << user_sample_start() << endl;
    cout << "user sample end: " << user_sample_end() << endl;

    cout << "volume: " << (int)volume() << endl;
    cout << "lofi: " << (int) is_lofi() << endl;
    cout << "loop: " << (int) is_loop() << endl;
    cout << "gate: " << (int) is_gate() << endl;
    cout << "reverse: " << (int) is_reverse() << endl;

    cout << "channels: " << (int) num_channels() << endl;
    cout << "tempo mode: " << (int) tempo_mode() << endl;
    cout << "original tempo: " << (int) original_tempo() << endl;
    cout << "user tempo: " << (int) user_tempo() << endl;
}
