#pragma once

#include <iostream>
#include <map>
#include <typeinfo>
#include <utility>
#include <string>

#include <Utils/InfoTag.H>
#include <Images/Image.H>
#include <Images/ImageIO.H>
#include <Utils/CpuUtils.H>
#include <Utils/triplet.H>

namespace Images {

#pragma GCC visibility push(default)
    extern "C" const char identity[]; // Cannot be made static !!!
#pragma GCC visibility pop

    namespace Inrimage {

//#pragma GCC visibility push(hidden)

        typedef Types::info_tag             DataTag;
        typedef std::pair<unsigned,DataTag> IOTag;

        //  Rename those enums to be Integral, Unsigned and Float ???

        typedef enum { Signed, Unsigned, Float } Type;
        typedef enum { Fixed, Packed }           Packing;

        class DataType {
        public:

            DataType():endian(Cpu::ENDIANNESS) { }
            DataType(const std::type_info& ptype) {
                const DataType& dt = registery().find(DataTag(ptype))->second;
                *this = dt;
            }
            DataType(const Type t,const unsigned sz):
                type(t),packing(Fixed),bpp(8*sz),endian(Cpu::ENDIANNESS) { }
            DataType(const Type t,const Packing p,const unsigned sz):
                type(t),packing(p),bpp(8*sz),endian(Cpu::ENDIANNESS) { }
        
            friend std::ostream& operator<<(std::ostream&,const DataType&);
            friend std::istream& operator>>(std::istream& is,DataType&);

            DataType& operator=(const DataType& dt) {
                type    = dt.type;
                packing = dt.packing;
                bpp     = dt.bpp;
                endian  = dt.endian;

                return *this;
            }

            bool operator==(const DataType& dt) const {
                return (type==dt.type) && (bpp==dt.bpp) && (packing==dt.packing);
            }

            bool is_float() const { return type==Float; }

            bool native() const { return endian==Cpu::ENDIANNESS; }

            DataTag pixel_id() const {
                for (Registery::const_iterator i=registery().begin();i!=registery().end();++i)
                    if (i->second==*this)
                        return i->first;
                throw "Internal error in Inrimage plugin";
            }

            static bool known(const DataTag& t) {
                return registery().find(t)!=registery().end();
            }

            template <typename Pixel> static void
            AddType(const Type t,const Packing p=Fixed) {
                registery().insert(Registery::value_type(DataTag(typeid(Pixel)),DataType(t,p,sizeof(Pixel))));
            }

        private:

            Type            type;    // Data type.
            Packing         packing; // Data packing. Not meaningful for type==Float.
            unsigned        bpp;     // Data size in bits per pixel.
            Cpu::Endianness endian;  // Data endianness.

            static const char* TypeString[];
            static const char* PackingString[];
            static const char *EndianString[];

            static const char TYPE[];
            static const char PIXSIZE[];
            static const char BITS[];
            static const char SCALE[];
            static const char CPU[];

            typedef std::map<DataTag,DataType> Registery;

            static Registery reg;

        public:
            static Registery& registery() { return reg; }
        };

        // An Inrimage header structure.

        class Header {
        public:

            // Constructors

            Header() { }
            Header(std::istream&);
            Header(const Image& im): dim(im.dimension()),type(im.pixel_id()) {
                for(int i=0;i<dim;++i) size[i] = im.size(i);
                for(int i=dim;i<4;++i) size[i] = 1;
            }
            Header(const Dimension dx,const Dimension dy,const DataType& type): dim(2),type(type) {
                size[0] = dx; size[1] = dy; size[2] = 1; size[3] = 1;
            }
            Header(const Dimension dx,const Dimension dy,const Dimension dz,const DataType& type):
                dim(3),type(type) {
                size[0] = dx; size[1] = dy; size[2] = dz; size[3] = 1;
            }
            Header(const Dimension dx,const Dimension dy,const Dimension dz,const Dimension dt,const DataType& type):
                dim(4),type(type) {
                size[0] = dx; size[1] = dy; size[2] = dz; size[3] = dt;
            }

            DataTag pixel_id() const { return type.pixel_id(); }

            friend std::istream& operator>>(std::istream&,Header&);
            friend std::ostream& operator<<(std::ostream&,const Header&);
            friend class IO;

        private:

            Dimension dim;
            Dimension size[4];

            DataType  type;

            static const unsigned header_size = 256;

            static const char MagicTag[];
            static const char XDIM[];
            static const char YDIM[];
            static const char ZDIM[];
            static const char VDIM[];
            static const char HeaderEnd[];
        };
//#pragma GCC visibility pop

        //  This class is a concrete implementation of an ImageIO which is able to read/write inrimages.

        class IO: public ImageIO {
        public:

            using ImageIO::Id;

#pragma GCC visibility push(default)

            virtual ~IO() { }

            bool known(const char* buffer,const unsigned buffer_size) const throw() {
                if (buffer_size<strlen(Header::MagicTag))
                    return false;
                return (strncmp(buffer,Header::MagicTag,strlen(Header::MagicTag)) == 0);
            }

            bool known(const Image& im) throw() {
                const IOTag tag(im.dimension(),im.pixel_id());
                Registery::iterator it = registery().find(tag);
                const bool known = (it!=registery().end());
                if (known)
                    desc = it->second;
                return known;
            }

            Suffixes& known_suffixes() const throw() { return suffixes; }

            //  Read and parse the header of the file. At this point the header has already
            //  been identified as known.

            void identify(std::istream&);

            const Id& pixel_id()  const throw() { return header.pixel_id().info; }
            Dimension dimension() const throw() { return header.dim; }

            Image* create() const { return (desc.first)(); }

            void read(std::istream& is,Image& image) const {
                image.resize(header.size);
                return (this->*(desc.second))(is,image);
            }

            void write(std::ostream& os,const Image& image) const {
                os << Header(image);
                (desc.third)(os,image);
            }

            //  Return a new object for this IO.

            IO* clone() const { return new IO; }

            //  Return the name of the file format.

            const std::string& identity() const throw() { return id; }
        
#pragma GCC visibility pop

            //  Except for AddIO, all those functions should be private.

            template <unsigned Dim,typename Pixel>
            static Image* CreateImage() { return new BaseImage<Dim,Pixel>(); }

            template <unsigned Dim,typename Pixel>
            static void WriteImage(std::ostream& os,const Image& image) {
                typedef BaseImage<Dim,Pixel> RealImage;
                const RealImage& im = static_cast<const RealImage&>(image);
                os.write(reinterpret_cast<const char*>(im.data()),im.size()*sizeof(Pixel));
            }

            template <unsigned Dim,typename Pixel>
            void ReadImage(std::istream& is,Image& image) const {

                typedef BaseImage<Dim,Pixel> RealImage;
                RealImage& im = static_cast<RealImage&>(image);

                const unsigned size = im.size();
                Pixel* const   data = reinterpret_cast<Pixel*>(im.data());
                
                try {
                    is.read(reinterpret_cast<char*>(data),size*sizeof(Pixel));
                } catch(...) {
                    throw BadData(is,identity());
                }

                if (!header.type.native())
                    Cpu::ChangeEndianness<Pixel>(data,data+size);
            }

            template <typename Pixel>
            static void AddIO(const Type t,const Packing p=Fixed) {
                DataType::AddType<Pixel>(t,p);
                const Entry e1(&CreateImage<1,Pixel>,&IO::ReadImage<1,Pixel>,&WriteImage<1,Pixel>);
                registery().insert(Registery::value_type(IOTag(1,typeid(Pixel)),e1));
                const Entry e2(&CreateImage<2,Pixel>,&IO::ReadImage<2,Pixel>,&WriteImage<2,Pixel>);
                registery().insert(Registery::value_type(IOTag(2,typeid(Pixel)),e2));
                const Entry e3(&CreateImage<3,Pixel>,&IO::ReadImage<3,Pixel>,&WriteImage<3,Pixel>);
                registery().insert(Registery::value_type(IOTag(3,typeid(Pixel)),e3));
                const Entry e4(&CreateImage<4,Pixel>,&IO::ReadImage<4,Pixel>,&WriteImage<4,Pixel>);
                registery().insert(Registery::value_type(IOTag(4,typeid(Pixel)),e4));
            }

        private:

            //  Currently known pixel types for Inrimage.

            IO() { }

            IO(const Internal::Register& reg): ImageIO(reg) {

                IO::AddIO<char>(Signed);
                IO::AddIO<short>(Signed);
                IO::AddIO<int>(Signed);
                IO::AddIO<long>(Signed);
                IO::AddIO<unsigned char>(Unsigned);
                IO::AddIO<unsigned short>(Unsigned);
                IO::AddIO<unsigned int>(Unsigned);
                IO::AddIO<unsigned long>(Unsigned);
                IO::AddIO<float>(Float);
                IO::AddIO<double>(Float);
            }

            //  TODO: 
            //  Create an instance of this object. It will be registered automatically via the above private constructor.
            //  This does not work with plugins, so it works only with static libraries. 

            static const IO          prototype;
            static const std::string id;

            //  Description of the various image types this plugins knows.

            typedef void   (*WriteFunc)(std::ostream&,const Image&);
            typedef void   (IO::*ReadFunc)(std::istream&,Image&) const;
            typedef Image* (*CreateFunc)();

            typedef stdext::triplet<CreateFunc,ReadFunc,WriteFunc> Entry;

            Header header;
            Entry  desc;

            typedef std::map<IOTag,Entry> Registery;

            static Registery  reg;
            static Registery& registery() { return reg; }

            static Suffixes suffixes;
        };
    }
}
