#pragma once

#include <stdexcept>
#include <format>


inline void char_utf32_to_utf8(char32_t utf32, const char* buffer)
{
    char* end = const_cast<char*>(buffer);
    if(utf32 < 0x7F) *(end++) = static_cast<unsigned>(utf32);
    else if(utf32 < 0x7FF) {
        *(end++) = 0b1100'0000 + static_cast<unsigned>(utf32 >> 6);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else if(utf32 < 0x10000){
        *(end++) = 0b1110'0000 + static_cast<unsigned>(utf32 >> 12);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else if(utf32 < 0x110000) {
        *(end++) = 0b1111'0000 + static_cast<unsigned>(utf32 >> 18);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 12) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else {
        throw std::runtime_error(std::format( "invalid utf-32 character: {:#x}", static_cast<unsigned>(utf32) ));
    }

    *end = '\0';
}