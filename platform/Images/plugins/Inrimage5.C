#include <string>
#include <sstream>
#include <Utils/IOUtils.H>

#include <Inrimage5.H>

namespace Images {

    extern "C" const char identity[] = "Inrimage-5";

    namespace Inrimage5 {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("inr");
                suffixes.push_back("inr5");
                return suffixes;
            }

            std::string chop(std::string& s,const std::string& delims=" ") {
                const unsigned ind = s.find_last_not_of(delims)+1;
                return s.substr(0,ind);
            }
        }

        using namespace io_utils;

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        const unsigned Header::header_size;

        const std::string Header::MagicTag  = "#INRIMAGE-5#{";
        const std::string Header::SIZE      = "SIZE   = ";
        const std::string Header::TYPE      = "TYPE   = ";
        const std::string Header::CPU       = "CPU    = ";
        const std::string Header::COMPRESS  = "COMPRESSION = ";
        const std::string Header::HeaderEnd = "##}\n";

        const std::string Header::EndianStrings[]   = { "BigEndian", "LittleEndian", "Neutral" };
        const std::string Header::CompressStrings[] = { "None", "zlib", "bzip2" };

        template <typename T>
        T validate(const std::string& str,const T first,const T last,const std::string strings[]) {
            for (int i=first;i<=last;++i)
                if (str==strings[i])
                    return static_cast<T>(i);
            throw "Unknown value.";
        }

        std::istream&
        operator>>(std::istream& is,Header& header) try {

            std::string typestr;
            bool alias = false;
            is >> match(Header::MagicTag)
               >> match(Header::SIZE) >> header.sz
               >> match(Header::TYPE) >> typestr >> match_optional("(",alias);
             
            header.type = IODesc::find(header.sz.dimension(),typestr);

            if (alias) {
                std::string alias_name;
                is >> alias_name;
                if (alias_name[alias_name.size()-1]==')')
                    alias_name = alias_name.substr(0,alias_name.size()-1);
                else
                    is >> match(')');
                header.props->define("name",alias_name);
            }
                    
            std::string endianstr;
            is >> match(Header::CPU) >> endianstr;

            header.endian = validate(endianstr,Header::BigEndian,Header::Neutral,Header::EndianStrings);

            bool compression = false;
            header.compression = Header::NoCompression;
            is >> match_optional(Header::COMPRESS,compression);
            if (compression) {
                std::string compression_str;
                is >> compression_str;
                header.compression = validate(compression_str,Header::NoCompression,Header::Bzip2,Header::CompressStrings);
            }

            //  Header option parsing.

            while (1) {
                is >> std::ws;
                if (is.peek()=='#') {
                    bool header_end = false;
                    is >> match_optional(Header::HeaderEnd,header_end);
                    if (header_end)
                        break;
                    is >> skip_line;
                } else {
                    std::string key;
                    std::string value;
                    is >> std::ws;
                    getline(is,key,'=');
                    key = chop(key);
                    is >> std::ws;
                    getline(is,value);
                    value = chop(value);
                    header.props->define(key,value);
                }
            }

            return is;

        } catch(...) {
            throw BadHeader(is,Images::identity);
        }

        std::ostream&
        operator<<(std::ostream &os,const Header& header) {

            const Images::Properties* props = header.properties();

            std::ostringstream ost;
            ost << Header::MagicTag << std::endl
                << Header::SIZE     << header.sz << std::endl
                << Header::TYPE     << header.type->second->name();

            if (props && props->has_property("name"))
                ost << " (" << props->find("name") << ')';

            ost << std::endl
                << Header::CPU      << Header::EndianStrings[header.endian]        << std::endl
                << Header::COMPRESS << Header::CompressStrings[header.compression] << std::endl
                << '#' << std::endl;

            //   Dump image properties.

            if (props)
                for (Images::Properties::const_iterator i=props->begin();i!=props->end();++i)
                    if (i->first!="name")
                        ost << i->first << " = " << i->second << std::endl;

            std::string hstring = ost.str();
            const int size = hstring.length()+256-hstring.length()%256;
            hstring.resize(size-Header::HeaderEnd.length(),'\n');
            hstring += Header::HeaderEnd;

            os.write(hstring.c_str(),size);

            return os;
        }
    }
}
