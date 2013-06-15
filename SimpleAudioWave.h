/*
 *  SimpleAudioWave.h
 *
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
