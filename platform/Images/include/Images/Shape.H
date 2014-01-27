#pragma once

#include <Utils/GeneralizedIterators.H>
#include <Images/Defs.H>
#include <Images/Index.H>
//#include <Images/MultiDimCounter.H>

namespace Images {

    //  Declarations related to the various iterators.

    template <unsigned DIM> class domain_iterator;
    template <unsigned DIM> class domain_const_iterator;

    //  Shapes

    struct BaseShape { };

    //! A class to describe the image sizes.

    template <unsigned N>
    class Shape: public BaseShape, public Index<N> {

        typedef Index<N> base;
        typedef Shape    self;

    public:

        typedef domain_iterator<N>       iterator;
        typedef domain_const_iterator<N> const_iterator;

        Shape():                     base()  { }
        Shape(const Index<N>& s):    base(s) { }
        Shape(const Dimension s[N]): base(s) { }

        bool operator==(const Shape& shape) const {
            for (unsigned i=1;i<=N;++i)
                if ((*this)(i)!=shape(i))
                    return false;
            return true;
        }

        bool operator!=(const Shape& shape) const { return !(*this==shape); }

        iterator       begin()       { return Iterators::BEGIN<self&>(*this);       }
        const_iterator begin() const { return Iterators::BEGIN<const self&>(*this); }

        iterator       end()       { return Iterators::END<self&>(*this);       }
        const_iterator end() const { return Iterators::END<const self&>(*this); }

        //@name Shape characteristics.

        unsigned size() const {
            unsigned sz = 1;
            for (unsigned i=1;i<=N;++i)
                sz *= (*this)(i);
            return sz;
        }

        unsigned size(const int i) const { return (*this)[i]; }

        //@name Shape manipulation
        //@{

        void resize(const Shape& s) {
            for (unsigned i=1;i<=N;++i)
                (*this)(i) = s(i);
        }

        Shape extend(const int n) const {
            base res;
            for (unsigned i=1;i<=N;++i)
                res(i) = (*this)(i)+n;
            return Shape(res);
        }

        Shape zoom(const unsigned n) const {
            base res;
            for (unsigned i=1;i<=N;++i)
                res(i) = (*this)(i)*n;
            return Shape(res);
        }

        //@}
    };
}
