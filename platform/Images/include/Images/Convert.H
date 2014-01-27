#ifndef IMAGE_CONVERT_H
#define IMAGE_CONVERT_H

#include <Image.H>

namespace Images {
    
    template <typename Pixel,unsigned Dimension>
    const typename ImageType<Dimension,Pixel>::type&
    concretize(const Image* image) {
        typedef typename ImageType<Dimension,Pixel>::type ResultImage;
        return *static_cast<const ResultImage*>(image);
    }

    template <typename Pixel,unsigned Dimension>
    typename ImageType<Dimension,Pixel>::type&
    concretize(Image* image) {
        typedef typename ImageType<Dimension,Pixel>::type ResultImage;
        return *static_cast<ResultImage*>(image);
    }

    template <typename PixelOut,typename PixelIn,unsigned Dimension>
    typename ImageType<Dimension,PixelOut>::type
    convert(const typename ImageType<Dimension,PixelIn>::type& image) {
        typedef typename ImageType<Dimension,PixelOut>::type ResultImage;
        return static_cast<ResultImage>(image);
    }

    template <typename PixelOut,typename PixelIn>
    Image2D<PixelOut>
    convert(const Image2D<PixelIn>& image) {
        typedef Image2D<PixelOut> ResultImage;
        return static_cast<ResultImage>(image);
    }

    template <typename PixelOut,typename PixelIn,unsigned Dimension>
    typename ImageType<Dimension,PixelOut>::type
    convert(const Image* image) {
        typedef typename ImageType<Dimension,PixelOut>::type ResultImage;
        return convert<PixelOut,PixelIn,Dimension>(concretize<PixelIn,Dimension>(image));
    }

    template <typename Pixel,unsigned Dimension,typename Switcher>
    typename ImageType<Dimension,Pixel>::type
    convert1(const Image* const image) {
        typedef typename ImageType<Dimension,Pixel>::type ResultImage;

        Switcher sw;
        return convert<Pixel>(sw(image));
    }

#if 0
    class Conversions {
    private:

        typedef std::pair<InfoTag,
    };

    template <typename Pixel,unsigned Dimension>
    typename ImageType<Dimension,Pixel>::type
    NaturalType(const Image& im)
    {
        typedef typename ImageType<Dimension,Pixel>::type  ImageIn;
        return dynamic_cast<const ImageIn&>(im);
    }
#endif
}

#endif  //  ! IMAGE_CONVERT_H
