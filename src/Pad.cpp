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

#include "Pad.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iomanip>
#include "SimpleAudioWave.h"
#include "PatternEvent.h"
#include "PadInfo.h"

using namespace std;

Pad::Pad( const PadInfo & pad_info )
    :
      pad_info_(pad_info),
      song_bpm_(0.0f),
      playing_(false),
      samples_remaining_(0),
      play_pos_(0),
      event_velocity_(0)
{
}

bool Pad::load_sample(const boost::filesystem::path samples_path)
{
    using namespace boost::filesystem;

    path sample_file = samples_path / sample_name();

    if ( !exists(sample_file) || !is_regular_file(sample_file))
    {
        cout << "Couldn't find sample " << sample_file << endl;
        return false;
    }

    sample_.reset( new SimpleAudioWave( sample_file.string() ));
    cout << "loaded sample with " << sample_->max_size() << " frames" << endl;

    return true;
}

std::string Pad::sample_name() const
{
    using boost::format;
    using boost::io::group;

    char bank = 'A' + (pad_info_.idx()) / 12;
    unsigned relative_pad_idx = ((pad_info_.idx()) % 12)+1;
    std::string extension = pad_info_.is_wav() ? ".WAV" : ".AIF";

    return (boost::format("%1%%2%%3%") % bank % group(setfill('0'), setw(7), relative_pad_idx) % extension).str();
}

std::vector< float > Pad::render_sample()
{
    process_queue();

    if ( samples_remaining_ == 0 && pad_info_.is_gate() )
    {
        playing_ = false;
    }

    if ( play_pos_ < 0 )
    {
        if ( pad_info_.is_loop() && pad_info_.is_reverse() )
        {
            play_pos_ = pad_info_.user_sample_end();
        }
        else
        {
            playing_ = false;
        }
    }

    if ( play_pos_ >= (int)pad_info_.user_sample_end() )
    {
        if ( pad_info_.is_loop() )
        {
            play_pos_ = (int)pad_info_.user_sample_start();
        }
        else
        {
            playing_ = false;
        }
    }

    std::vector<float> temp;

    if ( !playing_ )
    {
        return temp;
    }

    float gain = ((float)pad_info_.volume()/127.0f) *((float)event_velocity_/127.0f);

    for ( unsigned int c = 0; c < pad_info_.num_channels(); ++c )
    {
        temp.push_back(sample_->get_samples(c)[play_pos_]*gain);
    }

    play_pos_ += pad_info_.is_reverse() ? -1 : 1;

    if ( samples_remaining_ > 0 )
    {
        --samples_remaining_;
    }

    return temp;
}

const PadInfo &Pad::pad_info() const
{
    return pad_info_;
}

void Pad::queue(int samples_to_start, const PatternEvent & evt)
{
    queue_.push_back( std::make_pair( samples_to_start, evt ) );
}

void Pad::set_song_bpm(float bpm)
{
    song_bpm_ = bpm;
}

bool Pad::is_queue_empty() const
{
    return queue_.empty();
}

bool Pad::playing() const
{
    return playing_;
}

void Pad::process_queue()
{
    if ( song_bpm_ == 0.0f ) return;

    const float seconds_per_quarter = 60.0f / song_bpm_;

    for ( auto & i : queue_ )
    {
        if ( i.first == 0 )
        {
            const PatternEvent & evt = i.second;

            // reset play cursor
            play_pos_ = pad_info_.is_reverse() ? (pad_info_.user_sample_end()-1) : pad_info_.user_sample_start();
            samples_remaining_ = unsigned((float)evt.quarters_held() * seconds_per_quarter * (float)sample_->sample_rate());
            event_velocity_ = evt.velocity();

            if ( pad_info_.is_loop() && playing_ ) // loops stop playing when pressed a second time
            {
                playing_ = false;
            }
            else // all other start/retrigger
            {
                playing_ = true;
            }
        }

        i.first--;
    }

    queue_.erase(
        std::remove_if(queue_.begin(), queue_.end(),
            [](const std::pair< int, PatternEvent > & i) { return i.first < 0; }),
        queue_.end());
}
