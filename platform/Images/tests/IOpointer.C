#include <iostream>
#include <Image.H>

// Read and rewrite any image.

//  Example: ./IOpointer < images/bear.ppm > bear.ppm ## images/bear.ppm
//  Example: ./IOpointer < images/bear.pgm > bear.pgm ## images/bear.pgm

int
main() try
{
    Images::Image* image;

    std::cin >> image;
    std::cout << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
