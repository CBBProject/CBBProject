#pragma once

#include <iostream>
#include <string>

#pragma GCC visibility push(default)
#include <Images/ImageIO.H>
#pragma GCC visibility pop

#include <Images/RGBPixel.H>
#include <png.h>

#include <Types.H>

namespace Images {

#pragma GCC visibility push(default)
    extern "C" CBB_WINDOWS_EXPORT const char identity[]; // Cannot be made static !!!
#pragma GCC visibility pop

    namespace PNG {

        // This class is a concrete implementation of an ImageIO
        // which is able to read and write raw PNG images.

        typedef enum { PBM_ASCII, PGM_ASCII, PPM_ASCII, PBM_RAW, PGM_RAW, PPM_RAW } FormatTypes;

#pragma GCC visibility push(default)

        class CBB_WINDOWS_EXPORT IO: public Image2DIO {
        public:
          
            using Image2DIO::Id;

            //  This function must return true if this ImageIO knows how to read/write
            //  images of the effective type of its argument.

            bool known(const Image&) throw();

            //  This function must return true if the first bytes (int parameter)
            //  of the image file contained in (char* parameter) describe the appropriate
            //  file format which this ImageIO is able to read.

            bool known(const char*,const unsigned) const throw();

            //  This function must parse the header of the file and collect all the
            //  information needed to read the remaining of the file (image size, bpp,...).
            //  It will called only if known_header has returned true.

            void identify(std::istream&);

            //  This function will be called only after the function identify().
            //  It will return the natural Pixel type requested by the file.

            const Id& pixel_id() const throw();

            //  This function will be called only if the function known_header() has returned true.
            //  It has to read the image from the input stream.
            //  It is assumed that the Pixel type of the image is correct.

            void read(std::istream&,Image&) const;

            //  This function will be called only if known_image() has returned true.
            //  It has to write the image.

            void write(std::ostream&,const Image&) const;

            //  This function is used to create an image. It will be called iff the image
            //  is read through a pointer.

            Image* create() const;

            //  Return a new object for this IO.

            IO* clone() const { return new IO; }

            //  Return the name of the file format.

            const std::string& identity() const throw() { return id; }

        private:

            IO() { }
            IO(const Internal::Register& reg): Image2DIO(reg) { }

            template <typename Pixel>
            const char* data(const Image& image) const {
                typedef BaseImage<2,Pixel> RealImage;
                return reinterpret_cast<const char*>(static_cast<const RealImage&>(image).data());
            }

            png_uint_32 width;
            png_uint_32 height;
            int         depth;
            int         type;

            //  libpng structures and callabcks.

            png_structp png_ptr;
            png_infop   info;

            static void png_read_func(png_structp,png_bytep,png_size_t);
            static void png_write_func(png_structp,png_bytep,png_size_t);
            static void png_flush_func(png_structp);

            static const IO          prototype;
            static const std::string id;
        };
    }
}
