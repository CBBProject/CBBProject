#include <iostream>
#include <Image.H>

// Test the pixel access via pixel iterator by inverting an unsigned char image.

int
main()
{
    using namespace Images;
    typedef Image3D<unsigned char> Image3D;

    Image3D image(3,4,5);

    unsigned char l = 0;
    for(Image3D::iterator<pixel> i=image.begin();i!=image.end();++i)
        *i = l++;
    
    for(Image3D::const_iterator<domain> i=image.begin();i!=image.end();++i)
        std::cout << *i << ' ' << image(i) << std::endl;
    
    return 0;
}
