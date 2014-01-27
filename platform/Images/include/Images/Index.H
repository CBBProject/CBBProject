#pragma once

#include <cmath> //    For round
#include <Maths/Arith.H>
#include <Maths/Vectors.H>

namespace Images {

    template <unsigned DIM,typename T=Coord> struct Index { };

    template <typename T>
    struct Index<1,T>: public Maths::Vectors<T,1> {
        typedef Maths::Vectors<T,1> base;

        using base::operator=;

        Index() { }
        Index(const base& b): base(b)   { }

        template <typename U>
        Index(const Maths::Vectors<U,1>& v): base(v) { }

        Index(const T ind[1]) { (*this)(1) = ind[0]; }
        Index(const T ind)    { (*this)(1) = ind;    }
    };

    template <typename T>
    struct Index<2,T>: public Maths::Vectors<T,2> {
        typedef Maths::Vectors<T,2> base;

        using base::operator=;

        Index() { }
        explicit Index(const T ind) { *this = ind; }

        template <typename VECTOR>
        explicit Index(const VECTOR& V): base(2) {
            for (unsigned i=0,j=1;i<2;i=j++)
                (*this)[i] = static_cast<T>(V(j));
        }

        Index(const base& b): base(b) { }

        template <typename U>
        Index(const Maths::Vectors<U,2>& v) { *this = v; }

        Index(const T ind[2]) {
            (*this)(1) = ind[0];
            (*this)(2) = ind[1];
        }

        Index(const T& t1,const T& t2) {
            (*this)(1) = t1;
            (*this)(2) = t2;
        }
    };

    template <typename T>
    struct Index<3,T>: public Maths::Vectors<T,3> {
        typedef Maths::Vectors<T,3> base;

        using base::operator=;

        Index() { }
        explicit Index(const T ind) { *this = ind; }
        Index(const base& b): base(b) { }

        template <typename U>
        Index(const Maths::Vectors<U,3>& v): base(v) { }

        Index(const T ind[3]) {
            (*this)(1) = ind[0];
            (*this)(2) = ind[1];
            (*this)(3) = ind[2];
        }

        Index(const T& t1,const T& t2,const T& t3) {
            (*this)(1) = t1;
            (*this)(2) = t2;
            (*this)(3) = t3;
        }
    };

    template <typename T>
    struct Index<4,T>: public Maths::Vectors<T,4> {
        typedef Maths::Vectors<T,4> base;

        using base::operator=;

        Index() { }
        explicit Index(const T ind) { *this = ind; }
        Index(const base& b): base(b) { }

        template <typename U>
        Index(const Maths::Vectors<U,4>& v): base(v) { }

        Index(const T ind[4]) {
            (*this)(1) = ind[0];
            (*this)(2) = ind[1];
            (*this)(3) = ind[2];
            (*this)(4) = ind[5];
        }

        Index(const T& t1,const T& t2,const T& t3,const T& t4) {
            (*this)(1) = t1;
            (*this)(2) = t2;
            (*this)(3) = t3;
            (*this)(4) = t4;
        }
    };

    template <typename T>
    struct Index<5,T>: public Maths::Vectors<T,5> {
        typedef Maths::Vectors<T,5> base;

        using base::operator=;

        Index() { }
        explicit Index(const T ind) { *this = ind; }
        Index(const base& b): base(b) { }

        template <typename U>
        Index(const Maths::Vectors<U,4>& v): base(v) { }

        Index(const T ind[5]) {
            (*this)(1) = ind[0];
            (*this)(2) = ind[1];
            (*this)(3) = ind[2];
            (*this)(4) = ind[3];
            (*this)(5) = ind[4];
        }

        Index(const T& t1,const T& t2,const T& t3,const T& t4,const T& t5) {
            (*this)(1) = t1;
            (*this)(2) = t2;
            (*this)(3) = t3;
            (*this)(4) = t4;
            (*this)(5) = t5;
        }
    };

    //  These are reduntant with Vectors....
    //  But necessary as otherwise the compiler does not recognize some simple operations.
    //  There are some days I hate C++.

    template <unsigned DIM,typename T>
    Index<DIM,T> operator+(const Index<DIM,T>& i1,const Index<DIM,T>& i2) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = i1[i]+i2[i];
        return res;
    }

    template <unsigned DIM,typename T>
    Index<DIM,T> operator-(const Index<DIM,T>& i1,const Index<DIM,T>& i2) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = i1[i]-i2[i];
        return res;
    }

    template <unsigned DIM,typename S,typename T>
    Index<DIM,T> operator*(const S s,const Index<DIM,T>& i1) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = s*i1[i];
        return res;
    }

    template <unsigned DIM,typename T>
    Index<DIM,T> abs(const Index<DIM,T>& i1) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = std::abs(i1[i]);
        return res;
    }

    template <unsigned DIM,typename T>
    Index<DIM,T> sign(const Index<DIM,T>& i1) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = Maths::sign(i1[i]);
        return res;
    }

    template <unsigned DIM,typename T>
    Index<DIM,T> round(const Index<DIM,T>& i1) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = ::round(i1[i]);
        return res;
    }

    template <unsigned DIM,typename T>
    Index<DIM,T> floor(const Index<DIM,T>& i1) {
        Index<DIM,T> res;
        for (unsigned i=0;i<DIM;++i)
            res[i] = ::floor(i1[i]);
        return res;
    }
}
