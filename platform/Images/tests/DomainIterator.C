#include <iostream>
#include <Image.H>

// Test the pixel access via pixel iterator by inverting an unsigned char image.

int
main()
{
    using namespace Images;

    Image2D<unsigned char> image(4,4);

    for(Image2D<unsigned char>::iterator<domain> i=image.begin();i!=image.end();++i)
        image(i) = 2*(*i)(1)+3*(*i)(2);
    
    for(Image2D<unsigned char>::const_iterator<domain> i=image.begin();i!=image.end();++i)
        std::cout << (*i)[0] << ' ' << (*i)[1] << ' ' << static_cast<unsigned>(image(i)) << std::endl;

#if 0
    for(std::reverse_iterator<Image2D<unsigned char>::iterator<domain> > i=image.end();i!=image.begin();++i)
        image(i) = 2*(*i)(1)+3*(*i)(2);
    
    for(std::reverse_iterator<Image2D<unsigned char>::const_iterator<domain> > i=image.end();i!=image.begin();++i)
        std::cout << (*i)[0] << ' ' << (*i)[1] << ' ' << static_cast<unsigned>(image(i)) << std::endl;
#endif

    return 0;
}
