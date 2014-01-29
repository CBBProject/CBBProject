#include <Image.H>

//  Suppress the n higher/lower bits of an image.

//  Example: ./LowBits images/bear.pgm 2 LowBits.inr ### Null.output results/LowBits.inr

int main(int argc,char *argv[]) {

    using namespace Images;

    //  Read the images.

    std::ifstream ifs(argv[1],std::ios::binary);
    Images::Image2D<unsigned char> image;
    ifs >> image;

    const int select = atoi(argv[2]);
    const int nb     = std::abs(select);
    unsigned char mask = 0;
    for (unsigned i=0;i<nb;++i)
        mask = mask<<1|1;
    if (select<0)
        mask = ~mask;

    for (Image2D<unsigned char>::iterator<pixel> i=image.begin();i!=image.end();++i)
        *i = (*i)&mask;

    std::ofstream ofs(argv[3],std::ios::binary);
    ofs << image;

    return 0;
}
