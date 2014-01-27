#include <iostream>
#include <Image.H>

using namespace Images;

//  Example: ./ReadWrite < images/bear.pgm ### results/ReadWrite-%arch.inr

int main() try
{
    Image2D<unsigned char> image;
    std::cin >> image;

    Image2D<unsigned char> curve(50,50);

    curve = 128;

    std::cout << curve;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
