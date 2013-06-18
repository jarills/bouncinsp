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

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "PadInfo.h"
#include <vector>
#include <queue>
#include <utility>
#include "PatternEvent.h"

class SimpleAudioWave;

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
    char event_velocity_;

    std::vector< std::pair< int, PatternEvent > > queue_;
};
