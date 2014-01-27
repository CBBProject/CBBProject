#ifndef IMAGE_RAW_PGM_H
#define IMAGE_RAW_PGM_H

#include <iostream>
#include <string>

#include <Images/Image.H>
#include <Images/ImageIO.H>

namespace Images {

#pragma GCC visibility push(default)
    extern "C" const char identity[]; // Cannot be made static !!!
#pragma GCC visibility pop

    namespace RawPgm {

        // This class is a concrete implementation of an ImageIO
        // which is able to read and write raw PGM images.
        // It is widely documented so that anyone can copy the
        // code and write its own Image reader/writer.

        class IO: public Image2DIO {
        public:

#pragma GCC visibility push(default)

            using Image2DIO::Id;

            //  This function must return true if this ImageIO knows how to read/write
            //  images of the effective type of its argument.

            bool known(const Image&) throw();

            //  This function must return true if the first bytes (int parameter)
            //  of the image file contained in (char* parameter) describe the appropriate
            //  file format which this ImageIO is able to read.

            bool known(const char*,const unsigned) const throw();

            //  This function must return the list of suffixes recognized by this plugin.
            //  This is used mostly at write time to select the file format from the suffix
            //  of the file name.

            Suffixes& known_suffixes() const throw() { return suffixes; }

            //  This function must parse the header of the file and collect all the
            //  information needed to read the remaining of the file (image size, bpp,...).
            //  It will called only if known_header has returned true.

            void identify(std::istream&);

            //  This function will be called only after the function identify().
            //  It will return the natural Pixel type requested by the file.

            const Id& pixel_id() const throw() { return typeid(unsigned char); }

            //  This function will be called only if the function known_header() has returned true.
            //  It has to read the image from the input stream.

            void read(std::istream&,Image&) const;

            //  This function will be called only if known_image() has returned true.
            //  It has to write the image.

            void write(std::ostream&,const Image&) const;

            //  This function is used to create an image. It will be called iff the image
            //  is read through a pointer.

            Image* create() const { return new Image2D<unsigned char>; }

            //  Return a new object for this IO.

            IO* clone() const { return new IO; }

            //  Return the name of the file format.

            const std::string& identity() const throw() { return id; }

#pragma GCC visibility pop
        
        private:

            IO() { }
            IO(const Internal::Register& reg): Image2DIO(reg) { }

            static const char MagicTag[];

            unsigned width;
            unsigned height;

            static const IO          prototype;
            static const std::string id;
            static Suffixes          suffixes;
        };
    }
}

#endif  // !IMAGE_RAW_PGM_H
