#include <iostream>
#include <algorithm>
#include <Image.H>

int
main(int argc,char* argv[])
{
    using namespace Images;

    Image2D<float> image1(10,10);    
    Image2D<float> image2(10,10);

    image1 = 200.0;

    for (unsigned iter=0;iter<10;++iter) {

        image2 = 0.5*image1;

        std::cout << "val  = " << image1(5,5) << ' ' << image2(5,5) << std::endl;
        swap(image2,image1);
        std::cout << "vals = " << image1(5,5) << ' ' << image2(5,5) << std::endl;
    }

    return 0;
}
