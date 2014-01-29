#pragma once

#include <typeinfo>
#include <string>
#include <fstream>
#include <list>

#include <Utils/GeneralizedIterators.H>

#include <Images/Defs.H>
#include <Images/Index.H>
#include <Images/Properties.H>
#include <Images/MultiDimCounter.H>
#include <Images/Exceptions.H>
#include <Images/Shape.H>

//! Images definitions:
//!     All classes are in the namespace Images.

namespace Images {
    template <typename Pixel> class Image1D;
    template <typename Pixel> class Image2D;
    template <typename Pixel> class Image3D;

    template <unsigned DIM,typename Pixel> class ImageType;
    template <unsigned DIM,typename Pixel> class BaseImage;
}

namespace Images {

    class Image;
    class ImageIO;

    //  Declarations related to the various iterators.

    const int BadDim = -1;

    enum { X, Y, Z };

    const Dimension row   = X;
    const Dimension col   = Y;;
    const Dimension depth = Z;;

    template <unsigned DIM> class domain_iterator;
    template <unsigned DIM> class domain_const_iterator;

    struct domain {
        template <typename T>
        struct Info {
            typedef domain_iterator<T::Dim>       type;
            typedef domain_const_iterator<T::Dim> const_type;
        };
    };

    template <typename REP> class pixel_iterator;
    template <typename REP> class pixel_const_iterator;

    struct pixel {
        template <typename T>
        struct Info {
            typedef typename T::PixelType Pixel;
            typedef pixel_iterator<T>       type;
            typedef pixel_const_iterator<T> const_type;
        };
    };

    // The abstract class for all Images.

    class Image {
    public:

        typedef Images::Properties Properties;

        Image():             io(0),props(0) { }
        Image(const Image&): io(0),props(0) { }

        virtual ~Image() { reset(); }

        virtual const std::type_info& type()     const { return typeid(Image); }
        virtual const std::type_info& pixel_id() const { return typeid(void);  }

        virtual Dimension dimension()           const { return 0; } //  Number of dimensions.
        virtual Dimension size()                const { return 0; } //  Number of pixels of an image.
        virtual Dimension pixel_size()          const { return 0; } //  Size of a pixel in bytes.
        virtual Dimension size(const Dimension) const { return 0; } //  Number of pixels along the requested dimension.

        virtual bool isStorageContiguous() const { return false; }

        virtual Image* clone() const = 0;

        virtual void resize(const Dimension[]) = 0;

        ImageIO* GetFormat() const { return io; }
        ImageIO* SetFormat(ImageIO* fmt) {
            ImageIO* prev_io = io;
            io = fmt;
            return prev_io;
        }

        bool has_properties() const { return props!=0; }
        bool has_property(const std::string& id) const {
            return has_properties() && props->has_property(id);
        }

        const Properties& properties() const { return *props; }

        Properties& properties() {
            if (!has_properties())
                props = new Properties();
            return *props;
        }

        //  ImageIO is not known in this file so it is not possible to call the
        //  destructor here...

        void reset_io()         { io = 0;                             }
        void reset_properties() { if (has_properties()) delete props; }

        void reset() {
            reset_io();
            reset_properties();
        }

    protected:

        friend class ImageIO;

        ImageIO*    io;
        Properties* props;

        static std::list<Image*> Images;
    };

    std::ostream& operator<<(std::ostream&,const Image*&);
    std::ostream& operator<<(std::ostream&,const Image&);

    template <unsigned DIM,typename Pixel>
    class BaseImage: public Image {

        typedef BaseImage self;

    public:

        typedef Pixel                    PixelType;
        typedef Images::Shape<DIM>       Shape;
        typedef Images::Index<DIM,Coord> Index;

        static const unsigned Dim = DIM;

        //  Generic iterators.

        template <typename IteratorType>
        struct iterator: public Iterators::generic<typename IteratorType::template Info<self>::type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::type> Base;
            template <typename T> iterator(T t): Base(t) { }
        };

        template <typename IteratorType>
        struct const_iterator: public Iterators::generic<typename IteratorType::template Info<self>::const_type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::const_type> Base;
            template <typename T> const_iterator(T t): Base(t) { }
        };

        Iterators::BEGIN<self&>       begin()       { return Iterators::BEGIN<self&>(*this);       }
        Iterators::BEGIN<const self&> begin() const { return Iterators::BEGIN<const self&>(*this); }

        Iterators::END<self&>       end()       { return Iterators::END<self&>(*this);       }
        Iterators::END<const self&> end() const { return Iterators::END<const self&>(*this); }

        BaseImage(): pixels(0) { }
        BaseImage(const Shape& s): pixels(0) { resize(s); }

        BaseImage(const std::string& name): pixels(0) { Read(name.c_str()); }
        BaseImage(const char* const name):  pixels(0) { Read(name); }
        BaseImage(char* const name):        pixels(0) { Read(name); }

        BaseImage(const Shape& s,Pixel *const data): pixels(0) {
            resize(s);
            std::copy(&data[0],&data[size()],pixels);
        }

        template <typename Pixel2>
        BaseImage(const BaseImage<DIM,Pixel2>& im): pixels(0) {
            resize(im.shape());
            std::copy(&im.data()[0],&im.data()[size()],pixels);
        }

        BaseImage(const BaseImage& I): pixels(0) { *this = I; }

        ~BaseImage() {
            if (pixels)
                delete[] pixels;
        }

        //  Beware there is the case of float to char... TODO.

        template <typename Pixel2>
        operator BaseImage<DIM,Pixel2>() const {
            BaseImage<DIM,Pixel2> newimage(*this);
            return newimage;
        }

        BaseImage& operator=(const Pixel p) {
            std::fill(&pixels[0],&pixels[size()],p);
            return *this;
        }

        BaseImage& operator=(const BaseImage& im) {
            resize(im.shape());
            std::copy(&im.pixels[0],&im.pixels[size()],pixels);
            return *this;
        }

        template <typename Scalar>
        BaseImage operator*(const Scalar p) const {
            BaseImage res(*this);
            for (unsigned i=0;i<size();++i)
                res.pixels[i] *= p;
            return res;
        }

        BaseImage operator-(const Pixel p) const {
            BaseImage res(*this);
            for (unsigned i=0;i<size();++i)
                res.pixels[i] -= p;
            return res;
        }

        BaseImage operator-(const BaseImage& im) const {
            if (im.shape()!=shape())
                throw DifferentImages();

            BaseImage res(*this);
            for (unsigned i=0;i<size();++i)
                res.pixels[i] -= im.pixels[i];
            return res;
        }

        //  Dimension.

        Dimension dimension() const { return Dim; }

        const std::type_info& pixel_id()   const { return typeid(Pixel); }
        virtual Dimension     pixel_size() const { return sizeof(Pixel); }

        static bool same_format(const Image& im) { return ((im.dimension()==DIM) && (im.pixel_id()==typeid(Pixel))); }

        //  Various size functions.

        virtual Dimension size()                  const { return shp.size();  }
        virtual Dimension size(const Dimension d) const { return shp.size(d); }

        Shape shape() const { return shp; }

        void resize(const Shape& s)      {
            shp.resize(s);
            if (pixels)
                delete[] pixels;
            pixels = new Pixel[size()];
        }
        void resize(const Dimension s[]) { resize(Shape(s)); }

        //  Indexing.

        Pixel& operator()(const iterator<domain>& it)       { return (*this)(it.position()); }
        Pixel  operator()(const iterator<domain>& it) const { return (*this)(it.position()); }

        Pixel& operator()(const const_iterator<domain>& it)       { return (*this)(it.position()); }
        Pixel  operator()(const const_iterator<domain>& it) const { return (*this)(it.position()); }

        Pixel& operator()(const domain_iterator<Dim>& it)       { return (*this)(it.position()); }
        Pixel  operator()(const domain_iterator<Dim>& it) const { return (*this)(it.position()); }

        Pixel& operator()(const domain_const_iterator<Dim>& it)       { return (*this)(it.position()); }
        Pixel  operator()(const domain_const_iterator<Dim>& it) const { return (*this)(it.position()); }

              Pixel& operator()(const Index& ind)       { return pixels[index(ind)]; }
        const Pixel& operator()(const Index& ind) const { return pixels[index(ind)]; }

        Pixel& operator()(const Images::Coord ind[Dim]) {
            const Index index(ind);
            return (*this)(index);
        }

        const Pixel& operator()(const Images::Coord ind[Dim]) const {
            const Index index(ind);
            return (*this)(index);
        }

        bool InRange(const iterator<pixel>& it) const            { return it.is_valid(); } // This probably does not work. const_iterator version ?
        bool InRange(const domain_iterator<Dim>& it) const       { return it.is_valid(); }
        bool InRange(const domain_const_iterator<Dim>& it) const { return it.is_valid(); }

        template <typename T>
        bool InRangeForDim(const T& ind,const unsigned dim) const {
            return ((ind>=0) && (ind<size(dim)));
        }

        template <typename T>
        bool InRange(const Images::Index<DIM,T>& ind) const {
            for (unsigned i=0;i<DIM;++i)
                if (!InRangeForDim(ind[i],i))
                    return false;
            return true;
        }

        //  Direct data access.

        virtual const Pixel* __restrict__ data() const { return pixels; }
        virtual       Pixel* __restrict__ data()       { return pixels; }

        virtual const Pixel* __restrict__ data_end() const { return pixels+size(); }
        virtual       Pixel* __restrict__ data_end()       { return pixels+size(); }

        virtual bool isStorageContiguous() const { return true; }

        void swap(BaseImage& im) {
            std::swap(shp,im.shp);
            std::swap(pixels,im.pixels);
        }

    private:

        //  Read an image.

        void Read(const char* name) {
            try {
                std::ifstream ifs(name,std::ios::binary);
                ifs >> *this;
            } catch(...) {
                throw UnknownNamedFileFormat(name);
            }
        }

        virtual self* clone() const { return new self(shp); }

        unsigned index(const Index& ind) const {
            unsigned pixind = ind(DIM);
            for (unsigned i=DIM-1;i>0;--i)
                pixind = pixind*shp(i)+ind(i);
            return pixind;
        }

        Shape  shp;
        Pixel* pixels;   
    };

    template <typename Scalar,unsigned DIM,typename Pixel>
    BaseImage<DIM,Pixel> operator*(const Scalar s,const BaseImage<DIM,Pixel>& im) {
        return im*s;
    }
    
    template <unsigned DIM,typename Pixel>
    void swap(BaseImage<DIM,Pixel>& im1,BaseImage<DIM,Pixel>& im2) {
        im1.swap(im2);
    }

    //! A concrete class for 1D Images.

    template <typename Pixel>
    class Image1D: public BaseImage<1,Pixel> {

        template <typename REP> friend struct pixel_iterator;
        template <typename REP> friend struct const_pixel_iterator;

        typedef BaseImage<1,Pixel> base;
        typedef Image1D            self;

    public:

        typedef Pixel            PixelType;
        typedef Pixel            value_type;
        typedef Images::Shape<1> Shape;

        using base::shape;

        //  Generic iterators.

        template <typename IteratorType>
        struct iterator: public Iterators::generic<typename IteratorType::template Info<self>::type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::type> Base;
            template <typename T> iterator(T t): Base(t) { }
        };

        template <typename IteratorType>
        struct const_iterator: public Iterators::generic<typename IteratorType::template Info<self>::const_type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::const_type> Base;
            template <typename T> const_iterator(T t): Base(t) { }
        };

        using base::begin;
        using base::end;

        Image1D(): base() { }
        Image1D(const Shape& s,const unsigned border): base(s,border) { }
        Image1D(const Dimension size): base(Shape(size)) { }
        Image1D(const Dimension size,Pixel* const data): base(Shape(size),data) { }

        template <typename Expr>
        Image1D(const Expr& expr): base(expr) { }

        template <typename Pixel2>
        Image1D(const Image1D<Pixel2>& im): base(im) { }

        virtual ~Image1D() { }

        virtual const std::type_info& type() const { return typeid(Image1D<Pixel>); }

        //@{
        //! Resizing the image..

        using base::resize;
        void resize(const Dimension dim)  { base::resize(Shape(dim));   }

        //@}

        //  Indexing the image.

        using base::InRange;

        bool InRange(const Images::Coord i) const                      { return base::InRangeForDim(i,0); }

        using base::operator();

        Pixel operator()(const unsigned i) {
            return base::operator()(i);
        }

        Pixel operator()(const unsigned i) const {
            return base::operator()(i);
        }

        // These are needed here even though those are already defined in the BaseImage class because
        // of the following one parameter template operators. This is not the case for higher dimension
        // images.

        Pixel& operator()(const iterator<domain>& it)       { return base::operator()(it); }
        Pixel  operator()(const iterator<domain>& it) const { return base::operator()(it); }

        Pixel  operator()(const const_iterator<domain>& it)       { return base::operator()(it); }
        Pixel  operator()(const const_iterator<domain>& it) const { return base::operator()(it); }

        Pixel& operator()(const domain_iterator<base::Dim>& it)       { return base::operator()(it); }
        Pixel  operator()(const domain_iterator<base::Dim>& it) const { return base::operator()(it); }

        Pixel  operator()(const domain_const_iterator<base::Dim>& it)       { return base::operator()(it); }
        Pixel  operator()(const domain_const_iterator<base::Dim>& it) const { return base::operator()(it); }

        iterator<domain> position(const Images::Coord i) const {
            const ImageBounds<base::Dim>& bounds(shape());
            MultiDimCounter<base::Dim> pos(bounds);
            pos[0] = i;
            return iterator<domain>(domain_iterator<base::Dim>(pos));
        }

        template <typename T>
        Pixel operator()(const Images::Index<1,T>& pos) const {
            typedef float T;
            typedef Images::Index<1,T> Position;

            const Position pi  = floor(pos);
            const Position ipp = pos-pi;

            const Position         pp[2]  = { Position(static_cast<T>(1))-ipp, ipp };
            const Images::Index<1> ind[2] = { pi, pi+sign(ipp) };

            T value = 0.0;
            for (unsigned d1=0;d1<2;++d1) {
                const Coord i = ind[d1](1);
                const T coeff1 = pp[d1](1);
                value += (*this)(i)*coeff1;
            }

            return value;
        }

        template <typename T>
        Pixel operator()(const Images::Index<1,T>& pos) {
            return const_cast<const Image1D&>(*this)(pos);
        }

        using base::operator=;

        Dimension dim() const { return shape(1); }
    };

    // A concrete class for bidimensional Images.

    template <typename Pixel>
    class Image2D: public BaseImage<2,Pixel> {

        template <typename REP> friend struct pixel_iterator;
        template <typename REP> friend struct const_pixel_iterator;

        typedef BaseImage<2,Pixel> base;
        typedef Image2D            self;

    public:

        typedef Pixel                PixelType;
        typedef Pixel                value_type;
        typedef Images::Shape<2>     Shape;
        typedef typename base::Index Index;

        using base::shape;
        using base::size;

        //  Generic iterators.

        template <typename IteratorType>
        struct iterator: public Iterators::generic<typename IteratorType::template Info<self>::type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::type> Base;
            template <typename T> iterator(T t): Base(t) { }
        };

        template <typename IteratorType>
        struct const_iterator: public Iterators::generic<typename IteratorType::template Info<self>::const_type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::const_type> Base;
            template <typename T> const_iterator(T t): Base(t) { }
        };

        using base::begin;
        using base::end;

        //  Constructors.

        Image2D(): base() { }
        Image2D(const Shape& s,const unsigned border): base(s,border) { }
        Image2D(const Dimension dimx,const Dimension dimy): base(Shape(Index(dimx,dimy))) { }
        Image2D(const Dimension dimx,const Dimension dimy,Pixel *const data): base(Shape(Index(dimx,dimy)),data) { }

        template <typename Expr>
        Image2D(const Expr& expr): base(expr) { }

        template <typename Pixel2>
        Image2D(const Image2D<Pixel2>& im): base(im) { }

        virtual ~Image2D() { }

        virtual const std::type_info &type() const { return typeid(Image2D<Pixel>); }

        //  Resizing functions.

        using base::resize;

        void resize(const Dimension dimx,const Dimension dimy) {
            base::resize(Shape(Index(dimx,dimy)));
        }

        //  Range checking operations.

        using base::InRange;

        bool InRange(const Images::Coord i,const Images::Coord j) const {
            return base::InRangeForDim(i,0) && base::InRangeForDim(j,1);
        }

        //  Indexing operations.

        using base::operator();

        Pixel& operator()(const unsigned i,const unsigned j) {
            return base::operator()(Index(i,j));
        }

        Pixel operator()(const unsigned i,const unsigned j) const {
            return base::operator()(Index(i,j));
        }

              Pixel& operator()(const Images::Coord i,const Images::Coord j)       { return base::operator()(Index(i,j)); }
        const Pixel& operator()(const Images::Coord i,const Images::Coord j) const { return base::operator()(Index(i,j)); }

        iterator<domain> position(const Images::Coord i,const Images::Coord j) const {
            const ImageBounds<base::Dim>& bounds(shape());
            MultiDimCounter<base::Dim> pos(bounds);
            pos[0] = i;
            pos[1] = j;
            return iterator<domain>(domain_iterator<base::Dim>(pos));
        }

        template <typename T>
        Pixel& operator()(const Images::Index<2,T>& pos) const {
            typedef float T;
            typedef Images::Index<2,T> Position;

            const Position pi  = floor(pos);
            const Position ipp = pos-pi;

            const Position         pp[2]  = { Position(static_cast<T>(1))-ipp, ipp };
            const Images::Index<2> ind[2] = { pi, pi+sign(ipp) };

            T value = 0.0;
            for (unsigned d1=0;d1<2;++d1) {
                const Coord i = ind[d1](1);
                const T coeff1 = pp[d1](1);
                for (unsigned d2=0;d2<2;++d2) {
                    const Coord j = ind[d2](2);
                    const T coeff2 = pp[d2](2);
                    value += (*this)(i,j)*coeff1*coeff2;
                }
            }

            return value;
        }

        template <typename T>
        Pixel& operator()(const Images::Index<2,T>& pos) {
            return const_cast<const Image2D&>(*this)(pos);
        }

        // Assignment

        using base::operator=;

        // Image size

        Dimension dimx() const { return size(0); }
        Dimension dimy() const { return size(1); }
    };

    // A concrete class for tridimensional images.

    template <typename Pixel>
    class Image3D: public BaseImage<3,Pixel> {

        template <typename REP> friend struct pixel_iterator;
        template <typename REP> friend struct const_pixel_iterator;

        typedef BaseImage<3,Pixel> base;
        typedef Image3D            self;

    public:

        typedef Pixel                PixelType;
        typedef Pixel                value_type;
        typedef typename base::Index Index;
        typedef Images::Shape<3>     Shape;

        using base::shape;
        using base::size;

        //  Generic iterators.

        template <typename IteratorType>
        struct iterator: public Iterators::generic<typename IteratorType::template Info<self>::type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::type> Base;
            template <typename T> iterator(T t): Base(t) { }
        };

        template <typename IteratorType>
        struct const_iterator: public Iterators::generic<typename IteratorType::template Info<self>::const_type> {
            typedef Iterators::generic<typename IteratorType::template Info<self>::const_type> Base;
            template <typename T> const_iterator(T t): Base(t) { }
        };

        using base::begin;
        using base::end;

        Image3D(): base() { }
        Image3D(const Shape& s,const unsigned border): base(s,border) { }
        Image3D(const Dimension dimx,const Dimension dimy,const Dimension dimz): base(Shape(Index(dimx,dimy,dimz))) { }
        Image3D(const Dimension dimx,const Dimension dimy,const Dimension dimz,Pixel *const data):
            base(Shape(Index(dimx,dimy,dimz)),data) { }

        template <typename Expr>
        Image3D(const Expr& expr): base(expr) { }

        template <typename Pixel2>
        Image3D(const Image3D<Pixel2>& im): base(im) { }

        virtual ~Image3D() { }

        virtual const std::type_info &type() const { return typeid(Image3D<Pixel>); }

        //  Resizing functions.

        using base::resize;

        void resize(const Dimension dimx,const Dimension dimy,const Dimension dimz) {
            base::resize(Shape(Index(dimx,dimy,dimz)));
        }

        //  Indexing operations.

        using base::InRange;

        bool InRange(const Images::Coord i,const Images::Coord j,const Images::Coord k) const {
            return base::InRangeForDim(i,0) && base::InRangeForDim(j,1) && base::InRangeForDim(k,2);
        }

        using base::operator();

        Pixel& operator()(const unsigned i,const unsigned j,const unsigned k) {
            return base::operator()(Index(i,j,k));
        }

        Pixel operator()(const unsigned i,const unsigned j,const unsigned k) const {
            return base::operator()(Index(i,j,k));
        }

        iterator<domain> position(const Images::Coord i,const Images::Coord j,const Images::Coord k) const {
            const ImageBounds<base::Dim>& bounds(shape());
            MultiDimCounter<base::Dim> pos(bounds);
            pos[0] = i;
            pos[1] = j;
            pos[2] = k;
            return iterator<domain>(domain_iterator<base::Dim>(pos));
        }

        template <typename T>
        Pixel operator()(const Images::Index<3,T>& pos) const {
            typedef Images::Index<3,T> Position;

            const Position pi  = floor(pos);
            const Position ipp = pos-pi;

            const Position         pp[2]  = { Position(static_cast<T>(1))-ipp, ipp };
            const Images::Index<3> ind[2] = { pi, pi+sign(ipp) };

            T value = 0.0;
            for (unsigned d1=0;d1<2;++d1) {
                const Coord i = ind[d1](1);
                const T coeff1 = pp[d1](1);
                for (unsigned d2=0;d2<2;++d2) {
                    const Coord j = ind[d2](2);
                    const T coeff2 = pp[d2](2);
                    for (unsigned d3=0;d3<2;++d3) {
                        const Coord k = ind[d3](3);
                        const T coeff3 = pp[d3](3);
                        value += (*this)(i,j,k)*coeff1*coeff2*coeff3;
                    }
                }
            }

            return value;

        }

        template <typename T>
        Pixel operator()(const Images::Index<3,T>& pos) {
            return const_cast<const Image3D&>(*this)(pos);
        }

        //  Assignment

        using base::operator=;

        //  Image size

        Dimension dimx() const { return size(0); }
        Dimension dimy() const { return size(1); }
        Dimension dimz() const { return size(2); }
    };

    //  Traits to find generically the image type associated with a
    //  Pixel and a dimension.

    template <unsigned DIM,typename Pixel>
    struct ImageType {
    };

    template <typename Pixel>
    struct ImageType<0,Pixel> {
        typedef Pixel& type;
        typedef Pixel  const_type;
    };

    template <typename Pixel>
    struct ImageType<1,Pixel> {
        typedef Image1D<Pixel>       type;
        typedef const Image1D<Pixel> const_type;
    };

    template <typename Pixel>
    struct ImageType<2,Pixel> {
        typedef Image2D<Pixel>       type;
        typedef const Image2D<Pixel> const_type;
    };

    template <typename Pixel>
    struct ImageType<3,Pixel> {
        typedef Image3D<Pixel>       type;
        typedef const Image3D<Pixel> const_type;
    };
}
