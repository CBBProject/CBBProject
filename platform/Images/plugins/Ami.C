#include <sstream>

#include <Utils/InfoTag.H>
#include <Ami.H>

//  BrainVoyager image loader.

namespace Images {

    extern "C" const char identity[] = "ami";

    namespace Ami {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("ami");
                return suffixes;
            }
        }

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        const char IO::MagicTag[] = "#IMG {";
        const char IO::XDIMTag[] = "dimx ";
        const char IO::YDIMTag[] = "dimy ";

        using namespace io_utils;

        //  This function by default returns true.
        //  It must be defined to return false for formats that are difficult/impossible
        //  to identify from their content.

        bool IO::autodetectable() const throw() { return false; }

        //  This function must return true if the first "buffer_size" bytes
        //  of the image file contained in "buffer" describe the appropriate
        //  file format which this ImageIO is able to read.

        bool IO::known(const char* buffer,const unsigned buffer_size) const throw() {
            //  We only do simple check here.
            //  This file format does not allow for type detection.
            if (buffer_size<strlen(MagicTag))
                    return false;
            return (strncmp(buffer,MagicTag,strlen(MagicTag)) == 0);
        }

        //  This function must return true if this ImageIO knows how to read/write
        //  images of the effective type of its argument.

        bool IO::known(const Image& image) throw() {
            return (image.dimension()==2) && (Types::info_tag(image.pixel_id())==typeid(float));
        }

        //  This function read the header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is) {
            // Parse ami file header to get image size.
            is >> match(MagicTag) >> match(XDIMTag) >> size[0] >> match(YDIMTag) >> size[1] >> match("}");
            is.seekg(255);
        }

        //  This function will be called only if the function known() has
        //  returned true.  It has to read the image from "is" and build the
        //  corresponding Image that is to be returned in the parameter
        //  "image".  If (image==0), the image has to be allocated.  If
        //  (image!=0), an Image instance with the correct type is already
        //  pointed by "image".

        void IO::read(std::istream &is,Image& image) const {

            // Read the data.

            Image2D<float>& im = static_cast<Image2D<float>&>(image);

            im.resize(size);
            is.read(reinterpret_cast<char*>(im.data()),im.size()*im.pixel_size());
            if (!is)
                throw BadData(is,identity());
        }

        //  This function will be called only if known_type() has returned
        //  true. It has to write the image.

        void IO::write(std::ostream &os,const Image& image) const {
            const Image2D<float>& im = static_cast<const Image2D<float>&>(image);
            std::ostringstream ost;
            ost << MagicTag << std::endl << ' ' << XDIMTag  << image.size(0) << ' ' << YDIMTag  << image.size(1)
                << "\n}" << std::endl;
            std::string hstring = ost.str();
            hstring.resize(255,' ');
            os.write(hstring.c_str(),255);
            os.write(reinterpret_cast<const char*>(im.data()),im.size()*im.pixel_size());
        }
    }
}
