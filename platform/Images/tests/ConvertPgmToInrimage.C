#include <iostream>
#include <Image.H>

//  Example: ./ConvertPgmToInrimage < images/bear.pgm ## images/bear-%arch.inr

int
main() try
{
    using namespace Images;

    Image2D<unsigned char> image;

    std::cin  >> format("rawpgm") >> image;
    std::cout << format("inrimage") << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
