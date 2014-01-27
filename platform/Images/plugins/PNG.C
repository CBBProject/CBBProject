#include <string>

#include <PluginDefs.H>

#include <Image.H>
#include <InfoTag.H>

#include <PNG.H>
#include <CpuUtils.H>

namespace Images {

    extern "C" const char identity[] = "png";

    namespace PNG {

        const IO          IO::prototype(Internal::RegisterMe);
        const std::string IO::id(Images::identity);

        using namespace io_utils;

        //  IO routine so that libpng uses streams instead of FILE*.

        void IO::png_read_func(png_structp png_ptr,png_bytep buff,png_size_t n) {
            std::istream& is = *reinterpret_cast<std::istream*>(png_get_io_ptr(png_ptr));
            is.read(reinterpret_cast<char*>(buff),n);
        }

        void IO::png_write_func(png_structp png_ptr,png_bytep buff,png_size_t n) {
            std::ostream& os = *reinterpret_cast<std::ostream*>(png_get_io_ptr(png_ptr));
            os.write(reinterpret_cast<char*>(buff),n);
        }

        void IO::png_flush_func(png_structp png_ptr) {
            std::ostream& os = *reinterpret_cast<std::ostream*>(png_get_io_ptr(png_ptr));
            os.flush();
        }
        
        //  This function must return true if the first "buffer_size" bytes
        //  of the image file contained in "buffer" describe the appropriate
        //  file format which this ImageIO is able to read.

        bool IO::known(const char *buffer,const unsigned buffer_size) const throw() {
            return png_sig_cmp(reinterpret_cast<png_byte*>(const_cast<char*>(buffer)),0,buffer_size)==0;
        }

        //  This function must return true if this ImageIO knows how to read/write
        //  images of the effective type of its argument.

        bool IO::known(const Image& image) throw() {
            const Types::info_tag tag(image.pixel_id());
            const bool valid_png_image = (image.dimension()==2) &&
                                         ((tag==typeid(bool))                       ||
                                          (tag==typeid(unsigned char))              ||
                                          (tag==typeid(unsigned short))             ||
                                          (tag==typeid(Pixels::RGB<unsigned char>)) ||
                                          (tag==typeid(Pixels::RGB<unsigned short>)));

            if (valid_png_image) {
                png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
                if (!png_ptr)
                    throw UnexpectedError();

                info = png_create_info_struct(png_ptr);
                if (!info)
                    throw UnexpectedError();
            }

            return valid_png_image;
        }

        //  This function read the PNG header and parses it to get the useful info
        //  for allocating the image and reading the file data in it.

        void IO::identify(std::istream& is) {

            // Since it is a PNG file, create the png structs.

            png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
            if (!png_ptr)
                throw UnexpectedError();

            info = png_create_info_struct(png_ptr);
            if (!info)
                throw UnexpectedError();

            png_set_read_fn(png_ptr,static_cast<void*>(&is),static_cast<png_rw_ptr>(png_read_func));
            png_read_info(png_ptr,info);
            png_get_IHDR(png_ptr,info,&width,&height,&depth,&type,NULL,NULL,NULL);
        }

        const IO::Id& IO::pixel_id() const throw() {
            switch (type) {
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                    return typeid(bool);
                case PNG_COLOR_TYPE_GRAY:
                    if (depth<=8)
                        return typeid(unsigned char);
                    else 
                        return typeid(unsigned short);
                case PNG_COLOR_TYPE_PALETTE:
                case PNG_COLOR_TYPE_RGB:
                    if (depth<=8)
                        return typeid(Pixels::RGB<unsigned char>);
                    else 
                        return typeid(Pixels::RGB<unsigned short>);
                case PNG_COLOR_TYPE_RGB_ALPHA:
                    if (depth<=8)
                        return typeid(Pixels::RGB<unsigned char>);
                    else 
                        return typeid(Pixels::RGB<unsigned short>);
            }

            throw UnexpectedError();
        }

        Image* IO::create() const {
            switch (type) {
                case PNG_COLOR_TYPE_GRAY_ALPHA:
                    return new Image2D<bool>;
                case PNG_COLOR_TYPE_GRAY:
                    if (depth<=8)
                        return new Image2D<unsigned char>;
                    else 
                        return new Image2D<unsigned short>;
                case PNG_COLOR_TYPE_PALETTE:
                case PNG_COLOR_TYPE_RGB:
                    if (depth<=8)
                        return new Image2D<Pixels::RGB<unsigned char> >;
                    else 
                        return new Image2D<Pixels::RGB<unsigned short> >;
                case PNG_COLOR_TYPE_RGB_ALPHA:
                    if (depth<=8)
                        return new Image2D<Pixels::RGB<unsigned char> >;
                    else 
                        return new Image2D<Pixels::RGB<unsigned short> >;
            }

            throw UnexpectedError();
        }

        //  This function will be called only if the function known() has returned true.
        //  It has to read the image from "is" and build the corresponding Image that is to be returned in
        //  the parameter "image". An Image instance with the correct type is already pointed by "image".

        void IO::read(std::istream &is,Image& image) const {

            const Dimension size[2] = { width, height };
            image.resize(size);

            //  Convert palette images to RBG.

            if (type==PNG_COLOR_TYPE_PALETTE)
               png_set_palette_to_rgb(png_ptr);

            //  Convert small pixel depths to 8 bits.

            unsigned bpp = depth/8;
            if (type==PNG_COLOR_TYPE_GRAY && depth<8) {
                png_set_expand_gray_1_2_4_to_8(png_ptr);
                bpp = 1;
            }

            if (png_get_valid(png_ptr,info,PNG_INFO_tRNS))
                png_set_tRNS_to_alpha(png_ptr);

            // Read the data.

            const unsigned row_width = width*png_get_channels(png_ptr,info)*bpp;
            png_bytep row_pointers[height];
            for (unsigned i=0;i<height;++i)
                row_pointers[i] = static_cast<png_bytep>(image.data())+i*row_width;

            png_read_image(png_ptr,row_pointers);

            //png_destroy_read_struct(&png_ptr,&info,&end_info);
        }

        //  This function will be called only if known_type() has returned true. It has to write the image.

        void IO::write(std::ostream& os,const Image& image) const {

            png_set_write_fn(png_ptr,static_cast<void*>(&os),static_cast<png_rw_ptr>(png_write_func),static_cast<png_flush_ptr>(png_flush_func));
            png_set_IHDR(png_ptr,info,width,height,depth,type,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
            png_write_info(png_ptr,info);

            //  Set bpp to a proper value.

            const unsigned bpp = 1;

            const unsigned row_width = width*png_get_channels(png_ptr,info)*bpp;
            png_bytep row_pointers[height];
            for (unsigned i=0;i<height;++i)
                row_pointers[i] = static_cast<png_bytep>(const_cast<void*>(image.data()))+i*row_width;

            png_write_image(png_ptr,row_pointers);
            png_write_end(png_ptr,info);

            //png_destroy_write_struct(&png_ptr,&info,&end_info);

#if 0
            unsigned size = 1;

            const char* dataptr;
            if (image.pixel_id()==typeid(bool)) {
                fmt = PBM_RAW;
                size = 1;
                dataptr = data<bool>(image);
            } else if (image.pixel_id()==typeid(unsigned char))  {
                fmt = PGM_RAW;
                size = 255;
                dataptr = data<unsigned char>(image);
            } else if (image.pixel_id()==typeid(unsigned short)) {
                fmt = PGM_RAW;
                size = 65536;
                dataptr = data<unsigned short>(image);
            } else if (image.pixel_id()==typeid(Pixels::RGB<unsigned char>)) {
                fmt = PPM_RAW;
                size = 255;
                dataptr = data<Pixels::RGB<unsigned char> >(image);
            } else if (image.pixel_id()==typeid(Pixels::RGB<unsigned short>)) {
                fmt = PPM_RAW;
                size = 65536;
                dataptr = data<Pixels::RGB<unsigned short> >(image);
            }
            os << 'P' << static_cast<char>('1'+fmt) << std::endl
               << image.size(0) << " " << image.size(1) << std::endl;
            
            if ((fmt!=PBM_RAW) && (fmt!=PBM_ASCII))
               os << size << std::endl;
            
            switch (fmt) {
                case PBM_ASCII: {
                    const Image2D<bool>& im = static_cast<const Image2D<bool>&>(image);
                    for (Image2D<bool>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << ((*i) ? '1' : '0') << ' ';
                }
                case PGM_ASCII: {
                    if (size==255) {
                        const Image2D<unsigned char>& im = static_cast<const Image2D<unsigned char>&>(image);
                        for (Image2D<unsigned char>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << *i << ' ';
                    } else {
                        const Image2D<unsigned short>& im = static_cast<const Image2D<unsigned short>&>(image);
                        for (Image2D<unsigned short>::const_iterator<pixel> i=im.begin();i!=im.end();++i)
                            os << *i << ' ';
                    }
                }
                case PPM_ASCII: { //    TODO
                }
                case PBM_RAW:
                    break;
                case PGM_RAW:
                case PPM_RAW:
                    //  Handle Endianness (TODO).
                    os.write(dataptr,image.size()*image.pixel_size());
                    break;
            }
#endif
        }
    }
}
