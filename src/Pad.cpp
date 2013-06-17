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
      play_pos_(0)
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

    return (boost::format("%1%%2%.WAV") % bank % group(setfill('0'), setw(7), relative_pad_idx)).str();
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

    for ( unsigned int c = 0; c < pad_info_.num_channels(); ++c )
    {
        temp.push_back(sample_->get_samples(c)[play_pos_]);
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

            if ( !pad_info_.is_loop() || !playing_ )
            {
                // reset play cursor
                play_pos_ = pad_info_.is_reverse() ? (pad_info_.user_sample_end()-1) : pad_info_.user_sample_start();
            }

            samples_remaining_ = unsigned((float)evt.quarters_held() * seconds_per_quarter * (float)sample_->sample_rate());
            playing_ = true;
        }

        i.first--;
    }

    queue_.erase(
        std::remove_if(queue_.begin(), queue_.end(),
            [](const std::pair< int, PatternEvent > & i) { return i.first < 0; }),
        queue_.end());
}
