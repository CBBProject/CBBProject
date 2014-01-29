#pragma once

#include <Utils/Cpu.H>

namespace Cpu {

    //  This function is separated from Cpu.H so that it
    //  can be included after specializations of void ChangeEndianness(T&)
    //  have been made. This is an annoying thing in the C++ standard:
    //  the method ChangeEndianness used in this function must be seen
    //  before the definition (or is it the declaration?) of the function.

    template <typename T>
    inline void ChangeEndianness(T* begin,const T* end) {
        for (T* ptr=begin;ptr<end;++ptr)
            ChangeEndianness(*ptr);
    }
}

