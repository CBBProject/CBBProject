#include <Utils/InfoTag.H>
#include <RawPgm.H>

namespace Images {

    extern "C" const char identity[] = "rawpgm";

    namespace RawPgm {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("pgm");
                suffixes.push_back("pgmraw");
                return suffixes;
            }
        }

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        using namespace io_utils;

        const char IO::MagicTag[]  = "P5";

        //  This function must return true if the first "buffer_size" bytes
        //  of the image file contained in "buffer" describe the appropriate
        //  file format which this ImageIO is able to read.

        bool IO::known(const char* buffer,const unsigned buffer_size) const throw() {
            if (buffer_size<strlen(MagicTag))
                return false;
            return (strncmp(buffer,MagicTag,strlen(MagicTag))==0);
        }

        //  This function must return true if this ImageIO knows how to read/write
        //  images of the effective type of its argument.

        bool IO::known(const Image& image) throw() {
            return (image.dimension()==2) && (Types::info_tag(image.pixel_id())==typeid(unsigned char));
        }

        //  This function read the header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is)
        {
            // Parse pgm file header.

            unsigned depth;

            is >> match(MagicTag) >> std::ws
               >> skip_comments('#')
               >> width >> height >> depth
               >> skip_to("\n");
        }

        //  This function will be called only if the function known() has
        //  returned true.  It has to read the image from "is" and build the
        //  corresponding Image that is to be returned in the parameter
        //  "image".  If (image==0), the image has to be allocated.  If
        //  (image!=0), an Image instance with the correct type is already
        //  pointed by "image".

        void IO::read(std::istream &is,Image& image) const
        {
            // Read the data.

            Image2D<unsigned char>& im = static_cast<Image2D<unsigned char>&>(image);

            im.resize(width,height);
            is.read(reinterpret_cast<char*>(im.data()),im.size()*im.pixel_size());
            if (!is)
                throw BadData(is,identity());
        }

        //  This function will be called only if known_type() has returned
        //  true. It has to write the image.

        void IO::write(std::ostream &os,const Image& image) const {
            const Image2D<unsigned char>& im = static_cast<const Image2D<unsigned char>&>(image);
            os << MagicTag << std::endl
               << im.dimx() << " " << im.dimy() << std::endl
               << 255 << std::endl;
            os.write(reinterpret_cast<const char*>(im.data()),im.size()*im.pixel_size());
        }
    }
}
