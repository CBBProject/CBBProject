#include <iostream>
#include <Image.H>

// Creates a simple 3D image for checking purposes.

//  Example: ./SimpleImage3D ### Null.output results/Simple3D.inr5

int
main()
{
    using namespace Images;
    typedef Image3D<float> Image3D;

    Image3D image(2,2,2);

    unsigned k = 0;
    for(Image3D::iterator<pixel> i=image.begin();i!=image.end();++i)
        *i = k++;
    
    std::ofstream ofs("Simple3D.inr5",std::ios::binary);
    ofs << image;

    return 0;
}
