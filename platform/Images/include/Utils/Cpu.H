#pragma once

#include <Utils/config.h>
#ifndef WORDS_BIGENDIAN
#define WORDS_LITTLEENDIAN
#endif

#include <algorithm>

namespace Cpu {

	typedef enum { BigEndian, LittleEndian } Endianness;

    template <unsigned SIZE>
    struct SwapEndianness {
        //  Empty to cause an error.
    };

    template <>
    struct SwapEndianness<1> {
        static void swap(char*) { }
    };

    template <>
    struct SwapEndianness<2> {
        static void swap(char* mem) { std::swap(mem[0],mem[1]); }
    };

    template <>
    struct SwapEndianness<4> {
        static void swap(char* mem) {
            std::swap(mem[0],mem[3]);
            std::swap(mem[1],mem[2]);
        }
    };

    template <>
    struct SwapEndianness<8> {
        static void swap(char* mem) {
            SwapEndianness<4>::swap(mem);
            SwapEndianness<4>::swap(mem+4);
        }
    };

    template <typename T>
    inline void ChangeEndianness(T& t) {
        SwapEndianness<sizeof(T)>::swap(reinterpret_cast<char*>(&t));
    }

#   if (defined(WORDS_BIGENDIAN) && defined(WORDS_LITTLEENDIAN)) || \
        !(defined(WORDS_BIGENDIAN) || defined(WORDS_LITTLEENDIAN))
#   error exactly one of WORDS_BIGENDIAN or WORDS_LITTLEENDIAN must be specified.
#   endif

#   ifdef WORDS_BIGENDIAN
    const Cpu::Endianness ENDIANNESS = Cpu::BigEndian;
#   endif

#   ifdef WORDS_LITTLEENDIAN
    const Cpu::Endianness ENDIANNESS = Cpu::LittleEndian;
#   endif

}

