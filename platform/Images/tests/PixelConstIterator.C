#include <iostream>
#include <Image.H>

// Test the pixel access via pixel const iterator by computing the min and max of an image.

//  Example: ./PixelConstIterator < images/bear.pgm

int
main() try
{
    typedef Images::Image2D<unsigned char> Image;

    Image image;

    std::cin >> image;

    unsigned char max = 0;
    unsigned char min = 255;
    for(Image::const_iterator<Images::pixel> i=image.begin();i!=image.end();++i) {
        if (*i<=min)
            min = *i;
        else if (*i>max)
            max = *i;
    }
        
    std::cout << "Min = " << static_cast<unsigned>(min)
              << " Max = " << static_cast<unsigned>(max) << std::endl;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
