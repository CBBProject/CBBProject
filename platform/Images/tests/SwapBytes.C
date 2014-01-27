#include <Image.H>

//  Swapping bytes of an image of short int.

//  Example: ./SwapBytes results/irm.short-le.inr SwapBytes.output ### results/SwapBytes.inr

int main(int argc,char *argv[]) {

    using namespace Images;

    //  Read the images.

    std::ifstream ifs(argv[1]);
    Images::Image3D<unsigned short> image;
    ifs >> image;

    for (Image3D<unsigned short>::iterator<pixel> i=image.begin();i!=image.end();++i)
        *i = ((*i)&255)*256+(*i)/256;

    std::ofstream ofs(argv[2]);
    ofs << image;

    return 0;
}
