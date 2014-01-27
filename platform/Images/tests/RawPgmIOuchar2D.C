#include <iostream>
#include <Image.H>

//  Example: ./RawPgmIOuchar2D < images/bear.pgm > bear.pgm ## images/bear.pgm

int
main() try
{
    using namespace Images;
    using namespace std;

    Image2D<unsigned char> image;

    std::cin >> format("rawpgm") >> image;
    std::cout << image;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
