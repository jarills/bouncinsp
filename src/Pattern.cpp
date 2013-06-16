#include "Pattern.h"

#include "BouncinSP.h"

Pattern::Pattern( unsigned idx )
    :
      pattern_length_(0),
      running_quarter_(0.0f),
      idx_(idx),
      pattern_name_("Pattern " + BouncinSP::idx_to_pad_name(idx_))
{
}

std::string Pattern::pattern_name() const
{
    return pattern_name_;
}

void Pattern::set_pattern_name(const std::string &str)
{
    pattern_name_ = str;
}

void Pattern::reset()
{
    running_quarter_ = 0.0f;
    events_.clear();
}

unsigned Pattern::index() const
{
    return idx_;
}

bool Pattern::read_pattern( std::istream & in )
{
    reset();

    char block[8];

    bool note_mode = true;
    bool read_footer = false;

    while ( !read_footer && in.read(block, 8) )
    {
        if ( note_mode )
        {
            PatternEvent pe(block, running_quarter_);

            if ( pe.note_number() == 0x8C )
            {
                note_mode = false;
                continue;
            }

            events_.push_back(pe);
            pe.print();
            running_quarter_ += pe.quarters_offset_to_next();
        }
        else
        {
            pattern_length_ = block[1];
            read_footer = true;
            std::cout << "footer found. pattern length: " << (int) pattern_length_ << std::endl;
        }
    }

    return read_footer;
}

std::set< unsigned > Pattern::pads_playing() const
{
    std::set< unsigned > out;

    for ( const auto & e : events_ )
    {
        unsigned pad = e.note_number();
        if ( pad != 0x80 && pad >= BouncinSP::SP404SX::PadA1MidiOffset )
        {
            out.insert(pad - BouncinSP::SP404SX::PadA1MidiOffset);
        }
    }

    return out;
}

unsigned Pattern::pattern_length() const
{
    return pattern_length_;
}

const std::vector<PatternEvent> &Pattern::events() const
{
    return events_;
}
