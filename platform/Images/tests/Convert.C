#include <Image.H>

//  Test image conversion pixel wise by producing a bidimensional float INRIMAGE (a ramp)
//  and convert it before outputting it on stdout.

//  Example: ./Convert ###

int
main() try
{
    using namespace Images;

    Dimension nx=200;
    Dimension ny=300;

    Image2D<float> image(nx,ny);

    const double scale = 255.0/(nx*ny);

    for(Coord i=0;i<image.dimx();i++)
        for(Coord j=0;j<image.dimy();j++)
            image(i,j) = scale*i*j;

    std::cout << static_cast<Image2D<unsigned char> >(image);

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
