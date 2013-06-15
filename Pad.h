#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "PadInfo.h"
#include <vector>
#include <queue>
#include <utility>

class SimpleAudioWave;
class PatternEvent;

class Pad
{
public:
    Pad( const PadInfo &pad_info );

    bool load_sample(const boost::filesystem::path samples_path);

    std::string sample_name() const;

    std::vector<float> render_sample();

    const PadInfo & pad_info() const;

    void queue(int samples_to_start, const PatternEvent &evt);

    void set_song_bpm(float bpm);

    bool is_queue_empty() const;

    bool playing() const;

private:

    void process_queue();

private:

    boost::shared_ptr< SimpleAudioWave > sample_;
    int play_pos_;
    unsigned samples_remaining_; // for gate mode
    bool playing_;
    PadInfo pad_info_;
    float song_bpm_;

    std::vector< std::pair< int, PatternEvent > > queue_;
};
