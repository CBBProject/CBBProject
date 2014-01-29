#pragma once

#include <stdint.h>

#include <iostream>
#include <map>
#include <utility>
#include <string>

#include <Utils/Cpu.H>
#include <Images/RGBPixel.H>
#include <Images/PixelIO.H>

#include <Utils/InfoTag.H>
#include <Images/Image.H>
#include <Images/Iterators.H>
#include <Images/Utils.H>
#include <Images/ImageIO.H>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/device/array.hpp>

namespace Images {

#pragma GCC visibility push(default)
    extern "C" const char identity[]; // Cannot be made static !!!
#pragma GCC visibility pop

    namespace Inrimage5 {
        namespace Tools {
            template <typename T> struct Name { };
        }
    }
}
            
#include <Inrimage5Tools.H>

NAME_SPECIALIZATION(int8_t, "int(8)");
NAME_SPECIALIZATION(int16_t,"int(16)");
NAME_SPECIALIZATION(int32_t,"int(32)");
NAME_SPECIALIZATION(int64_t,"int(64)");

NAME_SPECIALIZATION(uint8_t, "unsigned(8)");
NAME_SPECIALIZATION(uint16_t,"unsigned(16)");
NAME_SPECIALIZATION(uint32_t,"unsigned(32)");
NAME_SPECIALIZATION(uint64_t,"unsigned(64)");

NAME_SPECIALIZATION(float, "ieee(single)");
NAME_SPECIALIZATION(double,"ieee(double)");

NAME_SPECIALIZATION(Images::Pixels::RGB<uint8_t>,"RGB(unsigned(8))");
NAME_SPECIALIZATION(Images::Pixels::RGB<float>,  "RGB(ieee(single))");
NAME_SPECIALIZATION(Images::Pixels::RGB<double>, "RGB(ieee(double))");

#undef NAME_SPECIALIZATION

namespace Images {
    namespace Inrimage5 {

//#pragma GCC visibility push(hidden)

        //  Description of the various image types this plugins knows.

        class IODesc {

            typedef boost::iostreams::filtering_streambuf<boost::iostreams::input>  instream;
            typedef boost::iostreams::filtering_streambuf<boost::iostreams::output> outstream;

        private:

            typedef Image* (*CreateFunc)();
            typedef void   (*ReadFunc)(instream&,Image&,const bool);
            typedef void   (*WriteFunc)(outstream&,const Image&);

            IODesc(const char* str,const CreateFunc c,const ReadFunc r,const WriteFunc w): id(str),creator(c),reader(r),writer(w) { }

            template <unsigned Dim,typename Pixel>
            static Image* CreateImage() { return new BaseImage<Dim,Pixel>(); }

            template <unsigned Dim,typename Pixel>
            static void ReadImage(instream& is,Image& image,const bool native) {

                using namespace Images::Pixels;
                typedef BaseImage<Dim,Pixel> RealImage;
                RealImage& im = static_cast<RealImage&>(image);
                try {
                    const bool compact = false; // Remove if one day we want to do memory mapping (which is incompatible with compression !!!).
                    if (compact) {
                        const unsigned dataSize = im.size()*sizeof(Pixel);
                        char* buffer = reinterpret_cast<char*>(im.data());
                        is.sgetn(buffer,dataSize);
                    } else {
                        for (typename RealImage::template iterator<pixel> i=im.begin();i!=im.end();++i) {
                            char* buffer = reinterpret_cast<char*>(&(*i));
                            is.sgetn(buffer,sizeof(Pixel));
                            BufferToPixel(*i,native);
                        }
                    }
                } catch(...) {
                    throw BadData(Images::identity);
                }
            }

            template <unsigned Dim,typename Pixel>
            static void WriteImage(outstream& out,const Image& image) {

                using namespace Images::Pixels;

                typedef BaseImage<Dim,Pixel> RealImage;
                const RealImage& im = static_cast<const RealImage&>(image);
                const bool compact = false; // Remove if one day we want to do memory mapping.

                if (compact)
                    out.sputn(const_cast<char*>(reinterpret_cast<const char*>(im.data())),im.size()*sizeof(Pixel));
                else
                    for (typename RealImage::template const_iterator<pixel> i=im.begin();i!=im.end();++i)
                        out.sputn(const_cast<char*>(PixelToBuffer(*i)),sizeof(Pixel));
            }

            typedef Types::info_tag DataTag;

        public:

            typedef std::map<DataTag,const IODesc*> Registery;

            //  The main IO functions.

            const char* name() const                                       { return id;                      }
            Image* create() const                                          { return creator();               }
            void   read(instream& is,Image& image,const bool native) const { return reader(is,image,native); }
            void   write(outstream& os,const Image& image)           const { writer(os,image);               }

            //  Adding an IO.

            template <unsigned Dim,typename Pixel>
            static void add(const char* str) {
                const IODesc* desc = new IODesc(str,&CreateImage<Dim,Pixel>,&ReadImage<Dim,Pixel>,&WriteImage<Dim,Pixel>);
                if (!registery(Dim).insert(Registery::value_type(DataTag(typeid(Pixel)),desc)).second)
                    throw AlreadyKnownTag(str,Images::identity);
            }

            //  Finding the IO corresponding to a given header.

            static const Registery::const_iterator find(const unsigned n,const std::string& str) {
                for (Registery::const_iterator i=registery(n).begin();i!=registery(n).end();++i)
                    if (str==i->second->id)
                        return i;
                
                throw UnknownPixelType(str,Images::identity);
            }

            //  Finding the IO corresponding to a given Pixel.

            static const Registery::const_iterator find(const unsigned n,const std::type_info& ptype) {
                return registery(n).find(DataTag(ptype));
            }

            static const bool find(const unsigned n,const std::type_info& ptype,Registery::const_iterator& ref) {
                const Registery::const_iterator& it = registery(n).find(DataTag(ptype));
                const bool found = (it!=registery(n).end());
                if (found)
                    ref = it;
                return found;
            }

        private:

            const char*      id;       // Name associated to the pixel type.
            const CreateFunc creator;  // Creates the proper image.
            const ReadFunc   reader;   // Read the image.
            const WriteFunc  writer;   // Write the image.
            
            static Registery& registery(const unsigned n) {
                const unsigned max_dim = 6;
                static Registery reg[max_dim];

                return reg[n-1];
            }
        };

        //  A tool to add the IOs for all the allowed dimensions.

        template <unsigned N,typename Pixel>
        struct IOAdder {
            static void add() {
                IODesc::add<N,Pixel>(Tools::Name<Pixel>::string);
                IOAdder<N-1,Pixel>::add();
            }
        };

        template <typename Pixel>
        struct IOAdder<1,Pixel> {
            static void add() {
                IODesc::add<1,Pixel>(Tools::Name<Pixel>::string);
            }
        };

        // An Inrimage header structure.

        class Header {
        public:

            typedef enum { BigEndian = Cpu::BigEndian, LittleEndian = Cpu::LittleEndian, Neutral } Endianness;
            typedef enum { NoCompression, Zlib, Bzip2 } CompressionType;

            // Constructors.

            Header(): compression(Bzip2),props(new Images::Properties()) { }
            Header(const Image& im): sz(im.dimension()),type(IODesc::find(im.dimension(),im.pixel_id())),
                endian(static_cast<Endianness>(Cpu::ENDIANNESS)),compression(Bzip2),
                props(&const_cast<Images::Properties&>(im.properties()))
            {
                for(int i=0;i<sz.dimension();++i)
                    sz[i] = im.size(i);
            }

            const Dimension  dimension() const { return sz.dimension(); }
            const Dimension* size()      const { return sz.size();      }

            bool native() const { return (endian==Neutral) || (endian==static_cast<Endianness>(Cpu::ENDIANNESS)); }

            friend std::istream& operator>>(std::istream&,Header&);
            friend std::ostream& operator<<(std::ostream&,const Header&);
            friend class IO;

                  Images::Properties* properties()       { return props; }
            const Images::Properties* properties() const { return props; }

        private:

            Utils::ImageSize<>                sz;          // Dimension and size of the image.
            IODesc::Registery::const_iterator type;        // Data type.
            Endianness                        endian;      // Data endianness.
            CompressionType                   compression; // Compression type used to encode the image data.
            Images::Properties*               props;       // Eventual properties attached to the image.

            static const unsigned header_size = 256;

            static const std::string MagicTag;
            static const std::string SIZE;
            static const std::string TYPE;
            static const std::string CPU;
            static const std::string COMPRESS;
            static const std::string HeaderEnd;

            static const std::string EndianStrings[];
            static const std::string CompressStrings[];
        };

        std::istream& operator>>(std::istream&,Header&);
        std::ostream& operator<<(std::ostream&,const Header&);

//#pragma GCC visibility pop

        //  This class is a concrete implementation of an ImageIO which is able to read/write inrimages.

        class IO: public ImageIO {
        private:

            static const unsigned max_dim = 6;

        public:

#pragma GCC visibility push(default)

            using ImageIO::Id;

            virtual ~IO() { }

            bool known(const char* buffer,const unsigned buffer_size) const throw() {
                if (buffer_size<Header::MagicTag.size())
                    return false;
                return (strncmp(buffer,Header::MagicTag.c_str(),Header::MagicTag.size()) == 0);
            }

            bool known(const Image& im) throw() { return IODesc::find(im.dimension(),im.pixel_id(),desc); }

            Suffixes& known_suffixes() const throw() { return suffixes; }

            //  Read and parse the header of the file. At this point the header has already
            //  been identified as known.

            void identify(std::istream& is) {
                is >> header;
                desc = header.type;
            }

            const Id& pixel_id()  const throw() { return desc->first.info;   }
            Dimension dimension() const throw() { return header.dimension(); }

            Image* create() const { return desc->second->create(); }

            void read(std::istream& is,Image& image) const {
                image.resize(header.size());
                ImageIO::SetProperties(image,const_cast<Image::Properties*>(header.properties()));
                boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
                switch (header.compression) {
                    case Header::Bzip2:
                        in.push(boost::iostreams::bzip2_decompressor());
                        break;
                    case Header::Zlib:
                        in.push(boost::iostreams::zlib_decompressor());
                        break;
                }
                in.push(is);
                desc->second->read(in,image,header.native());
            }

            void write(std::ostream& os,const Image& image) const {
                os << Header(image);

                boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
                switch (header.compression) {
                    case Header::Bzip2:
                        out.push(boost::iostreams::bzip2_compressor());
                        break;
                    case Header::Zlib:
                        out.push(boost::iostreams::zlib_compressor());
                        break;
                }
                out.push(os);

                desc->second->write(out,image);
            }

            //  Return a new object for this IO.

            IO* clone() const { return new IO; }

            //  Return the name of the file format.

            const std::string& identity() const throw() { static const std::string idd(Images::identity); return idd; }
        
#pragma GCC visibility pop

            template <typename Pixel>
            static void AddIO() throw() {
                IOAdder<5,Pixel>::add();
            }

            template <unsigned DIM,typename Pixel>
            static void AddIO() throw() {
                IODesc::add<DIM,Pixel>(Tools::Name<Pixel>::string);
            }

        private:

            Header                            header;
            IODesc::Registery::const_iterator desc; 

            IO() { }

            //  Currently known pixel types for Inrimage.

            IO(const Internal::Register& reg): ImageIO(reg) {
                IO::AddIO<Images::Pixels::RGB<uint8_t> >();
                IO::AddIO<Images::Pixels::RGB<float> >();
                IO::AddIO<int8_t>();
                IO::AddIO<int16_t>();
                IO::AddIO<int32_t>();
                IO::AddIO<int64_t>(); //ooops
                IO::AddIO<uint8_t>();
                IO::AddIO<uint16_t>();
                IO::AddIO<uint32_t>();
                IO::AddIO<uint64_t>();
                IO::AddIO<float>();
                IO::AddIO<double>();
            }

            static const IO          prototype;
            static const std::string id;
            static Suffixes          suffixes;
        };

        namespace Tools {

            template <unsigned DIM,typename TYPE,unsigned VECTDIM,typename VECTTYPE>
            void AddFormat(const char* str) {
                std::ostringstream ost;
                ost << str << '<' << VECTDIM << ',' << Images::Inrimage5::Tools::Name<VECTTYPE>::string << '>';
                static const std::string tag = ost.str();
                Images::Inrimage5::IODesc::add<DIM,TYPE>(tag.c_str());
            }

            template <unsigned DIM,typename TYPE>
            void AddFormat(const char* str) {
                Images::Inrimage5::IODesc::add<DIM,TYPE>(str);
            }
        }
    }
}
