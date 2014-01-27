#include <iostream>
#include <Image.H>

// Test the pixel acces by producing a bidimensional float INRIMAGE (a ramp) on stdout.

//  Example: ./PixelAccess ###

int
main() try
{
    using namespace Images;

    Dimension nx=200;
    Dimension ny=300;

    Image2D<float> image(nx,ny);

    const double scale = 1.0/(nx*ny);

    for(Coord i=0;i<image.dimx();++i)
        for(Coord j=0;j<image.dimy();++j)
            image(i,j) = scale*i*j;

    std::cout << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
