#include <string>

#include <PluginDefs.H>

#include <Image.H>
#include <Utils/InfoTag.H>

#include <Pnm.H>
#include <Utils/CpuUtils.H>

namespace Images {

    extern "C" const char identity[] = "pnm";

    namespace Pnm {

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        namespace {

            template <typename IMAGE>
            void ReadAsciiImage(std::istream &is,Image& image) {
                IMAGE& im = static_cast<IMAGE&>(image);
                for (typename IMAGE::template iterator<pixel> i=im.begin();i!=im.end();++i)
                    is >> *i;
            }

            template <typename IMAGE>
            void ReadBinaryImage(std::istream &is,Image& image,const bool reorder=false) {
                typedef typename IMAGE::PixelType Pixel;
                IMAGE& im = static_cast<IMAGE&>(image);
                Pixel* const   data = reinterpret_cast<Pixel*>(im.data());
                const unsigned size = im.size();
                is.read(reinterpret_cast<char*>(data),size*sizeof(Pixel));
                if (reorder && (Cpu::ENDIANNESS!=Cpu::BigEndian))
                    Cpu::ChangeEndianness<Pixel>(data,data+size);
            }
        }

        using namespace io_utils;

        //  This function must return true if the first "buffer_size" bytes
        //  of the image file contained in "buffer" describe the appropriate
        //  file format which this ImageIO is able to read.

        bool IO::known(const char *buffer,const unsigned buffer_size) const throw() {
            if (buffer[0]!='P')
                return false;
            return ((buffer[1]>'0') && (buffer[1]<'7'));
        }

        //  This function must return true if this ImageIO knows how to read/write
        //  images of the effective type of its argument.

        bool IO::known(const Image& image) throw() {
            const Types::info_tag tag(image.pixel_id());
            return (image.dimension()==2) &&
                   ((tag==typeid(bool))                       ||
                    (tag==typeid(unsigned char))              ||
                    (tag==typeid(unsigned short))             ||
                    (tag==typeid(Pixels::RGB<unsigned char>)) ||
                    (tag==typeid(Pixels::RGB<unsigned short>)));
        }

        //  This function read the PNM header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is) {

            unsigned char typechar;

            is >> match("P") >> typechar >> std::ws
               >> skip_comments('#')
               >> width >> height;
            
            const int type = typechar-'1';

            if ((type<PBM_ASCII) || (type>PPM_RAW))
                throw BadHeader(is,identity());

            FmtType = static_cast<FormatTypes>(type);

            if ((FmtType!=PBM_ASCII) || (FmtType!=PBM_RAW))
               is >> depth;

            is >> skip_to("\n");
        }

        const IO::Id& IO::pixel_id() const throw()
        {
            switch (FmtType) {
                case PBM_ASCII:
                case PBM_RAW:
                    return (typeid(bool));
                case PGM_ASCII:
                case PGM_RAW:
                    return ((depth<256) ? typeid(unsigned char) : typeid(unsigned short));
                case PPM_ASCII:
                case PPM_RAW:
                    return ((depth<256) ? typeid(Pixels::RGB<unsigned char>) : typeid(Pixels::RGB<unsigned short>));
            }

            throw UnexpectedError();
        }

        Image* IO::create() const {

            switch (FmtType) {
                case PBM_ASCII:
                case PBM_RAW:
                    return new Image2D<bool>;
                case PGM_ASCII:
                case PGM_RAW:
                    if (depth<256)
                        return new Image2D<unsigned char>;
                    else 
                        return new Image2D<unsigned short>;
                case PPM_ASCII:
                case PPM_RAW:
                    if (depth<256)
                        return new Image2D<Pixels::RGB<unsigned char> >;
                    else 
                        return new Image2D<Pixels::RGB<unsigned short> >;
            }

            throw UnexpectedError();
        }

        //  This function will be called only if the function known() has returned true.
        //  It has to read the image from "is" and build the corresponding Image that is to be returned in
        //  the parameter "image". An Image instance with the correct type is already pointed by "image".

        void IO::read(std::istream &is,Image& image) const
        {
            const Dimension size[2] = { width, height };

            image.resize(size);

            // Read the data.

            switch (FmtType) {
                case PBM_ASCII:
                    ReadAsciiImage<Image2D<bool> >(is,image);
                    return;
                case PGM_ASCII:
                    if (depth<256) {
                        ReadAsciiImage<Image2D<unsigned char> >(is,image);
                    } else {
                        ReadAsciiImage<Image2D<unsigned short> >(is,image);
                    }
                    return;
                case PPM_ASCII:
                    if (depth<256) {
                        ReadAsciiImage<Image2D<Pixels::RGB<unsigned char> > >(is,image);
                    } else {
                        ReadAsciiImage<Image2D<Pixels::RGB<unsigned short> > >(is,image);
                    }
                    return;
                case PBM_RAW: {
                    typedef Image2D<bool> IMAGE;
                    IMAGE& im = static_cast<IMAGE&>(image);
                    for (Coord i =0;i<height;++i)
                        for (Coord j =0;j<width;) {
                            unsigned char data;
                            is >> data;
                            for (unsigned k=0;(k<8) && (j<width);++k,++j) {
                                im(i,j) = (data&128==1) ? true : false;
                                data = data << 1;
                            }
                        }
                    return;
                }
                case PGM_RAW:
                    if (depth<256) {
                        ReadBinaryImage<Image2D<unsigned char> >(is,image);
                    } else {
                        ReadBinaryImage<Image2D<unsigned short> >(is,image,true);
                    }
                    return;
                case PPM_RAW:
                    if (depth<256) {
                        ReadBinaryImage<Image2D<Pixels::RGB<unsigned char> > >(is,image);
                    } else {
                        ReadBinaryImage<Image2D<Pixels::RGB<unsigned short> > >(is,image,true);
                    }
                    return;
            }

            throw UnexpectedError();
        }

        //  This function will be called only if known_type() has returned true. It has to write the image.

        void IO::write(std::ostream &os,const Image& image) const {

            FormatTypes fmt;

            unsigned size = 1;

            const char* dataptr;
            if (image.pixel_id()==typeid(bool)) {
                fmt = PBM_RAW;
                size = 1;
                dataptr = data<bool>(image);
            } else if (image.pixel_id()==typeid(unsigned char))  {
                fmt = PGM_RAW;
                size = 255;
                dataptr = data<unsigned char>(image);
            } else if (image.pixel_id()==typeid(unsigned short)) {
                fmt = PGM_RAW;
                size = 65536;
                dataptr = data<unsigned short>(image);
            } else if (image.pixel_id()==typeid(Pixels::RGB<unsigned char>)) {
                fmt = PPM_RAW;
                size = 255;
                dataptr = data<Pixels::RGB<unsigned char> >(image);
            } else if (image.pixel_id()==typeid(Pixels::RGB<unsigned short>)) {
                fmt = PPM_RAW;
                size = 65536;
                dataptr = data<Pixels::RGB<unsigned short> >(image);
            }
            os << 'P' << static_cast<char>('1'+fmt) << std::endl
               << image.size(0) << " " << image.size(1) << std::endl;
            
            if ((fmt!=PBM_RAW) && (fmt!=PBM_ASCII))
               os << size << std::endl;
            
            switch (fmt) {
                case PBM_ASCII: {
                    const Image2D<bool>& im = static_cast<const Image2D<bool>&>(image);
                    for (Image2D<bool>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << ((*i) ? '1' : '0') << ' ';
                }
                case PGM_ASCII: {
                    if (size==255) {
                        const Image2D<unsigned char>& im = static_cast<const Image2D<unsigned char>&>(image);
                        for (Image2D<unsigned char>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << *i << ' ';
                    } else {
                        const Image2D<unsigned short>& im = static_cast<const Image2D<unsigned short>&>(image);
                        for (Image2D<unsigned short>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << *i << ' ';
                    }
                }
                case PPM_ASCII: { //    TODO
                }
                case PBM_RAW:
                    break;
                case PGM_RAW:
                case PPM_RAW:
                    //  Handle Endianness (TODO).
                    os.write(dataptr,image.size()*image.pixel_size());
                    break;
            }
        }
    }
}
