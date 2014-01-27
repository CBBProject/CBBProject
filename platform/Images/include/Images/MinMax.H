#pragma once

#include <cmath>
#include <Images/Image.H>
#include <Images/PixelsMinMax.H>

namespace Images {

    template <unsigned DIM,typename Pixel>
    Pixel min(const BaseImage<DIM,Pixel>& im) {
        Pixel m = im.data()[0];
        for (typename BaseImage<DIM,Pixel>::template const_iterator<pixel> i=im.begin();i!=im.end();++i)
            m = min(m,*i);
        return m;
    }

    template <unsigned DIM,typename Pixel>
    Pixel max(const BaseImage<DIM,Pixel>& im) {
        Pixel m = im.data()[0];
        for (typename BaseImage<DIM,Pixel>::template const_iterator<pixel> i=im.begin();i!=im.end();++i)
            m = max(m,*i);
        return m;
    }

    template <unsigned DIM,typename Pixel>
    void minmax(const BaseImage<DIM,Pixel>& im,Pixel& min,Pixel& max) {
        min = max = im.data()[0];
        for (typename BaseImage<DIM,Pixel>::template const_iterator<pixel> i=im.begin();i!=im.end();++i)
            minmax(*i,min,max);
    }
}
