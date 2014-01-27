#include <iostream>
#include <Image.H>

//  Example: ./Copy < images/bear.pgm ### images/bear-%arch.inr

int
main(int argc,char *argv[]) try
{
    typedef Images::Image2D<unsigned char> CharImage2D;

    CharImage2D I1;
    std::cin >> I1;

    CharImage2D I2 = I1;
    std::cout << I2;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
