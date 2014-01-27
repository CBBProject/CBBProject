#pragma once

#include <typeinfo>
#include <string>

#include <Images/Exceptions.H>

#define DeclarePolymorphicHandle(name,proto) \
struct name ## Handle { \
    typedef proto; \
    template <typename Pixel,unsigned Dimension> \
    prototype get() { return &name<Pixel,Dimension>; } \
};

namespace Images {
    namespace Polymorphic {

        struct NoDim {
            static void dimension(const Image* const image) {
                throw UnknownDimension(image->dimension());
            }
        };

        template <typename IMAGE>
        struct NoType {
            template <typename Pixel>
            struct Converter {
                static IMAGE convert(const Image* const image) {
                    throw UnknownPixelType(image->pixel_id());
                }
            };
        };

        template <unsigned Dim,typename DBase=NoDim>
        struct MultiDim: public DBase {

            static Image* dimension(Image* image) {
                if (image->dimension()==Dim)
                    return image;
                DBase::dimension(image);
            }

            static const Image* const dimension(const Image* const image) {
                if (image->dimension()==Dim)
                    return image;
                DBase::dimension(image);
            }

            template <typename Pixel,typename TBase=NoType<typename ImageType<Dim,Pixel>::type> >
            struct MultiType: public TBase {

                template <typename Pixel2>
                class Converter: public TBase::template Converter<Pixel2> {

                    typedef typename ImageType<Dim,Pixel2>::type       ResultImage;
                    typedef typename TBase::template Converter<Pixel2> base;

                public:

                    ResultImage operator()(const Image* const image) const { return convert(MultiDim::dimension(image)); }

                protected:

                    static ResultImage
                    convert(const Image* const image) {
                        if (image->pixel_id()==typeid(Pixel))
                            return static_cast<const ResultImage>(*static_cast<const typename ImageType<Dim,Pixel>::type*>(image));

                        return base::convert(image);
                    }
                };
            };

            //  Can we manage easily that Pixel2 is also a MultiType class so that one can use trees instead of linear lists ??
        };

        // Pixel type known, dimension unknown.

        template <typename T>
        class Type {
        public:

            template <unsigned D1,unsigned D2,unsigned D3>
            class Dimensions {
            public:

                Dimensions(const std::type_info& t,const unsigned d): tid(t),dimension(d) { }

                // Switch according to dimension.

                template <typename fun>
                typename fun::prototype Call() {
                    fun f;
                    if (tid!=typeid(T)) throw UnknownPixelType(tid);
                    if (dimension==D1) return f.template get<T,D1>();
                    if (dimension==D2) return f.template get<T,D2>();
                    if (dimension==D3) return f.template get<T,D3>();
                    throw UnknownDimension(dimension);
                }

            private:

                const std::type_info& tid;
                const unsigned        dimension;
            };
        };

        // Pixel type and dimension unknown.

        template <typename T1,typename T2=T1,typename T3=T2,typename T4=T3,typename T5=T4,typename T6=T5,typename T7=T6,typename T8=T7,typename T9=T8>
        struct Types
        {
            template <unsigned D1,unsigned D2=D1,unsigned D3=D2>
            class Dimensions {
            public:

                Dimensions(const std::type_info& t,const unsigned d): tid(t),dimension(d) { }

                // Switch according to type.

                template <typename fun>
                typename fun::prototype Function() {

#ifdef NO_POLYMORPHIC_ICE
                    Type<T1>::Dimensions<D1,D2,D3> s1(tid,dimension);
                    Type<T2>::Dimensions<D1,D2,D3> s2(tid,dimension);
                    Type<T3>::Dimensions<D1,D2,D3> s3(tid,dimension);
                    Type<T4>::Dimensions<D1,D2,D3> s4(tid,dimension);
                    Type<T5>::Dimensions<D1,D2,D3> s5(tid,dimension);
                    Type<T6>::Dimensions<D1,D2,D3> s6(tid,dimension);
                    Type<T7>::Dimensions<D1,D2,D3> s7(tid,dimension);
                    Type<T8>::Dimensions<D1,D2,D3> s8(tid,dimension);
                    Type<T9>::Dimensions<D1,D2,D3> s9(tid,dimension);

                    try { return s1.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s2.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s3.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s4.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s5.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s6.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s7.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s8.template Call<fun>(); } catch(UnknownPixelType& e) { }
                    try { return s9.template Call<fun>(); } catch(UnknownPixelType& e) { }
#else
                    try { return Call2<fun,T1>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T2>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T3>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T4>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T5>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T6>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T7>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T8>(); } catch(UnknownPixelType& e) { }
                    try { return Call2<fun,T9>(); } catch(UnknownPixelType& e) { }
#endif
                    throw UnknownPixelType(tid);
                }

            private:

#ifndef NO_POLYMORPHIC_ICE

                // Switch according to dimension.

                template <typename fun,typename T>
                typename fun::prototype Call2() {
                    fun f;
                    if (tid!=typeid(T)) throw UnknownPixelType(tid);
                    if (dimension==D1) return f.template get<T,D1>();
                    if (dimension==D2) return f.template get<T,D2>();
                    if (dimension==D3) return f.template get<T,D3>();
                    throw UnknownDimension(dimension);
                }
#endif

                const std::type_info& tid;
                const unsigned        dimension;
            };
        };

#ifdef NO_POLYMORPHIC_ICE
        struct UnsignedIntegers { };
        struct SignedIntegers { };
        struct Floats { };
        struct All { };

        template <>
        class Type<UnsignedIntegers>: public Types<unsigned char,unsigned short,
                                                   unsigned int,unsigned long,unsigned long long> { };

        template <>
        class Type<SignedIntegers>: public Types<char,short,int,long,long long> { };

        template <>
        class Type<Floats>: public Types<float,double,long double> { };

        template<>
        class Type<All>: public Type<Floats>, Type<SignedIntegers>, Type<UnsignedIntegers> { };
#endif
    }
}
