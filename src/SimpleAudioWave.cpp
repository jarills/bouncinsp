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

#include "SimpleAudioWave.h"

#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <sndfile.h>
#include <cstring>
#include <functional>

using namespace std;

SimpleAudioWave::SimpleAudioWave( unsigned int channels, unsigned sample_rate )
	:
sample_rate_(sample_rate)
{
	assert( channels != 0 );
	assert( sample_rate_ != 0);

	for ( unsigned int i = 0; i < channels; ++i )
	{
        data_.push_back( std::vector< float >() );
	}
}

SimpleAudioWave::ptr_t SimpleAudioWave::clone( SimpleAudioWave & src )
{
    ptr_t new_clone = std::make_shared< SimpleAudioWave >( src.channels(), src.sample_rate() );
	new_clone->append( src );
	return new_clone;
}

SimpleAudioWave::SimpleAudioWave( const std::string& filename )
{
	assert( !filename.empty() );

	SF_INFO sf_info;

	SNDFILE* sf = sf_open( filename.c_str(), SFM_READ, &sf_info );

	if ( !sf )
	{
		throw AudioException( std::string("loading of '" + filename + "' failed: " + sf_strerror(sf)).c_str() );
	}

	unsigned channels = sf_info.channels;
	sample_rate_ = sf_info.samplerate;

	assert( channels != 0 );

	if ( channels == 0 )
	{
		throw AudioException( std::string("loading of '" + filename + "' failed: no channels of audio found." ).c_str() );
	}

	for ( unsigned int i = 0; i < channels; ++i )
	{
        data_.push_back( std::vector< float >() );
        data_[i].resize((unsigned)sf_info.frames);
	}

    std::vector< float > buf;
    buf.resize( channels * (unsigned)sf_info.frames );

    if ( sf_info.frames != sf_readf_float( sf, &buf[0], sf_info.frames ) )
    {
        sf_close(sf);
        throw AudioException("could not read enough frames.");
    }

    for ( unsigned int i = 0; i < channels; ++i )
    {
        for ( unsigned frame = 0; frame < sf_info.frames; ++frame )
        {
            data_[ i ][frame] = buf[ (frame*channels) + i ];
        }
    }

	sf_close( sf );

    cout << "Finished reading '" + filename + "': " << channels << " channels, " << max_size() << " samples at " << sample_rate_ << "hz." << endl;
}

unsigned SimpleAudioWave::sample_rate() const
{
	return sample_rate_;
}

float SimpleAudioWave::length() const
{
    return min_size() / (float) sample_rate();
}

bool SimpleAudioWave::save_to_wav( const std::string& filename ) const
{
    assert( min_size() == max_size() );

    if ( min_size() != max_size() )
	{
        cout << "Cannot save to wav, min_size != max_size" << endl;
		return false;
	}

	if ( sample_rate_ == 0 )
	{
		cout << "Cannot save to wav, sample rate is 0." << endl;
		return false;
	}

    const float sec = (float)min_size() / (float)sample_rate_;

    cout << "SimpleAudioWave::saveToWav: Going to write " << min_size() << " samples @ " << sample_rate_ << "hz (" << sec << " sec) to " << filename << "...";

	SF_INFO sf_info;

	memset( &sf_info, 0, sizeof( sf_info ) ) ;

	sf_info.samplerate = sample_rate_;
    sf_info.frames = min_size();
	sf_info.channels = channels();
	sf_info.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);

	SNDFILE* sf = sf_open( filename.c_str(), SFM_WRITE, &sf_info );

	if ( !sf )
	{
		cout << "writing of '" << filename << "' failed: " << sf_strerror(sf) << endl;
		return false;
	}

    std::vector< float > buf;
    buf.resize( channels() * min_size() );

    for ( unsigned int i = 0; i < min_size(); ++i )
	{
		for ( unsigned c = 0; c < channels(); ++c )
		{
            buf[ (i * channels()) + c ] = get_samples( c )[ i ];
		}
	}

    if ( sf_writef_float( sf, &buf[0], min_size() ) != min_size() )
    {
        sf_close( sf );
        cout << "error writing bytes" << endl;
        return false;
    }

	sf_close( sf );

	cout << "done." << endl;

	return true;
}

void SimpleAudioWave::append_frame(const vector<float> &samples)
{
    for ( unsigned i = 0; i < samples.size() && i < data_.size(); ++i )
    {
        data_[i].push_back(samples[i]);
    }
}

void SimpleAudioWave::append_samples( unsigned int channel, const std::vector< float >& samples, unsigned offset, unsigned size )
{
    if ( channel > channels() )
    {
        throw AudioException("append_samples: channel > channels()");
    }

    if ( offset > samples.size() || size > samples.size() || offset + size > samples.size() )
    {
        throw AudioException("append_samples: offset + size > sample size");
    }

    data_[ channel ].insert( data_[ channel ].end(), samples.begin() + offset, samples.begin() + offset + size );
}

void SimpleAudioWave::append_samples( unsigned int channel, const std::vector< float >& samples )
{
    append_samples( channel, samples, 0, samples.size() );
}

void SimpleAudioWave::append( const SimpleAudioWave& other )
{
	for ( unsigned int i = 0; i < other.channels() && i < channels(); ++i )
	{
        append_samples( i, other.get_samples( i ) );
	}
}

void SimpleAudioWave::add( const SimpleAudioWave& other, float gain )
{
    add( other, 0, 0, other.min_size(), gain );
}

void SimpleAudioWave::add( const SimpleAudioWave& other, unsigned dst_offset, unsigned src_offset, unsigned size, float gain )
{
	for ( unsigned int i = 0; i < other.channels() && i < channels(); ++i )
	{
        std::vector< float >& this_data = data_[i];
        const std::vector< float >& other_data = other.get_samples(i);

        const unsigned dst_start = std::min<unsigned>(dst_offset, this_data.size());
        const unsigned src_start = std::min<unsigned>(src_offset, other_data.size());

        assert( dst_start <= this_data.size());
        assert( src_start <= other_data.size());

        const unsigned dst_max = this_data.size() < dst_start ? 0 : (this_data.size() - dst_start);
        const unsigned src_max = other_data.size() < src_start ? 0 : (other_data.size() - src_start);

        const unsigned max_sample = std::min( dst_max, src_max );
        const unsigned real_size = std::min( max_sample, size );

        assert( src_start + real_size <= other_data.size() );
        assert( dst_start + real_size <= this_data.size() );

        for ( unsigned int s = 0; s < real_size; ++s )
		{
            const unsigned src_off = src_start + s;
            this_data[dst_start + s] += gain * other_data[src_off];
		}
	}
}

const std::vector< float >& SimpleAudioWave::get_samples( unsigned int channel ) const
{
	if ( channel > channels() )
	{
        throw AudioException("get_samples: channel > channels()");
	}

    return data_[ channel ];
}

std::vector< float >& SimpleAudioWave::get_samples( unsigned int channel )
{
	if ( channel > channels() )
	{
        throw AudioException("get_samples: channel > channels()");
	}

    return data_[ channel ];
}

unsigned int SimpleAudioWave::min_size() const
{
	unsigned int temp = 0;

    for ( unsigned int i = 0; i < data_.size(); ++i )
	{
		if ( i == 0 )
		{
            temp = data_[ i ].size();
		}

        temp = std::min< unsigned int >( temp, data_[ i ].size() );
	}

	return temp;
}

unsigned int SimpleAudioWave::max_size() const
{
	unsigned int temp = 0;

    for ( unsigned int i = 0; i < data_.size(); ++i )
	{
        temp = std::max< unsigned int >( temp, data_[ i ].size() );
	}

	return temp;
}

void SimpleAudioWave::to_stereo( float balance_left, float balance_right )
{
	if ( channels() != 1 )
	{
		return;
	}

    std::vector< float > left( max_size(), 0.0f );
    std::vector< float > right( max_size(), 0.0f );

    for ( unsigned int i = 0; i < max_size(); ++i )
	{
        left[i] = data_[0][i] * balance_left;
        right[i] = data_[0][i] * balance_right;
	}

    data_.clear();
    data_.push_back( left );
    data_.push_back( right );
}

void SimpleAudioWave::mix_to_mono()
{
	// no data, no mono
    if ( data_.size() == 0 )
	{
		return;
	}

    std::vector< float > mono( max_size(), 0.0f );

	// merge all into mono
    for ( unsigned int i = 0; i < data_.size(); ++i )
	{
        for ( unsigned int s = 0; s < data_[ i ].size(); ++s )
		{
			if ( s >= mono.size() ) // can't be actually.
			{
				assert( s < mono.size() );
				continue;
			}

            mono[ s ] += data_[ i ][ s ] / (float)data_.size();
		}
	}

    data_.clear();
    data_.push_back( mono );
}

unsigned int SimpleAudioWave::channel_size( unsigned int channel ) const
{
	if ( channel > channels() )
	{
		throw AudioException("channelSize: channel > channels()");
	}

    return data_[ channel ].size();
}

void SimpleAudioWave::scale_channel( unsigned int channel, float factor )
{
	if ( channel > channels() )
	{
		throw AudioException("scaleChannel: channel > channels()");
	}

    for ( unsigned i = 0; i < channel_size( channel ); ++i )
	{
        data_[ channel ][ i ] *= factor;
	}
}

void SimpleAudioWave::scale( float factor )
{
	for ( unsigned int i = 0; i < channels(); ++i )
	{
        scale_channel( i, factor );
	}
}

float SimpleAudioWave::max_level( unsigned int channel ) const
{
	if ( channel > channels() )
	{
		throw AudioException("maxLevel: channel > channels()");
	}

    std::vector< float >::const_iterator i_max = std::max_element( data_[ channel ].begin() , data_[ channel ].end() );
    std::vector< float >::const_iterator i_min = std::min_element( data_[ channel ].begin() , data_[ channel ].end() );

    float temp = 0.0f;

    if ( i_max != data_[ channel ].end() )
	{
        temp = std::max( temp, fabs(*i_max) );
	}

    if ( i_min != data_[ channel ].end() )
    {
        temp = std::max( temp, fabs(*i_min) );
    }

    return temp;
}

float SimpleAudioWave::max_level() const
{
	float temp = 0.0f;

	for ( unsigned int i = 0; i < channels(); ++i )
	{
        temp = std::max( temp, max_level( i ) );
	}

	return temp;
}

void SimpleAudioWave::normalize()
{
    if ( channels() == 0 || min_size() == 0 ) return;

    const float maxlevel = max_level();

    if ( maxlevel == 0.0f ) return;

    scale( 1.0f / maxlevel );
}

bool SimpleAudioWave::is_uniform_size() const
{
    return min_size() == max_size();
}

unsigned int SimpleAudioWave::channels() const
{
    return data_.size();
}

void SimpleAudioWave::resize( unsigned newSize, float fillWith )
{
	for ( unsigned int i = 0; i < channels(); ++i )
	{
        data_[ i ].resize( newSize, fillWith );
	}
}
