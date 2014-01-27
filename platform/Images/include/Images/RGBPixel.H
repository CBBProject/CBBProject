#pragma once

#include <iostream>
#include <limits>

#include <Utils/Cpu.H>
#include <Utils/IOUtils.H>
#include <Images/PixelsMinMax.H>
#include <Maths/Vectors.H>

namespace Images {
    namespace Pixels {
        template <typename T,unsigned CHANNELS> class RGB;
    }
}

namespace Images {
    namespace Pixels {

        class RGBtraits {
        public:
            static const float RedFactor;
            static const float GreenFactor;
            static const float BlueFactor;
        };

        template <typename T,unsigned CHANNELS>
        class RGB: public Maths::Vectors<T,CHANNELS> {
        public:

            typedef Maths::Vectors<T,CHANNELS> base;

            //  Enum cannot be used as we need those values to be unsigned.

            static const unsigned RED   = 0;
            static const unsigned GREEN = 1;
            static const unsigned BLUE  = 2;

        private:
            
            void Init(const T& r,const T& g,const T& b) {
                (*this)[RED]   = r;
                (*this)[GREEN] = g;
                (*this)[BLUE]  = b;
            }

            void Init(const RGB& p) {
                Init(p[RED],p[GREEN],p[BLUE]);
            }

        public:

            typedef T ChannelType;

            RGB(const T& r,const T& g,const T& b) { Init(r,g,b); }
            RGB(const T& g)                       { Init(g,g,g); }
            RGB(const RGB& p)                     { Init(p);     }
            RGB() {};

            operator T() const { return grey(); }

            template <typename U>
            operator RGB<U>() const {
                return RGB<U>(*this);
            }

            //  For computations results.

            template <typename U>
            explicit RGB(const U& v) {
                const T g = static_cast<T>(v);
                Init(g,g,g);
            }

            template <typename U>
            explicit RGB(const RGB<U>& v) {
                const T r = static_cast<T>(v.red());
                const T g = static_cast<T>(v.green());
                const T b = static_cast<T>(v.blue());
                Init(r,g,b);
            }

            //	Assignement operator.

            RGB& operator=(const RGB& p) {
                Init(p);
                return *this;
            }

            //  Pixel operations.

            RGB& operator+=(const RGB& p) {
                red()   += p.red();
                green() += p.green();
                blue()  += p.blue();

                return *this;
            }

            RGB operator+(const RGB& p) const {
                RGB res(*this);
                return res+=p;
            }

            RGB& operator-=(const RGB& p) {
                red()   -= p.red();
                green() -= p.green();
                blue()  -= p.blue();

                return *this;
            }

            RGB operator-(const RGB& p) const {
                RGB res(*this);
                return res-=p;
            }

            RGB operator-() const {
                RGB res;
                res.red()   = -red();
                res.green() = -green();
                res.blue()  = -blue();
                return res;
            }

            template <typename S>
            RGB& operator*=(const S fact) {
                red()   = static_cast<T>(fact*red());
                green() = static_cast<T>(fact*green());
                blue()  = static_cast<T>(fact*blue());

                return *this;
            }

            template <typename S>
            RGB operator*(const S p) const {
                RGB res(*this);
                return res *= p;
            }

            template <typename S>
            RGB& operator/=(const S fact) {
                return operator*=(T(1)/fact);
            }

            template <typename S>
            RGB operator/(const S p) const {
                RGB res(*this);
                return res /= p;
            }

            //	Color accessors.

                  T& red()         { return (*this)[RED];   }
            const T& red()   const { return (*this)[RED];   }
                  T& green()       { return (*this)[GREEN]; }
            const T& green() const { return (*this)[GREEN]; }
                  T& blue()        { return (*this)[BLUE];  }
            const T& blue()  const { return (*this)[BLUE];  }

            const T  luminance()  const { return grey(); }

            const T  grey()  const {
                return static_cast<T>(RGBtraits::RedFactor*red()+
                                      RGBtraits::GreenFactor*green()+
                                      RGBtraits::BlueFactor*blue());
            }

            bool is_grey() const { return (red()==green()) && (red()==blue()); }

            bool operator==(const RGB& p) const {
                return (red()==p.red()) && (green()==p.green()) && (blue==p.blue());
            }

            bool operator!=(const RGB& p) const {
                return !operator==(p);
            }

        #if 0
            //  The above formula is truncated instead of rounded when using
            //  eg unsigned char data. The following will allow the user to specify
            //  the return type he wants and to deal with it.

            //  Another idea would be have a trait on T to define an additive offset.

            template <typename U>
            const U grey() const {
                return static_cast<U>(RGBtraits::RedFactor*red()+
                                      RGBtraits::GreenFactor*green()+
                                      RGBtraits::BlueFactor*blue());
            }
        #endif
        };

        template <typename T,typename S>
        inline RGB<T> operator*(const S s,const RGB<T>& pix) {
            return pix*s;
        }

        template <typename T> inline
        std::ostream& operator<<(std::ostream& os,const RGB<T>& rgb) {
            return os << rgb.red() << ' ' << rgb.green() << ' ' << rgb.blue();
        }

        inline
        std::ostream& operator<<(std::ostream& os,const RGB<unsigned char>& p) {
            const RGB<unsigned> rgb(p);
            return os << rgb.red() << ' ' << rgb.green() << ' ' << rgb.blue();
        }

        template <typename T> inline
        std::istream& operator>>(std::istream &is,RGB<T>& rgb) {
            using namespace io_utils;
            return is >> numeric(rgb.red()) >> numeric(rgb.green()) >> numeric(rgb.blue());
        }
    }
}

namespace std {
    template <typename T,unsigned CHANNELS>
    struct numeric_limits<Images::Pixels::RGB<T,CHANNELS> >: public numeric_limits<T> {
        typedef Images::Pixels::RGB<T,CHANNELS> RGB;
        static RGB min() { return RGB(numeric_limits<T>::min()); }
        static RGB max() { return RGB(numeric_limits<T>::max()); }
    };
}

namespace Cpu {
    template <typename T>
    inline void ChangeEndianness(Images::Pixels::RGB<T>& p) {
        Cpu::ChangeEndianness<T>(p.red());
        Cpu::ChangeEndianness<T>(p.green());
        Cpu::ChangeEndianness<T>(p.blue());
    }
}
