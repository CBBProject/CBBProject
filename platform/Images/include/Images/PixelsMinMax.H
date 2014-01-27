#pragma once

namespace Images {
    namespace Pixels {
        template <typename T,unsigned CHANNELS=3> class RGB;
    }

    template <typename T>
    T min(const T& v1,const T& v2) {
        return (v1<v2) ? v1 : v2;
    }

    template <typename T,unsigned CHANNELS>
    Images::Pixels::RGB<T,CHANNELS> min(const Images::Pixels::RGB<T,CHANNELS>& p1,const Images::Pixels::RGB<T,CHANNELS>& p2) {
        Images::Pixels::RGB<T,CHANNELS> min;
        for (unsigned i=1;i<=CHANNELS;++i)
            min(i) = (p1(i)<p2(i)) ? p1(i) : p2(i);
        return min;
    }

    template <typename T>
    T max(const T& v1,const T& v2) {
        return (v1<v2) ? v2 : v1;
    }

    template <typename T,unsigned CHANNELS>
    Images::Pixels::RGB<T,CHANNELS> max(const Images::Pixels::RGB<T,CHANNELS>& p1,const Images::Pixels::RGB<T,CHANNELS>& p2) {
        Images::Pixels::RGB<T,CHANNELS> max;
        for (unsigned i=1;i<=CHANNELS;++i)
            max(i) = (p1(i)<p2(i)) ? p2(i) : p1(i);
        return max;
    }

    template <typename T>
    T minmax(const T& v,T& min,T& max) {
        if (v<min)
            min = v;
        else if (v>max)
            max = v;
    }

    template <typename T,unsigned CHANNELS>
    void minmax(const Images::Pixels::RGB<T,CHANNELS>& p,Images::Pixels::RGB<T,CHANNELS>& min,Images::Pixels::RGB<T,CHANNELS>& max) {
        for (unsigned i=1;i<=CHANNELS;++i)
            if (p(i)<min(i))
                min(i) = p(i);
            else if (p(i)>max(i))
                max(i) = p(i);
    }
}
