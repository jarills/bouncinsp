#pragma once

#include <string>

namespace BouncinSP
{
    namespace SP404SX
    {
        const static float PPQ = 96.0f;
        const static float SampleRate = 44100.0f;
        const static unsigned PadA1MidiOffset = 47;

        struct Banks
        {
            enum
            {
                A = 0,
                B = A + 12,
                C = B + 12,
                D = C + 12,
                E = D + 12,
                F = E + 12,
                G = F + 12,
                H = G + 12,
                I = H + 12,
                J = I + 12
            };

        };

    };

    std::string idx_to_pad_name( unsigned idx );
};

