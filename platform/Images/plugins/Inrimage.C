#include <string>
#include <sstream>
#include <Utils/IOUtils.H>

#include <Inrimage.H>

namespace Images {

    extern "C" const char identity[] = "inrimage";

    namespace Inrimage {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("inr");
                suffixes.push_back("inr4");
                return suffixes;
            }
        }

        using namespace io_utils;

        DataType::Registery DataType::reg;
        IO::Registery       IO::reg;

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        //  Make this array dynamic...
        //  Create a user function AddType, that takes a name and a Pixel type.

        const char* DataType::TypeString[]    = { "signed", "unsigned", "float" };
        const char* DataType::PackingString[] = { "fixed", "packed" };
        const char* DataType::EndianString[]  = { "sun", "decm" };

        const char DataType::TYPE[]    = "TYPE=";
        const char DataType::PIXSIZE[] = "PIXSIZE=";
        const char DataType::BITS[]    = "bits";
        const char DataType::SCALE[]   = "SCALE=2**0";
        const char DataType::CPU[]     = "CPU=";

        const unsigned Header::header_size;

        const char Header::MagicTag[]  = "#INRIMAGE-4#{";
        const char Header::XDIM[]      = "XDIM=";
        const char Header::YDIM[]      = "YDIM=";
        const char Header::ZDIM[]      = "ZDIM=";
        const char Header::VDIM[]      = "VDIM=";
        const char Header::HeaderEnd[] = "##}\n";

        void IO::identify(std::istream& is) {
            try {
                is >> header;
                const IOTag tag(header.dim,header.pixel_id());
                Registery::iterator it = registery().find(tag);
                if (it==registery().end())
                    throw "Unknown header to image correspondence.";
                desc = it->second;
            } catch(...) {
                throw BadHeader(is,identity());
            }
        }

        std::ostream& operator<<(std::ostream &os,const DataType& dt) {
            os << DataType::TYPE << DataType::TypeString[dt.type];
            if (dt.type!=Float)
                os << ' ' << DataType::PackingString[dt.packing];
            return os << std::endl
                      << DataType::PIXSIZE << dt.bpp << ' ' << DataType::BITS << std::endl
                      << DataType::SCALE << std::endl
                      << DataType::CPU << DataType::EndianString[dt.endian];
        }

        template <typename T>
        T validate(const char* buffer,const T first,const T last,const char* strings[]) {
            for (int i=first;i<=last;++i)
                if (!strcmp(buffer,strings[i]))
                    return static_cast<T>(i);
            throw "Unknown value.";
        }
        
        std::istream& operator>>(std::istream& is,DataType& dt) {

            //  The order is allowed to vary Insert this in a loop (TODO).

            char buffer[256];
            is >> match(DataType::TYPE) >> buffer;

            dt.type = validate(buffer,Signed,Float,DataType::TypeString);

            if (dt.type!=Float) {
                is >> buffer;
                dt.packing = validate(buffer,Fixed,Packed,DataType::PackingString);
            } else {
                dt.packing = Fixed;
            }

            is >> match(DataType::PIXSIZE) >> dt.bpp >> match(DataType::BITS)
               >> eat(DataType::SCALE)
               >> match(DataType::CPU) >> buffer;

            dt.endian = validate(buffer,Cpu::BigEndian,Cpu::LittleEndian,DataType::EndianString);

            return is;
        }

        std::istream&
        operator>>(std::istream& is,Header& header) {

            is >> match(Header::MagicTag)
               >> match(Header::XDIM) >> header.size[0]
               >> match(Header::YDIM) >> header.size[1]
               >> match(Header::ZDIM) >> header.size[2]
               >> match(Header::VDIM) >> header.size[3]
               >> header.type
               >> skip_to(Header::HeaderEnd);

            header.dim = 4;
            while (header.dim>0 && header.size[--header.dim]==1);
            ++header.dim;

            return is;
        }

        std::ostream&
        operator<<(std::ostream &os,const Header& header) {

            std::ostringstream ost;
            ost << Header::MagicTag << std::endl
                << Header::XDIM << header.size[0] << std::endl
                << Header::YDIM << header.size[1] << std::endl
                << Header::ZDIM << header.size[2] << std::endl
                << Header::VDIM << header.size[3] << std::endl
                << header.type << std::endl << "#"; 

            std::string hstring = ost.str();
            const int size = hstring.length()+256-hstring.length()%256;
            hstring.resize(size-(sizeof(Header::HeaderEnd)-1),'\n');
            hstring += Header::HeaderEnd;

            os.write(hstring.c_str(),size);

            return os;
        }
    }
}
