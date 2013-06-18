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

#include <vector>
#include <memory>

#include "AudioException.h"

class SimpleAudioWave
{
public:

    typedef std::shared_ptr< SimpleAudioWave > ptr_t;

public:

    SimpleAudioWave( unsigned int channels, unsigned sample_rate );

    explicit SimpleAudioWave( const std::string& filename );

    static ptr_t clone( SimpleAudioWave & src );

    bool save_to_wav( const std::string& filename ) const;

    void append_frame( const std::vector< float > & samples );

    void append( const SimpleAudioWave& other );

    void append_samples( unsigned int channel, const std::vector< float >& samples, unsigned offset, unsigned size );
    void append_samples( unsigned int channel, const std::vector< float >& samples );

    const std::vector< float >& get_samples( unsigned int channel ) const;
    std::vector< float >& get_samples( unsigned int channel );

    unsigned int min_size() const;

    unsigned int max_size() const;

    void mix_to_mono();
    void to_stereo( float balance_left, float balance_right );

    void add( const SimpleAudioWave& other, unsigned dst_offset, unsigned src_offset, unsigned size, float gain = 1.0f );
    void add( const SimpleAudioWave& other, float gain = 1.0f ); // add whole wave

    unsigned int channel_size( unsigned int channel ) const;

    void scale_channel( unsigned int channel, float factor );

    void scale( float factor );

    float max_level( unsigned int channel ) const;

    float max_level() const;

	void normalize();

    bool is_uniform_size() const;

	unsigned int channels() const;

	void resize( unsigned int newSize, float fillWith = 0.0f );

	unsigned sample_rate() const;

    float length() const; // in seconds

private:

    std::vector< std::vector< float > > data_;
	unsigned sample_rate_;

};
