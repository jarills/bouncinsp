#pragma once

class PatternEvent
{
public:
    PatternEvent( const char * block, float absolute_position_in_quarters );

    unsigned char offset_to_next() const;
    float quarters_offset_to_next() const;
    unsigned char note_number() const;
    unsigned char velocity() const;
    float quarters_held() const;
    float quarters_absolute_position() const;

    const char *data() const;

    void print() const;

private:

    char data_[8]; // each event has 8 bytes
    float quarters_absolute_position_;
};

