#pragma once

#include <Images/RGBPixel.H>
#include <Image.H>

namespace Images {

    static const float highColorValue = 255.0;

    template <typename T>
    static float PixelScale(const T min,const T max) {
        return highColorValue/(max-min);
    }

    template <typename T>
    static float PixelScale(const Images::Pixels::RGB<T>& min,const Images::Pixels::RGB<T>& max) {
        const Images::Pixels::RGB<T>& diff = max-min;
        return highColorValue/(Maths::max(diff.red(),diff.green(),diff.blue()));
    }

    template <typename T>
    struct IntensityMap {

        IntensityMap(): base(std::numeric_limits<T>::min()),
                        fact(PixelScale(std::numeric_limits<T>::min(),std::numeric_limits<T>::max())) { }

        //  This is templated so that people can give scalar values for pixels
        //  as well as pixel (ie vectorial) values.

        template <typename U>
        IntensityMap(const U& min,const U& max): base(min), fact(PixelScale(min,max)) { }

        template <typename IMAGE>
        IntensityMap(const IMAGE& I) {
            typedef typename IMAGE::PixelType Pixel;
            Pixel min,max;
            minmax(I,min,max);
            base = min;
            fact = PixelScale(min,max);
        }

        Images::Pixels::RGB<unsigned char> operator()(const T& p) const { return Images::Pixels::RGB<unsigned char>(fact*(p-base)); }

    private:
        
        T     base;
        float fact;
    };
}
