#include <fstream>
#include <vector>
#include <Image.H>

//  Example: ./Permute image1 result 1000 ## results/Null.output results/irm.vmr

using namespace std;
using namespace Images;

//  The user writes any function template depending upon the Pixel type
//  and the Dimension of the image.

template <typename Pixel,unsigned>
Image*
Permute(const Image* im,const std::vector<unsigned>& perm) {

    typedef typename ImageType<2,Pixel>::type IMAGE;
    typedef typename ImageType<1,Pixel>::type Line;
    
    const IMAGE& image = *reinterpret_cast<const IMAGE*>(im);
    const unsigned n = image.dimy();

    IMAGE* res = new IMAGE(image.shape());
    for (unsigned i=0;i<n;++i)
        (*res)(Range::all(),perm[i]) = image(Range::all(),i);

    return res;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(Permute,Image* (*prototype)(const Image*,const std::vector<unsigned>&));

int
main(const int argc,const char* argv[]) try {

    using namespace Images;

    Image* image;

    std::ifstream ifs(argv[1],std::ios::binary);
    std::ofstream ofs(argv[2],std::ios::binary);
    std::ifstream ifp(argv[3],std::ios::binary);

    ifs >> image;

    typedef Polymorphic::Types<float,double,unsigned char>::Dimensions<2> Switch;
    Switch item(image->pixel_id(),image->dimension());

    std::vector<unsigned> perm;
    typedef std::istream_iterator<unsigned> iit;
    back_insert_iterator<vector<unsigned> > inserter(perm);

    std::copy(iit(ifp),iit(),inserter);

    Image* res = (*item.Function<PermuteHandle>())(image,perm);

    ofs << res;

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
