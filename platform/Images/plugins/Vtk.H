#pragma once

#include <stdint.h>

#include <iostream>
#include <map>
#include <utility>
#include <string>

#include <Utils/Cpu.H>
#include <Utils/InfoTag.H>
#include <Images/Image.H>
#include <Images/Iterators.H>
#include <Images/Utils.H>
#include <Images/ImageIO.H>
#include <Images/PixelIO.H>

namespace Images {

#pragma GCC visibility push(default)
    extern "C" CBB_WINDOWS_EXPORT const char identity[]; // Cannot be made static !!!
#pragma GCC visibility pop

    namespace Vtk {

#pragma GCC visibility push(hidden)

        //  Description of the various image types this plugins knows.

        class IODesc {
        private:

            typedef Image* (*CreateFunc)();
            typedef void   (*ReadFunc)(std::istream&,Image&,const bool);
            typedef void   (*WriteFunc)(std::ostream&,const Image&);

            IODesc(const char* str,const CreateFunc c,const ReadFunc r,const WriteFunc w): id(str), creator(c), reader(r), writer(w) { }

            template <unsigned Dim,typename Pixel>
            static Image* CreateImage() { return new BaseImage<Dim,Pixel>(); }

            template <unsigned Dim,typename Pixel>
            static void ReadImage(std::istream& is,Image& image,const bool native) {

                using namespace Images::Pixels;
                typedef BaseImage<Dim,Pixel> RealImage;
                RealImage& im = static_cast<RealImage&>(image);

                try {
                    const bool compact = false; // Remove if one day we want to do memory mapping.
                    is >> io_utils::match('_');
                    unsigned sizetag;
                    is.read(const_cast<char*>(reinterpret_cast<const char*>(&sizetag)),sizeof(unsigned));
                    if (compact) {
                        const unsigned dataSize = im.size()*sizeof(Pixel);
                        is.read(reinterpret_cast<char*>(im.data()),dataSize);
                    } else {
                        for (typename RealImage::template iterator<pixel> i=im.begin();i!=im.end();++i)
                            ReadPixel(is,*i,native);
                    }
                } catch(...) {
                    throw BadData(is,Images::identity);
                }
            }

            template <unsigned Dim,typename Pixel>
            static void WriteImage(std::ostream& os,const Image& image) {

                using namespace Images::Pixels;

                typedef BaseImage<Dim,Pixel> RealImage;
                const RealImage& im = static_cast<const RealImage&>(image);

                os << '_';
                const unsigned sizetag = sizeof(unsigned)+im.size()*sizeof(Pixel);
                os.write(const_cast<char*>(reinterpret_cast<const char*>(&sizetag)),sizeof(unsigned));

                const bool compact = false; // Remove if one day we want to do memory mapping.
                if (compact)
                    os.write(reinterpret_cast<const char*>(im.data()),im.size()*sizeof(Pixel));
                else {
                    for (typename RealImage::template const_iterator<pixel> i=im.begin();i!=im.end();++i)
                        WritePixel(os,*i);
                }
            }

            typedef Types::info_tag DataTag;

        public:

            typedef std::map<DataTag,const IODesc*> Registery;

            //  The main IO functions.

            const char* name() const                                           { return id;                      }
            Image* create() const                                              { return creator();               }
            void   read(std::istream& is,Image& image,const bool native) const { return reader(is,image,native); }
            void   write(std::ostream& os,const Image& image)            const { writer(os,image);               }

            //  Adding an IO.

            template <unsigned Dim,typename Pixel>
            static void add(const char* str) {
                const IODesc* desc = new IODesc(str,&CreateImage<Dim,Pixel>,&ReadImage<Dim,Pixel>,&WriteImage<Dim,Pixel>);
                if (!registery(Dim).insert(Registery::value_type(DataTag(typeid(Pixel)),desc)).second)
                    throw AlreadyKnownTag(str,Images::identity);
            }

            //  Finding the IO corresponding to a given header.

            static const IODesc::Registery::iterator find(const unsigned dim,const std::string& str) {
                for (Registery::iterator i=registery(dim).begin();i!=registery(dim).end();++i)
                    if (str==i->second->id)
                        return i;
                
                throw "Unknown header to image correspondence.";
            }

            //  Finding the IO corresponding to a given Pixel.

            static const IODesc::Registery::iterator find(const unsigned dim,const std::type_info& ptype) {
                return registery(dim).find(DataTag(ptype));
            }

            static const bool find(const unsigned dim,const std::type_info& ptype,Registery::iterator& ref) {
                const Registery::iterator& it = registery(dim).find(DataTag(ptype));
                const bool found = (it!=registery(dim).end());
                if (found)
                    ref = it;
                return found;
            }

        private:

            const char*      id;       // Name associated to the pixel type.
            const CreateFunc creator;  // Creates the proper image.
            const ReadFunc   reader;   // Read the image.
            const WriteFunc  writer;   // Write the image.

            static Registery& registery(const unsigned dim) {
                const unsigned max_dim = 6;
                static Registery reg[max_dim];
                return reg[dim-1];
            }
        };

        //  A tool to add the IOs for all the allowed dimensions.

        template <unsigned N,typename Pixel>
        struct IOAdder {
            static void add(const char* name) {
                IODesc::add<N,Pixel>(name);
                IOAdder<N-1,Pixel>::add(name);
            }
        };

        template <typename Pixel>
        struct IOAdder<1,Pixel> {
            static void add(const char* name) {
                IODesc::add<1,Pixel>(name);
            }
        };

        // An Inrimage header structure.

        class Header {
        public:

            typedef enum { BigEndian = Cpu::BigEndian, LittleEndian = Cpu::LittleEndian } Endianness;

            // Constructors

            Header() { }
            Header(std::istream&);
            Header(const Image& im):
                sz(im.dimension()),type(IODesc::find(im.dimension(),im.pixel_id())),endian(static_cast<Endianness>(Cpu::ENDIANNESS)),
                name(im.has_property("name") ? im.properties().find("name") : "value")
            {
                for (int i=0;i<sz.dimension();++i)
                    sz[i] = im.size(i);
            }

            const Dimension  dimension() const { return sz.dimension(); }
            const Dimension* size()      const { return sz.size();      }

            bool native() const { return (endian==static_cast<Endianness>(Cpu::ENDIANNESS)); }

            friend std::istream& operator>>(std::istream&,Header&);
            friend std::ostream& operator<<(std::ostream&,const Header&);
            friend class IO;

            Utils::ImageSize<>          sz;      // Dimension and size of the image.
        private:

            IODesc::Registery::iterator type;    // Data type.
            Endianness                  endian;  // Data endianness.
            const std::string           name;    // Data name.

            static const std::string MagicTag;
            static const std::string TYPE;
            static const std::string Version;
            static const std::string CPU;
            //static const std::string COMPRESS;
            static const std::string Origin;
            static const std::string Spacing;
            static const std::string HeaderEnd;
            static const std::string ImageData;
            static const std::string ImageDataEnd;
            static const std::string Piece;
            static const std::string PieceEnd;
            static const std::string PointData;
            static const std::string PointDataEnd;
            static const std::string DataArray;
            static const std::string DataArrayType;
            static const std::string DataArrayFmt;
            static const std::string AppendedData;
            static const std::string AppendedDataEnd;

            static const char* EndianString[];
        };

        std::istream& operator>>(std::istream&,Header&);
        std::ostream& operator<<(std::ostream&,const Header&);

#pragma GCC visibility pop

        //  This class is a concrete implementation of an ImageIO which is able to read/write inrimages.

        class CBB_WINDOWS_EXPORT IO: public ImageIO {
        private:

            static const unsigned max_dim = 6;

        public:

            using ImageIO::Id;

#pragma GCC visibility push(default)

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
                try {
                    is >> header;
                } catch(...) {
                    throw BadHeader(is,identity());
                }
                desc = header.type;
            }

            const Id& pixel_id()  const throw() { return desc->first.info;   }
            Dimension dimension() const throw() { return header.dimension(); }

            Image* create() const { return desc->second->create(); }

            void read(std::istream& is,Image& image) const {
                using namespace io_utils;
                image.resize(header.size());
                is >> match(Header::AppendedData);
                desc->second->read(is,image,header.native());
                is >> match(Header::AppendedDataEnd);
                is >> match(Header::HeaderEnd);
            }

            void write(std::ostream& os,const Image& image) const {
                os << Header(image)
                   << "   " << Header::AppendedData << std::endl;
                   
                desc->second->write(os,image);

                os << std::endl
                   << "\t" << Header::AppendedDataEnd << std::endl
                   << Header::HeaderEnd;
            }

            //  Return a new object for this IO.

            IO* clone() const { return new IO; }

            //  Return the name of the file format.

            const std::string& identity() const throw() { return id; }

#pragma GCC visibility pop
        
            template <typename Pixel>
            static void AddIO(const char* name) throw() {
                IOAdder<5,Pixel>::add(name);
            }

            template <unsigned DIM,typename Pixel>
            static void AddIO(const char* name) throw() {
                IODesc::add<DIM,Pixel>(name);
            }

        private:

            Header                      header;
            IODesc::Registery::iterator desc; 

            //  Currently known pixel types for Inrimage.

            IO() { }
            IO(const Internal::Register& reg): ImageIO(reg) {
                IO::AddIO<int8_t>("Int8");
                IO::AddIO<int16_t>("Int16");
                IO::AddIO<int32_t>("Int32");
                IO::AddIO<int64_t>("Int64");
                IO::AddIO<uint8_t>("UInt8");
                IO::AddIO<uint16_t>("UInt16");
                IO::AddIO<uint32_t>("UInt32");
                IO::AddIO<uint64_t>("UInt64");
                IO::AddIO<float>("Float32");
                IO::AddIO<double>("Float64");
            }

            static const IO          prototype;
            static const std::string id;

            static Suffixes suffixes;
        };
    }
}
