#include <cstdlib>
#include <fstream>
#include <vector>
#include <Image.H>

//  Example: ./Scramble images/irm.inr irm.vmr 1000 ## results/Null.output results/irm.vmr

using namespace std;
using namespace Images;

template <typename T>
T uniform(const T max) {
    return static_cast<T>(max*(static_cast<double>(random())/RAND_MAX));
}

//  The user writes any function template depending upon the Pixel type
//  and the Dimension of the image.

template <typename Pixel,unsigned>
Image*
Scramble(const Image* im,const unsigned n,std::vector<unsigned>& perm) {

    typedef typename ImageType<2,Pixel>::type IMAGE;
    typedef typename ImageType<1,Pixel>::type Line;
    
    const IMAGE& image = *reinterpret_cast<const IMAGE*>(im);

    const unsigned dimy = image.dimy();

    perm.resize(dimy);
    for (unsigned i=0;i<dimy;++i)
        perm[i] = i;

    for (unsigned i=0;i<n;++i) {
        const unsigned ind1 = uniform(dimy);
        const unsigned ind2 = uniform(dimy);
        std::swap(perm[ind1],perm[ind2]);
    }

    IMAGE* res = new IMAGE(image.shape());
    for (unsigned i=0;i<dimy;++i)
        (*res)(Range::all(),i) = image(Range::all(),perm[i]);

    return res;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(Scramble,Image* (*prototype)(const Image*,const unsigned,std::vector<unsigned>&));

int
main(const int argc,const char* argv[]) try {

    using namespace Images;

    Image* image;

    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);

    const unsigned n = atoi(argv[3]);

    ifs >> image;

    typedef Polymorphic::Types<float,double,unsigned char>::Dimensions<2> Switch;
    Switch item(image->pixel_id(),image->dimension());

    std::vector<unsigned> perm;
    Image* res = (*item.Function<ScrambleHandle>())(image,n,perm);

    for (std::vector<unsigned>::const_iterator i=perm.begin();i!=perm.end();++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    
    ofs << res;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
