#pragma once

#include <iostream>
#include <cmath>

namespace Maths {
    
    template <typename TYPE,unsigned DIM>
    class Vectors {
    public:

        Vectors() { }

        Vectors(const Vectors& V) { std::copy(V.elmts,V.elmts+DIM,elmts); }

              TYPE& operator()(const unsigned int i)       { return elmts[i-1]; }
        const TYPE& operator()(const unsigned int i) const { return elmts[i-1]; }

              TYPE& operator[](const unsigned int i)       { return elmts[i]; }
        const TYPE& operator[](const unsigned int i) const { return elmts[i]; }

        unsigned dim() const { return DIM; }

        Vectors& operator=(const Vectors& V) {
            for (unsigned i=1;i<=dim();++i)
                (*this)(i) = V(i);
            return *this;
        }

        Vectors& operator=(const TYPE val) {
            for (unsigned i=1;i<=dim();++i)
                (*this)(i) = val;
            return *this;
        }

        Vectors& operator+=(const Vectors& V) {
            for (unsigned i=1;i<=dim();++i)
                (*this)(i) += V(i);
            return *this;
        }

        Vectors operator+(const Vectors& V) const {
            Vectors res = *this;
            return res += V;
        }

        Vectors operator-() const {
            Vectors res = *this;
            for (unsigned i=1;i<=dim();++i)
                res(i) = -(*this)(i);
            return res;
        }

        Vectors& operator-=(const Vectors& V) {
            for (unsigned i=1;i<=dim();++i)
                (*this)(i) -= V(i);
            return *this;
        }

        Vectors& operator*=(const TYPE val) {
            for (unsigned i=1;i<=dim();++i)
                (*this)(i) *= val;
            return *this;
        }

        Vectors& operator/=(const TYPE val) {
            *this *= 1.0/val;
            return *this;
        }

        Vectors operator*(const TYPE val) const {
            Vectors res = *this;
            return res *= val;
        }

        Vectors operator/(const TYPE val) const {
            Vectors res = *this;
            return res /= val;
        }

    private:

        TYPE    elmts[DIM];
    };

    template <typename TYPE,unsigned DIM>
    inline Vectors<TYPE,DIM> operator*(const TYPE s,const Vectors<TYPE,DIM>& V) {
        return V.operator*(s);
    }

    template <typename TYPE,unsigned DIM>
    inline TYPE dotprod(const Vectors<TYPE,DIM>& V1,const Vectors<TYPE,DIM>& V2) {
        assert(V1.dim()==V2.dim());
        TYPE dp = 0;
        for (unsigned i=1;i<=V1.dim();++i)
            dp += V1(i)*V2(i);
        return dp;
    }

    template <typename TYPE,unsigned DIM>
    inline TYPE norm(const Vectors<TYPE,DIM>& V) {
        return std::sqrt(dotprod(V,V));
    }

    template <typename TYPE,unsigned DIM>
    std::istream& operator>>(std::istream& is,Vectors<TYPE,DIM>& V) {
        for (unsigned i=1;i<=V.dim();++i)
            is >> V(i);
        return is;
    }

    template <typename TYPE,unsigned DIM>
    std::ostream& operator<<(std::ostream& os,const Vectors<TYPE,DIM>& V) {
        for (unsigned i=1;i<=V.dim();++i)
            os << V(i) << ' ';
        return os;
    }
}
