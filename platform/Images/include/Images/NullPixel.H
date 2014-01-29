#pragma once

namespace Images {
    namespace Pixels {

        class Null {
        public:

            Null() {};

            //	Assignement operator.

            Null& operator=(const Null) { return *this; }

            //  Pixel operations.

            template <typename T> Null& operator+=(const T)      { return *this; }
            template <typename T> Null& operator-=(const T)      { return *this; }
            template <typename T> Null  operator+(const T) const { return *this; }
            template <typename T> Null  operator-(const T) const { return *this; }

            Null operator-() const { return *this; }

            template <typename S> Null& operator*=(const S)      { return *this; }
            template <typename S> Null  operator*(const S) const { return *this; }
        };

        template <typename S>
        inline Null operator*(const S,const Null& pix) { return pix; }
    }

    template <unsigned DIM>
    class BaseImage<DIM,Images::Pixels::Null>: public BaseDim<DIM> {

        typedef BaseImage self;

    public:

        typedef Images::Pixels::Null          PixelType;
        typedef typename BaseDim<DIM>::Shape  Shape;
        typedef typename BaseDim<DIM>::Domain Domain;
        typedef typename BaseDim<DIM>::Index  Index;

        static const unsigned Dim = DIM;
        // TODO: should be using BaseDim<DIM>::Dim;

        //  Generic iterators.

        template <typename IteratorType>
        struct iterator {
            template <typename T> iterator(T t) { }
            PixelType operator*() { return PixelType(); }
            void operator++()    const { }
            void operator++(int) const { }
            void operator--()    const { }
            void operator--(int) const { }
        };

        template <typename IteratorType>
        struct const_iterator {
            template <typename T> const_iterator(T t) { }
            PixelType operator*() { return PixelType(); }
            void operator++()    const { }
            void operator++(int) const { }
            void operator--()    const { }
            void operator--(int) const { }
        };

        Iterators::BEGIN<self&>       begin()       { return Iterators::BEGIN<self&>(*this);       }
        Iterators::BEGIN<const self&> begin() const { return Iterators::BEGIN<const self&>(*this); }

        Iterators::END<self&>       end()       { return Iterators::END<self&>(*this);       }
        Iterators::END<const self&> end() const { return Iterators::END<const self&>(*this); }

        BaseImage() { }
        BaseImage(const Shape&) { }

        BaseImage(const Shape&,const unsigned) { }
        BaseImage(const BaseImage&) { }

        //  Dimension.

        Dimension dimension() const { return Dim; }

        const std::type_info& pixel_id()   const { return typeid(PixelType); }
        virtual Dimension     pixel_size() const { return 0;                 }
    
#if 0
        //  Various size functions.
        //  Replace size() with using directive (TODO).

        virtual Dimension size()                  const { return base::size();    }
        virtual Dimension size(const Dimension d) const { return base::extent(d); }

        Shape shape() const { return Shape(base::shape()); }
#endif

        void resize(const BaseShape&)  { }
        void resize(const Shape&)      { }
        void resize(const Dimension[]) { }

        //  Indexing.

              PixelType operator()(const Index& ind)       { return PixelType(); }
        const PixelType operator()(const Index& ind) const { return PixelType(); }

              PixelType operator()(const Images::Coord ind[Dim])       { return PixelType(); }
        const PixelType operator()(const Images::Coord ind[Dim]) const { return PixelType(); }

              self operator()(const Range ind[Dim])       { return *this; }
        const self operator()(const Range ind[Dim]) const { return *this; }

              self operator()(const Domain& domain)       { return *this; }
        const self operator()(const Domain& domain) const { return *this; }
        
#if 0
        template <typename T>
        bool InRange(const Images::Index<DIM,T>& ind) const {
            for (unsigned i=0;i<DIM;++i)
                if ((ind[i]<0) || (ind[i]>=size(i)))
                    return false;
            return true;
        }
#endif

        //  Get the inner part of an image (ie remove some given border data).

        BaseImage interior(const unsigned border) { return *this; }

        template <typename IMAGE>
        BaseImage operator+(const IMAGE& I) const { return *this; }

        template <typename IMAGE>
        BaseImage operator-(const IMAGE& I) const { return *this; }

        template <typename IMAGE>
        BaseImage operator*(const IMAGE& I) const { return *this; }

        template <typename IMAGE>
        BaseImage operator/(const IMAGE& I) const { return *this; }
    };
}
