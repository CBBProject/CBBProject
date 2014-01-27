#ifndef IMAGE_IMAGEFILTERS_H
#define IMAGE_IMAGEFILTERS_H

#include <Images/Image.H>
#include <Images/Iterators.H>
#include <Images/Signal.H>
#include <Utils/Types.H>

namespace Images {

    template <unsigned N,typename IMAGE,typename FILTER,typename OUT>
    inline void Apply1DFilter(const IMAGE& image,OUT& result,FILTER& filter) {

        typename IMAGE::template const_iterator<line<N> > i = image.begin();
        typename OUT::template iterator<line<N> >         o = result.begin();

        const unsigned dim = i.dim();           //  image.size(N)
        const unsigned istride = i.stride();    //  image.stride(N)
        const unsigned ostride = o.stride();    //  result.stride(N)

        //  Some filters depend on the image size, so pass the size through the initialize function
        //  which is void in most classic cases.

        filter.initialize(dim);

        //  Is there a generic N dimensional easy way to walk through all image lines in dimension N.
        //  If yes, it would be easy to generate a dynamic version of this function that would not
        //  require N to be a template parameter. Maybe a solution based on MultiDimCounter could be
        //  done.

        typedef typename IMAGE::PixelType PixelIn;
        typedef typename OUT::PixelType   PixelOut;

        while (i!=image.end()) {
            const Images::Signal1D<const PixelIn> sin(dim,istride,reinterpret_cast<const PixelIn*>((*i).data()));
                  Images::Signal1D<PixelOut>      sout(dim,ostride,reinterpret_cast<PixelOut*>((*o).data()));
            filter(sin,sout);
            ++i;
            ++o;
        }
        typename OUT::Index ind(0);
    }

    template <unsigned N,bool Inside>
    struct NonSeparableFilter {
        template <typename IMAGE,typename RECT_DOMAIN,typename INDEX,typename OUT,typename FILTER>
        static void
        Apply(const IMAGE& image,RECT_DOMAIN& imask,OUT& result,INDEX& ind,FILTER& filter,RECT_DOMAIN& fmask) {
            ind[N] = 0;
            imask.lbound(N) = fmask.lbound(N) = 1;
            imask.ubound(N) = fmask.ubound(N) = filter.ubound(N);
            for (--imask.lbound(N);--fmask.lbound(N)>filter.lbound(N);++imask.ubound(N),++ind[N])
                NonSeparableFilter<N-1,false>::Apply(image,imask,result,ind,filter,fmask);
            for (;imask.ubound(N)<image.size(N);++imask.lbound(N),++imask.ubound(N),++ind[N])
                NonSeparableFilter<N-1,Inside>::Apply(image,imask,result,ind,filter,fmask);
            for (--imask.ubound(N);--fmask.ubound(N)>=0;++imask.lbound(N),++ind[N])
                NonSeparableFilter<N-1,false>::Apply(image,imask,result,ind,filter,fmask);
        }
    };

    template <bool Inside>
    struct NonSeparableFilter<0,Inside> {
        template <typename IMAGE,typename RECT_DOMAIN,typename INDEX,typename OUT,typename FILTER>
        static void
        Apply(const IMAGE& image,RECT_DOMAIN& imask,OUT& result,INDEX& ind,FILTER& filter,RECT_DOMAIN& fmask) {
            ind[0] = 0;
            imask.lbound(0) = fmask.lbound(0) = 1;
            imask.ubound(0) = fmask.ubound(0) = filter.ubound(0);
            for (--imask.lbound(0);--fmask.lbound(0)>filter.lbound(0);++imask.ubound(0),++ind[0]) {
                filter.update(fmask);
                result(ind) = filter(image(imask),ind);
            }
            filter.update(fmask);
            for (;imask.ubound(0)<image.size(0);++imask.lbound(0),++imask.ubound(0),++ind[0])
                result(ind) = (Inside) ? filter(image(imask),ind,Inside) : filter(image(imask),ind);
            for (--imask.ubound(0);--fmask.ubound(0)>=0;++imask.lbound(0),++ind[0]) {
                filter.update(fmask);
                result(ind) = filter(image(imask),ind);
            }
        }
    };

    template <unsigned N,typename OUT,typename FILTER>
    struct SeparableFilter {
        template <typename IMAGE> static void
        Apply(const IMAGE& image,OUT& result,FILTER& filter) {
            Apply1DFilter<N,IMAGE,FILTER,OUT>(image,result,filter);
            SeparableFilter<N-1,OUT,FILTER>::Apply(result,result,filter);
        }
        template <typename IMAGE> static void
        Apply1D(const unsigned M,const IMAGE& image,OUT& result,FILTER& filter) {
            if (N==M)
                Apply1DFilter<N,IMAGE,FILTER,OUT>(image,result,filter);
            SeparableFilter<N-1,OUT,FILTER>::Apply1D(M,image,result,filter);
        }
#if 1
        template <typename IMAGE> static void
        Apply(const IMAGE& image,OUT& result,FILTER filters[]) {
            Apply1DFilter<N,IMAGE,FILTER,OUT>(image,result,filters[N]);
            SeparableFilter<N-1,OUT,FILTER>::Apply(result,result,filters);
        }
#endif
    };

    template <typename OUT,typename FILTER>
    struct SeparableFilter<0,OUT,FILTER> {
        template <typename IMAGE> static void
        Apply(const IMAGE& image,OUT& result,FILTER& filter) {
            Apply1DFilter<0,IMAGE,FILTER,OUT>(image,result,filter);
        }
        template <typename IMAGE> static void
        Apply1D(const unsigned M,const IMAGE& image,OUT& result,FILTER& filter) {
            if (M==0)
                Apply1DFilter<0,IMAGE,FILTER,OUT>(image,result,filter);
        }
        static void
        Apply(OUT& result,FILTER& filter) {
            Apply1DFilter<0,OUT,FILTER>(result,filter);
        }
#if 1
        template <typename IMAGE> static void
        Apply(const IMAGE& image,OUT& result,FILTER filters[]) {
            Apply1DFilter<0,IMAGE,FILTER,OUT>(image,result,filters[0]);
        }
        static void
        Apply(OUT& result,FILTER filters[]) {
            Apply1DFilter<0,OUT,FILTER>(result,filters[0]);
        }
#endif
    };

    template <typename IMAGE,typename FILTER,typename OUT>
    void filter_aux_1D(const unsigned N,const IMAGE& image,OUT& result,FILTER& filter,TrueType) {
        SeparableFilter<IMAGE::Dim-1,OUT,FILTER>::Apply1D(N,image,result,filter);
    }

    template <typename IMAGE,typename FILTER,typename OUT>
    void filter_aux(const IMAGE& image,OUT& result,FILTER& filter,TrueType) {
        SeparableFilter<IMAGE::Dim-1,OUT,FILTER>::Apply(image,result,filter);
    }

    template <typename IMAGE,typename FILTER,typename OUT>
    void filter_aux(const IMAGE& image,OUT& result,FILTER filters[IMAGE::Dim],TrueType) {
        SeparableFilter<IMAGE::Dim-1,OUT,FILTER>::Apply(image,result,filters);
    }

    template <typename IMAGE,typename FILTER,typename OUT>
    void filter_aux(const IMAGE& image,OUT& result,FILTER& filter,FalseType) {
        RectDomain<IMAGE::Dim> imask,fmask;
        typename IMAGE::Index index;
        NonSeparableFilter<IMAGE::Dim-1,true>::Apply(image,imask,result,index,filter,fmask);
    }

    //  User functions.

    //  Applying the filter only on one dimension.

    template <typename IMAGE,typename FILTER,typename OUT>
    inline void Filter1D(const unsigned N,const IMAGE& image,OUT& result,FILTER& filter) {
        typedef typename FILTER::IsSeparable separable;
        filter_aux_1D(N,image,result,filter,separable());
    }

    template <typename OUT,typename IMAGE,typename FILTER>
    OUT Filter1D(const unsigned N,const IMAGE& image,FILTER& filter) {
        OUT result(image.shape());
        Filter1D<IMAGE,FILTER,OUT>(N,image,result,filter);
        return result;
    }

    //  Applying the filter to all dimensions.

    template <typename OUT,typename IMAGE,typename FILTER>
    void Filter(const IMAGE& image,OUT& result,FILTER& filter) {
        typedef typename FILTER::IsSeparable separable;
        filter_aux(image,result,filter,separable());
    }

    template <typename OUT,typename IMAGE,typename FILTER>
    OUT Filter(const IMAGE& image,FILTER& filter) {
        OUT result(image.shape());
        Images::Filter<OUT>(image,result,filter);
        return result;
    }

    //  Applying one different filter (all of the same type) for each dimension.
    //  Meaningful only with separable filters since each filter is 1D.

    template <typename IMAGE,typename FILTER,typename OUT>
    void Filter(const IMAGE& image,OUT& result,FILTER filters[IMAGE::Dim]) {
        filter_aux(image,result,filters,TrueType());
    }

    template <typename OUT,typename IMAGE,typename FILTER>
    OUT Filter(const IMAGE& image,FILTER filters[IMAGE::Dim]) {
        OUT result(image.shape());
        Filter(image,result,filters);
        return result;
    }
}

#endif  //  !IMAGE_IMAGEFILTERS_H
