#include <iostream>
#include <Image.H>

//  Example: ./IOuchar2D < images/bear.pgm > bear.pgm ## images/bear.pgm

int
main() try
{
    Images::Image2D<unsigned char> image;

    std::cin  >> image;
    std::cout << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
