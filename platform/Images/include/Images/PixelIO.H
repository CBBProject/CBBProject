#pragma once

#include <iostream>
#include <string>

#include <Utils/Cpu.H>

namespace Images {
    namespace Pixels {

        //  These functions are for standard types.
        //  They need to be overloaded properly for specific pixel types.

        template <typename Pixel>
        void BufferToPixel(Pixel& p,const bool native) {
            if (!native)
                Cpu::ChangeEndianness(p);
        }

        template <typename Pixel>
        void ReadPixel(std::istream& is,Pixel& p,const bool native) {
            char* data = reinterpret_cast<char*>(&p);
            is.read(data,sizeof(Pixel));
            if (!native)
                Cpu::ChangeEndianness(p);
        }

        template <typename Pixel>
        const char* PixelToBuffer(const Pixel& p) {
            return reinterpret_cast<const char*>(&p);
        }

        template <typename Pixel>
        void WritePixel(std::ostream& os,const Pixel& p) {
            os.write(reinterpret_cast<const char*>(&p),sizeof(Pixel));
        }
    }
}
