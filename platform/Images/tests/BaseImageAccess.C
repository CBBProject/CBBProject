#include <Image.H>

//  Example: ./BaseImageAccess ##

// Test the pixel acces by producing a bidimensional float INRIMAGE (a ramp) on stdout.

int
main() try
{
    using namespace Images;

    Dimension nx=200;
    Dimension ny=300;

    const Dimension dims[] = { nx, ny };

    typedef BaseImage<2,float> Image2D;
    typedef Image2D::Index     Index;

    Shape<2> shape(dims);
    Image2D image(shape);

    const double scale = 1.0/(nx*ny);

    for(Coord i=0;i<image.size(0);++i)
        for(Coord j=0;j<image.size(1);++j) {
            const Coord ind[2] = { i, j };
            image(ind) = scale*i*j;
        }

    std::cout << image(Index(100,150));

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
