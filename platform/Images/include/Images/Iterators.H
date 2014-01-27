#pragma once

///
///\section Iterators implementation
///
///     \note This file contains the basic implementations of the various iterators
///           that can be used for an image. Normal users do not need to know the content
///           of this file.
///
///     Image iterators take the form iterator<tag> or const_iterator<tag> where tag
///           can currently be pixel, domain, slice or line. These templated iterators
///           are implemented by inheriting from concrete types which are described in
///           this file. This file is utterly ugly as for some iterators, sepcialization
///           of class templates has been used a lot.
///
///     \internal
///

#include <Utils/Types.H>
#include <Images/Image.H>
#include <Images/MultiDimCounter.H>

namespace Images {

    using namespace Types;

    /// \subsection Domain iterators.

    template <unsigned DIM>
    struct domain_iterator: public MultiDimCounter<DIM> {
        typedef MultiDimCounter<DIM> base;

        typedef std::random_access_iterator_tag iterator_category;
        typedef typename base::value_type       value_type;
        typedef typename base::difference_type  difference_type;
        typedef const value_type*               pointer;
        typedef const value_type&               reference;

        template <typename T> domain_iterator(Iterators::BEGIN<T>& b): base(ImageBounds<DIM>(b.val.shape()))   { }
        template <typename T> domain_iterator(Iterators::END<T>& e):   base(ImageBounds<DIM>(e.val.shape()),0) { }

        domain_iterator(Iterators::BEGIN<Shape<DIM>&>& b): base(ImageBounds<DIM>(b.val.extend(-1)))   { }
        domain_iterator(Iterators::END<Shape<DIM>&>& e):   base(ImageBounds<DIM>(e.val.extend(-1)),0) { }

        domain_iterator(Iterators::BEGIN<const Shape<DIM>&>& b): base(ImageBounds<DIM>(b.val.extend(-1)))   { }
        domain_iterator(Iterators::END<const Shape<DIM>&>& e):   base(ImageBounds<DIM>(e.val.extend(-1)),0) { }

        explicit domain_iterator(const base& c): base(c) { }                       
      
        const base& operator*() const { return static_cast<const base&>(*this); }

        const domain_iterator<DIM>
        neighbor(const unsigned DIR,const int offset) const { return domain_iterator<DIM>(base::neighbor(DIR,offset)); }
    };

    template <unsigned DIM>
    struct domain_const_iterator: public MultiDimCounter<DIM> {
        typedef MultiDimCounter<DIM> base;

        typedef std::random_access_iterator_tag iterator_category;
        typedef typename base::value_type       value_type;
        typedef typename base::difference_type  difference_type;
        typedef const value_type*               pointer;
        typedef const value_type&               reference;

        template <typename T> domain_const_iterator(const Iterators::BEGIN<T>& b): base(ImageBounds<DIM>(b.val.shape()))   { }
        template <typename T> domain_const_iterator(const Iterators::END<T>& e):   base(ImageBounds<DIM>(e.val.shape()),0) { }

        domain_const_iterator(const Iterators::BEGIN<Shape<DIM>&>& b): base(ImageBounds<DIM>(b.val.extend(-1)))   { }
        domain_const_iterator(const Iterators::END<Shape<DIM>&>& e):   base(ImageBounds<DIM>(e.val.extend(-1)),0) { }

        domain_const_iterator(const Iterators::BEGIN<const Shape<DIM>&>& b): base(ImageBounds<DIM>(b.val.extend(-1)))   { }
        domain_const_iterator(const Iterators::END<const Shape<DIM>&>& e):   base(ImageBounds<DIM>(e.val.extend(-1)),0) { }

        explicit domain_const_iterator(const base& c): base(c) { }

        //  A const iterator can be initialized with a non const one.

        domain_const_iterator(const domain_iterator<DIM>& it):base(it) { }

        const base& operator*() const { return static_cast<const base&>(*this); }
        
        const domain_const_iterator
        neighbor(const unsigned DIR,const int offset) const { return domain_const_iterator(base::neighbor(DIR,offset)); }
    };

    ///  \subsection Pixel iterators.

    template <typename Pixel>
    struct Iterator {
        Iterator(Pixel* it): iter(it) { }

        Iterator& operator++() {
            ++iter;
            return *this;
        }

        Iterator& operator--() {
            --iter;
            return *this;
        }

        Pixel& operator*()  const { return *iter; }
        Pixel* operator->() const { return iter;  }

        Iterator operator++(int) { return Iterator(iter++); }
        Iterator operator--(int) { return Iterator(iter--); }

        bool operator==(const Iterator& it) const { return it.iter==iter; }
        bool operator!=(const Iterator& it) const { return it.iter!=iter; }

        Pixel* iter;
    };

    template <typename IMAGEREP>
    struct pixel_iterator: public Iterator<typename IMAGEREP::PixelType>  {
        typedef Iterator<typename IMAGEREP::PixelType> base;
        template <typename T> pixel_iterator(Iterators::BEGIN<T>& b): base(b.val.data())     { }
        template <typename T> pixel_iterator(Iterators::END<T>& e):   base(e.val.data_end()) { }

        explicit pixel_iterator(const base& c):base(c) { }
    };

    template <typename IMAGEREP>
    struct pixel_const_iterator: Iterator<const typename IMAGEREP::PixelType>  {
        typedef Iterator<const typename IMAGEREP::PixelType> base;
        template <typename T> pixel_const_iterator(const Iterators::BEGIN<T>& b): base(b.val.data())     { }
        template <typename T> pixel_const_iterator(const Iterators::END<T>& e):   base(e.val.data_end()) { }

        explicit pixel_const_iterator(const base& c):base(c) { }

        //  A const iterator can be initialized with a non const one.

        pixel_const_iterator(const pixel_iterator<IMAGEREP>& it): base(it) { }
    };
}
