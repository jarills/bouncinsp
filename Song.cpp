#include "Song.h"

#include <boost/format.hpp>
#include <iomanip>
#include <fstream>

#include "PatternEvent.h"
#include "Pattern.h"
#include "PadInfo.h"

#include "SimpleAudioWave.h"
#include "Pad.h"

#include "BouncinSP.h"

using namespace std;

Song::Song( const boost::filesystem::path & base_folder )
    :
      bpm_(0.0f)
{
    using namespace boost::filesystem;

    if ( !exists(base_folder) || !is_directory(base_folder))
    {
        return;
    }

    sample_path_ = base_folder / "SMPL";
    pattern_path_ = base_folder / "PTN";

    path pad_info_file = sample_path_ / "PAD_INFO.BIN";
    path stp_info_file = sample_path_ / "STPINFO.BIN";

    if ( !read_stp_info(stp_info_file) )
    {
        cout << "couldn't open " << stp_info_file << endl;
        return;
    }

    ifstream pad_info_io( pad_info_file.c_str(), std::ios::binary );

    if ( !pad_info_io )
    {
        cout << "couldnt open " << pad_info_file << endl;
        return;
    }

    const bool pad_info_ok = PadInfo::read_padinfo(pad_info_io, pad_infos_);
    if ( !pad_info_ok )
    {
        cout << "couldnt read " << pad_info_file << endl;
        return;
    }
}

bool Song::export_pattern( unsigned ptn_idx )
{
    if ( bpm_ == 0.0f )
    {
        cout << "bpm is 0" << endl;
        return false;
    }

    using namespace boost::filesystem;

    using boost::format;
    using boost::io::group;

    path ptn_file = pattern_path_ / (boost::format("PTN%1%.BIN") % group(setfill('0'), setw(5), ptn_idx)).str();

    if ( !exists(ptn_file) )
    {
        cout << "couldnt find " << ptn_file << endl;
        return false;
    }

    ifstream pattern_io( ptn_file.c_str(), std::ios::binary );

    if ( !pattern_io )
    {
        cout << "couldn't read " << ptn_file << endl;
        return false;
    }

    Pattern pattern;
    bool pattern_ok = pattern.read_pattern(pattern_io);

    if ( !pattern_ok )
    {
        cout << "couldn't parse " << ptn_file << endl;
        return false;
    }

    std::set< unsigned > pads_playing = pattern.pads_playing();

    if ( pads_playing.size() == 0 )
    {
        cout << "No pads playing in this pattern. Done." << endl;
        return true;
    }

    for ( const auto & p : pads_playing )
    {
        if ( !export_pad_in_pattern(pattern, p) )
        {
            cout << "Error exporting pad # " << p << endl;
            return false;
        }
    }

    return true;
}

bool Song::export_pad_in_pattern(const Pattern & ptn, unsigned pad_idx )
{
    if ( bpm_ == 0.0f ) return false;

    using namespace boost::filesystem;

    cout << "Exporting pad #" << pad_idx << endl;

    typedef std::vector< PatternEvent > evts_vec;

    const evts_vec & evts = ptn.events();

    Pad pad(pad_infos_[pad_idx]);
    if (!pad.load_sample(sample_path_))
    {
        return false;
    }

    SimpleAudioWave output(pad.pad_info().num_channels(), (unsigned)BouncinSP::SP404SX::SampleRate);

    float seconds_per_quarter = (60.0f/bpm_);

    pad.set_song_bpm(bpm_);

    unsigned samples_to_export = (float)(4.0f * ptn.pattern_length() * seconds_per_quarter * BouncinSP::SP404SX::SampleRate);

    for ( const auto & e : evts )
    {
        if ( pad_idx + BouncinSP::SP404SX::PadA1MidiOffset != e.note_number() )
        {
            continue;
        }

        unsigned samples_to_start = (float)(e.quarters_absolute_position() * seconds_per_quarter * BouncinSP::SP404SX::SampleRate);

        pad.queue(samples_to_start,e);
    }

    while ( samples_to_export-- )
    {
        std::vector< float > out = pad.render_sample();

        if ( out.empty() )
        {
            out.resize(output.channels(),0.0f);
        }

        output.append_frame(out);
    }

    output.normalize();
    output.save_to_wav("track_"+pad.sample_name()+".wav");

    return true;
}

bool Song::read_stp_info(const boost::filesystem::path &stp_path)
{
    ifstream in( stp_path.c_str(), std::ios::binary );

    if ( !in ) return false;

    in.seekg(0x6f);

    char bpm = 0;
    if ( !in.read(&bpm,1) ) return false;

    bpm_ = (float)bpm;

    return true;
}
