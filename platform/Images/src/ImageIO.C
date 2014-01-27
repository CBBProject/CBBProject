#include <iostream>

#include <Images/Image.H>
#include <Images/ImageIO.H>
#include <Utils/IOInit.H>

namespace Images {

    ImageIO* ImageIO::DefaultIO = 0;
    bool     ImageIO::permanent = false;

    ImageIO::Suffixes ImageIO::no_suffixes;

    namespace Internal {

        inline bool Write(std::ostream& os,ImageIO* fmt,const Image& image) {

            const bool known = fmt->known(image);
            //std::cerr << "Format: " << fmt->identity() << ' ' << known << std::endl;
            if (known) {
                if (image.isStorageContiguous()) {
                    fmt->write(os,image);
                } else {
                    const Image* contiguous_image = image.clone();
                    fmt->write(os,*contiguous_image);
                    delete contiguous_image;
                }
            }

            return known;
        }

        //  This methods looks for an ImageIO able to write image and writes it.

        void Write(std::ostream& os,const Image& image) {

            using namespace io_utils;

            IOInit<std::ostream> init(os);  //  Initialize the IOs.

            if (ImageIO* fmt = ImageIO::GetCurrentFormat()) {
                if (Write(os,fmt,image))
                    return;
                throw NonMatchingFormat(os,fmt->identity());
            }

            if (ImageIO* io = image.GetFormat()) {
                if (Write(os,io,image))
                    return;
                throw NonMatchingFormat(os,io->identity());
            }

            ImageIO::IOs& ios = ImageIO::ios();
            for(ImageIO::IOs::iterator i=ios.begin();i!=ios.end();++i)
                if (Write(os,*i,image))
                    return;

            throw NoMatchingFileFormat(os);
        }
    }
}
