#pragma once

namespace Types {

    //  Different void types.
    //  This is useful when deriving from types since one cannot derive
    //  multiple times from a given class. So this can be used for default
    //  template parameters.

    template <unsigned N> struct Void {
        typedef void iterator;
        typedef void const_iterator;
        typedef void size_type;
    };

    //  Idea taken from STL/type_traits.h

    template <bool Truth> struct BoolType {};
    typedef BoolType<true>  TrueType;
    typedef BoolType<false> FalseType;

    //  Templated constness.

    //  Create a const type corresponding to a type.

    template <typename T>
    struct Const {
        typedef const T type;
    };

    template <typename T>
    struct Const<const T> {
        typedef const T type;
    };

    //  Create a non-const type corresponding to a type.

    template <typename T>
    struct NoConst {
        typedef T type;
    };

    template <typename T>
    struct NoConst<const T> {
        typedef T type;
    };

    //  Create a type that has the same constness as another type.

    template <typename T,typename M>
    struct SameConstnessAs {
        typedef T type;
    };

    template <typename T,typename M> 
    struct SameConstnessAs<T,const M> {
        typedef const T type;
    };
    
    template <typename T,typename M>
    struct SameConstnessAs<const T,const M> {
        typedef const T type;
    };

    template <typename T,typename M>
    struct SameConstnessAs<const T,M> {
        typedef T type;
    };

    //  Transfert an iterator or const_iterator from one template instantiation
    //  to an another.

    template <typename C1,typename C2,typename Iter>
    struct SameIteratorAs {
        typedef void iterator;
    };

    //  Specialisation for standard containers.

    template <typename T1,typename T2,typename A1,typename A2,template <typename T,typename A> class C>
    struct SameIteratorAs<C<T1,A1>,C<T2,A2>,typename C<T1,A1>::iterator> {
        typedef typename C<T2,A2>::iterator iterator;
    };

    template <typename T1,typename T2,typename A1,typename A2,template <typename T,typename A> class C>
    struct SameIteratorAs<C<T1,A1>,C<T2,A2>,typename C<T1,A1>::const_iterator> {
        typedef typename C<T2,A2>::const_iterator iterator;
    };

#if 0
    // Note: This currently cannot be made to work due to subtle C++ rules.

    //  Specialisation for containers with extended iterators (eg Image[123]D).

    template <template <typename T> class C,typename T1,typename T2,typename type>
    struct SameIteratorAs<C<T1>,C<T2>,typename C<T1>::iterator<type> > {
        typedef typename C<T2>::iterator<type> iterator;
    };

    template <template <typename T> class C,typename T1,typename T2,typename type>
    struct SameIteratorAs<C<T1>,C<T2>,typename C<T1>::const_iterator<type> > {
        typedef typename C<T2>::const_iterator<type> iterator;
    };

    //  Specialisation for containers with extended iterators with 2 parameters
    //  (eg blitz).

    template <template <unsigned,typename T> class C,unsigned N,typename T1,typename T2,typename type>
    struct SameIteratorAs<C<N,T1>,C<N,T2>,typename C<N,T1>::iterator<type> > {
        typedef typename C<N,T2>::iterator<type> iterator;
    };

    template <template <unsigned,typename T> class C,unsigned N,typename T1,typename T2,typename type>
    struct SameIteratorAs<C<N,T1>,C<N,T2>,typename C<N,T1>::const_iterator<type> > {
        typedef typename C<N,T2>::const_iterator<type> iterator;
    };
#endif
}
