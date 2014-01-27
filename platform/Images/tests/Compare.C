#include <iostream>
#include <fstream>
#include <exception>
#include <Utils/IOUtils.H>
#include <Image.H>

//  Example: ./Compare images/bear.pgm images/bear.pgm 2> Compare.output > NullImage.inr ## results/NullImage.inr results/Compare.output

using namespace std;
using namespace Images;

//  The user writes any function template depending upon the Pixel type
//  and the Dimension of the image.

template <typename Pixel,unsigned Dimension>
Image*
Compare(const Image* im1,const Image* im2)
{
    typedef typename ImageType<Dimension,Pixel>::type IMAGE;
    typedef BaseImage<Dimension,double>               DImage;
    
    const IMAGE& image1 = *reinterpret_cast<const IMAGE*>(im1);
    const IMAGE& image2 = *reinterpret_cast<const IMAGE*>(im2);

    DImage* res = new DImage(image1.shape());

    const DImage imd1 = static_cast<DImage>(image1);
    const DImage imd2 = static_cast<DImage>(image2);

    *res = imd1-imd2;

    std::cerr << "Min: " << min(*res) << " Max: " << max(*res) << std::endl;

    return res;
}

//  Declare  a handle, MainHandle, to that function template.

DeclarePolymorphicHandle(Compare,Image* (*prototype)(const Image*,const Image*));

void Usage(const char* name) { cerr << "Usage: " << name << " [--fromSuffix] image1 [--fromSuffix] image2 [image3]" << endl; }

int
main(int argc,char *argv[]) try
{
    if (argc<3) {
        Usage(argv[0]);
        return 1;
    }

    bool FromSuffixForInput = false;
    if (argc>1 && std::string(argv[1])=="--fromSuffix") {
        --argc; ++argv;
        FromSuffixForInput = true;
    }

    //  Read the images.

    ifstream ifs1(argv[1]);
    if (FromSuffixForInput)
        ifs1 >> Images::format(argv[1],Images::format::FromSuffix);
    Image* image1;
    ifs1 >> image1;

    FromSuffixForInput = false;
    if (argc>2 && std::string(argv[2])=="--fromSuffix") {
        --argc; ++argv;
        FromSuffixForInput = true;
    }

    if (argc<3 || argc>4) {
        Usage(argv[0]);
        return 1;
    }

    ifstream ifs2(argv[2]);
    if (FromSuffixForInput)
        ifs2 >> Images::format(argv[2],Images::format::FromSuffix);
    Image* image2;
    ifs2 >> image2;

    if (image1->pixel_id()!=image2->pixel_id()) {
        Usage(argv[0]);
        cerr << "image1 and image2 should have the same pixel type." << endl;
        return 2;
    }

    if (image1->dimension()!=image2->dimension()) {
        Usage(argv[0]);
        cerr << "image1 and image2 should have the same dimension." << endl;
        return 3;
    }

    //  Check that the images have the same size.

    for (Dimension i=0;i<image1->dimension();++i)
        if (image1->size(i)!=image2->size(i)) {
            Usage(argv[0]);
            cerr << "image1 and image2 should have the same sizes and size along dimension "
                 << i << " differs!" << endl;
            return 4;
        }

    //  Authorized types for this program.

    //typedef Polymorphic::Types<Polymorphic::All>::Dimensions<1,2,3> Switch;
    typedef Polymorphic::Types<float,double,unsigned char,unsigned short,unsigned,char,short,int>::Dimensions<1,2,3> Switch;

    // Instanciate for the effective type of image1 (which is equal to type of image2).

    Switch item(image1->pixel_id(),image1->dimension());

    // Instanciate correctly the Main function template and call it.

    Image* res = static_cast<Image*>((*item.Function<CompareHandle>())(image1,image2));

    if (argc==4) {
        ofstream ofs(argv[3]);
        ofs << *res;
    } else {
        cout << *res;
    }

    return 0;
}
catch (const Images::Exception& e) {
    std::cerr << e.what() << std::endl;
    return e.code();
}
