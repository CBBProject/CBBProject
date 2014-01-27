#include <string>
#include <sstream>
#include <vector>
#include <Utils/IOUtils.H>

#include <Vtk.H>

namespace Images {

    extern "C" const char identity[] = "vtk";

    namespace Vtk {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("vtk");
                suffixes.push_back("vti");
                return suffixes;
            }
        }

        using namespace io_utils;

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);
        
        ImageIO::Suffixes IO::suffixes = init_suffixes();

        const std::string Header::MagicTag        = "<?xml version=\"1.0\"?>\n<VTKFile ";
        const std::string Header::TYPE            = "type=\"ImageData\"";
        const std::string Header::Version         = "version=\"0.1\"";
        const std::string Header::CPU             = "byte_order=";
        //const std::string Header::COMPRESS = "compressor=";
        const std::string Header::Origin          = "Origin=\"";
        const std::string Header::Spacing         = "Spacing=\"";
        const std::string Header::HeaderEnd       = "</VTKFile>";
        const std::string Header::ImageData       = "<ImageData WholeExtent=\"";
        const std::string Header::ImageDataEnd    = "</ImageData>";
        const std::string Header::Piece           = "<Piece Extent=\"";
        const std::string Header::PieceEnd        = "</Piece>";
        const std::string Header::PointData       = "<PointData>";
        const std::string Header::PointDataEnd    = "</PointData>";
        const std::string Header::DataArray       = "<DataArray Name=\"";
        const std::string Header::DataArrayType   = "type=\"";
        const std::string Header::DataArrayFmt    = "format=\"appended\" offset=\"0\" />";
        const std::string Header::AppendedData    = "<AppendedData encoding=\"raw\">";
        const std::string Header::AppendedDataEnd = "</AppendedData>";
        const char* Header::EndianString[]  = { "\"BigEndian\"", "\"LittleEndian\"" };

        std::istream&
        operator>>(std::istream& is,Header& header) {

            //std::string compressorstr;

            bool bgendian;
            is >> match(Header::MagicTag)
               >> match(Header::TYPE)
               >> match(Header::Version)
               >> match(Header::CPU)
               >> match_optional(Header::EndianString[0],bgendian);
               
            if (bgendian) 
                header.endian = Header::BigEndian;
            else {
                is >> match(Header::EndianString[1]);
                header.endian = Header::LittleEndian;
            }

               //>> match(Header::COMPRESS) >> compressorstr
            is >> match('>');

            Dimension s[3][2];
            Dimension sz[3];
            is >> match(Header::ImageData);
            Dimension dim = 0;
            while (is.peek()!='"') {
                is >> s[dim][0] >> s[dim][1];
                if (s[dim][1]<s[dim][0])
                    throw "Wrong size specification";
                sz[dim] = s[dim][1]-s[dim][0]+1;
                dim += 1;
                is >> std::ws;
            }
            is.get();

            Utils::ImageSize<> imagesz(dim,sz);
            header.sz = imagesz;

            // For now this header information is ignored.

            unsigned x0,y0,z0,dx,dy,dz;
            is >> match(Header::Origin)  >> x0 >> y0 >> z0 >> match('"')
               >> match(Header::Spacing) >> dx >> dy >> dz >> match("\">")
               >> match(Header::Piece);

            dim = 0;
            while (is.peek()!='"') {
                is >> s[dim][0] >> s[dim][1];
                if (s[dim][1]<s[dim][0])
                    throw "Wrong size specification";
                sz[dim] = s[dim][1]-s[dim][0]+1;
                dim += 1;
                is >> std::ws;
            }
            is.get();

            is >> match('>') >> match(Header::PointData) >> match(Header::DataArray);

            char namestr[256];
            is.get(namestr,256,'\"');
            
            is >> match('"') >> match(Header::DataArrayType);

            char typestr[256];
            is.get(typestr,256,'\"');
            header.type = IODesc::find(header.sz.dimension(),typestr);

            is >> match('"')
               >> match(Header::DataArrayFmt)
               >> match(Header::PointDataEnd)
               >> match(Header::PieceEnd)
               >> match(Header::ImageDataEnd);

            //  Insert here options in header parsing.

            return is;
        }

        std::ostream&
        operator<<(std::ostream &os,const Header& header) {

            std::ostringstream ost;
            for (unsigned i=0;i<header.sz.dimension();++i)
                ost << "0 " << header.sz[i]-1 << ' ';
            const std::string& sz_string = ost.str();

            os << Header::MagicTag << Header::TYPE << ' ' << Header::Version << ' '
               << Header::CPU << Header::EndianString[header.endian] << '>' << std::endl
               << "\t" << Header::ImageData << sz_string << "\" " << Header::Origin << "0 0 0\" " << Header::Spacing << "1 1 1\">" << std::endl
               << "\t\t" << Header::Piece << sz_string << "\">" << std::endl
               << "\t\t\t" << Header::PointData << std::endl
               << "\t\t\t\t" << Header::DataArray << header.name << "\" " << Header::DataArrayType << header.type->second->name() << "\" " << Header::DataArrayFmt << std::endl
               << "\t\t\t" << Header::PointDataEnd << std::endl
               << "\t\t" << Header::PieceEnd << std::endl
               << "\t" << Header::ImageDataEnd << std::endl;

            return os;
        }
    }
}
