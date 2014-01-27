#include <iostream>
#include <Image.H>

using namespace Images;

//  Example: ./ReadColor < images/bear.ppm ## results/Color.output

int main() {
    typedef Pixels::RGB<unsigned char> RGBPixel;
    typedef Image2D<RGBPixel> ColorImage;

    ColorImage I;
    std::cin >> I;
    std::cout << I(20,20) << std::endl;
    return 0;
}
