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

#include <string>

namespace BouncinSP
{
    namespace SP404SX
    {
        const static float PPQ = 96.0f;
        const static float SampleRate = 44100.0f;
        const static unsigned PadA1MidiOffset = 47;
        const static unsigned ExternalPadNoteNumber = 46;

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

