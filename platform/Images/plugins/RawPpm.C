#include <RawPpm.H>

#ifndef NO_UNRESOLVED_EPRINTF_BUG
// Under Solaris, this symbol is unresolved when the plugins is dlopened, (TODO).
extern "C" {
    void __eprintf (const char *, const char *, unsigned, const char *) { }
    void __pure_virtual() {}
}
#endif

namespace Images {

    extern "C" const char identity[] = "rawppm";

    namespace RawPpm {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("ppm");
                suffixes.push_back("ppmraw");
                return suffixes;
            }
        }

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        using namespace io_utils;

        const char IO::MagicTag[]  = "P6";  // tag for raw ppm

        //  This function read the header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is)
        {
            // Parse ppm file header.

            unsigned depth;

            is >> match(MagicTag) >> std::ws
               >> skip_comments('#')
               >> width >> height >> depth
               >> skip_to("\n");

            if (!is)
                throw BadHeader(is,identity());
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

            Image2D<Pixels::RGB<unsigned char> >& im 
                  = static_cast<Image2D<Pixels::RGB<unsigned char> >&>(image);

            im.resize(width,height);
            is.read(reinterpret_cast<char*>(im.data()),im.size()*im.pixel_size());
            if (!is)
                throw BadData(is,identity());
        }

        //  This function will be called only if known_type() has returned
        //  true. It has to write the image.

        void IO::write(std::ostream &os,const Image& image) const {
            const Image2D<Pixels::RGB<unsigned char> >& im 
                  = static_cast<const Image2D<Pixels::RGB<unsigned char> >&>(image);
            os << MagicTag << std::endl
               << im.dimx() << " " << im.dimy() << std::endl
               << 255 << std::endl;
            os.write(reinterpret_cast<const char*>(im.data()),im.size()*sizeof(Pixels::RGB<unsigned char>));
        }
    }
}
