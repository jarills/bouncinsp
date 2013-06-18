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

namespace
{
    boost::filesystem::path resolve_song_folder(const boost::filesystem::path & base, const std::string & sp_folder)
    {
        using namespace boost::filesystem;

        if ( base.stem() == sp_folder )
        {
            return base;
        }

        path one_down = base / sp_folder;

        if ( is_directory( one_down ) )
        {
            return one_down;
        }

        path two_down = base / "ROLAND" / sp_folder;
        if ( is_directory( two_down ) )
        {
            return two_down;
        }

        return base;
    }
}

Song::Song( const boost::filesystem::path & base_folder )
    :
      bpm_(0.0f)
{
    using namespace boost::filesystem;

    boost::system::error_code ec;

    base_folder_ = resolve_song_folder(base_folder, "SP-404SX");
    cout << "resolved folder: " << base_folder_ << endl;

    if ( !is_directory(base_folder_, ec))
    {
        cout << "Couldn't find song folder " << ec.value() << ec.message() << endl;
        return;
    }

    sample_path_ = base_folder_ / "SMPL";
    pattern_path_ = base_folder_ / "PTN";

    path pad_info_file = sample_path_ / "PAD_INFO.BIN";
    path stp_info_file = sample_path_ / "STPINFO.BIN";

    if ( !read_stp_info(stp_info_file) )
    {
        cout << "couldn't open " << stp_info_file << endl;
        return;
    }

    ifstream pad_info_io( pad_info_file.string().c_str(), std::ios::binary );

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

std::shared_ptr< Pattern > Song::load_pattern(const boost::filesystem::path & ptn_file, unsigned idx )
{
    using namespace boost::filesystem;

    if ( !exists(ptn_file) )
    {
        cout << "couldnt find " << ptn_file << endl;
        return std::shared_ptr< Pattern >();
    }

    ifstream pattern_io( ptn_file.string().c_str(), std::ios::binary );

    if ( !pattern_io )
    {
        cout << "couldn't read " << ptn_file << endl;
        return std::shared_ptr< Pattern >();
    }

    std::shared_ptr< Pattern > pattern = std::make_shared< Pattern >(idx);
    bool pattern_ok = pattern->read_pattern(pattern_io);

    if ( !pattern_ok )
    {
        cout << "couldn't parse " << ptn_file << endl;
        return std::shared_ptr< Pattern >();
    }

    return pattern;
}

bool Song::export_pattern(const Pattern &ptn , const ExportOptions &export_options)
{
    if ( bpm_ == 0.0f )
    {
        cout << "bpm is 0" << endl;
        return false;
    }

    cout << "Exporting pattern '" << ptn.pattern_name() << "'" << endl;

    std::set< unsigned > pads_playing = ptn.pads_playing();

    if ( pads_playing.size() == 0 )
    {
        cout << "No pads playing in this pattern. Done." << endl;
        return true;
    }

    unsigned done_count = 0;
    sig_track_exported_(0);
    for ( const auto & p : pads_playing )
    {
        if ( !export_pad_in_pattern(ptn, p, export_options) )
        {
            cout << "Error exporting pad # " << p << endl;
            return false;
        }

        sig_track_exported_(++done_count);
    }

    return true;
}

bool Song::export_pad_in_pattern(const Pattern & ptn, unsigned pad_idx, const ExportOptions &export_options )
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

    unsigned pattern_length_in_samples = (float)(4.0f * ptn.pattern_length() * seconds_per_quarter * BouncinSP::SP404SX::SampleRate);
    unsigned samples_to_export = pattern_length_in_samples * export_options.loop_count_;

    for ( const auto & e : evts )
    {
        if ( (int)pad_idx != e.pad_index() )
        {
            continue;
        }

        unsigned samples_to_start = (float)(e.quarters_absolute_position() * seconds_per_quarter * BouncinSP::SP404SX::SampleRate);

        // queue as many times as we need loops
        for ( unsigned int i = 0; i < export_options.loop_count_; ++i )
        {
            pad.queue(samples_to_start + i*pattern_length_in_samples,e);
        }
    }

    while ( samples_to_export ||
            (export_options.keep_tails_ && pad.playing() && (!pad.pad_info().is_loop()||pad.pad_info().is_gate())))
    {
        if ( samples_to_export > 0 ) samples_to_export--;

        std::vector< float > out = pad.render_sample();

        if ( out.empty() )
        {
            out.resize(output.channels(),0.0f);
        }

        output.append_frame(out);
    }

    if ( export_options.normalize_ )
    {
        output.normalize();
    }

    std::string bar_count = (boost::format("_%1%bars") % (ptn.pattern_length() * export_options.loop_count_)).str();

    std::string filename = "track_" + ptn.pattern_name() + "_" + BouncinSP::idx_to_pad_name(pad.pad_info().idx()) + bar_count + ".wav";
    path out_path = export_options.path_ / filename;
    output.save_to_wav(out_path.string().c_str());

    return true;
}

bool Song::load_patterns()
{
    using namespace boost::filesystem;

    const unsigned int max_patterns = 120;

    for ( unsigned int i = 0; i < max_patterns; ++i )
    {
        path ptn_file = pattern_path_ / pattern_name(i);

        if ( !exists(ptn_file) )
        {
            continue;
        }

        auto ptn = load_pattern(ptn_file, i);

        if ( ptn )
        {
            patterns_.push_back(ptn);
        }
    }

    return true;
}

std::string Song::pattern_name(unsigned idx) const
{
    using boost::format;
    using boost::io::group;

    return (boost::format("PTN%1%.BIN") % group(setfill('0'), setw(5), idx+1)).str();
}

const vector<std::shared_ptr<Pattern> > &Song::patterns() const
{
    return patterns_;
}

float Song::bpm() const
{
    return bpm_;
}

const boost::filesystem::path &Song::base_folder() const
{
    return base_folder_;
}

bool Song::read_stp_info(const boost::filesystem::path &stp_path)
{
    ifstream in( stp_path.string().c_str(), std::ios::binary );

    if ( !in ) return false;

    in.seekg(0x6f);

    char bpm = 0;
    if ( !in.read(&bpm,1) ) return false;

    bpm_ = (float)bpm;

    return true;
}
