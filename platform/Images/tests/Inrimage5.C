#include <iostream>
#include <Image.H>

//  Example: ./Inrimage5 < images/bear.pgm ## images/bear-%arch-compressed.inr5

int
main() try
{
    using namespace Images;

    Image* image;

    std::cin  >> image;
    std::cout << format("Inrimage-5") << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
