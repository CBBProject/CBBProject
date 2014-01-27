#include <Utils/InfoTag.H>
#include <Vmr.H>

//  BrainVoyager image loader.

namespace Images {

    extern "C" const char identity[] = "vmr";

    namespace Vmr {

        namespace {
            ImageIO::Suffixes
            init_suffixes() {
                ImageIO::Suffixes suffixes;
                suffixes.push_back("vmr");
                return suffixes;
            }
        }

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        ImageIO::Suffixes IO::suffixes = init_suffixes();

        using namespace io_utils;

        //  This function by default returns true.
        //  It must be defined to return false for formats that are difficult/impossible
        //  to identify from their content.

        bool IO::autodetectable() const throw() { return false; }

        //  This function must return true if the first "buffer_size" bytes
        //  of the image file contained in "buffer" describe the appropriate
        //  file format which this ImageIO is able to read.

        bool IO::known(const char* buffer,const unsigned buffer_size) const throw() {
            //  We do not check this file format as it is very crude.
            //  There might be a way by checking that the sizes of the image, maps the
            //  size of the file, but this is difficult here. A new function working
            //  at the filename level ?
            return true;
        }

        //  This function must return true if this ImageIO knows how to read/write
        //  images of the effective type of its argument.

        bool IO::known(const Image& image) throw() {
            return (image.dimension()==3) && (Types::info_tag(image.pixel_id())==typeid(unsigned char));
        }

        //  This function read the header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is)
        {
            // Parse vmr file header to get image size.

            for (unsigned i=0;i<3;++i) {
                unsigned short value;
                is.read(reinterpret_cast<char*>(&value),sizeof(unsigned short));
                size[i] = value;
            }
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

            Image3D<unsigned char>& im = static_cast<Image3D<unsigned char>&>(image);

            im.resize(size);
            is.read(reinterpret_cast<char*>(im.data()),im.size()*im.pixel_size());
            if (!is)
                throw BadData(is,identity());
        }

        //  This function will be called only if known_type() has returned
        //  true. It has to write the image.

        void IO::write(std::ostream &os,const Image& image) const {
            const Image3D<unsigned char>& im = static_cast<const Image3D<unsigned char>&>(image);
            for (unsigned i=0;i<3;++i) {
                const unsigned short sz = image.size(i);
                os.write(reinterpret_cast<const char*>(&sz),sizeof(unsigned short));
            }
            os.write(reinterpret_cast<const char*>(im.data()),im.size()*im.pixel_size());
        }
    }
}
