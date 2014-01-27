#ifndef MATH_SCALARTYPE_H
#define MATH_SCALARTYPE_H

#include <complex>

namespace Maths {
    namespace LinearAlgebra {
        namespace Details {
            template <typename T>
            struct Scalar {
                typedef typename T::scalar_type scalar_type;
            };

            template <>
            struct Scalar<double> {
                typedef double scalar_type;
            };

            template <>
            struct Scalar<float> {
                typedef float scalar_type;
            };

            template <>
            struct Scalar<bool> {
                typedef bool scalar_type;
            };

            template <>
            struct Scalar<short> {
                typedef short scalar_type;
            };

            template <>
            struct Scalar<int> {
                typedef int scalar_type;
            };

            template <>
            struct Scalar<unsigned> {
                typedef unsigned scalar_type;
            };

            template <>
            struct Scalar<unsigned char> {
                typedef unsigned char scalar_type;
            };

            template <>
            struct Scalar<unsigned short> {
                typedef unsigned short scalar_type;
            };

            template <typename T>
            struct Scalar<std::complex<T> > {
                typedef std::complex<T> scalar_type;
            };
        }
    }
}

#endif //! MATH_SCALARTYPE_H
