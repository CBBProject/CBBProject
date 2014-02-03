#pragma once

#include <algorithm>
#include <cmath>

namespace Maths {

    template <typename TYPE> inline TYPE sqr(const TYPE& x) { return x*x;      }
    template <typename TYPE> inline TYPE cub(const TYPE& x) { return x*sqr(x); }

    namespace LinearAlgebra {
        inline int    dotprod(const int x,const int y)       { return x*y;          }
        inline double dotprod(const double x,const double y) { return x*y;          }
        inline double norm2S(const double x)                 { return dotprod(x,x); }
        inline double norm2(const double x)                  { return fabs(x);      }
    }

    template <typename TYPE>
    inline int sign(const TYPE& x) {
        return (x>0.0) ? 1 : (x<0.0) ? -1 : 0;
    }

    //  Remove those as soon as tr1 implements this and is ubiquitous.

    inline float  floor(const float f)  { return ::floor(static_cast<double>(f)); }
    inline double floor(const double d) { return ::floor(d); }
        
    template <typename TYPE>
    inline TYPE max(const TYPE& x,const TYPE& y,const TYPE& z) { return std::max(x,std::max(y,z)); }

    template <typename TYPE>
    inline TYPE min(const TYPE& x,const TYPE& y,const TYPE& z) { return std::min(x,std::min(y,z)); }

    template <typename TYPE>
    void minmax(const TYPE& val,TYPE& min,TYPE& max) {
        if (val<min)
            min = val;
        else if (val>max)
            max = val;
    }

    //  Factorial function.

    template <unsigned N>
    inline unsigned fact() {
        return N*fact<N-1>();
    }

    template<>
    inline unsigned fact<0>() {
        return 1;
    }

    //  Power (integer) function.

    template <unsigned N,typename T> T pow(const T);

    template <unsigned N,typename T,unsigned even>
    struct POW {
        static T compute(const T x);
    };

    template <unsigned N,typename T>
    struct POW<N,T,0> {
        static T compute(const T x) {
            return sqr(pow<N/2,T>(x));
        }
    };

    template <typename T>
    struct POW<0,T,0> {
        static T compute(const T x) {
            return 1;
        }
    };

    template <unsigned N,typename T>
    struct POW<N,T,1> {
        static T compute(const T x) {
            return x*sqr(pow<(static_cast<unsigned>(N/2)),T>(x));
        }
    };
    
    template <unsigned N,typename T>
    inline
    T pow(const T x) {
        return POW<N,T,N%2>::compute(x);
    }

    template <int N>
    struct Sign: public Sign<N/2> { };

    template <> struct Sign<-1> { static const int value = -1; };
    template <> struct Sign< 0> { static const int value =  0; };
    template <> struct Sign< 1> { static const int value =  1; };
}
