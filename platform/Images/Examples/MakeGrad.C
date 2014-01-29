#include <fstream>
#include <iostream>
#include <Image.H>
#include <Images/Grad.H>
#include <Inrimage5.H>

namespace Images {
    namespace Pixels {
        template <> const Property& Properties<Images::Vector<float,2> >::prop() { static Property p(false,"f(2)"); return p; }
    }
}

void usage(const char* name) { std::cerr << "Usage: " << name << " grad_x grad_y result" << std::endl; }

int
main(const int argc,const char* argv[]) try
{
    if (argc!=4) {
        usage(argv[0]);
        return 1;
    }
        
    const char* gxfile = argv[1];
    const char* gyfile = argv[2];
    const char* res    = argv[3];

    typedef Images::Image2D<float>           Image;
    typedef Images::Vector<float,Image::Dim> Grad;
    typedef Images::Image2D<Grad>            ImageGrad;

    Images::Inrimage5::IODesc::add<2,Grad>("Grad");

    const Image gx(gxfile);
    const Image gy(gyfile);

    if (gx.shape()!=gy.shape()) {
        std::cerr << "The gradient component images must have the same size!!" << std::endl;
        return 2;
    }

    ImageGrad grad(gx.shape());
    grad[0] = gx;
    grad[1] = gy;

    std::ofstream ofs(res,std::ios::binary);
    ofs << grad;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
