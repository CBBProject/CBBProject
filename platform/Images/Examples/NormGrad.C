#include <iostream>
#include <Image.H>
#include <Images/Grad.H>

//  Example: ./Grad < images/bear.float.inr ## results/Grad-%arch.inr

namespace Maths {
    namespace LinearAlgebra {
        namespace FixedDimensions {
            namespace {
                template <typename T>
                struct Scalar<Images::Pixels::RGB<T> > {
                    typedef T scalar_type;
                };
            }
        }
    }
}

template <typename Pixel, unsigned Dimension>
Images::Image*
NormGrad(const Images::Image* im) {

    using namespace Images;

    typedef typename GradPixel<Pixel>::Type GradPixel;

    typedef typename ImageType<Dimension,Pixel>::type IMAGE;
    typedef typename ImageType<Dimension,float>::type NIMAGE;
    
    const IMAGE& image = *reinterpret_cast<const IMAGE*>(im);

    NIMAGE* res = new NIMAGE(image.shape());
    *res = sqrt(norm2S(grad<GradPixel>(image)));

    return res;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(NormGrad,Images::Image* (*prototype)(const Images::Image*));

void Usage(const char* name) { std::cerr << "Usage: " << name << " image [result]" << std::endl; }

int
main(const int argc,const char* argv[]) try {

    using namespace Images;
    using namespace Maths::LinearAlgebra::FixedDimensions;

    if (argc<2 || argc>3) {
        Usage(argv[0]);
        return 1;
    }

    //  Read the images.

    std::ifstream ifs(argv[1]);
    Image* image;
    ifs >> image;

    //  Authorized types for this program.

    //typedef Polymorphic::Types<Polymorphic::All>::Dimensions<1,2,3> Switch;
    typedef Polymorphic::Types<float,double,unsigned char,unsigned short,Images::Pixels::RGB<unsigned char> >::Dimensions<1,2,3> Switch;

    // Instanciate for the effective type of image1 (which is equal to type of image2).

    Switch item(image->pixel_id(),image->dimension());

    // Instanciate correctly the Main function template and call it.

    Image* res = static_cast<Image*>((*item.Function<NormGradHandle>())(image));

    if (argc==3) {
        std::ofstream ofs(argv[2]);
        ofs << *res;
    } else {
        std::cout << *res;
    }

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
