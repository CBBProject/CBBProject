#pragma once

#include <typeinfo>
#include <cstring>

#include <list>
#include <vector>
#include <string>
#include <iomanip>

#include <cbbExport.h>
#include <Images/Defs.H>
#include <Images/Exceptions.H>

#include <Utils/IOUtils.H>
#include <Utils/IOInit.H>

namespace Images {

    namespace Internal {
        //  A dummy structure to mark constructors that should register plugins.
        struct Register {
            Register() { }
        };
        const Register RegisterMe;
    }

    //  This is the abstract base class for all image readers/writers.
    //  It implements a virtual constructor and the automatic registration of
    //  derived classes.

    class CBB_WINDOWS_EXPORT ImageIO {
    public:

        typedef std::type_info           Id;
        typedef std::list<ImageIO*>      IOs;
        typedef std::vector<const char*> Suffixes;

        virtual ~ImageIO() { }

        static IOs& ios() { static IOs registery = *new IOs; return registery; }

        // All these virtual functions are meant to be overloaded by concrete image readers/writers.

        //  Cloning the io.

        virtual ImageIO* clone() const = 0;

        //  Give the generic name of the kind of files the io knows to handle.

        virtual const std::string& identity() const throw() = 0;
        
        //  This function checks whether this kind of file can be autodetected from its first few bytes.

        virtual bool autodetectable() const throw() { return true; }

        //  This function is used to recover the list of suffixes that the image format is supposed to handle.

        virtual Suffixes& known_suffixes() const throw() { return no_suffixes; }

        //  These function are used to check whether this format knows a given image (first) or a file
        //  (second) given by its first few characters, or (third) given its file name.

        virtual bool known(const Image&)                     throw() = 0;
        virtual bool known(const char*,const unsigned) const throw() = 0;

        //  For the most complex cases, one might want to redefine this function.

        virtual bool known_suffix(const char* suffix)  const throw() {
            const Suffixes& suffixes = known_suffixes();
            for (Suffixes::const_iterator i=suffixes.begin();i!=suffixes.end();++i) {
                if (!strcmp(suffix,*i))
                    return true;
            }
            return false;
        }

        //  Useful for readers in anonymous mode.

        bool known(Image*& image) { return known(*image); }

        //  Decode the header of the file. Called only when the file has been positively identified.

        virtual void identify(std::istream&) = 0;

        //  These functions are called after the function identify and respectively give the type of pixel
        //  and the dimension of the image image.

        virtual const Id& pixel_id()  const throw() = 0;
        virtual Dimension dimension() const throw() = 0;

        //  Allocating an image (when reading).

        virtual Image* create() const = 0;

        //  Reading/writing images.

        virtual void read(std::istream&,Image&)        const = 0;
        virtual void write(std::ostream&,const Image&) const = 0;

        //  Handle the default io.

        static ImageIO* GetCurrentFormat() {
            ImageIO* tmp = DefaultIO;
            if (!permanent)
                DefaultIO = 0;
            return tmp;
        }

        static void SetCurrentFormat(ImageIO* io,const bool perm=false) {
            DefaultIO = io;
            permanent = perm;
        }

        static ImageIO& format(const std::string& fmt) {
            for(IOs::const_iterator i=begin();i!=end();++i)
                if (fmt==(*i)->identity())
                    return **i;
            throw UnknownFileFormat(fmt);
        }

        static ImageIO& format_from_suffix(const std::string& name) {
            const std::string::size_type pos = name.find_last_of(".");
            if (pos==std::string::npos)
                throw NoSuffix(name);
            
            const std::string suffix = name.substr(pos+1);
            for(IOs::const_iterator i=begin();i!=end();++i) {
                if ((*i)->known_suffix(suffix.c_str()))
                    return **i;
            }
            throw UnknownFileSuffix(suffix);
        }

        static void SetCurrentFormat(const std::string& fmt,const bool perm) {
            if (fmt=="default") {
                SetCurrentFormat(0,perm);
                return;
            }
            SetCurrentFormat(&(format(fmt)),perm);
        }

        static void SetCurrentFormatFromSuffix(const std::string& name,const bool perm) {
            SetCurrentFormat(&(format_from_suffix(name)),perm);
        }

        static void SetProperties(Image& im,Image::Properties* props) { im.props = props; }

        //  This would allow the applications to get a list of supported image formats.

        struct const_iterator: public IOs::const_iterator {

            typedef IOs::const_iterator base;

            const_iterator(const base& it): base(it) { }

            const std::string& operator*()  const { return base::operator*()->identity(); }
            const std::string* operator->() const { return &(operator*());                }
        };

        static const_iterator begin() { return const_cast<const IOs&>(ios()).begin(); }
        static const_iterator end()   { return const_cast<const IOs&>(ios()).end();   }

    protected:

        ImageIO() { }
        ImageIO(const Internal::Register&) { ios().push_back(this); }

    private:

        static ImageIO* DefaultIO;
        static bool     permanent;

        static Suffixes no_suffixes; // Empty vector for the default known suffixes.
    };

    class Image;
    template <unsigned DIM,typename Pixel> class CBB_WINDOWS_EXPORT BaseImage;

    namespace Internal {

        //  Read a few bytes to figure out the file format and put them back into the stream.

        static const unsigned maxtagsize = 32;

        static const char*
        ReadTag(std::istream& is) {

            static char  buffer[maxtagsize];

            try {
                is.read(buffer,maxtagsize);
            } catch(...) {
                throw BadHeader(is);
            }

            for(int i=maxtagsize-1;i>=0;--i)
                is.putback(buffer[i]);

            return buffer;
        }

        template <typename T> struct Variant { };

        template <unsigned DIM,typename Pixel>
        struct Variant<BaseImage<DIM,Pixel> > {

            static inline Image&
            Create(std::istream& is,const ImageIO* io,BaseImage<DIM,Pixel>& image) {

                //  Check the pixel type and the dimension.

                if (image.dimension()!=io->dimension())
                    throw BadDimension(is,image.dimension());

                //  If the image pixel type is not correct create an appropriate image.

                if (image.pixel_id()!=io->pixel_id())
                    return *(io->create());

                return image;
            }

            static inline void
            Finish(const Image& image1,ImageIO* io,BaseImage<DIM,Pixel>& image2) {
                if (&image1!=static_cast<Image*>(&image2)) {
                    std::cerr << "Automatic conversion is not yet implemented !!" << std::endl;
                    throw 1;
                    //image = Convert(im);
                    if (!io->known(image2))
                        return;
                }
                image2.SetFormat(io);
            }
        };

        template <>
        struct Variant<Image*> {
            static inline Image& Create(std::istream&,const ImageIO* io,Image*& image) { return *(image = io->create()); }
            static inline void   Finish(Image& image,ImageIO* io,const Image*)         { image.SetFormat(io);            }
        };

        template <>
        struct Variant<Image> {

            static inline Image&
            Create(std::istream& is,const ImageIO* io,Image& image) {

                //  Check the pixel type and the dimension.

                if (image.dimension()!=io->dimension())
                    throw BadDimension(is,image.dimension());

                if (image.pixel_id()!=io->pixel_id())
                    throw BadPixelType(image.pixel_id(),io->pixel_id());

                return image;
            }

            static inline void
            Finish(Image& image,ImageIO* io,const Image&) { image.SetFormat(io); }
        };

        inline bool Identify(const char* buffer,ImageIO* IO) {
            return IO->known(buffer,maxtagsize);
        }

        template <typename IMAGE>
        void Read(std::istream& is,const char* buffer,ImageIO* IO,IMAGE& image) {

            //  The copy is useful for multithreaded programs.

            ImageIO* io = IO->clone();
            const std::string& identity = io->identity();

            //  Read the header.

            try {
                io->identify(is);
            } catch(std::ios_base::failure&) {
                delete io;
                throw BadHeader(is,identity);
            }

            //  Create the appropriate image if necessary.

            Image& im = Variant<IMAGE>::Create(is,io,image);

            //  Read the image data.

            try {
                io->read(is,im);
            } catch(std::ios_base::failure&) {
                delete io;
                throw BadData(is,identity);
            }
            delete io;

            Variant<IMAGE>::Finish(im,IO,image);
        }

        void Write(std::ostream&,const Image&);

        template <typename IMAGE>
        void Read(std::istream& is,IMAGE& image) {
            using namespace io_utils;

            IOInit<std::istream> init(is);  //  Initialize the IOs.
            const char* buff = ReadTag(is); //  Read the first characters of the file.

            //  Did the user (with a manip) specified a particular format ?

            if (ImageIO* fmt = ImageIO::GetCurrentFormat()) {
                if (Internal::Identify(buff,fmt)) { //  If possible, we verify that the format is the proper one.
                    Internal::Read(is,buff,fmt,image);
                    return;
                }
                throw BadFormat(is,fmt->identity());
            }

            //  Try all the available formats.

            ImageIO::IOs& ios = ImageIO::ios();
            for (ImageIO::IOs::iterator i=ios.begin();i!=ios.end();++i)
                if ((*i)->autodetectable() && Internal::Identify(buff,*i)) {
                    Internal::Read(is,buff,*i,image);
                    return;
                }
            
            //  No good format has been found!

            throw UnknownFileFormat(is);
        }
    }

#if 0
    template <unsigned DIM,typename Pixel>
    bool ImageIO::Convert(std::istream& is,const ImageIO* IO,BaseImage<DIM,Pixel>& image)
    {
    }
#endif

    //  Base classes for 2D/3D image readers/writers.

    class Image2DIO: public ImageIO {
    public:

        using ImageIO::Id;

        virtual ~Image2DIO() { }
        virtual Dimension dimension() const throw() { return 2; }

    protected:

        Image2DIO() { }
        Image2DIO(const Internal::Register& reg): ImageIO(reg) { }
    };

    class Image3DIO: public ImageIO {
    public:

        using ImageIO::Id;

        virtual ~Image3DIO() { }
        virtual Dimension dimension() const throw() { return 3; }

    protected:

        Image3DIO() { }
        Image3DIO(const Internal::Register& reg): ImageIO(reg) { }
    };

    inline std::istream&
    operator>>(std::istream& is,Image*& image) {
        Internal::Read(is,image);
        return is;
    }

    inline std::istream&
    operator>>(std::istream& is,Image& image) {
        Internal::Read(is,image);
        return is;
    }

    template <unsigned DIM,typename Pixel>
    inline std::istream&
    operator>>(std::istream& is,BaseImage<DIM,Pixel>& image) {
        Internal::Read(is,image);
        return is;
    }

    inline std::ostream&
    operator<<(std::ostream& os,const Image* image) {
        Internal::Write(os,*image);
        return os;
    }

    inline std::ostream&
    operator<<(std::ostream& os,const Image& image) {
        Internal::Write(os,image);
        return os;
    }

    // The manip format() used to specify explicitely a format.

    struct format {
        
        typedef enum { FromFormatName, FromSuffix } FormatSpec;

        format(const char* id,       const FormatSpec sp=FromFormatName,const bool perm=false): identity(id),type(sp),permanent(perm) { }
        format(const std::string& id,const FormatSpec sp=FromFormatName,const bool perm=false): identity(id),type(sp),permanent(perm) { }

        friend std::istream& operator>>(std::istream&,const format&);
        friend std::ostream& operator<<(std::ostream&,const format&);

    private:

        void set() const {
            switch (type) {
                case FromFormatName:
                    ImageIO::SetCurrentFormat(identity,permanent);
                    break;
                case FromSuffix:
                    ImageIO::SetCurrentFormatFromSuffix(identity,permanent);
                    break;
            }
        }

        const std::string identity;
        const FormatSpec  type;
        const bool        permanent;
    };

#if 0
    //TODO : should be in a "Formats.H" file ????

    struct inrimage : public format {
        inrimage(const bool permanent=false) : format("inrimage",permanent) {};
    };

    struct rawpgm : public format {
        rawpgm(const bool permanent=false) : format("rawpgm",permanent) {};
    };

    struct rawppm : public format {
        rawppm(const bool permanent=false) : format("rawppm",permanent) {};
    };
#endif 

    inline std::istream& operator>>(std::istream &is,const format& f) { f.set(); return is; }
    inline std::ostream& operator<<(std::ostream &os,const format& f) { f.set(); return os; }
}
