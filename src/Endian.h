#pragma once

namespace Endian
{
    unsigned big_endian_to_native_32(unsigned big);
    unsigned short big_endian_to_native_16(unsigned short big);

    unsigned data_32bit_to_native_endian(const char *data);
    unsigned short data_16bit_to_native_endian(const char *data);
};

