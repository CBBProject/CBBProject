#include <iostream>
#include <Image.H>

int
main() try
{
    using namespace Images;

    unsigned data[] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
                        12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };

    Image1D<unsigned> I1(24,data);
    Image2D<unsigned> I2(6,4,data);
    Image3D<unsigned> I3(2,3,4,data);
                              
    for (Image1D<unsigned>::iterator<domain> i=I1.begin();i!=I1.end();++i)
        std::cout << I1(i) << std::endl;

    for (Image2D<unsigned>::iterator<domain> i=I2.begin();i!=I2.end();++i)
        std::cout << I2(i) << std::endl;
                              
    for (Image3D<unsigned>::iterator<domain> i=I3.begin();i!=I3.end();++i)
        std::cout << I3(i) << std::endl;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
